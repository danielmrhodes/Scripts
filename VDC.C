//#include "kinematics.C"

double ATS(int Ne = 41) {
  
  if ( Ne <= 0 || Ne > 96 ) {
    return 0.0;
  }
    
  int m = Ne/2 + 1;
  if (Ne%2) {
    
    if ( m==1 || m==2 || m==3 || m==6 || 
	 m==7 || m==10 || m==15 || m==16 || 
	 m==19 || m==24 || m==25 || m==28 || 
	 m==31 || m==35 || m==37 || m==40 || 
	 m==41 || m==44 ) {
      return 0.5;
    }
    else if ( m==4 || m==5 || m==8 || m==9 || 
	      m==11 || m==17 || m==18 || m==20 ||
	      m==26 || m==27 || m==36 || m==42 ||
	      m==43 || m==45 ) {
      return 1.5;
    }
    else if ( m==12 || m==14 || m==21 || m==23 ||
	      m==32 || m==39 ) {
      return 2.5;
    }
    else if ( m==13 || m==22 || m==38 ) {
      return 4.5;
    }
    else if ( m==29 || m==30 || m==48 ) {
      return 3.5;
    }
    else if ( m==33 ) {
      return 7.5;
    }
    else if ( m==34 ) {
      return 6.5;
    }
    else if ( m==46 || m==47 ) {
      return 5.5;
    }
  }
	   
  m -= 1;
  if ( m==4 || m==8 || m==17 || m==26 || 
       m==28 || m==30 || m==32 || m==42 || 
       m==45 || m==48 ) {
    return 2.0;
  }
  else if ( m==10 || m==36 ) {
    return 1.0;
  }
  else if ( m==12 || m==21 || m==37 ) {
    return 3.0;
  }
  else if ( m==13 || m==22 || m==29 || m==31 || 
	    m==34 || m==38 || m==47 ) {
    return 4.0;
  }
  else if ( m==33 ) {
    return 8.0;
  }
  else if ( m==46 ) {
    return 6.0;
  }

  return 0.0;
   
}

void XSTATIC(int iz, double beta, int& id, int& iu, double& qcen, double& dq, double& xnor) {

  double h = 1.0/(1.0 + TMath::Power(TMath::Power(iz,0.45)*0.012008/beta,5.0/3.0));
  qcen = iz*TMath::Power(h,0.6);
  dq = TMath::Sqrt(qcen*(1.0-h))/2.0;

  iu = int(qcen + 3.0*dq + 0.5);
  id = int(qcen - 3.0*dq - 0.5);

  if(iu > iz) {
    iu = iz;
  }
  if(id < 1) {
    id = 1;
  }

  xnor = 0.0;
  for(int i=id;i<iu+1;i++) {
    xnor += TMath::Exp(-TMath::Power((qcen-i)/dq,2)/2.0);
  }

  return;
  
}

std::array<double,7> GKK(const int iz, const int ia, const double beta, const double spin, const double time) {
  
  //model parameters
  const double Avji = 3.0; //Average atomic spin
  const double Gam = 0.02; //FWHM of frequency distribution (ps^-1)
  const double Xlamb = 0.0345; //Fluctuating state to static state transition rate (ps^-1)
  const double TimeC = 3.5; //Mean time between random reorientations of fluctuating state  (ps)
  const double Gfac = iz/double(ia); //Nuclear gyromagnetic factor
  const double Field = 6.0*TMath::Power(10.0,-6.0); //Hyperfine field coefficient (600 T)
  const double Power = 0.6; //Hyperfine field exponent
  
  int  inq, ifq;
  double qcen, dq, xnor;
  XSTATIC(iz,beta,inq,ifq,qcen,dq,xnor);

  double aks[6] = {0.0,0.0,0.0,0.0,0.0,0.0}; //alpha_k
  double sum[3] = {0.0,0.0,0.0}; //stores sum over 6j symbols

  for(int j=inq;j<ifq+1;j++) {

    int nz = iz - j;
    double xji = ATS(nz);
    double sm = spin;
    if(spin > xji) {
      sm = xji;
    }

    int ncoup = int(2.0*sm + 0.5) + 1;
    sum[0] = 0.0;
    sum[1] = 0.0;
    sum[2] = 0.0;

    double valmi = spin - xji;
    if(valmi < 0.0) {
      valmi *= -1;
    }

    for(int m=0;m<ncoup;m++) {

      double f = valmi + m;
      for(int k=0;k<3;k++) {
	
	double rk = 2.0*k + 2.0;
	int if2 = int(2.0*f + 0.0001);
	int irk2 = int(2.0*rk + 0.0001);
	int ispin2 = int(2.0*spin + 0.0001);
	int ixji2 = int(2.0*xji + 0.0001);

	sum[k] += TMath::Power((2.0*f + 1.0)*ROOT::Math::wigner_6j(if2,if2,irk2,ispin2,ispin2,ixji2),2.0)/(2.0*xji + 1.0);
      }
      
    }

    for(int k=0;k<3;k++) {
      int k1 = 2*k;
      aks[k1] += sum[k]*TMath::Exp(-TMath::Power((qcen-j)/dq,2)/2.0)/xnor;
    }
    
  } //end loop over j (charge states)

  double xji = Avji;
  double sm = spin;
  if(spin > xji) {
    sm = xji;
  }

  int ncoup = int(2.0*sm + 0.5) + 1;
  sum[0] = 0.0;
  sum[1] = 0.0;
  sum[2] = 0.0;

  double valmi = spin - xji;
  if(valmi < 0.0) {
    valmi *= -1;
  }

  for(int m=0;m<ncoup;m++) {

    double f = valmi + m;
    for(int k=0;k<3;k++) {
	
      double rk = 2.0*k + 2.0;
      int if2 = int(2.0*f + 0.0001);
      int irk2 = int(2.0*rk + 0.0001);
      int ispin2 = int(2.0*spin + 0.0001);
      int ixji2 = int(2.0*xji + 0.0001);

      sum[k] += TMath::Power((2.0*f+1.0)*ROOT::Math::wigner_6j(if2,if2,irk2,ispin2,ispin2,ixji2),2.0)/(2.0*xji+1.0);
    }
      
  }

  for(int k=0;k<3;k++) {   
    int k1 = 2*k + 1;
    aks[k1] += sum[k];
  }

  double hmean = Field*iz*TMath::Power(beta,Power);
  double wsp = 4789.0*Gfac*hmean/Avji; // 4789 is mu_N
  wsp *= TimeC;
  wsp *= (wsp*Avji*(Avji+1.0)/3.0);

  std::array<double,7> Gk = {1.0,1.0,1.0,1.0,1.0,1.0,1.0};
  for(int k=0;k<3;k++) {
    
    int k2 = 2*k + 2;
    int k1 = 2*k + 1;

    double wrt = wsp*k2*(k2+1);
    double w2 = wrt;

    wrt *= (-1.0/(1.0 - aks[k2-1]));

    double xlam = (1.0 - aks[k2-1])*(1.0 - TMath::Exp(wrt))/TimeC;
    double up = (Gam*time*aks[k1-1] + 1.0)/(time*Gam + 1.0);
    up *= (Xlamb*time);
    up += 1.0;

    double down = time*(xlam+Xlamb) + 1.0;
    Gk.at(k2) = up/down;
    
    double alp = TMath::Sqrt(9.0*xlam*xlam + 8.0*xlam*TimeC*(w2 - xlam*xlam)) - 3.0*xlam;
    alp /= (4.0*xlam*TimeC);
    
    double upc = xlam*time*(down - 2.0*alp*alp*time*TimeC);
    double dwc = (down + alp*time)*(down + 2.0*alp*time);
    double ccf = 1.0 + upc/dwc;
    Gk.at(k2) *= ccf;

  }
  
  return Gk;

}

void VDC() {
  
  //48Ti first 2+
  int Z = 22;
  int A = 48;
  double spin = 2.0;
  double time = 5.84; //ps
  

  //196Pt first 2+
  /*
  int Z = 78;
  int A = 196;
  double spin = 2.0;
  double time = 49.3; //ps
  */
  
  const int nBins = 100;
  TGraph* g2 = new TGraph(nBins);
  TGraph* g4 = new TGraph(nBins);
  TGraph* g6 = new TGraph(nBins);

  /*
  double tmp_beta = 0.0937831;

  std::cout << "Z: " << Z << std::endl; 
  std::cout << "A: " << A << std::endl; 
  std::cout << "beta: " << tmp_beta << std::endl; 
  std::cout << "spin: " << spin << std::endl; 
  std::cout << "time: " << time << std::endl; 

  std::array<double,7> tmp_Gk = GKK(Z,A,tmp_beta,spin,time);
  for(int i=0;i<3;i++) {
    std::cout << "G" << 2*i + 2 << ": " << tmp_Gk.at(2*i + 2) << std::endl;
  }
  */

  const double beta_max = 0.11;
  for(int i=0;i<nBins;i++) {

    //double theta = (i+1)*TMath::Pi()/double(nBins);
    //double beta = Recoil_Beta_LAB(theta);
    double beta = (i+1)*beta_max/(double)nBins;
    
    std::array<double,7> Gk = GKK(Z,A,beta,spin,time);
    
    g2->SetPoint(i,beta,Gk.at(2));
    g4->SetPoint(i,beta,Gk.at(4));
    g6->SetPoint(i,beta,Gk.at(6));

    //g2->SetPoint(i,theta,Gk.at(2));
    //g4->SetPoint(i,theta,Gk.at(4));
    //g6->SetPoint(i,theta,Gk.at(6));
    
  }

  int lineWidth = 2;
  double markerSize = 2;

  g2->SetTitle("Vacuum Depolarization Coefficients");
  g2->GetYaxis()->SetTitle("Value");
  g2->GetYaxis()->SetTitleSize(0.06);
  g2->GetYaxis()->SetTitleOffset(0.7);
  g2->GetXaxis()->SetTitle("#beta");
  //g2->GetXaxis()->SetTitle("#theta_{CM} (rad)");
  g2->GetXaxis()->SetTitleSize(0.06);
  g2->GetXaxis()->SetTitleOffset(0.7);
  
  g2->SetMarkerColor(kRed);
  g2->SetLineColor(kRed);
  g2->SetMarkerStyle(kFullCircle);
  g2->SetLineWidth(lineWidth);
  g2->SetMarkerSize(markerSize);

  g4->SetMarkerColor(kBlue);
  g4->SetLineColor(kBlue);
  g4->SetMarkerStyle(kFullCircle);
  g4->SetLineWidth(lineWidth);
  g4->SetMarkerSize(markerSize);

  g6->SetMarkerColor(kGreen);
  g6->SetLineColor(kGreen);
  g6->SetMarkerStyle(kFullCircle);
  g6->SetLineWidth(lineWidth);
  g6->SetMarkerSize(markerSize);

  g2->Draw("APL");
  g4->Draw("samePL");
  g6->Draw("samePL");

  TLegend* leg = new TLegend(0.8,0.6,0.9,0.9);
  leg->AddEntry(g2,"G_{2}","PL");
  leg->AddEntry(g4,"G_{4}","PL");
  leg->AddEntry(g6,"G_{6}","PL");
  leg->Draw();

  //g2->GetYaxis()->SetRangeUser(0.0,1.1);
  
  return;
}
