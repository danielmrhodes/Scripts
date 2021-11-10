//#include "kinematics.C"

void prob_reader() {

  std::string fn = "cd106_on_pb208.prb";
  std::ifstream file;
  file.open(fn.c_str(),std::ios::in); 

  std::vector<double> thetas;
  std::vector<std::vector<double>> probs;

  std::string line, word;

  int line_num = 0;
  while(std::getline(file,line)) {
    
    std::stringstream linestream1(line);
    
    int word_num = 0;
    while(linestream1 >> word) {

      double temp;
      std::stringstream ss(word);
      ss >> temp;

      if(!line_num && word_num) {
	std::vector<double> tmpvec;
	probs.push_back(tmpvec);
      }

      if(!word_num) {
	thetas.push_back(temp);
      }
      else {
	probs.at(word_num-1).push_back(temp);
      }
      
      word_num++;
    }
    line_num++;
  }

  //106Cd
  std::map<int,std::string> myMap = {{0,"0_{1}^{+}"},{1,"2_{1}^{+}"},{2,"4_{1}^{+}"},{3,"2_{2}^{+}"},
  				     {4,"0_{2}^{+}"},{5,"4_{2}^{+}"},{6,"3_{1}^{-}"},{7,"6_{1}^{+}"},
  				     {8,"5_{1}^{-}"},{9,"8_{1}^{+}"},{10,"2_{3}^{+}"},{11,"6_{2}^{+}"}};

  //48Ti
  //std::map<int,std::string> myMap = {{0,"0_{1}^{+}"},{1,"2_{1}^{+}"},{2,"4_{1}^{+}"},{3,"2_{2}^{+}"},
  //				     {4,"6_{1}^{+}"},{5,"4_{2}^{+}"}};

  //80Ge
  //std::map<int,std::string> myMap = {{0,"0_{1}^{+}"},{1,"2_{1}^{+}"},{2,"2_{2}^{+}"},{3,"4_{1}^{+}"},
  //				     {4,"4_{2}^{+}"},{5,"6_{1}^{+}"}};

  //80Kr
  //std::map<int,std::string> myMap = {{0,"0_{1}^{+}"},{1,"2_{1}^{+}"},{2,"2_{2}^{+}"},{3,"0_{1}^{+}"},
  //				     {4,"4_{1}^{+}"},{5,"3_{1}^{+}"},{6,"4_{2}^{+}"},{7,"6_{1}^{+}"},
  //				     {8,"8_{1}^{+}"}};

  //196Pt
  //std::map<int,std::string> myMap = {{0,"0_{1}^{+}"},{1,"2_{1}^{+}"},{2,"2_{2}^{+}"},{3,"4_{1}^{+}"},
  //				     {4,"4_{2}^{+}"},{5,"6_{1}^{+}"},{6,"6_{2}^{+}"},{7,"8_{1}^{+}"},
  //				     {8,"0_{2}^{+}"},{9,"3_{1}^{+}"}};
  
  double ymax = 1.0;
  double ymin = 0.0;
  
  TLegend* leg = new TLegend(0.9,0.1,0.95,0.9);
  TCanvas* c = new TCanvas();
  //c->SetLogy(1);
  for(int i=0;i<probs.size();i++) {

    TGraph* gr = new TGraph(thetas.size());
    for(int j=0;j<thetas.size();j++) {

      double thCM = thetas.at(j);
      //double thLB = Theta_LAB(thCM)*TMath::RadToDeg();
      
      gr->SetPoint(j,thCM,probs.at(i).at(j));
    }

    gr->SetMarkerStyle(kFullCircle);
    gr->SetMarkerSize(2.0);
    if(i<9) {
      gr->SetMarkerColor(i+1);
      gr->SetLineColor(i+1);
    }
    else {
      gr->SetMarkerColor(i+31);
      gr->SetLineColor(i+31);
    }
    leg->AddEntry(gr,myMap[i].c_str(),"PL");

    if(!i) {

      //gr->SetTitle("Coulomb Excitation Probabilities");
      gr->SetTitle("");
      gr->SetLineWidth(2);

      gr->GetYaxis()->SetTitleSize(0.06);
      gr->GetYaxis()->SetTitleOffset(0.6);
      gr->GetYaxis()->SetTitle("Probability");

      gr->GetXaxis()->SetTitleSize(0.05);
      gr->GetXaxis()->SetTitleOffset(0.8);
      gr->GetXaxis()->SetTitle("#theta_{CM} (rad)");
      //gr->GetXaxis()->SetTitle("Projectile #theta_{LAB} (deg)");
      //gr->GetXaxis()->SetTitle("Energy (MeV)");
      
      gr->Draw();
      gr->GetYaxis()->SetRangeUser(ymin,ymax);
      gr->GetXaxis()->SetRangeUser(0.0,180.0);
      
    }
    else {
      gr->SetLineWidth(2);
      gr->Draw("samePL");
    }
   
  }
  leg->Draw();

  /*
  //Proj LAB angles (deg)
  double a1 = 21.45;
  double a2 = 32.74;
  double a3 = 41.76;
  double a4 = 51.34;
  
  double a5 = Theta_LAB(Theta_CM_FR(a4*TMath::DegToRad()))*TMath::RadToDeg();
  double a6 = Theta_LAB(Theta_CM_FR(40.60*TMath::DegToRad()))*TMath::RadToDeg();
  double a7 = Theta_LAB(Theta_CM_FR(a1*TMath::DegToRad()))*TMath::RadToDeg();
  
  double a8 = 132.44;
  double a9 = 161.03;

  //std::cout << "Angles:\n " << a1 << "\n " << a2 << "\n " << a3 << "\n " << a4 << "\n " << a5
  //<< "\n " << a6 << "\n " << a7 << "\n " << a8 << "\n " << a9 << "\n ";
  
  TLine* l = new TLine();
  l->SetLineStyle(9);
  l->SetLineWidth(3);
  
  l->DrawLine(a1,ymin,a1,ymax);
  l->DrawLine(a2,ymin,a2,ymax);
  l->DrawLine(a3,ymin,a3,ymax);
  l->DrawLine(a4,ymin,a4,ymax);

  l->SetLineColor(28);
  l->DrawLine(a5,ymin,a5,ymax);
  l->DrawLine(a6,ymin,a6,ymax);
  l->DrawLine(a7,ymin,a7,ymax);

  l->SetLineColor(kBlack);
  l->DrawLine(a8,ymin,a8,ymax);
  l->DrawLine(a9,ymin,a9,ymax);
  */
  
  return;
  
}
