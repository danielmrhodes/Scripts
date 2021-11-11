//std::string file = "gosia/Cd/high_and_low/With3Minus/cd106_on_pb208_W3M.out"
//std::string file = "gosia/Cd/all_data_sets/cd106_tot.out"
//std::string file = "gosia/Cd/all_data_sets/cd106_SecondSet.out"

struct Yield {

  int ex; //Experiment Number
  double ch; //Chi2 subtotal
  
  std::vector<double> en; //Transtion Energies (MeV)
  std::vector<double> yc; //Calculated Yields
  std::vector<double> ye; // Experimental Yields
  std::vector<double> zs; //Z-Scores

  bool GoodSize() {
    if((en.size() == yc.size()) && (en.size() == ye.size()) && (en.size() == zs.size())) {
      return true;
    }
    else {
      std::cout << "Not all vectors have the same size!" << std::endl;
      return false;
    }
  }

  int Size() {
    if(GoodSize()) {
      return en.size();
    }
    else {
      return 0;
    }
  }

  void Print() {

    std::cout << "\nExperiment " << ex << " (" << Size() << " Yields)\nEnergy (MeV) \tYCAL \tYEXP \t(YE-YC)/SIGMA\n";
    for(int i=0;i<Size();i++) {
      std::cout << en.at(i) << " \t\t" << yc.at(i) << " \t" << ye.at(i) << " \t" << zs.at(i) << std::endl;
    }
    std::cout << "\t\tChi2 Subtotal: " << ch << std::endl;
    
    return;
  }
  
};

struct YieldErrors : Yield { //This requires another print option when running GOSIA
  
  std::vector<double> er; //Experimental Errors

  bool GoodSize() {
    if((en.size() == yc.size()) && (en.size() == ye.size()) && (en.size() == er.size()) && (en.size() == zs.size())) {
      return true;
    }
    else if(ye.size() != yc.size()) {
      std::cout << "Number of calculated and experimental yields is different for EXP " << ex << "!" << std::endl;
      return false;
    }
    else if(en.size() != ye.size()) {
      std::cout << "Yield structure for EXP " << ex << " Incomplete!" << std::endl;
      return false;
    }
    else {
      std::cout << "Not all vectors have the same size!" << std::endl;
      return false;
    }
  }

  int Size() {
    if(GoodSize()) {
      return en.size();
    }
    else {
      return 0;
    }
  }

  void Print() {
    
    std::cout << "\nExperiment " << ex << " (" << Size() << " Yields)\nEnergy (MeV) \tYCAL \tYEXP \t\t(YE-YC)/SIGMA\n";
    for(int i=0;i<Size();i++) {
      std::cout << en.at(i) << " \t\t" << yc.at(i) << " \t" << ye.at(i) << "    \t" << zs.at(i) << " (Sigma = "
		<< er.at(i) << ")" << std::endl;;
    }
    std::cout << "\t\tChi2 Subtotal: " << ch << std::endl;

    return;
  }
     
};

std::vector<YieldErrors> GrabYieldErrors(std::string gosia_output) {

  std::vector<YieldErrors> yields;
  
  std::ifstream out;
  out.open(gosia_output.c_str(),std::ios::in);
  
  if(!out.is_open()) {
    std::cout << "Could not open Gosia output file " << gosia_output << "!" << std::endl;
    return yields;
  }

  std::string qry1 = "                                               REPRINT OF EXPERIMENTAL DATA TO BE FITTED";
  std::string mrk1 = "     EXPERIMENT";
  std::string end1 = "MATRIX ELEMENTS TO BE VARIED";
  
  std::string qry2 = "     CALCULATED AND EXPERIMENTAL YIELDS   EXPERIMENT";
  std::string mrk2 = "                                                  CHISQ SUBTOTAL =   ";

  std::string line,word;
  int word_num;
  int EXP = 1;
  
  while(std::getline(out,line)) {
    
    if(!line.compare(0,qry1.size(),qry1)) { //Found reprint of experimental data
      while(line.find(end1) == std::string::npos) { //While we haven't reached the end of this section
	
	std::getline(out,line);	
	if(!line.compare(0,mrk1.size(),mrk1)) { //Found an experiment
	  
	  YieldErrors yield;

	  std::stringstream linestream(line);
	  word_num=0;
	  while(linestream >> word) {
	    
	    int temp;
	    std::stringstream ss(word);
	    ss >> temp;

	    if(word_num==1) { //Experiment Number
	      yield.ex=temp;
	    }  
	    word_num++;
	  }
	  
	  //Move to begginning of yields
	  std::getline(out,line);
	  std::getline(out,line);
	  std::getline(out,line);
	  std::getline(out,line);
	  std::getline(out,line);
	
	  while(line.compare("")) { //While we're still in the list of yields for an experiment
      
	    std::stringstream linestream(line);
	    word_num = 0;
	    while(linestream >> word) {

	      double temp;
	      std::stringstream ss(word);
	      ss >> temp;

	      if(word_num==3) { //Experimental yield
		yield.ye.push_back(temp);
	      }
	      else if(word_num==4) { //Exerimental error
		yield.er.push_back(temp);
	      }
	      word_num++;
	    }
	    std::getline(out,line);
	  }
	  yields.push_back(yield);
	}
      } 
    }

    if(yields.size()) { //Make sure we actually got the experimental yields and errors
      if(!line.compare(0,qry2.size(),qry2)) { //Found a printout of fitted yields

	//Move to begginning of yields
	std::getline(out,line);
	std::getline(out,line);
	std::getline(out,line);
	while(line.compare("")) { //While we're still in the list of yields for an experiment
      
	  std::stringstream linestream(line);
	  word_num = 0;
	  while(linestream >> word) {

	    double temp;
	    std::stringstream ss(word);
	    ss >> temp;

	    if(word_num==4) { //Transition energy
	      yields.at(EXP-1).en.push_back(temp);
	    }
	    else if(word_num==5) { //Calculated yield
	      yields.at(EXP-1).yc.push_back(temp);
	    }
	    else if(word_num==8) { //Z-score
	      yields.at(EXP-1).zs.push_back(temp);
	    }
	  
	    word_num++;
	  }
	  std::getline(out,line);	
	}

	//Grab chi2 subtotal for the experiment
	std::getline(out,line);
      
	std::string sub(line.substr(mrk2.size()));
	double temp_ch;
	std::stringstream ss_ch(sub);
	ss_ch >> temp_ch;

	yields.at(EXP-1).ch=temp_ch;
	
	EXP++;
	
      }
    }
    
  }

  for(auto yld: yields) {
    if(!yld.GoodSize()) {
      yields.clear();
      break;
    }
  }
  
  return yields;
}

std::vector<Yield> GrabYields(std::string gosia_output) {

  std::vector<Yield> yields;
  
  std::ifstream out;
  out.open(gosia_output.c_str(),std::ios::in);
  
  if(!out.is_open()) {
    std::cout << "Could not open Gosia output file " << gosia_output << "!" << std::endl;
    return yields;
  }
  
  std::string qry = "     CALCULATED AND EXPERIMENTAL YIELDS   EXPERIMENT";
  std::string mrk = "                                                  CHISQ SUBTOTAL =   ";
  std::string line,word;

  int EXP = 0;
  int word_num;
  while(std::getline(out,line)) {
    if(!line.compare(0,qry.size(),qry)) { //Found a printout of fitted yields for an experiment
      
      EXP++;
      
      Yield yield;
      yield.ex=EXP;

      //Move to begginning of yields
      std::getline(out,line);
      std::getline(out,line);
      std::getline(out,line);
      while(line.compare("")) { //While we're still in the list of yields for an experiment
      
	std::stringstream linestream(line);
	
	word_num = 0;
	while(linestream >> word) {

	  double temp;
	  std::stringstream ss(word);
	  ss >> temp;

	  if(word_num==4) { //Transition energy
	    yield.en.push_back(temp);
	  }
	  else if(word_num==5) { //Calculated yield
	    yield.yc.push_back(temp);
	  }
	  else if(word_num==6) { //Experimental yield
	    yield.ye.push_back(temp);
	  }
	  else if(word_num==8) { //Z-score
	    yield.zs.push_back(temp);
	  }
	  
	  word_num++;
	}
	std::getline(out,line);	
      }

      //Grab chi2 subtotal for the experiment
      std::getline(out,line);
      
      std::string sub(line.substr(mrk.size()));
      double temp_ch;
      std::stringstream ss_ch(sub);
      ss_ch >> temp_ch;

      yield.ch=temp_ch;

      yields.push_back(yield);
    }
  }

  for(auto yld: yields) {
    if(!yld.GoodSize()) {
      yields.clear();
      break;
    }
  }
  
  return yields;
}

void PrintYields(std::string file) {

  std::vector<Yield> yields = GrabYields(file);
  for(auto yld : yields) {
    yld.Print();
  }
  std::cout << "\n";

  return;
}

void PrintYieldErrors(std::string file) {

  std::vector<YieldErrors> yields = GrabYieldErrors(file);

  for(auto yld : yields) {
    yld.Print();
  }
  std::cout << "\n";

  return;
}

bool CheckYields(std::string file) {

  std::vector<YieldErrors> yieldE = GrabYieldErrors(file);
  std::vector<Yield> yields = GrabYields(file);

  if(yieldE.size() != yields.size()) {
    std::cout << "Yields (" << yields.size() << ") and YieldErrros (" << yieldE.size() << ") dont have the same size!"
	      << std::endl;
    return false;
  }

  for(int i=0;i<yields.size();i++) {

    if(yields.at(i).ex != yieldE.at(i).ex) {
      std::cout << "Bad EXP number at yield " << i+1 << std::endl;
      return false;
    }

    if(yields.at(i).ch != yieldE.at(i).ch) {
      std::cout << "Bad Chi2 at yield " << i+1 << std::endl;
      return false;
    }
    
    if(yields.at(i).Size() != yieldE.at(i).Size()) {
      std::cout << "Bad size at yield " << i+1 << std::endl;
      return false;
    }

    for(int j=0;j<yields.at(i).Size();j++) {

      if(yields.at(i).en.at(j) != yieldE.at(i).en.at(j)) {
	std::cout << "Bad energy at yield " << i+1 << " index " << j<< std::endl;
	return false;
      }
      
      /*
      //YieldErrors gets more significant digits...
      if(yields.at(i).ye.at(j) != yieldE.at(i).ye.at(j)) {
	std::cout << "Bad YEXP at yield " << i+1 << " index " << j<< std::endl;
	return false;
      }
      */

      if(yields.at(i).yc.at(j) != yieldE.at(i).yc.at(j)) {
	std::cout << "Bad YCAL at yield " << i+1 << " index " << j<< std::endl;
	return false;
      }

      if(yields.at(i).zs.at(j) != yieldE.at(i).zs.at(j)) {
	std::cout << "Bad Z-Sore at yield " << i+1 << " index " << j<< std::endl;
	return false;
      }
      
    }
  }

  return true;
}

void PlotYield(int EXP, std::string file) {

  std::vector<Yield> yields = GrabYields(file);
  if(yields.size() == 0) {
    return;
  }
  
  Yield yield;
  if(yields.size() < EXP) {
    std::cout << "Only " << yields.size() << " yields were retrieved! Using last yield." << std::endl;
    yield = yields.back();
  }
  else {
    yield = yields.at(EXP-1);
  }
  int num = yield.en.size();

  TGraph* gc = new TGraph(num);
  TGraph* ge = new TGraph(num);
  TGraph* gs = new TGraph(num); 

  for(int i=0;i<num;i++) {

    gc->SetPoint(i,num-i,yield.yc.at(i));
    ge->SetPoint(i,num-i,yield.ye.at(i));
    gs->SetPoint(i,num-i,yield.zs.at(i));	  

  }

  TCanvas* c = new TCanvas();
  ge->SetMarkerStyle(kFullSquare);
  ge->SetTitle(Form("Experiment %d Yield Comparision",EXP));
  ge->GetYaxis()->SetTitle("Yield (arb.)");
  ge->GetXaxis()->SetTitle("Transition");
  //ge->SetMarkerSize(2);
  ge->Draw("AP");

  gc->SetMarkerStyle(kFullSquare);
  gc->SetMarkerColor(kRed);
  //gc->SetMarkerSize(2);
  gc->Draw("sameP");

  TLegend* leg = new TLegend(0.75,0.7,0.9,0.9);
  leg->SetTextAlign(22);
  leg->AddEntry(gc,"Calculated","P");
  leg->AddEntry(ge,"Experimental","P");
  leg->Draw();
 
  TCanvas* c1 = new TCanvas();
  gs->SetMarkerStyle(kFullSquare);
  gs->SetMarkerSize(2);
  gs->SetTitle(Form("Experiment %d Z-Scores",EXP));
  gs->GetYaxis()->SetTitle("Z-Score");
  gs->GetXaxis()->SetTitle("Transition");
  gs->Draw("AP");

  TLine* line = new TLine();
  line->SetLineStyle(9);
  
  if(gs->GetYaxis()->GetXmax() > 1.0) {
    line->DrawLine(gs->GetXaxis()->GetXmin(),1,gs->GetXaxis()->GetXmax(),1);
  }
  if(gs->GetYaxis()->GetXmin() < -1.0) {
    line->DrawLine(gs->GetXaxis()->GetXmin(),-1,gs->GetXaxis()->GetXmax(),-1);
  }
  
  return;
  
}

void PlotYieldError(int EXP, std::string file) {

  std::vector<YieldErrors> yields = GrabYieldErrors(file);
  if(yields.size() == 0) {
    return;
  }
  
  YieldErrors yield;
  if(yields.size() < EXP) {
    std::cout << "Only " << yields.size() << " yields were retrieved! Using last yield." << std::endl;
    yield = yields.back();
  }
  else if(EXP < 1) {
    std::cout << "Experiment numbering starts at 1! Using first experiment." << std::endl;
    yield = yields.at(0);
    EXP=1;
  }
  else {
    yield = yields.at(EXP-1);
  }
  int num = yield.en.size();

  TGraph* gc = new TGraph(num);
  TGraphErrors* ge = new TGraphErrors(num);
  TGraph* gs = new TGraph(num); 

  for(int i=0;i<num;i++) {

    gc->SetPoint(i,num-i,yield.yc.at(i));

    ge->SetPoint(i,num-i,yield.ye.at(i));
    ge->SetPointError(i,0,yield.er.at(i));

    gs->SetPoint(i,num-i,yield.zs.at(i));	  

  }

  TCanvas* c = new TCanvas();
  c->SetLogy(1);
  ge->SetMarkerStyle(kFullSquare);
  ge->SetTitle(Form("Experiment %d Yield Comparision",EXP));
  ge->GetYaxis()->SetTitle("Yield (arb.)");
  ge->GetXaxis()->SetTitle("Transition");
  //ge->SetMarkerSize(2);
  ge->Draw("APE");

  gc->SetMarkerStyle(kFullSquare);
  gc->SetMarkerColor(kRed);
  //gc->SetMarkerSize(2);
  gc->Draw("sameP");

  TLegend* leg = new TLegend(0.75,0.7,0.9,0.9);
  leg->SetTextAlign(22);
  leg->AddEntry(gc,"Calculated","P");
  leg->AddEntry(ge,"Experimental","PE");
  leg->Draw();
 
  TCanvas* c1 = new TCanvas();
  gs->SetMarkerStyle(kFullSquare);
  gs->SetMarkerSize(2);
  gs->SetTitle(Form("Experiment %d Z-Scores",EXP));
  gs->GetYaxis()->SetTitle("Z-Score");
  gs->GetXaxis()->SetTitle("Transition");
  gs->Draw("AP");

  TLine* line = new TLine();
  line->SetLineStyle(9);
  
  if(gs->GetYaxis()->GetXmax() > 1.0) {
    line->DrawLine(gs->GetXaxis()->GetXmin(),1,gs->GetXaxis()->GetXmax(),1);
  }
  if(gs->GetYaxis()->GetXmin() < -1.0) {
    line->DrawLine(gs->GetXaxis()->GetXmin(),-1,gs->GetXaxis()->GetXmax(),-1);
  }
  
  return;
  
}

void PlotZScores(std::string file) {


  std::map<int,double> map = {{0,0.6326},{1,0.8612},{2,1.0093},{3,1.0839},{4,1.1626},{5,1.7165},{6,1.7459}};
  std::map<double,int> map2 = {{0.6326,0},{0.8612,1},{1.0093,2},{1.0839,3},{1.1626,4},{1.7165,5},{1.7459,6}};
  
  std::array<std::vector<double>,7> scs;
  std::array<std::vector<int>,7> exs;
  std::array<std::vector<double>,7> yes;
  std::array<std::vector<double>,7> ycs;
  std::array<std::vector<double>,7> ers;

  std::array<int,7> nys = {0,0,0,0,0,0,0};
  std::array<double,7> chs = {0,0,0,0,0,0,0};

  std::vector<YieldErrors> ylds = GrabYieldErrors(file);
  for(auto y : ylds) {
    for(int i=0;i<y.Size();i++) {
      
      exs[map2[y.en.at(i)]].push_back(y.ex);
      scs[map2[y.en.at(i)]].push_back((double)y.zs.at(i));
      yes[map2[y.en.at(i)]].push_back((double)y.ye.at(i));
      ycs[map2[y.en.at(i)]].push_back((double)y.yc.at(i));
      ers[map2[y.en.at(i)]].push_back((double)y.er.at(i));

      nys[map2[y.en.at(i)]]++;
      chs[map2[y.en.at(i)]] += pow((double)(y.ye.at(i) - y.yc.at(i))/(double)y.er.at(i),2);
      
    }
    
  }

  int chk = 0;
  double cht = 0;
  for(int i=0;i<7;i++) {
    if(nys[i] && chs[i]) {
      
      chk += nys[i];
      cht += chs[i];
	
      std::cout << "Transtion " << i << " (" << 1000*map[i] << " keV): Chi2/Nt = " << chs[i] << "/" << nys[i] << " = "
		<< chs[i]/(double)nys[i] << std::endl;
    }
  }
  std::cout << "\n";

  std::vector<TGraph*> graphs;

  double chi1=0;
  double chi2=0;
  double chi3=0;

  double chi11=0;
  double chi21=0;
  double chi31=0;

  int nd1=0;
  int nd2=0;
  int nd3=0;

  int num_yields = 0;
  for(int i=0;i<7;i++) {
    if(scs[i].size()) {
    
      if(scs[i].size() != exs[i].size()) {
	std::cout << "Transition " << i << " has a bad size!" << std::endl;
	continue;
      }
    
      TGraph* g = new TGraph(scs[i].size());
      for(int j=0;j<scs[i].size();j++) {

	if(exs[i].at(j) < 8) {
	  chi1 += pow(scs[i].at(j),2);
	  chi11 += pow((double)(yes[i].at(j) - ycs[i].at(j))/(double)ers[i].at(j),2);
	  nd1++;
	}
	else if(exs[i].at(j) < 14) {
	  chi2 += pow(scs[i].at(j),2);
	  chi21 += pow((double)(yes[i].at(j) - ycs[i].at(j))/(double)ers[i].at(j),2);
	  nd2++;
	}
	else {
	  chi3 += pow(scs[i].at(j),2);
	  chi31 += pow((double)(yes[i].at(j) - ycs[i].at(j))/(double)ers[i].at(j),2);
	  nd3++;
	}
	
	g->SetPoint(j,exs[i].at(j),scs[i].at(j));
      }
      std::cout << g->GetN() << " yields for the " << 1000*map[i] <<  " keV transition." << std::endl;
      num_yields += g->GetN();

      if(nd1+nd2+nd3 != num_yields) {
	std::cout << "Bad counting!" << std::endl; 
      }
      
      graphs.push_back(g);
    }   
  }
  std::cout << num_yields << " total yields" << std::endl;
  
  if(chk != num_yields) {
    std::cout << "New bad counting! Chk = " << chk << std::endl; 
  }  

  //std::cout << "\nFirst data set: Chi2 / Nt = " << chi1 << "/" << nd1 << " = " << chi1/(double)nd1 << std::endl;
  //std::cout << "Second data set: Chi2 / Nt = " << chi2 << "/" << nd2 << " = " << chi2/(double)nd2 << std::endl;
  //std::cout << "Third data set: Chi2 / Nt = " << chi3 << "/" << nd3 << " = " << chi3/(double)nd3 << std::endl;
  
  std::cout << "\nFirst data set: Chi2/Nt = " << chi11 << "/" << nd1 << " = " << chi11/(double)nd1 << std::endl;
  std::cout << "Second data set: Chi2/Nt = " << chi21 << "/" << nd2 << " = " << chi21/(double)nd2 << std::endl;
  std::cout << "Third data set: Chi2/Nt = " << chi31 << "/" << nd3 << " = " << chi31/(double)nd3 << std::endl;

  std::cout << "\nYield Sum: Chi2 = " << cht << "\nData Set Sum: Chi2 = " << chi11+chi21+chi31 << std::endl;
  
  double ymin = -3.0;
  double ymax = 3.0;
  
  TLegend* leg = new TLegend(0.75,0.7,0.9,0.9);
  for(int i=0;i<graphs.size();i++) {
    
    graphs.at(i)->SetMarkerStyle(kFullSquare);
    graphs.at(i)->SetMarkerSize(2);
    graphs.at(i)->SetMarkerColor(i+1);
    graphs.at(i)->SetLineColor(i+1);

    leg->AddEntry(graphs.at(i),Form("%.0f keV",1000*map[i]),"PL");
    
    if(i==0) {
      graphs.at(i)->SetTitle("All Z-Scores");
      graphs.at(i)->GetYaxis()->SetTitle("Z-Score");;
      graphs.at(i)->GetXaxis()->SetTitle("Experiment Number");
      graphs.at(i)->Draw("APL");
      graphs.at(i)->GetYaxis()->SetRangeUser(ymin,ymax);
    }
    else {
      graphs.at(i)->Draw("samePL");
    }
    
  }
  leg->Draw();

  TLine* line = new TLine();
  line->DrawLine(7.5,ymin,7.5,ymax);
  line->DrawLine(13.5,ymin,13.5,ymax);  
  
  line->SetLineStyle(9);
  line->DrawLine(graphs.at(0)->GetXaxis()->GetXmin(),1,graphs.at(0)->GetXaxis()->GetXmax(),1);
  line->DrawLine(graphs.at(0)->GetXaxis()->GetXmin(),-1,graphs.at(0)->GetXaxis()->GetXmax(),-1);
  
  return;
  
}

TH1* ZScores(std::string file) {

  TH1* hz = new TH1D("Z-Score","Z-Scores",60,-3.05,2.95); 
  
  std::vector<Yield> ylds = GrabYields(file);
  for(auto y : ylds) {
    for(auto s : y.zs) {
      hz->Fill(s);
    }
  }
  int total_yields = hz->GetEntries();

  int sig05 = hz->Integral(hz->FindBin(-0.5),hz->FindBin(0.5));
  int sig10 = hz->Integral(hz->FindBin(-1.0),hz->FindBin(1.0));
  int sig15 = hz->Integral(hz->FindBin(-1.5),hz->FindBin(1.5));
  int sig20 = hz->Integral(hz->FindBin(-2.0),hz->FindBin(2.0));

  std::cout << "\n" << total_yields << " total yields fitted\n"
	    << sig05 << " within 0.5 sigma (" <<  100*(double)sig05/(double)total_yields << " %)\n"
	    << sig10 << " within 1.0 sigma (" <<  100*(double)sig10/(double)total_yields << " %)\n"
	    << sig15 << " within 1.5 sigma (" <<  100*(double)sig15/(double)total_yields << " %)\n"
	    << sig20 << " within 2.0 sigma (" <<  100*(double)sig20/(double)total_yields << " %)\n"
	    << std::endl;

  std::cout << "Standards: \n0.5 Sigma: 38.3% \n1.0 Sigma: 68.3% \n1.5 Sigma: 86.6% \n2 Sigma: 95.4%\n" << std::endl;

  if(hz->GetBinContent(0) || hz->GetBinContent(hz->GetNbinsX()+1)) {
    std::cout << hz->GetBinContent(0) << " underflows and " << hz->GetBinContent(hz->GetNbinsX()+1 )<< " overflows." << std::endl;
  }
  else {
    std::cout << "No overflows or underflows." << std::endl;
  }

  hz->GetXaxis()->SetTitle("Z-Score");
  hz->GetXaxis()->SetTitleColor(kBlack);
  hz->GetXaxis()->SetRangeUser(-3,3);
  hz->GetYaxis()->SetTitle("Number of Yields");
  hz->SetLineColor(kBlack);
  hz->SetLineWidth(2);
  
  return hz;

}

TF1* GausComp(TH1* hz) {

  TF1* f = new TF1("f","[0]*TMath::Gaus(x,0,1,kTRUE)",-3.0,3.0);
  
  f->SetParameter(0,hz->GetEntries()*hz->GetBinWidth(1));
  f->SetLineColor(kBlue);

  int ndf = 0;
  for(int i=1;i<hz->GetNbinsX()+1;i++) {
    if(hz->GetBinContent(i) > 0) {
      ndf++;
    }
  }
  std::cout << "Reduced Chi2: " << hz->Chisquare(f) << "/" << ndf << " = " << hz->Chisquare(f)/(double)ndf << std::endl;
  
  return f;
}

TGraph* ChiSquares(std::string file) {

  std::vector<Yield> ylds = GrabYields(file);

  double chi1=0;
  double chi2=0;
  double chi3=0;
  
  TGraph* g = new TGraph(ylds.size());
  for(int i=0;i<ylds.size();i++) {

    if(ylds.at(i).ex < 8) {
      chi1 += (double)ylds.at(i).ch / (double)ylds.at(i).Size();
    }
    else if(ylds.at(i).ex < 14) {
      chi2 += (double)ylds.at(i).ch / (double)ylds.at(i).Size();
    }
    else {
      chi3 += (double)ylds.at(i).ch / (double)ylds.at(i).Size();
    }
    
    g->SetPoint(i,ylds.at(i).ex,(double)ylds.at(i).ch / (double)ylds.at(i).Size());
    
    std::cout << "Experiment " << ylds.at(i).ex << ": Chi2 = " << (double)ylds.at(i).ch << " (" << ylds.at(i).Size() << " Yields)"
    << std::endl;
  }

  std::cout << "\nFirst data set: Chi2/Ne = " << chi1 << "/" << 7 << " = " << chi1/7.0 << std::endl;
  std::cout << "Second data set: Chi2/Ne = " << chi2 << "/" << 6 << " = " << chi2/6.0 << std::endl;
  std::cout << "Third data set: Chi2/Ne = " << chi3 << "/" << 5 << " = " << chi3/5.0 << std::endl;

  TCanvas* c = new TCanvas();
  g->SetMarkerStyle(kFullSquare);
  g->SetMarkerSize(2);
  g->SetTitle("#chi^{2} Subtotals");
  g->GetYaxis()->SetTitle("#chi^{2} / Transition");
  g->GetXaxis()->SetTitle("Experiement Number");
  g->Draw("APL");

  TLine* line = new TLine();
  if(g->GetXaxis()->GetXmin() < 7.0 && g->GetXaxis()->GetXmax() > 7.5) {
    line->DrawLine(7.5,g->GetYaxis()->GetXmin(),7.5,g->GetYaxis()->GetXmax());
  }
  if(g->GetXaxis()->GetXmax() > 13.0 && g->GetXaxis()->GetXmin() < 13.5) {
    line->DrawLine(13.5,g->GetYaxis()->GetXmin(),13.5,g->GetYaxis()->GetXmax());
  }
  
  return g;
}

void SingleYield(int trans, std::string file) {

  std::map<int,double> map = {{0,0.6326},{1,0.8612},{2,1.0093},{3,1.0839},{4,1.1626},{5,1.7165},{6,1.7459}};
  
  std::vector<Yield> ylds = GrabYields(file);
  std::vector<double> calc;
  std::vector<double> expm;
  std::vector<double> scor;
  std::vector<int> nums;
  
  for(auto y : ylds) {
    for(int i=0;i<y.Size();i++) {
      if(map[trans]==y.en.at(i)) {

	calc.push_back(y.yc.at(i));
	expm.push_back(y.ye.at(i));
	scor.push_back(y.zs.at(i));
	nums.push_back(y.ex);
	
      }
    }
  }
  int count = calc.size();
  
  TGraph* gs = new TGraph(count);
  TGraph* gc = new TGraph(count);
  TGraph* ge = new TGraph(count);
  
  for(int i=0;i<count;i++) {
    gs->SetPoint(i,nums.at(i),scor.at(i));
    gc->SetPoint(i,nums.at(i),calc.at(i));
    ge->SetPoint(i,nums.at(i),expm.at(i));
  }

  TCanvas* c = new TCanvas();
  ge->SetMarkerStyle(kFullSquare);
  ge->SetTitle(Form("%.01f keV Yield Comparison",1000*map[trans]));
  ge->GetYaxis()->SetTitle("Yield (arb.)");
  ge->GetXaxis()->SetTitle("Experiment Number");
  //ge->SetMarkerSize(2);
  ge->Draw("AP");

  gc->SetMarkerStyle(kFullSquare);
  gc->SetMarkerColor(kRed);
  //gc->SetMarkerSize(2);
  gc->Draw("sameP");

  TLine* line = new TLine();
  if(ge->GetXaxis()->GetXmin() < 7.0 && ge->GetXaxis()->GetXmax() > 7.5) {
    line->DrawLine(7.5,ge->GetYaxis()->GetXmin(),7.5,ge->GetYaxis()->GetXmax());
  }
  if(ge->GetXaxis()->GetXmax() > 13.0 && ge->GetXaxis()->GetXmin() < 13.5) {
    line->DrawLine(13.5,ge->GetYaxis()->GetXmin(),13.5,ge->GetYaxis()->GetXmax());
  }

  TLegend* leg = new TLegend(0.75,0.7,0.9,0.9);
  leg->SetTextAlign(22);
  leg->AddEntry(gc,"Calculated","P");
  leg->AddEntry(ge,"Experimental","PE");
  leg->Draw();
  
  TCanvas* c1 = new TCanvas();
  gs->SetMarkerStyle(kFullSquare);
  gs->SetMarkerSize(2);
  gs->SetTitle(Form("%.01f keV Transition Z-Scores",1000*map[trans]));
  gs->GetYaxis()->SetTitle("Z-Score");
  gs->GetXaxis()->SetTitle("Experiment Number");
  gs->Draw("APL");

  if(gs->GetXaxis()->GetXmin() < 7.0 && gs->GetXaxis()->GetXmax() > 7.5) {
    line->DrawLine(7.5,gs->GetYaxis()->GetXmin(),7.5,gs->GetYaxis()->GetXmax());
  }
  if(gs->GetXaxis()->GetXmax() > 13.0 && gs->GetXaxis()->GetXmin() < 13.5) {
    line->DrawLine(13.5,gs->GetYaxis()->GetXmin(),13.5,gs->GetYaxis()->GetXmax());
  }

  line->SetLineStyle(9);
  if(gs->GetYaxis()->GetXmax() > 1.0) {
    line->DrawLine(gs->GetXaxis()->GetXmin(),1,gs->GetXaxis()->GetXmax(),1);
  }
  if(gs->GetYaxis()->GetXmin() < -1.0) {
    line->DrawLine(gs->GetXaxis()->GetXmin(),-1,gs->GetXaxis()->GetXmax(),-1);
  }
  
  return;
  
}

void SingleYieldError(int trans, std::string file) {

  std::map<int,double> map = {{0,0.6326},{1,0.8612},{2,1.0093},{3,1.0839},{4,1.1626},{5,1.7165},{6,1.7459}};
  
  std::vector<YieldErrors> ylds = GrabYieldErrors(file);
  std::vector<double> calc;
  std::vector<double> expm;
  std::vector<double> errs;
  std::vector<double> scor;
  std::vector<int> nums;
  
  for(auto y : ylds) {
    for(int i=0;i<y.Size();i++) {
      if(map[trans]==y.en.at(i)) {

	calc.push_back(y.yc.at(i));
	expm.push_back(y.ye.at(i));
	errs.push_back(y.er.at(i));
	scor.push_back(y.zs.at(i));
	nums.push_back(y.ex);
	
      }
    }
  }
  int count = calc.size();
  
  TGraph* gs = new TGraph(count);
  TGraph* gc = new TGraph(count);
  TGraphErrors* ge = new TGraphErrors(count);
  
  for(int i=0;i<count;i++) {
    gs->SetPoint(i,nums.at(i),scor.at(i));
    gc->SetPoint(i,nums.at(i),calc.at(i));
    ge->SetPoint(i,nums.at(i),expm.at(i));
    ge->SetPointError(i,0,errs.at(i));
  }

  TCanvas* c = new TCanvas();
  ge->SetMarkerStyle(kFullSquare);
  ge->SetTitle(Form("%.01f keV Yield Comparison",1000*map[trans]));
  ge->GetYaxis()->SetTitle("Yield (arb.)");
  ge->GetXaxis()->SetTitle("Experiment Number");
  //ge->SetMarkerSize(2);
  ge->Draw("APE");

  gc->SetMarkerStyle(kFullSquare);
  gc->SetMarkerColor(kRed);
  //gc->SetMarkerSize(2);
  gc->Draw("sameP");

  TLine* line = new TLine();
  if(ge->GetXaxis()->GetXmin() < 7.0 && ge->GetXaxis()->GetXmax() > 7.5) {
    line->DrawLine(7.5,ge->GetYaxis()->GetXmin(),7.5,ge->GetYaxis()->GetXmax());
  }
  if(ge->GetXaxis()->GetXmax() > 13.0 && ge->GetXaxis()->GetXmin() < 13.5) {
    line->DrawLine(13.5,ge->GetYaxis()->GetXmin(),13.5,ge->GetYaxis()->GetXmax());
  }

  TLegend* leg = new TLegend(0.75,0.7,0.9,0.9);
  leg->SetTextAlign(22);
  leg->AddEntry(gc,"Calculated","P");
  leg->AddEntry(ge,"Experimental","PE");
  leg->Draw();
  
  TCanvas* c1 = new TCanvas();
  gs->SetMarkerStyle(kFullSquare);
  gs->SetMarkerSize(2);
  gs->SetTitle(Form("%.01f keV Transition Z-Scores",1000*map[trans]));
  gs->GetYaxis()->SetTitle("Z-Score");
  gs->GetXaxis()->SetTitle("Experiment Number");
  gs->Draw("APL");

  if(gs->GetXaxis()->GetXmin() < 7.0 && gs->GetXaxis()->GetXmax() > 7.5) {
    line->DrawLine(7.5,gs->GetYaxis()->GetXmin(),7.5,gs->GetYaxis()->GetXmax());
  }
  if(gs->GetXaxis()->GetXmax() > 13.0 && gs->GetXaxis()->GetXmin() < 13.5) {
    line->DrawLine(13.5,gs->GetYaxis()->GetXmin(),13.5,gs->GetYaxis()->GetXmax());
  }

  line->SetLineStyle(9);
  if(gs->GetYaxis()->GetXmax() > 1.0) {
    line->DrawLine(gs->GetXaxis()->GetXmin(),1,gs->GetXaxis()->GetXmax(),1);
  }
  if(gs->GetYaxis()->GetXmin() < -1.0) {
    line->DrawLine(gs->GetXaxis()->GetXmin(),-1,gs->GetXaxis()->GetXmax(),-1);
  }
  
  return;
  
}

int counter = 0;
void AlignedGraph(int trans, std::string file) {
  
  std::map<int,double> map = {{0,0.6326},{1,0.8612},{2,1.0093},{3,1.0839},{4,1.1626},{5,1.7165},{6,1.7459}};

  //std::map<int,double> map = {{0,0.3557},{1,0.3330},{2,0.5212}};
  //std::map<int,double> map = {{0,0.6592},{1,0.9144},{2,1.5736},{3,1.0834}};
  
  std::vector<YieldErrors> ylds = GrabYieldErrors(file);
  std::vector<double> calc;
  std::vector<double> expm;
  std::vector<double> errs;
  std::vector<double> scor;
  std::vector<int> nums;
  
  for(auto y : ylds) {
    for(int i=0;i<y.Size();i++) {
      if(map[trans]==y.en.at(i)) {

	calc.push_back(y.yc.at(i));
	expm.push_back(y.ye.at(i));
	errs.push_back(y.er.at(i));
	scor.push_back(y.zs.at(i));
	nums.push_back(y.ex);
	
      }
    }
  }
  int count = calc.size();
  
  TGraph* gs = new TGraph(count);
  TGraph* gc = new TGraph(count);
  TGraphErrors* ge = new TGraphErrors(count);

  double chi2 = 0;
  for(int i=0;i<count;i++) {

    chi2 += TMath::Power((calc.at(i) - expm.at(i))/errs.at(i),2.0);
    /*
    if(nums.at(i) == 5) {
      gs->SetPoint(i,nums.at(i),scor.at(i+1));
      gc->SetPoint(i,nums.at(i),calc.at(i+1));
      ge->SetPoint(i,nums.at(i),expm.at(i+1));
      ge->SetPointError(i,0,errs.at(i+1));
      continue;
    }
    if(nums.at(i) == 6) {
      gs->SetPoint(i,nums.at(i),scor.at(i-1));
      gc->SetPoint(i,nums.at(i),calc.at(i-1));
      ge->SetPoint(i,nums.at(i),expm.at(i-1));
      ge->SetPointError(i,0,errs.at(i-1));
      continue;
    }
    if(nums.at(i) == 11) {
      gs->SetPoint(i,nums.at(i),scor.at(i+1));
      gc->SetPoint(i,nums.at(i),calc.at(i+1));
      ge->SetPoint(i,nums.at(i),expm.at(i+1));
      ge->SetPointError(i,0,errs.at(i+1));
      continue;
    }
    if(nums.at(i) == 12) {
      gs->SetPoint(i,nums.at(i),scor.at(i-1));
      gc->SetPoint(i,nums.at(i),calc.at(i-1));
      ge->SetPoint(i,nums.at(i),expm.at(i-1));
      ge->SetPointError(i,0,errs.at(i-1));
      continue;
    }
    
    gs->SetPoint(i,nums.at(i),scor.at(i));
    gc->SetPoint(i,nums.at(i),calc.at(i));
    ge->SetPoint(i,nums.at(i),expm.at(i));
    ge->SetPointError(i,0,errs.at(i));
    */

    gs->SetPoint(i,nums.at(i)+13,scor.at(i));
    gc->SetPoint(i,nums.at(i)+13,calc.at(i));
    ge->SetPoint(i,nums.at(i)+13,expm.at(i));
    ge->SetPointError(i,0,errs.at(i));
    
  }

  std::cout << "Chi2/N = " << chi2 << "/" << count << " = " << chi2/double(count) << std::endl;
  
  TStyle *plain = new TStyle("Plain","Plain style"); // This is to make everything behave properly
  plain->SetCanvasBorderMode(0);
  plain->SetPadBorderMode(0);
  plain->SetPadColor(0);
  plain->SetCanvasColor(0);
  //gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);
  
  TCanvas *c1 = new TCanvas(Form("c%d",counter),"multipad",100,100,600,900); // Master canvas which we fill with pads
  counter++;
  c1->SetFrameBorderMode(0); // This makes the borders invisible, so we don't have margins... I think

  double graph_height = 0.85/2; // Setup the graph height so things all line up

  TPad* pad0 = new TPad("Pad0","Pad0",0,0,1,1); 
  pad0->Draw();
  pad0->cd(); // Change to the pad
  pad0->SetFillStyle(4000); // Make the pad transparent, stops margins making things ugly
  pad0->SetTopMargin(0.05); // Line things up
  pad0->SetBottomMargin(0.95-graph_height); // Line things up
  pad0->SetLeftMargin(0.08); // You can vary this
  pad0->SetRightMargin(0.05);
  //pad0->SetTickx(1); // Sets ticks to be at the top and bottom (I think)
  
  ge->Draw("APE");
  //ge->GetXaxis()->SetRangeUser(0,19);
  gc->Draw("sameP");

  ge->GetXaxis()->SetLabelSize(0);
  gc->GetXaxis()->SetLabelSize(0);

  TLine* line = new TLine();
  
  if(ge->GetXaxis()->GetXmin() < 7.0 && ge->GetXaxis()->GetXmax() > 7.5) {
    line->DrawLine(7.5,ge->GetYaxis()->GetXmin(),7.5,ge->GetYaxis()->GetXmax());
  }
  if(ge->GetXaxis()->GetXmax() > 13.0 && ge->GetXaxis()->GetXmin() < 13.5) {
    line->DrawLine(13.5,ge->GetYaxis()->GetXmin(),13.5,ge->GetYaxis()->GetXmax());
  }
  

  TPad* pad1 = new TPad("Pad1","Pad1",0,0,1,1); // Initialise
  pad1->Draw();
  pad1->cd(); // Change to the pad
  pad1->SetFillStyle(4000); // Make the pad transparent, stops margins making things ugly
  pad1->SetTopMargin(0.05+graph_height); // Line things up
  pad1->SetBottomMargin(0.95 - (2*graph_height)); // Line things up
  pad1->SetLeftMargin(0.08); // You can vary this
  pad1->SetRightMargin(0.05);
  //pad1->SetTickx(1); // Sets ticks to be at the top and bottom (I think)
  
  gs->Draw("APL"); // Draw the ith graph to the ith pad
  //gs->GetXaxis()->SetRangeUser(0,19);
  
  if(gs->GetXaxis()->GetXmin() < 7.0 && gs->GetXaxis()->GetXmax() > 7.5) {
    line->DrawLine(7.5,gs->GetYaxis()->GetXmin(),7.5,gs->GetYaxis()->GetXmax());
  }
  if(gs->GetXaxis()->GetXmax() > 13.0 && gs->GetXaxis()->GetXmin() < 13.5) {
    line->DrawLine(13.5,gs->GetYaxis()->GetXmin(),13.5,gs->GetYaxis()->GetXmax());
  }
  
  
  line->SetLineStyle(9);
  if(gs->GetYaxis()->GetXmax() > 1.0) {
    line->DrawLine(gs->GetXaxis()->GetXmin(),1,gs->GetXaxis()->GetXmax(),1);
    //line->DrawLine(0,1,19,1);
  }
  if(gs->GetYaxis()->GetXmin() < -1.0) {
    line->DrawLine(gs->GetXaxis()->GetXmin(),-1,gs->GetXaxis()->GetXmax(),-1);
    //line->DrawLine(0,-1,19,-1);
  }

  ge->SetMarkerStyle(kFullSquare);
  ge->SetNameTitle("","");
  //ge->GetYaxis()->SetTitle("Yield (arb.)");
  ge->GetYaxis()->CenterTitle();
  ge->GetYaxis()->SetTitleSize(0.05);
  //ge->GetYaxis()->SetLabelSize(0.05);
  ge->GetYaxis()->SetTitleOffset(0.8);
  gc->SetMarkerStyle(kFullSquare);
  gc->SetNameTitle("","");
  gc->SetMarkerColor(kRed);
  gs->SetMarkerStyle(kFullSquare);
  gs->SetMarkerSize(2);
  gs->SetNameTitle("","");
  //gs->GetYaxis()->SetTitle("Z-Score");
  //gs->GetXaxis()->SetTitle("Experiment Numer");
  gs->GetXaxis()->SetTitleColor(kBlack);
  gs->GetYaxis()->CenterTitle();
  gs->GetYaxis()->SetTitleSize(0.05);
  //gs->GetYaxis()->SetLabelSize(0.05);
  gs->GetYaxis()->SetTitleOffset(0.8);
  gs->GetXaxis()->CenterTitle();
  gs->GetXaxis()->SetTitleSize(0.05);
  gs->GetXaxis()->SetTitleOffset(0.95);
  //gs->GetXaxis()->SetLabelSize(0.05);
  
  return;
}
