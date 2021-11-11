TH2* Interpolate(TH2* hc2, int binx1=5, int binx2=16, int biny1=7, int biny2=29, int ni_DME=100, int ni_TME=100) {

  //double DME_low = hc2->GetXaxis()->GetBinCenter(binx1); 
  //double DME_high = hc2->GetXaxis()->GetBinCenter(binx2);
  double DME_low = hc2->GetXaxis()->GetBinLowEdge(binx1); 
  double DME_high = hc2->GetXaxis()->GetBinLowEdge(binx2+1);
  int n_DMEs = ni_DME*(binx2 + 1 - binx1) + 1;

  double DME_spacing = (DME_high-DME_low)/(double)(n_DMEs-1);

  //double TME_low = hc2->GetYaxis()->GetBinCenter(biny1); 
  //double TME_high = hc2->GetYaxis()->GetBinCenter(biny2);
  double TME_low = hc2->GetYaxis()->GetBinLowEdge(biny1); 
  double TME_high = hc2->GetYaxis()->GetBinLowEdge(biny2+1);
  int n_TMEs = ni_TME*(biny2 + 1 - biny1) + 1;
  
  double TME_spacing = (TME_high-TME_low)/(double)(n_TMEs-1);

  std::cout << n_DMEs*n_TMEs << " Total Points\n" << n_DMEs << " DMEs from " << DME_low << " to " << DME_high
	    << " (step size = " << DME_spacing << ")\n" <<  n_TMEs << " TMEs from " << TME_low << " to " << TME_high
	    << " (step size = " << TME_spacing << ")\n"
	    << std::endl;

  //std::string htitle = Form("Interpolation of #chi^{2} Surface (DME,TME) = (%f,%f) to (%f,%f)",DME_low,TME_low,DME_high,TME_high);
  std::string htitle = "Smooth Interpolation of the #chi^{2} Surface";
  std::string hname = "Interpolation";
  TH2D* h2 = new TH2D(hname.c_str(),htitle.c_str(),n_DMEs,DME_low-(DME_spacing/2.0),DME_high+(DME_spacing/2.0),
		      n_TMEs,TME_low-(TME_spacing/2.0),TME_high+(TME_spacing/2.0));

  double DME = DME_low;
  double TME = TME_low;

  int i=1;
  while(DME < DME_high+(DME_spacing/2.0)) {
    while(TME < TME_high+(TME_spacing/2.0)) {
	
      h2->Fill(DME,TME,hc2->Interpolate(DME,TME));

      if(i%100000 == 0) {
	std::cout << "Point " << i << std::endl;
      }
      i++;
      
      TME+=TME_spacing;
    }
    TME=TME_low;
    DME+=DME_spacing;
  }

  int min_bin = h2->GetMinimumBin();
  int bx;
  int by;
  int bz;

  h2->GetBinXYZ(min_bin,bx,by,bz);
  
  double min_chi2 = h2->GetBinContent(min_bin);
  double min_DME = h2->GetXaxis()->GetBinCenter(bx);
  double min_TME = h2->GetYaxis()->GetBinCenter(by);

  h2->GetXaxis()->SetTitle("<2^{+}_{1}||E2||2^{+}_{1}> (eb)");
  h2->GetYaxis()->SetTitle("<0^{+}_{1}||E2||2^{+}_{1}> (eb)");
  h2->GetXaxis()->SetTitleColor(kBlack);

  std::cout << "\nMinimum interpolated chi2 of " << min_chi2 << " at (binx,biny) = (" << bx << "," << by << ") => (DME,TME) = ("
	    << min_DME << "," << min_TME << ")" << std::endl;

  h2->GetYaxis()->SetTitleOffset(1.1);
  h2->GetXaxis()->SetTitleOffset(1.1);
  
  hc2->GetYaxis()->SetRange(biny1,biny2);
  hc2->GetXaxis()->SetRange(binx1,binx2);
  
  return h2;
}

TF2* paraboloid() {

  TF2* f = new TF2("Parabaloid","[0]*x*x + [1]*y*y + [2]*x*y + [3]*x + [4]*y + [5]",-1.0,1.0,0.0,1.0);
  f->SetTitle("Fitted Paraboloid Function");
  f->SetParameters(1,1,1,1,1,1);

  return f;
}

void Mini(TF1* f1) {

  ROOT::Math::WrappedMultiTF1 f(*f1,f1->GetNdim());
  ROOT::Minuit2::Minuit2Minimizer min(ROOT::Minuit2::kMigrad);

  min.SetFunction(f);

  min.SetMaxFunctionCalls(1000000);
  min.SetMaxIterations(100000);
  min.SetTolerance(0.001);
  min.SetStorageLevel(1);
  
  // Set the free variables to be minimized!
  min.SetVariable(0,"x",0,0.01);
   
  min.Minimize();
   
  const double *xs = min.X();
  cout << "Minimum: f(" << xs[0] << "): " << f1->Eval(xs[0]) << endl;
   
  return;
}

double Mini2(TF2* f1) {

  ROOT::Math::WrappedMultiTF1 f(*f1,f1->GetNdim());
  ROOT::Minuit2::Minuit2Minimizer min(ROOT::Minuit2::kMigrad);

  min.SetFunction(f);

  min.SetMaxFunctionCalls(1000000);
  min.SetMaxIterations(100000);
  min.SetTolerance(0.001);
  min.SetStorageLevel(1);
  
  // Set the free variables to be minimized!
  min.SetVariable(0,"x",-0.471,0.01);
  min.SetVariable(1,"y",0.657,0.01);
   
  min.Minimize();
   
  const double *xs = min.X();
  const double mini = f1->Eval(xs[0],xs[1]);
  cout << "Minimum: f(" << xs[0] << "," << xs[1] << ") = " << mini << endl;
   
  return mini;
}

TH2* FittedSurface(TH2* h2) {

  gStyle->SetOptStat(0);
  
  TF2* f2 = paraboloid();
  h2->Fit(f2);

  /*
  new TCanvas();
  f2->SetNpx(1000);
  f2->SetNpy(1000);
  f2->Draw("colz");
  */
  
  TH2* hf = (TH2*)h2->Clone();
  hf->Clear();
  hf->GetListOfFunctions()->Clear();

  double min = 10000000.0;
  for(int i=1;i<hf->GetNbinsX()+1;i++) {
    for(int j=1;j<hf->GetNbinsY()+1;j++) {

      double x = hf->GetXaxis()->GetBinCenter(i);
      double y = hf->GetYaxis()->GetBinCenter(j);
      double val = f2->Eval(x,y);

      if(val < min) {
	min = val;
      }
      
    }
  }

  hf->Clear();
  for(int i=1;i<hf->GetNbinsX()+1;i++) {
    for(int j=1;j<hf->GetNbinsY()+1;j++) {

      double x = hf->GetXaxis()->GetBinCenter(i);
      double y = hf->GetYaxis()->GetBinCenter(j);
      double val = f2->Eval(x,y);

      if(val < min + 1.0) {
	hf->SetBinContent(i,j,val);
      }
      else {
	hf->SetBinContent(i,j,0);
      }
      
    }
  }

  new TCanvas();
  hf->Draw("colz");
  hf->SetMinimum(min);

  hf->SetTitle("");
  hf->GetXaxis()->SetTitle("");
  hf->GetYaxis()->SetTitle("");

  hf->GetXaxis()->SetLabelSize(0.05);
  hf->GetYaxis()->SetLabelSize(0.05);
  hf->GetZaxis()->SetLabelSize(0.05);

  hf->GetZaxis()->SetNdivisions(11111);
  
  return hf;
}
