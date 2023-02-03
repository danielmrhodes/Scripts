TGraph* PlotME(int index_y= 0, int index_x = 4, std::string fname = "output_me4_new.txt") {

  double chi_cut = 1.0;
  int nd = 23;

  std::vector<double> ys;
  std::vector<double> xs;

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

      if(word_num == index_y) {

	if(index_y == 0) {
	  temp *= nd;
	}
	ys.push_back(temp);
	
      }
      else if(word_num == index_x) {
	xs.push_back(temp);
      }
      
      word_num++;
    }
  }
  if(xs.size() != ys.size()) {
    std::cout << "Bad sizes!" << std::endl;
    return (TGraph*)NULL;
  }
  int size = xs.size();
  
  TGraph* gr = new TGraph(size);
  for(int i=0;i<size;i++) {
    gr->SetPoint(i,xs.at(i),ys.at(i));
  }
    
  gr->Sort();
  gr->SetLineWidth(2);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(1.5);
  gr->SetTitle("");
  //gr->GetXaxis()->SetTitle("Matrix Element");
  //gr->GetYaxis()->SetTitle("Matrix Element");
  //gr->GetYaxis()->SetTitleSize(0.06);
  //gr->GetYaxis()->SetTitleOffset(0.65);
  gr->GetXaxis()->SetLabelSize(0.05);
  gr->GetYaxis()->SetLabelSize(0.05);
  gr->SetEditable(kFALSE);
  gr->Draw("APL");

  //double xl = -0.95;
  //double xh = 0.95;
  //gr->GetXaxis()->SetRangeUser(xl,xh);
  
  if(index_y == 0) {
    //gr->GetYaxis()->SetTitle("Total #chi^{2}");

    double chi2_min = 999999.;
    double xmin = 0.0;
    for(int i=0;i<gr->GetN();i++) {

      double chi2 = gr->GetY()[i];
      if(chi2 < chi2_min) {
	chi2_min = chi2;
	xmin = gr->GetX()[i];
      }
    
    }

    std::cout << "Minimum chi2 (" << chi2_min << ") at " << xmin << std::endl;
    
    TLine* l = new TLine();
    l->SetLineColor(kRed);
    l->SetLineWidth(2);

    //l->DrawLine(xl,chi2_min + chi_cut,xh,chi2_min + chi_cut);
    l->DrawLine(gr->GetXaxis()->GetXmin(),chi2_min + chi_cut,
		gr->GetXaxis()->GetXmax(),chi2_min + chi_cut);
    
  }
  
  return gr;

}
