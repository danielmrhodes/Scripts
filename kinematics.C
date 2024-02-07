//106Cd
//double beam_Z = 48.;
//double beam_mass = 98626.9;

//24Ne
//double beam_Z = 10.;
//double beam_mass = 22349.9;

//68,80Ge
//double beam_Z = 32.;
//double beam_mass = 63274.6;
//double beam_mass = 74441.6;

//78,80,84,86Kr
double beam_Z = 36.;
double beam_mass = 72582.4;
//double beam_mass = 74442.6;
//double beam_mass = 78163.1;
//double beam_mass = 80025.2;

//40S
//double beam_Z = 16.;
//double beam_mass = 37236.9;

//128Xe
//double beam_Z = 54.;
//double beam_mass = 119141.4;

//208Pb
//double targ_Z = 82.;
//double targ_mass = 193688.0;

//48Ti
double targ_Z = 22.;
double targ_mass = 44652.;

//196Pt
//double targ_Z = 78.;
//double targ_mass = 182540.;

//197Au
//double targ_Z = 79.;
//double targ_mass = 183473.2;

//110Pd
//double targ_Z = 46.;
//double targ_mass = 102376.0;

//double Ep = 450.0; // 106Cd HE
//double Ep = 415.1; // 106Cd LE
//double Ep = 294.0; //106Cd Ti
//double Ep = 268.0; //80Ge, 80Kr
//double Ep = 281.6; //80Ge
//double Ep = 278.8; //68Ge
//double Ep = 131.0; //40S
//double Ep = 487.7; //128Xe
//double Ep = 77.0; //24Ne
//double Ep = 413.0; //78Kr HE
//double Ep = 278.0; //78Kr LE
//double Ep = 317.8; //2023 78Kr mid-target energy (196Pt)
double Ep = 200.;

double Ex = 0.0;

int nBins = 1800;
int lineWidth = 4;
int markerStyle = 20;
double markerSize = 1.5;

double Theta_CM_FP(double ThetaLAB, bool sol2 = false) {
  
  double tau = (beam_mass/targ_mass)/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));
  
  if(std::sin(ThetaLAB) > 1.0/tau) {

    ThetaLAB = std::asin(1.0/tau);
    if(ThetaLAB < 0)
      ThetaLAB += TMath::Pi();  

    return std::asin(tau*std::sin(ThetaLAB)) + ThetaLAB;
  }

  if(sol2)
    return std::asin(tau*std::sin(-ThetaLAB)) + ThetaLAB + TMath::Pi();
  
  return std::asin(tau*std::sin(ThetaLAB)) + ThetaLAB;
  
}

double Theta_CM_FR(double ThetaLAB, bool sol2 = false) {
  
  double tau = 1.0/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));
  
  if(std::sin(ThetaLAB) > 1.0/tau) {

    ThetaLAB = std::asin(1.0/tau);
    if(ThetaLAB < 0)
      ThetaLAB += TMath::Pi();

    return std::asin(tau*std::sin(ThetaLAB)) + ThetaLAB;
  }

  if(sol2)
    return -std::asin(tau*std::sin(-ThetaLAB)) - ThetaLAB;
  
  return TMath::Pi() - (std::asin(tau*std::sin(ThetaLAB)) + ThetaLAB);
  
}

double Theta_LAB(double thetaCM) {

  double tau = (beam_mass/targ_mass)/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));
  double tanTheta = std::sin(thetaCM)/(std::cos(thetaCM) + tau);

  if(tanTheta > 0)
    return std::atan(tanTheta);
  
  return std::atan(tanTheta) + TMath::Pi();

}

double Theta_LAB_Max() {

  double tau = (beam_mass/targ_mass)/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));
  if(tau < 1.0)
    return TMath::Pi();
  
  return std::asin(1.0/tau);
  
}

double Recoil_Theta_LAB(double thetaCM) {

  double tau = 1.0/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));
  double tanTheta = std::sin(TMath::Pi() - thetaCM)/(std::cos(TMath::Pi() - thetaCM) + tau);
  
  return std::atan(tanTheta);
  
}

double Recoil_Theta_LAB_Max() {

  double tau = 1.0/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));
  return std::asin(1.0/tau);
    
}

double KE_LAB(double thetaCM) {

  double tau = (beam_mass/targ_mass)/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));

  double term1 = std::pow(targ_mass/(beam_mass + targ_mass),2);
  double term2 = 1 + tau*tau + 2*tau*std::cos(thetaCM);
  double term3 = Ep - Ex*(1 + beam_mass/targ_mass);
  
  return term1*term2*term3;
}

double Recoil_KE_LAB(double thetaCM) {

  double tau = 1.0/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));

  double term1 = beam_mass*targ_mass/std::pow(beam_mass + targ_mass,2);
  double term2 = 1 + tau*tau + 2*tau*std::cos(TMath::Pi() - thetaCM);
  double term3 = Ep - Ex*(1 + beam_mass/targ_mass);
  
  return term1*term2*term3;
}

double RuthCM(double thetaCM) {

  double factor = 1.29596*TMath::Power(beam_Z*targ_Z,2.0);

  double Ecm = Ep/(1.0 + (beam_mass/targ_mass));
  double Esym2 = TMath::Power(Ecm,1.5)*TMath::Sqrt(Ecm-Ex);
  
  return factor/(Esym2*TMath::Power(TMath::Sin(thetaCM/2.0),4));
}

double RuthLAB(double thetaLAB, bool sol2=false) {

  double thetaCM = Theta_CM_FP(thetaLAB,sol2);
 
  double num = TMath::Power(TMath::Sin(thetaCM)/TMath::Sin(thetaLAB),2.0);
  double denom = TMath::Abs(TMath::Cos(thetaCM-thetaLAB));
  double jacobian = num/denom;

  return RuthCM(thetaCM)*jacobian;
  
}

double Beta_LAB(double thetaCM) {

  double energy = KE_LAB(thetaCM);
  double gam = energy/beam_mass + 1.0;

  return TMath::Sqrt(1.0 - 1.0/(gam*gam));
  
}

double Recoil_Beta_LAB(double thetaCM) {

  double energy = Recoil_KE_LAB(thetaCM);
  double gam = energy/targ_mass + 1.0;

  return TMath::Sqrt(1.0 - 1.0/(gam*gam));
  
}

double Esafe(int beam_A, int targ_A) {

  double Rmin = 1.25*(TMath::Power(double(beam_A),1./3.) + TMath::Power(double(targ_A),1./3.)) + 5.0;
  double fac = 1.439965*beam_Z*targ_Z*(beam_A + targ_A)/double(targ_A);

  return fac/Rmin;
}

double Esafe(double thetaCM, int beam_A, int targ_A) {

  double Rmin = 1.25*(TMath::Power(double(beam_A),1./3.) + TMath::Power(double(targ_A),1./3.)) + 5.0;
  double fac = 0.5*1.439965*beam_Z*targ_Z*(beam_A + targ_A)/double(targ_A);
  
  return (fac/Rmin)*(1 + 1.0/TMath::Sin(thetaCM/2.0));
}

double ECB(int beam_A, int targ_A) {

  double Rmin = 1.25*(TMath::Power(double(beam_A),1./3.) + TMath::Power(double(targ_A),1./3.));
  double fac = 1.439965*beam_Z*targ_Z*(beam_A + targ_A)/double(targ_A);
  
  return fac/Rmin;
}

double MinSep(int beam_A, int targ_A) {

  double Rsum = 1.25*(TMath::Power(double(beam_A),1./3.) + TMath::Power(double(targ_A),1./3.));
  double fac = 1.439965*beam_Z*targ_Z*(beam_A + targ_A)/double(targ_A);

  return fac/Ep - Rsum;
  
}

double ImpactParameter(double thetaCM) {

  double mu = 1.0/(1.0/beam_mass + 1.0/targ_mass);

  double gam = Ep/beam_mass + 1.0;
  double beta = TMath::Sqrt(1.0 - 1.0/(gam*gam));
  
  double a = 1.439965*beam_Z*targ_Z/(mu*beta*beta);

  return a/(gam*TMath::Tan(thetaCM/2.0));
  
}

////////Graphs////////
void KE_v_CM_Angle() {

  TGraph* gp = new TGraph(nBins);
  TGraph* gr = new TGraph(nBins);

  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    
    gp->SetPoint(i,theta*TMath::RadToDeg(),KE_LAB(theta));
    gr->SetPoint(i,theta*TMath::RadToDeg(),Recoil_KE_LAB(theta));
    
  }
  
  gp->SetTitle("");
  gp->GetYaxis()->SetTitleSize(0.06);
  gp->GetYaxis()->SetTitleOffset(0.6);
  gp->GetYaxis()->SetTitle("LAB Energy (MeV)");

  gp->GetXaxis()->SetTitleSize(0.05);
  gp->GetXaxis()->SetTitleOffset(0.8);
  gp->GetXaxis()->SetTitle("#theta_{CM} (deg)");

  gp->SetMarkerColor(kBlue);
  gp->SetLineColor(kBlue);
  gp->SetLineWidth(lineWidth);
  gp->SetMarkerSize(markerSize);
  gp->SetMarkerStyle(markerStyle);

  gr->SetMarkerColor(kRed);
  gr->SetLineColor(kRed);
  gr->SetLineWidth(lineWidth);
  gr->SetMarkerSize(markerSize);
  gr->SetMarkerStyle(markerStyle);

  TLegend* leg = new TLegend(0.8,0.8,0.9,0.9);
  leg->AddEntry(gp,"Projectile","PL");
  leg->AddEntry(gr,"Recoil","PL");

  new TCanvas();
  gp->Draw();
  gr->Draw("samePL");
  gp->GetYaxis()->SetRangeUser(0,-1111);
  gp->GetXaxis()->SetRangeUser(0,180);
  leg->Draw();

  return;
  
}

void KE_v_LAB_Angle() {

  TGraph* g = new TGraph(2);
  g->SetPoint(0,0,1.1*Ep);
  g->SetPoint(1,180,0);
  
  TGraph* gp = new TGraph(nBins);
  TGraph* gr = new TGraph(nBins);

  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    
    gp->SetPoint(i,Theta_LAB(theta)*TMath::RadToDeg(),KE_LAB(theta));
    gr->SetPoint(i,Recoil_Theta_LAB(theta)*TMath::RadToDeg(),Recoil_KE_LAB(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("LAB Energy (MeV)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("#theta_{LAB} (deg)");

  gp->SetMarkerColor(kBlue);
  gp->SetLineColor(kBlue);
  gp->SetLineWidth(lineWidth);
  gp->SetMarkerSize(markerSize);
  gp->SetMarkerStyle(markerStyle);

  gr->SetMarkerColor(kRed);
  gr->SetLineColor(kRed);
  gr->SetLineWidth(lineWidth);
  gr->SetMarkerSize(markerSize);
  gr->SetMarkerStyle(markerStyle);

  TLegend* leg = new TLegend(0.8,0.8,0.9,0.9);
  leg->AddEntry(gp,"Projectile","PL");
  leg->AddEntry(gr,"Recoil","PL");

  new TCanvas();
  g->Draw("sameAP");

  double t1 = Theta_LAB_Max();
  double t2 = Recoil_Theta_LAB_Max();

  if(t1 > t2) {
    g->GetXaxis()->SetRangeUser(0,t1*TMath::RadToDeg());
  }
  else {
    g->GetXaxis()->SetRangeUser(0,t2*TMath::RadToDeg());
  }
  
  gp->Draw("samePL");
  gr->Draw("samePL");
  leg->Draw();

  return;
  
}

void LAB_Angle_v_CM_Angle() {

  TGraph* gp = new TGraph(nBins);
  TGraph* gr = new TGraph(nBins);

  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    
    gp->SetPoint(i,theta*TMath::RadToDeg(),Theta_LAB(theta)*TMath::RadToDeg());
    gr->SetPoint(i,theta*TMath::RadToDeg(),Recoil_Theta_LAB(theta)*TMath::RadToDeg());
    
  }

  gp->SetTitle("");
  gp->GetYaxis()->SetTitleSize(0.06);
  gp->GetYaxis()->SetTitleOffset(0.6);
  gp->GetYaxis()->SetTitle("#theta_{LAB} (deg)");

  gp->GetXaxis()->SetTitleSize(0.05);
  gp->GetXaxis()->SetTitleOffset(0.8);
  gp->GetXaxis()->SetTitle("#theta_{CM} (deg)");

  gp->SetMarkerColor(kBlue);
  gp->SetLineColor(kBlue);
  gp->SetLineWidth(lineWidth);
  gp->SetMarkerSize(markerSize);
  gp->SetMarkerStyle(markerStyle);

  gr->SetMarkerColor(kRed);
  gr->SetLineColor(kRed);
  gr->SetLineWidth(lineWidth);
  gr->SetMarkerSize(markerSize);
  gr->SetMarkerStyle(markerStyle);

  TLegend* leg = new TLegend(0.8,0.8,0.9,0.9);
  leg->AddEntry(gp,"Projectile","PL");
  leg->AddEntry(gr,"Recoil","PL");

  new TCanvas();
  gp->Draw();
  gr->Draw("samePL");
  gp->GetXaxis()->SetRangeUser(0,180);

  double t1 = Theta_LAB_Max();
  double t2 = Recoil_Theta_LAB_Max();

  if(t1 > t2) {
    gp->GetYaxis()->SetRangeUser(0,t1*TMath::RadToDeg());
  }
  else {
    gp->GetYaxis()->SetRangeUser(0,t2*TMath::RadToDeg());
  }

  leg->Draw();

  return;
  
}

void CM_Angle_v_LAB_Angle() {

  TGraph* g = new TGraph(2);
  g->SetPoint(0,0,180);
  g->SetPoint(1,180,0);
  
  TGraph* gp = new TGraph(nBins);
  TGraph* gr = new TGraph(nBins);

  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    
    gp->SetPoint(i,Theta_LAB(theta)*TMath::RadToDeg(),theta*TMath::RadToDeg());
    gr->SetPoint(i,Recoil_Theta_LAB(theta)*TMath::RadToDeg(),theta*TMath::RadToDeg());
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("#theta_{CM} (deg)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("#theta_{LAB} (deg)");

  gp->SetMarkerColor(kBlue);
  gp->SetLineColor(kBlue);
  gp->SetLineWidth(lineWidth);
  gp->SetMarkerSize(markerSize);
  gp->SetMarkerStyle(markerStyle);

  gr->SetMarkerColor(kRed);
  gr->SetLineColor(kRed);
  gr->SetLineWidth(lineWidth);
  gr->SetMarkerSize(markerSize);
  gr->SetMarkerStyle(markerStyle);

  TLegend* leg = new TLegend(0.8,0.8,0.9,0.9);
  leg->AddEntry(gp,"Projectile","PL");
  leg->AddEntry(gr,"Recoil","PL");

  new TCanvas();
  g->Draw("sameAP");
  g->GetYaxis()->SetRangeUser(0,180);

  double t1 = Theta_LAB_Max();
  double t2 = Recoil_Theta_LAB_Max();

  if(t1 > t2) {
    g->GetXaxis()->SetRangeUser(0,t1*TMath::RadToDeg());
  }
  else {
    g->GetXaxis()->SetRangeUser(0,t2*TMath::RadToDeg());
  }

  gp->Draw("samePL");
  gr->Draw("samePL");
  leg->Draw();

  return;
  
}

void LAB_Angles() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,Recoil_Theta_LAB(theta)*TMath::RadToDeg(),Theta_LAB(theta)*TMath::RadToDeg());
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Projectile #theta_{LAB} (deg)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("Recoil #theta_{LAB} (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();
  g->GetYaxis()->SetRangeUser(0,Theta_LAB_Max()*TMath::RadToDeg());
  g->GetXaxis()->SetRangeUser(0,Recoil_Theta_LAB_Max()*TMath::RadToDeg());

  return;
  
}

void RuthCM_v_CM_Angle() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,theta*TMath::RadToDeg(),RuthCM(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Rutherford DCS (mb/sr)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("#theta_{CM} (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  TCanvas* c = new TCanvas();
  c->SetLogy(1);
  g->Draw();
  g->GetXaxis()->SetRangeUser(0,180);

  return;
  
}

void RuthLAB_v_LAB_Angle() {

  double turning_point = Theta_CM_FP(Theta_LAB_Max(),false);
  
  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    bool sol2 = false;
    if(theta > turning_point)
      sol2 = true;
    
    double thetaL = Theta_LAB(theta);
    g->SetPoint(i,thetaL*TMath::RadToDeg(),RuthLAB(thetaL,sol2));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Rutherford DCS (mb/sr)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("Projectile #theta_{LAB} (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  TCanvas* c = new TCanvas();
  c->SetLogy(1);
  g->Draw();
  g->GetXaxis()->SetRangeUser(0,Theta_LAB_Max()*TMath::RadToDeg());

  return;
  
}

TGraph* pKE_v_CMAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,theta*TMath::RadToDeg(),KE_LAB(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Projectile Energy (MeV)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("#theta_{CM} (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();
  g->GetXaxis()->SetRangeUser(0,180);

  return g;
  
}

TGraph* pAng_v_CMAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,theta*TMath::RadToDeg(),Theta_LAB(theta)*TMath::RadToDeg());
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Projectile Angle (deg)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("#theta_{CM} (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();
  g->GetXaxis()->SetRangeUser(0,180);

  return g;
  
}

TGraph* rKE_v_CMAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,theta*TMath::RadToDeg(),Recoil_KE_LAB(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Recoil Energy (MeV)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("#theta_{CM} (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();
  g->GetXaxis()->SetRangeUser(0,180);

  return g;
  
}

TGraph* rAng_v_CMAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,theta*TMath::RadToDeg(),Recoil_Theta_LAB(theta)*TMath::RadToDeg());
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Recoil Angle (deg)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("#theta_{CM} (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();
  g->GetXaxis()->SetRangeUser(0,180);

  return g;
  
}

TGraph* pKE_v_pAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,Theta_LAB(theta)*TMath::RadToDeg(),KE_LAB(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Projectile Energy (MeV)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("Projectile #theta_{LAB} (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();

  return g;
  
}

TGraph* pKE_v_rAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,Recoil_Theta_LAB(theta)*TMath::RadToDeg(),KE_LAB(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Projectile Energy (MeV)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("Recoil #theta_{LAB} (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();

  return g;
  
}

TGraph* rKE_v_rAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,Recoil_Theta_LAB(theta)*TMath::RadToDeg(),Recoil_KE_LAB(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Recoil Energy (MeV)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("Recoil #theta_{LAB} (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();

  return g;
  
}

TGraph* rKE_v_pAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,Theta_LAB(theta)*TMath::RadToDeg(),Recoil_KE_LAB(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Recoil Energy (MeV)");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("Projectile #theta_{LAB} (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();

  return g;
  
}

TGraph* pBeta_v_pAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,Theta_LAB(theta)*TMath::RadToDeg(),Beta_LAB(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Projectile Beta");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("Projectile Angle (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();

  return g;
  
}

TGraph* pBeta_v_rAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,Recoil_Theta_LAB(theta)*TMath::RadToDeg(),Beta_LAB(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Projectile Beta");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("Recoil Angle (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();

  return g;
  
}

TGraph* rBeta_v_rAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,Recoil_Theta_LAB(theta)*TMath::RadToDeg(),Recoil_Beta_LAB(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Recoil Beta");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("Recoil Angle (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();

  return g;
  
}

TGraph* rBeta_v_pAng() {

  TGraph* g = new TGraph(nBins);
  for(int i=0;i<nBins;i++) {
    
    double theta = (i+1)*TMath::Pi()/(double)nBins;
    g->SetPoint(i,Theta_LAB(theta)*TMath::RadToDeg(),Recoil_Beta_LAB(theta));
    
  }

  g->SetTitle("");
  g->GetYaxis()->SetTitleSize(0.06);
  g->GetYaxis()->SetTitleOffset(0.6);
  g->GetYaxis()->SetTitle("Recoil Beta");

  g->GetXaxis()->SetTitleSize(0.05);
  g->GetXaxis()->SetTitleOffset(0.8);
  g->GetXaxis()->SetTitle("Projectile Angle (deg)");

  g->SetLineWidth(lineWidth);
  g->SetMarkerSize(markerSize);
  g->SetMarkerStyle(markerStyle);

  new TCanvas();
  g->Draw();

  return g;
  
}

void PrintSystem() {

  std::cout << "\nProjectile Z: " << beam_Z << ", Projectile Mass: " << beam_mass << " MeV/c^2"
	      << "\nRecoil Z: " << targ_Z << ", Recoil Mass: " << targ_mass << " MeV/c^2"
	      << "\nEp = " << Ep << " MeV, Ex = " << Ex << " Mev\n"
	      << std::endl;

  return;
}

void Print(double thetaCM, bool print_system = false) {

  if(print_system)
    PrintSystem();
  else
    std::cout << "\n";

  double thetaL = Theta_LAB(thetaCM);
  double r2d = TMath::RadToDeg();
  
  bool sol2 = false;
  std::string pSol2 = "false";
  if(thetaCM > Theta_CM_FP(Theta_LAB_Max())) {
    sol2 = true;
    pSol2 = "true";
  }

  std::string rSol2 = "false";
  if(thetaCM < Theta_CM_FR(Recoil_Theta_LAB_Max())) {
    rSol2 = "true";
  }
  
  std::cout << "Theta_CM = " << thetaCM*r2d << " deg, Ruth_CM = " << RuthCM(thetaCM)
	    << " mb/sr \n\nProjectile:\nTheta_LAB = " << thetaL*r2d << " deg, Ruth_LAB = "
	    << RuthLAB(thetaL,sol2) << " mb/sr (Projectile Sol2 = " << pSol2 << ")\nKE_LAB = "
	    << KE_LAB(thetaCM) << " MeV, Beta_LAB = " << Beta_LAB(thetaCM)
	    << "\n\nRecoil:\nRecoil_Theta_LAB = " << Recoil_Theta_LAB(thetaCM)*r2d
	    << " deg (Recoil Sol2 = " << rSol2 << ")\nRecoil_KE_LAB = " << Recoil_KE_LAB(thetaCM)
	    << " MeV, Recoil_Beta_LAB = " << Recoil_Beta_LAB(thetaCM) << "\n"
	    << std::endl;

  return;
  
}

void DrawLinesLAB(bool include_recon = false, double y1 = 0, double y2 = 500) {

  TLine* line = new TLine();
  line->SetLineStyle(9);
  line->SetLineWidth(5);
  line->SetLineColor(kBlue);

  //z1 = 3.0 cm
  double x1 = 20.1;
  double x2 = 49.4;
  line->DrawLine(x1,y1,x1,y2);
  line->DrawLine(x2,y1,x2,y2);

  //z0 = 3.0 cm
  double x3 = 131.0;
  double x4 = 159.0;
  line->DrawLine(x3,y1,x3,y2);
  line->DrawLine(x4,y1,x4,y2);

  if(include_recon) {
    line->SetLineColor(kRed);
    double x5 = Theta_LAB(Theta_CM_FR(x1*TMath::DegToRad(),false))*TMath::RadToDeg();
    double x6 = Theta_LAB(Theta_CM_FR(x2*TMath::DegToRad(),false))*TMath::RadToDeg();
    line->DrawLine(x5,y1,x5,y2);
    line->DrawLine(x6,y1,x6,y2);
  }

  return;
  
}

void DrawLinesCM(bool include_recon = false, double y1 = 0, double y2 = 500) {

  TLine* line = new TLine();
  line->SetLineStyle(9);
  line->SetLineWidth(5);
  line->SetLineColor(kBlue);
  
  //z1 = 3.0 cm
  double x1 = Theta_CM_FP(20.1*TMath::DegToRad(),false)*TMath::RadToDeg();
  double x2 = Theta_CM_FP(49.4*TMath::DegToRad(),false)*TMath::RadToDeg();
  line->DrawLine(x1,y1,x1,y2);
  line->DrawLine(x2,y1,x2,y2);

  //z0 = 3.0 cm
  double x3 = Theta_CM_FP(131.0*TMath::DegToRad(),false)*TMath::RadToDeg();
  double x4 = Theta_CM_FP(159.0*TMath::DegToRad(),false)*TMath::RadToDeg();
  line->DrawLine(x3,y1,x3,y2);
  line->DrawLine(x4,y1,x4,y2);

  if(include_recon) {
    line->SetLineColor(kRed);
    double x5 = Theta_CM_FR(20.1*TMath::DegToRad(),false)*TMath::RadToDeg();
    double x6 = Theta_CM_FR(49.4*TMath::DegToRad(),false)*TMath::RadToDeg();
    line->DrawLine(x5,y1,x5,y2);
    line->DrawLine(x6,y1,x6,y2);
  }
  
}
