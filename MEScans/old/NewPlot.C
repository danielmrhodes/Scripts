void NewPlot(std::string fname = "output_best.txt") {

  int nd = 23;
  std::vector<double> chis;
  std::vector<double> dmes;
  std::vector<double> meEs;
  std::vector<double> meMs;
  
  std::ifstream file;
  file.open(fname.c_str(),std::ios::in);

  std::string line, word;
  while(std::getline(file,line)) {

    std::stringstream ss(line);
    
    int word_num = 0;
    while(ss >> word) {
      
      double temp;
      std::stringstream ss1(word);
      ss1 >> temp;

      if(word_num == 0) {
	temp *= nd;
	chis.push_back(temp);
      }
      else if(word_num == 3) {
	dmes.push_back(temp);
      }
      else if(word_num == 5) {
	meEs.push_back(temp);
      }
      else if(word_num == 16) {
	meMs.push_back(temp);
      }
      
      word_num++;
    }
  }
  int size = chis.size();
  
  TGraphErrors* gr1 = new TGraphErrors(size);
  for(int i=0;i<size;i++) {

    gr1->SetPoint(i,meEs.at(i),dmes.at(i));
    gr1->SetPointError(i,0.0,0.11);
    
  }

  //gr1->Sort();
  gr1->SetLineWidth(2);
  gr1->SetMarkerStyle(20);
  gr1->SetMarkerSize(1.5);
  gr1->SetFillColorAlpha(kBlack,0.80);
  gr1->SetFillStyle(3001);
  gr1->SetTitle("");
  //gr1->GetXaxis()->SetTitle("Mixing Ratio");
  //gr1->GetYaxis()->SetTitle("DME (eb)");
  //gr1->GetYaxis()->SetTitleSize(0.06);
  //gr1->GetYaxis()->SetTitleOffset(0.65);
  gr1->GetYaxis()->SetLabelSize(0.05);
  gr1->GetXaxis()->SetLabelSize(0.05);
  gr1->SetEditable(kFALSE);

  new GCanvas();
  gr1->Draw("APLE3");

  int start = 14;
  int stop = size - 16;
  int new_size = stop-start;

  int countN = 0;
  int countP = 0;

  TGraphErrors* grN = new TGraphErrors(30);
  TGraphErrors* grP = new TGraphErrors(29);
  for(int i=start;i<stop;i++) {

    double del = 0.835*0.91447*meEs.at(i)/meMs.at(i);
    double val = del*del/(del*del + 1);
    
    if(del <= 0) {
      grN->SetPoint(i-start-29,val,dmes.at(i));
      grN->SetPointError(i-start-29,0.0,0.11);
      countN++;
    }
    else {
      grP->SetPoint(i-start,val,dmes.at(i));
      grP->SetPointError(i-start,0.0,0.11);
      countP++;
    }
    
  }

  TGraph* g = new TGraph(2);
  g->SetPoint(0,-1,-1);
  g->SetPoint(1,1.5,1);

  g->SetTitle("");
  g->GetYaxis()->SetLabelSize(0.05);
  g->GetXaxis()->SetLabelSize(0.05);
  //g->GetXaxis()->SetTitle("Mixing Ratio");
  //g->GetYaxis()->SetTitle("DME (eb)");
  //g->GetYaxis()->SetTitleSize(0.06);
  //g->GetYaxis()->SetTitleOffset(0.65);
  
  new GCanvas();
  g->Draw("AP");
  g->GetYaxis()->SetRangeUser(-0.82,-0.13);
  g->GetXaxis()->SetRangeUser(-0.05,1.05);
  
  //grN->Sort();
  grN->SetLineWidth(2);
  grN->SetMarkerStyle(20);
  grN->SetMarkerSize(1.5);
  grN->SetLineColor(kBlack);
  grN->SetMarkerColor(kBlack);
  grN->SetFillStyle(3001);
  grN->SetFillColor(kBlack);
  //grN->SetFillColorAlpha(kRed,0.80);;
  //->SetEditable(kFALSE);

  grP->SetLineWidth(2);
  grP->SetMarkerStyle(20);
  grP->SetMarkerSize(1.5);
  grP->SetFillStyle(3001);
  grP->SetFillColor(kBlack);
  //grP->SetFillColorAlpha(kBlack,0.10);
  //grP->SetEditable(kFALSE);
  
  grN->Draw("smme PLE3");
  grP->Draw("smme PLE3");
  
  return;
}
