double chi_cut = 1.0;
const int nd = 38;

//_

//6_1+ lifetime from Stephen and Jack
std::string fname = "output_4-2.txt";
//std::string fname = "output_4-1.txt";
const double y_ll = 0.90;
const double y_ul = 1.30;
const int y_st = 81;

const double x_ll = -1.5;
const double x_ul = 0.0;
const int x_st = 10;
const int np = 8;

const int index_y = 6;
const int index_x = 14;


//_
/*
std::string fname = "output_4.txt";
const double y_ll = 0.90;
const double y_ul = 1.30;
const int y_st = 81;

const double x_ll = -1.1;
const double x_ul = 0.0;
const int x_st = 11;
const int np = 8;

const int index_y = 6;
const int index_x = 14;
*/

//_42
/*
const double y_ll = 0.06;
const double y_ul = 0.15;
const int y_st = 46;

const double x_ll = -0.96;
const double x_ul = 1.0;
const int x_st = 10;
const int np = 8;

const int index_y = 2;
const int index_x = 4;
*/

/*
//_43, _43-1
const double y_ll = -0.85;
const double y_ul = 0.05;
const int y_st = 46;

const double x_ll = -0.96;
const double x_ul = 1.0;
const int x_st = 10;
const int np = 8;

const int index_y = 3;
const int index_x = 4;
*/

int counter = 0;
void MERange(const int index_z) {
  counter++;

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
  
  GH2D* h2 = new GH2D(Form("temp%02d",counter),"tmp",x_bins,xl,xh,y_bins,yl,yh);
  GH2D* h2_1s = new GH2D(Form("Surf1%02d",counter),"Cut Surface",x_bins,xl,xh,y_bins,yl,yh);
  
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
      double ch2;

      int word_num = 0;
      while(ss >> word) {
	
	std::stringstream ss1(word);
	if(word_num == 0) {
	  ss1 >> ch2;
	  ch2 *= nd;
	}
	else if(word_num == index_x) {
	  ss1 >> x;
	}
	else if(word_num == index_y) {
	  ss1 >> y;
	}
	else if(word_num == index_z) {
	  ss1 >> z;
	}
	word_num++;
      }
      h2->Fill(x,y,ch2);
      h2_1s->Fill(x,y,z);
    }
  }

  double best_me = 0.0;
  double minimum_tot = h2->GetMaximum();
  for(int by=h2->GetYaxis()->GetFirst();by<=h2->GetYaxis()->GetLast();by++) {
    for(int bx=h2->GetXaxis()->GetFirst();bx<=h2->GetXaxis()->GetLast();bx++) {
	  
      if((h2->GetBinContent(bx,by) < minimum_tot) && (h2->GetBinContent(bx,by) > 0)) {
	minimum_tot = h2->GetBinContent(bx,by);
	best_me = h2_1s->GetBinContent(bx,by);
      }
    }
  }

  std::vector<double> vals;
  for(int i=1;i<x_bins+1;i++) {
    for(int j=1;j<y_bins+1;j++) {
			  
      if(h2->GetBinContent(i,j) < minimum_tot+chi_cut && h2->GetBinContent(i,j) > 0)
	vals.push_back(h2_1s->GetBinContent(i,j));
      else
	h2_1s->SetBinContent(i,j,0.0);
    }
  }

  double min_me = *std::min_element(vals.begin(),vals.end());
  double max_me = *std::max_element(vals.begin(),vals.end());

  double err_up = max_me - best_me;
  double err_dn = best_me - min_me;
  std::cout << "ME Range: " << best_me << " + " << err_up << " - " << err_dn << " (" << min_me << "," << max_me << ")"
	    << std::endl;
  
  
  if(min_me < 0.0)
    h2_1s->SetMinimum(1.00001*min_me);
  else
    h2_1s->SetMinimum(0.99999*min_me);

  if(max_me < 0.0)
    h2_1s->SetMaximum(0.99999*max_me);
  else
    h2_1s->SetMinimum(1.00001*max_me);

  new GCanvas();
  h2_1s->Draw("colz");
  
  return;

} 

void PlotME2D(const int index_z = 0) {

  counter++;
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  if(index_z == index_x || index_z == index_y) {
    std::cout << "Bad index_z" << std::endl;
    return;
  }

  if(index_z) {
    MERange(index_z);
    return;
  }
  
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
    
  int binx;
  int biny;
  int binz;
  h2->GetBinXYZ(min_bin,binx,biny,binz);

  
  double best_x = h2->GetXaxis()->GetBinCenter(binx);
  double best_y = h2->GetYaxis()->GetBinCenter(biny);

  double min_x = best_x;
  double min_y = best_y;
  double max_x = best_x;
  double max_y = best_y;
  
  GH2D* h2_1s = new GH2D(Form("Surf1%02d",counter),"Cut Surface",x_bins,xl,xh,y_bins,yl,yh);
  for(int i=1;i<x_bins+1;i++) {
    for(int j=1;j<y_bins+1;j++) {
			  
      if(h2->GetBinContent(i,j) < minimum_tot+chi_cut && h2->GetBinContent(i,j) > 0) {
	  
	h2_1s->SetBinContent(i,j,h2->GetBinContent(i,j));

	double x = h2_1s->GetXaxis()->GetBinCenter(i);
	if(x < min_x)
	  min_x = x;
	if(x > max_x)
	  max_x = x;
	
	double y = h2_1s->GetYaxis()->GetBinCenter(j);
	if(y < min_y)
	  min_y = y;
	if(y > max_y)
	  max_y = y;
      }
      else
	h2_1s->SetBinContent(i,j,0.0);

    }
  }

  new GCanvas();
  h2_1s->SetMinimum(0.99999*minimum_tot);
  h2_1s->Draw("colz");

  double xerr_up = max_x - best_x;
  double xerr_dn = best_x - min_x;
  double yerr_up = max_y - best_y;
  double yerr_dn = best_y - min_y;
  
  std::cout << "\nChi2 Surface:" << "\n Minimum at binx=" << binx << ", biny=" << biny << " (Chi2 = " << minimum_tot << ")"
	    << "\n  X Range: " << best_x << " + " << xerr_up << " - " << xerr_dn << " (" << min_x << "," << max_x << ")"
	    << "\n  Y Range: " << best_y << " + " << yerr_up << " - " << yerr_dn << " (" << min_y << "," << max_y << ")"
	    << std::endl;
  
    
  return;

}
