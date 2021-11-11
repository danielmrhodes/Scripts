int counter = 0;
void PlotME2D(const int index_z = 0, std::string fname = "output.txt") {

  counter++;
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  const int index_y = 5;
  const int index_x = 3;
  if(index_z == index_x || index_z == index_y) {
    std::cout << "Bad index_z" << std::endl;
    return;
  }
  
  double chi_cut = 1.0;
  int nd = 23;

  double y_ll = -0.855;
  double y_ul = 0.905;
  int y_st = 89;
  double x_ll = -0.96;
  double x_ul = -0.10;
  int x_st = 10;
  int np = 8;
  
  double x_bins = x_st*np + 1;
  double x_sp = 0.0;
  if(x_bins > 1) {
    x_sp = (x_ul-x_ll)/double(x_bins - 1.0);
  }

  double y_bins = y_st;
  double y_sp = 0.0;
  if(y_bins > 1) {
    y_sp = (y_ul-y_ll)/double(y_bins - 1.0);
  }

  double xl = x_ll - (x_sp/2.0);
  double xh = x_ul + (x_sp/2.0);
  double yl = y_ll - (y_sp/2.0);
  double yh = y_ul + (y_sp/2.0);
  
  GH2D* h2 = new GH2D(Form("Surf%02d",counter),"Surface",x_bins,xl,xh,y_bins,yl,yh);
  for(int i=0;i<np;i++) {

    std::string name = Form("Piece%i/",i+1) + fname;
    std::ifstream file;
    file.open(name.c_str(),std::ios::in);

    if(!(file.is_open())) {
      std::cout << "FAILED TO OPEN FILE " << name << std::endl;
      continue;
    }

    std::string line, word;
    while(std::getline(file,line)) {
      
      std::stringstream ss(line);
      double x;
      double y;
      double z;

      int word_num = 0;
      while(ss >> word) {
	
	std::stringstream ss1(word);
	if(word_num == index_x) {
	  ss1 >> x;
	}
	else if(word_num == index_y) {
	  ss1 >> y;
	}
	else if(word_num == index_z) {
	  ss1 >> z;
	  if(index_z == 0) {
	    z *= nd;
	  }
	}
	word_num++;
      }
      h2->Fill(x,y,z);
    }
  }

  new GCanvas();
  h2->Draw("colz");

  int min_bin;
  double minimum_tot = h2->GetMaximum();
    
  for(int by=h2->GetYaxis()->GetFirst();by<=h2->GetYaxis()->GetLast();by++) {
    for(int bx=h2->GetXaxis()->GetFirst();bx<=h2->GetXaxis()->GetLast();bx++) {
	  
      if((h2->GetBinContent(bx,by) < minimum_tot) && (h2->GetBinContent(bx,by) != 0)) {
	minimum_tot = h2->GetBinContent(bx,by);
	min_bin = h2->GetBin(bx,by);
      }
	
    }
  }
  h2->SetMinimum(0.99999*minimum_tot);
  
  if(index_z) {
    return;
  }
    
  int binx;
  int biny;
  int binz;
  h2->GetBinXYZ(min_bin,binx,biny,binz);

  double min_x = h2->GetXaxis()->GetBinCenter(binx);
  double min_y = h2->GetYaxis()->GetBinCenter(biny);
    
  GH2D* h2_1s = new GH2D(Form("Surf1%02d",counter),"Cut Surface",x_bins,xl,xh,y_bins,yl,yh);
  for(int i=1;i<x_bins+1;i++) {
    for(int j=1;j<y_bins+1;j++) {
			  
      if(h2->GetBinContent(i,j) < minimum_tot+chi_cut && h2->GetBinContent(i,j) > 0) {
	  
	h2_1s->SetBinContent(i,j,h2->GetBinContent(i,j));
	  
      }
      else {
	h2_1s->SetBinContent(i,j,0.0);
      }

    }
  }

  new GCanvas();
  h2_1s->Draw("colz");
  h2_1s->SetMinimum(0.99999*minimum_tot);

  std::cout << "\nChi2 Surface:"
	    << "\n\tMinimum at binx=" << binx << ", biny=" << biny
	    << " (Chi2 = " << minimum_tot << ")"
	    << "\n\tmin_x = " << min_x << " eb"
	    << "\n\tmin_y = " << min_y << " eb"
	    << std::endl;
  
  return;

}
