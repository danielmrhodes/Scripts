double qs(double tme, double dme) {

  double fac = 64.0*TMath::Pi()/49.0;
  double Q2P = TMath::Sqrt(16.0*TMath::Pi()/5.0)*TMath::Sqrt(2.0/7.0)/TMath::Sqrt(5.0)*dme;
  double BE2 = tme*tme/5.0;
  
  return Q2P/TMath::Sqrt(fac*BE2);
  
}

void MakeChiSqPlot(double chi_cut = 1.0) {

  gStyle->SetOptStat(0);
  std::string name = "ge80.chisq_7";
  int np = 8;

  //ge80.bst_7
  int TME_bins = 201;
  double TME_ll = 0.396;
  double TME_ul = 0.419;
  int DME_bins = 217;
  double DME_ll = -0.72;
  double DME_ul = -0.45;
  
  double DME_spacing = 0.0;
  if(DME_bins > 1) {
    DME_spacing = (DME_ul-DME_ll)/double(DME_bins-1);
  }

  double TME_spacing = 0.0;
  if(TME_bins > 1) {
    TME_spacing = (TME_ul-TME_ll)/double(TME_bins-1);
  }

  GH2D *chisq_tot = new GH2D("Total","Total #chi^{2} Surface #chi^{2}_{p} + #chi^{2}_{t}",
			     DME_bins,DME_ll-(DME_spacing/2.0),DME_ul+(DME_spacing/2.0),
			     TME_bins,TME_ll-(TME_spacing/2.0),TME_ul+(TME_spacing/2.0));

  GH2D *chisq_proj = new GH2D("Projectile","Projectile #chi^{2} Surface",
			      DME_bins,DME_ll-(DME_spacing/2.0),DME_ul+(DME_spacing/2.0),
			      TME_bins,TME_ll-(TME_spacing/2.0),TME_ul+(TME_spacing/2.0));

  GH2D *chisq_targ = new GH2D("Target","Target #chi^{2} Surface",
			      DME_bins,DME_ll-(DME_spacing/2.0),DME_ul+(DME_spacing/2.0),
			      TME_bins,TME_ll-(TME_spacing/2.0),TME_ul+(TME_spacing/2.0));

  GH2D *chisq_tot1sigma = new GH2D("Total_1s",Form("Total %.1f#sigma Surface",chi_cut),
				   DME_bins,DME_ll-(DME_spacing/2.0),DME_ul+(DME_spacing/2.0),
				   TME_bins,TME_ll-(TME_spacing/2.0),TME_ul+(TME_spacing/2.0));

  GH2D *chisq_proj1sigma = new GH2D("Proj_1s",Form("Projectile %.1f#sigma Surface",chi_cut),
				    DME_bins,DME_ll-(DME_spacing/2.0),DME_ul+(DME_spacing/2.0),
				    TME_bins,TME_ll-(TME_spacing/2.0),TME_ul+(TME_spacing/2.0));

  GH2D *chisq_targ1sigma = new GH2D("Targ_1s",Form("Target %.1f#sigma Surface",chi_cut),
				    DME_bins,DME_ll-(DME_spacing/2.0),DME_ul+(DME_spacing/2.0),
				    TME_bins,TME_ll-(TME_spacing/2.0),TME_ul+(TME_spacing/2.0));
  
  std::string line;
  for(int k=0;k<np;k++) {

    vector<double> DME;
    vector<double> TME;
    vector<double> pro_chisq;
    vector<double> tar_chisq;
    vector<double> tot_chisq;

    double temp_DME;
    double temp_TME;
    double temp_pro;
    double temp_tar;
    double temp_tot;
    
    std::string fname = Form("Piece%i/",k+1) + name;
    ifstream infile(fname.c_str());
  
    if(!infile.is_open()) {
      std::cout << "FAILED TO OPEN FILE " << fname << std::endl;
      continue;
    }
	
    std::getline(infile,line);
    while(std::getline(infile,line)) {

      std::istringstream iss(line);

      iss >> temp_DME;
      iss >> temp_TME;
      iss >> temp_pro;
      iss >> temp_tar;
      iss >> temp_tot;

      DME.push_back(temp_DME);
      TME.push_back(temp_TME);

      pro_chisq.push_back(temp_pro);
      tar_chisq.push_back(temp_tar);
      tot_chisq.push_back(temp_tot);
    }   
 
    for(int i=0;i<tot_chisq.size();i++) {
      chisq_tot->Fill(DME.at(i),TME.at(i),tot_chisq.at(i));
      chisq_proj->Fill(DME.at(i),TME.at(i),pro_chisq.at(i));
      chisq_targ->Fill(DME.at(i),TME.at(i),tar_chisq.at(i));
    }   
    
  }
    
  int min_bin;
  double minimum_tot = chisq_tot->GetMaximum();

  int min_bin_proj;
  double minimum_proj = chisq_proj->GetMaximum();

  int min_bin_targ;
  double minimum_targ = chisq_targ->GetMaximum();
    
  for(int by=chisq_tot->GetYaxis()->GetFirst();by<=chisq_tot->GetYaxis()->GetLast();by++) {
    for(int bx=chisq_tot->GetXaxis()->GetFirst();bx<=chisq_tot->GetXaxis()->GetLast();bx++) {
	  
      if((chisq_tot->GetBinContent(bx,by) < minimum_tot) && (chisq_tot->GetBinContent(bx,by) > 0)) {
	minimum_tot = chisq_tot->GetBinContent(bx,by);
	min_bin = chisq_tot->GetBin(bx,by);
      }

      if((chisq_proj->GetBinContent(bx,by) < minimum_proj) && (chisq_proj->GetBinContent(bx,by) > 0)) {
	minimum_proj = chisq_proj->GetBinContent(bx,by);
	min_bin_proj = chisq_proj->GetBin(bx,by);
      }

      if((chisq_targ->GetBinContent(bx,by) < minimum_targ) && (chisq_targ->GetBinContent(bx,by) > 0)) {
	minimum_targ = chisq_targ->GetBinContent(bx,by);
	min_bin_targ = chisq_targ->GetBin(bx,by);
      }
	
    }
  }
  int binx;
  int biny;
  int binz;
  chisq_tot->GetBinXYZ(min_bin,binx,biny,binz);
    
  double min_DME = chisq_tot->GetXaxis()->GetBinCenter(binx);
  double min_TME = chisq_tot->GetYaxis()->GetBinCenter(biny);

  int binx_proj;
  int biny_proj;
  int binz_proj;
  chisq_proj->GetBinXYZ(min_bin_proj,binx_proj,biny_proj,binz_proj);
    
  double min_DME_proj = chisq_proj->GetXaxis()->GetBinCenter(binx_proj);
  double min_TME_proj = chisq_proj->GetYaxis()->GetBinCenter(biny_proj);

  int binx_targ;
  int biny_targ;
  int binz_targ;
  chisq_targ->GetBinXYZ(min_bin_targ,binx_targ,biny_targ,binz_targ);
    
  double min_DME_targ = chisq_targ->GetXaxis()->GetBinCenter(binx_targ);
  double min_TME_targ = chisq_targ->GetYaxis()->GetBinCenter(biny_targ);

  int xl1 = DME_bins+1;
  int xh1 = 0;
  int yl1 = TME_bins+1;
  int yh1 = 0;

  int xl1_p = DME_bins+1;
  int xh1_p = 0;
  int yl1_p = TME_bins+1;
  int yh1_p = 0;

  int xl1_t = DME_bins+1;
  int xh1_t = 0;
  int yl1_t = TME_bins+1;
  int yh1_t = 0;
    
  for(int i=1;i<DME_bins+1;i++) {
    for(int j=1;j<TME_bins+1;j++) {
			  
      if(chisq_tot->GetBinContent(i,j) < minimum_tot+chi_cut && chisq_tot->GetBinContent(i,j) > 0) {
	  
	chisq_tot1sigma->SetBinContent(i,j,chisq_tot->GetBinContent(i,j));

	if(i<xl1)
	  {xl1=i;}
	if(i>xh1)
	  {xh1=i;}
	  
	if(j<yl1)
	  {yl1=j;}
	if(j>yh1)
	  {yh1=j;}
	  
      }
      else {
	chisq_tot1sigma->SetBinContent(i,j,0.);
      }

      if(chisq_proj->GetBinContent(i,j) < minimum_proj+chi_cut && chisq_proj->GetBinContent(i,j) > 0) {
	  
	chisq_proj1sigma->SetBinContent(i,j,chisq_proj->GetBinContent(i,j));

	if(i<xl1_p)
	  {xl1_p=i;}
	if(i>xh1_p)
	  {xh1_p=i;}
	  
	if(j<yl1_p)
	  {yl1_p=j;}
	if(j>yh1_p)
	  {yh1_p=j;}
	  
      }
      else {
	chisq_proj1sigma->SetBinContent(i,j,0.);
      }

      if(chisq_targ->GetBinContent(i,j) < minimum_targ+chi_cut && chisq_targ->GetBinContent(i,j) > 0) {
	  
	chisq_targ1sigma->SetBinContent(i,j,chisq_targ->GetBinContent(i,j));

	if(i<xl1_t)
	  {xl1_t=i;}
	if(i>xh1_t)
	  {xh1_t=i;}
	  
	if(j<yl1_t)
	  {yl1_t=j;}
	if(j>yh1_t)
	  {yh1_t=j;}
	  
      }
      else {
	chisq_targ1sigma->SetBinContent(i,j,0.);
      }

    }
  }

  double DME_tot_ll = chisq_tot->GetXaxis()->GetBinCenter(xl1);
  double DME_tot_ul = chisq_tot->GetXaxis()->GetBinCenter(xh1);
  double TME_tot_ll = chisq_tot->GetYaxis()->GetBinCenter(yl1);
  double TME_tot_ul = chisq_tot->GetYaxis()->GetBinCenter(yh1);

  double qs_tot = qs(min_TME,min_DME);
  double qs_tot_ll = qs(TME_tot_ll,DME_tot_ll);
  double qs_tot_ul = qs(TME_tot_ul,DME_tot_ul);

  double DME_proj_ll = chisq_proj->GetXaxis()->GetBinCenter(xl1_p);
  double DME_proj_ul = chisq_proj->GetXaxis()->GetBinCenter(xh1_p);
  double TME_proj_ll = chisq_proj->GetYaxis()->GetBinCenter(yl1_p);
  double TME_proj_ul = chisq_proj->GetYaxis()->GetBinCenter(yh1_p);

  double qs_proj = qs(min_TME,min_DME);
  double qs_proj_ll = qs(TME_proj_ll,DME_proj_ll);
  double qs_proj_ul = qs(TME_proj_ul,DME_proj_ul);

  double DME_targ_ll = chisq_targ->GetXaxis()->GetBinCenter(xl1_t);
  double DME_targ_ul = chisq_targ->GetXaxis()->GetBinCenter(xh1_t);
  double TME_targ_ll = chisq_targ->GetYaxis()->GetBinCenter(yl1_t);
  double TME_targ_ul = chisq_targ->GetYaxis()->GetBinCenter(yh1_t);

  double qs_targ = qs(min_TME,min_DME);
  double qs_targ_ll = qs(TME_targ_ll,DME_targ_ll);
  double qs_targ_ul = qs(TME_targ_ul,DME_targ_ul);
  
  std::cout << "\nTotal Surface:"
	    << "\n\tMinimum at binx=" << binx << ", biny=" << biny << " (Chi2 = " << minimum_tot << ")"
	    << "\n\t<2+||E2||2+> = " << min_DME << " eb"
	    << "\n\t<0+||E2||2+> = " << min_TME << " eb"
	    << "\n\tqs = " << qs_tot << "\n"

	    << "\n\t<2+||E2||2+> " << chi_cut << " Sigma: " << DME_tot_ll << " to " << DME_tot_ul
	    << "\n\t<0+||E2||2+> " << chi_cut << " Sigma: " << TME_tot_ll << " to " << TME_tot_ul
	    << "\n\tqs " << chi_cut << " Sigma: " <<  qs_tot_ll << " to " << qs_tot_ul
    
	    << "\n\nProjectile Surface:"
	    << "\n\tMinimum at binx=" << binx_proj << ", biny=" << biny_proj << " (Chi2 = "
	    << minimum_proj << ")"
	    << "\n\t<2+||E2||2+> = " << min_DME_proj << " eb"
	    << "\n\t<0+||E2||2+> = " << min_TME_proj << " eb"
	    << "\n\tqs = " << qs_proj << "\n"

	    << "\n\t<2+||E2||2+> " << chi_cut << " Sigma: " << DME_proj_ll << " to " << DME_proj_ul
	    << "\n\t<0+||E2||2+> " << chi_cut << " Sigma: " << TME_proj_ll << " to " << TME_proj_ul
	    << "\n\tqs " << chi_cut << " Sigma: " <<  qs_proj_ll << " to " << qs_proj_ul

	    << "\n\nTarget Surface:"
	    << "\n\tMinimum at binx=" << binx_targ << ", biny=" << biny_targ << " (Chi2 = "
	    << minimum_targ << ")"
	    << "\n\t<2+||E2||2+> = " << min_DME_targ << " eb"
	    << "\n\t<0+||E2||2+> = " << min_TME_targ << " eb"
	    << "\n\tqs = " << qs_targ << "\n"

	    << "\n\t<2+||E2||2+> " << chi_cut << " Sigma: " << DME_targ_ll << " to " << DME_targ_ul
	    << "\n\t<0+||E2||2+> " << chi_cut << " Sigma: " << TME_targ_ll << " to " << TME_targ_ul
	    << "\n\tqs " << chi_cut << " Sigma: " <<  qs_targ_ll << " to " << qs_targ_ul
    
	    << "\n\n";
    
  chisq_tot->GetXaxis()->SetTitle("<2^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_tot->GetYaxis()->SetTitle("<0^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_tot->GetYaxis()->SetTitleSize(0.06);
  chisq_tot->GetYaxis()->SetTitleOffset(0.65);
  chisq_tot->GetYaxis()->CenterTitle();
  chisq_tot->GetXaxis()->SetTitleSize(0.06);
  chisq_tot->GetXaxis()->CenterTitle();

  chisq_proj->GetXaxis()->SetTitle("<2^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_proj->GetYaxis()->SetTitle("<0^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_proj->GetYaxis()->SetTitleSize(0.06);
  chisq_proj->GetYaxis()->SetTitleOffset(0.65);
  chisq_proj->GetYaxis()->CenterTitle();
  chisq_proj->GetXaxis()->SetTitleSize(0.06);
  chisq_proj->GetXaxis()->CenterTitle();

  chisq_targ->GetXaxis()->SetTitle("<2^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_targ->GetYaxis()->SetTitle("<0^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_targ->GetYaxis()->SetTitleSize(0.06);
  chisq_targ->GetYaxis()->SetTitleOffset(0.65);
  chisq_targ->GetYaxis()->CenterTitle();
  chisq_targ->GetXaxis()->SetTitleSize(0.06);
  chisq_targ->GetXaxis()->CenterTitle();

  chisq_tot1sigma->GetXaxis()->SetTitle("<2^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_tot1sigma->GetYaxis()->SetTitle("<0^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_tot1sigma->GetYaxis()->SetTitleSize(0.06);
  chisq_tot1sigma->GetYaxis()->SetTitleOffset(0.65);
  chisq_tot1sigma->GetYaxis()->CenterTitle();
  chisq_tot1sigma->GetXaxis()->SetTitleSize(0.06);
  chisq_tot1sigma->GetXaxis()->CenterTitle();

  chisq_proj1sigma->GetXaxis()->SetTitle("<2^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_proj1sigma->GetYaxis()->SetTitle("<0^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_proj1sigma->GetYaxis()->SetTitleSize(0.06);
  chisq_proj1sigma->GetYaxis()->SetTitleOffset(0.65);
  chisq_proj1sigma->GetYaxis()->CenterTitle();
  chisq_proj1sigma->GetXaxis()->SetTitleSize(0.06);
  chisq_proj1sigma->GetXaxis()->CenterTitle();

  chisq_targ1sigma->GetXaxis()->SetTitle("<2^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_targ1sigma->GetYaxis()->SetTitle("<0^{+}_{1}||E2||2^{+}_{1}> (eb)");
  chisq_targ1sigma->GetYaxis()->SetTitleSize(0.06);
  chisq_targ1sigma->GetYaxis()->SetTitleOffset(0.65);
  chisq_targ1sigma->GetYaxis()->CenterTitle();
  chisq_targ1sigma->GetXaxis()->SetTitleSize(0.06);
  chisq_targ1sigma->GetXaxis()->CenterTitle();

  /*
  TLine* line1 = new TLine();
  double lit = 0.372827;
  double lit_ll = 0.334664;
  double lit_ul = 0.407431;
  */
  
  new GCanvas();
  chisq_tot->Draw("colz");
  chisq_tot->SetMinimum(0.99999*minimum_tot);
    
  new GCanvas;
  chisq_tot1sigma->Draw("colz");
  chisq_tot1sigma->SetMinimum(0.99999*minimum_tot);
  //ell->Draw("sameL");

  /*
  line1->SetLineStyle(1);
  line1->DrawLine(DME_ll,lit,DME_ul,lit);
  line1->SetLineStyle(9);
  line1->DrawLine(DME_ll,lit_ll,DME_ul,lit_ll);
  line1->DrawLine(DME_ll,lit_ul,DME_ul,lit_ul);
  */

  new GCanvas();
  chisq_proj->Draw("colz");
  chisq_proj->SetMinimum(0.99999*minimum_proj);
    
  new GCanvas;
  chisq_proj1sigma->Draw("colz");
  chisq_proj1sigma->SetMinimum(0.99999*minimum_proj);

  /*
  line1->SetLineStyle(1);
  line1->DrawLine(DME_ll,lit,DME_ul,lit);
  line1->SetLineStyle(9);
  line1->DrawLine(DME_ll,lit_ll,DME_ul,lit_ll);
  line1->DrawLine(DME_ll,lit_ul,DME_ul,lit_ul);
  */

  new GCanvas();
  chisq_targ->Draw("colz");
  chisq_targ->SetMinimum(0.99999*minimum_targ);
    
  new GCanvas;
  chisq_targ1sigma->Draw("colz");
  chisq_targ1sigma->SetMinimum(0.99999*minimum_targ);

  /*
  line1->SetLineStyle(1);
  line1->DrawLine(DME_ll,lit,DME_ul,lit);
  line1->SetLineStyle(9);
  line1->DrawLine(DME_ll,lit_ll,DME_ul,lit_ll);
  line1->DrawLine(DME_ll,lit_ul,DME_ul,lit_ul);
  */
  
  return; 
  
}
