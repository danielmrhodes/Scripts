//Calculation of the solid angle subtended by each ring of JANUS, formula taken from
//Pomme, Applied Radiation and Isotopes 65 (2007) 724-727

TGraph* SA0c;
TGraph* SA0o;
TGraph* SA1c;
TGraph* SA1o;

double sX = 0.051;
double sY = 0.104;

double DS_Offset = 28.0 - 0.150;
double US_Offset = 32.0 - 0.150;

void FillOffset() {

  TF1 f("Function","TMath::BesselJ1([0]*x)*TMath::BesselJ0([1]*x)*TMath::Exp(-[2]*x)");
  ROOT::Math::WrappedTF1 wf(f);

  ROOT::Math::GSLIntegrator ig(ROOT::Math::IntegrationOneDim::kADAPTIVE);
  ig.SetFunction(wf);
  ig.SetRelTolerance(0.00001);

  //Lengths are in millimeters
  double r0 = 11.0;
  double dr = 1.0; 

  double off = TMath::Sqrt(sX*sX + sY*sY);
  f.SetParameter(1,off);  
  f.SetParameter(2,US_Offset);

  //Solid angles are in steradians
  double prev_SA = 0.0;
  double tot_SA = 0.0;
  
  
  for(int i=0;i<25;i++) {

    f.SetParameter(0,r0 + i*dr);
    
    double tmp = 2.0*TMath::Pi()*f.GetParameter(0)*ig.IntegralUp(0);

    if(i) {

      double sa = tmp-prev_SA;
      
      SA0o->SetPoint(i-1,i,sa);
      tot_SA += sa;
      
      
    }

    prev_SA = tmp;
    
  }
  
  prev_SA = 0.0;
  tot_SA = 0.0;

  //Downstream Detector Offset
  f.SetParameter(2,DS_Offset);
  //std::cout << "\nDownstream Detector" << std::endl;
  for(int i=0;i<25;i++) {

    f.SetParameter(0,r0 + i*dr);
    
    double tmp = 2*TMath::Pi()*f.GetParameter(0)*ig.IntegralUp(0);

    if(i>0) {
      
      SA1o->SetPoint(i-1,i,tmp-prev_SA);
      tot_SA += tmp-prev_SA;
      
      //std::cout << "  Ring" << i << ": " << tmp-prev_SA << " sr" << std::endl;
    }
    
    prev_SA = tmp;
  }
}

void FillCentered() {

  TF1 f("Function","TMath::BesselJ1([0]*x)*TMath::BesselJ0([1]*x)*TMath::Exp(-[2]*x)");
  ROOT::Math::WrappedTF1 wf(f);

  ROOT::Math::GSLIntegrator ig(ROOT::Math::IntegrationOneDim::kADAPTIVE);
  ig.SetFunction(wf);
  ig.SetRelTolerance(0.00001);

  //Lengths are in millimeters
  double r0 = 11.0;
  double dr = 1.0; 
 
  //Solid angles are in steradians
  double prev_SA = 0.0;
  double tot_SA = 0.0;

  f.SetParameter(1,0);
  f.SetParameter(2,US_Offset);
  for(int i=0;i<25;i++) {

    f.SetParameter(0,r0 + i*dr);
    
    double tmp = 2.0*TMath::Pi()*f.GetParameter(0)*ig.IntegralUp(0);

    if(i) {

      double sa = tmp-prev_SA;
      
      SA0c->SetPoint(i-1,i,sa);
      tot_SA += sa;
      
    }

    prev_SA = tmp;
    
  }
  
  prev_SA = 0.0;
  tot_SA = 0.0;
  
  f.SetParameter(2,DS_Offset);
  for(int i=0;i<25;i++) {

    f.SetParameter(0,r0 + i*dr);
    
    double tmp = 2*TMath::Pi()*f.GetParameter(0)*ig.IntegralUp(0);

    if(i>0) {
      
      SA1c->SetPoint(i-1,i,tmp-prev_SA);
      tot_SA += tmp-prev_SA;
      
    }
    
    prev_SA = tmp;
  } 
  
  return;

}

void FillGraphs() {

  if(!SA0c) {
    SA0c = new TGraph(24);
    SA1c = new TGraph(24);
    SA0o = new TGraph(24);
    SA1o = new TGraph(24);
  }
  
  FillCentered();
  FillOffset();

  return;
}

void Compare(bool offset = true) {

  FillGraphs();
  
  TGraphErrors* g1 = new TGraphErrors(24);
  g1->SetMarkerStyle(20);
  g1->SetMarkerSize(2);
  g1->SetMarkerColor(kRed);
  g1->SetLineColor(kRed);
  g1->SetLineWidth(2);

  TGraph* z1 = new TGraph(24);
  z1->SetTitle("");
  z1->SetMarkerStyle(20);
  z1->SetMarkerSize(2);
  z1->SetLineWidth(2);
  
  TFile* f;
  if(offset) {
    f = new TFile("/user/rhodesd/pecan-2015/JANUS_SIM_Tests/Simulation_Runs/80Ge/scattering_iso.root");
  }
  else {
    //f = new TFile("/user/rhodesd/pecan-2015/JANUS_SIM/centered.root");
    return;
  }

  double Nsim = 50000000.0;
  //double Nsim = 10000000.0;
  GH2* h2 = (GH2*)f->Get("Bambino2/SecPID_Det1_proj");
  GH1* h = (GH1*)h2->ProjectionX();
  for(int i=0;i<24;i++) {
    
    double cnts = h->GetBinContent(i+1);
    
    double sa = 4.0*TMath::Pi()*cnts/Nsim;
    double er = 4.0*TMath::Pi()*TMath::Sqrt(cnts)/Nsim;
    double z;
    if(offset) {
      z = (sa - SA1o->GetY()[i])/er;
    }
    else {
      z = (sa - SA1c->GetY()[i])/er;
    }
    
    g1->SetPoint(i,i+1,sa);
    g1->SetPointError(i,0,er);

    z1->SetPoint(i,i+1,z);
  }
  z1->GetXaxis()->SetTitle("Ring");
  z1->GetXaxis()->SetTitleSize(0.06);
  z1->GetXaxis()->SetTitleOffset(0.65);
  z1->GetYaxis()->SetTitle("Z-Score");
  z1->GetYaxis()->SetTitleSize(0.06);
  z1->GetYaxis()->SetTitleOffset(0.65);

  new TCanvas();
  if(offset) {
    SA1o->Draw("APL");
  }
  else {
    SA1c->Draw("APL");
  }
  g1->Draw("samePLE");

  TLegend* leg = new TLegend(0.68,0.7,0.9,0.9);
  leg->AddEntry(SA1c,"Calculation","P");
  leg->AddEntry(g1,"Simulation","PE");
  leg->Draw();

  new TCanvas();
  z1->Draw();

  z1->GetYaxis()->SetRangeUser(-3,3);
  z1->GetXaxis()->SetRangeUser(0,25);

  TLine* l = new TLine();
  
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->DrawLine(0,0,25,0);

  l->SetLineStyle(9);
  l->DrawLine(0,1,25,1);
  l->DrawLine(0,-1,25,-1);
  
}

void Solid_Angles() {

  FillGraphs();

  SA1c->SetTitle("Solid Angle of a Janus Ring");
  SA1c->GetXaxis()->SetTitle("Ring");
  SA1c->GetXaxis()->SetTitleSize(0.06);
  SA1c->GetXaxis()->SetTitleOffset(0.65);
  SA1c->GetYaxis()->SetTitle("Solid Angle (sr)");
  SA1c->GetYaxis()->SetTitleSize(0.06);
  SA1c->GetYaxis()->SetTitleOffset(0.65);
  SA1c->SetMarkerStyle(20);
  SA1c->SetMarkerSize(2);
  SA1c->SetLineWidth(2);

  SA1o->SetTitle("Solid Angle of a Janus Ring (XY offsets)");
  SA1o->GetXaxis()->SetTitle("Ring");
  SA1o->GetXaxis()->SetTitleSize(0.06);
  SA1o->GetXaxis()->SetTitleOffset(0.65);
  SA1o->GetYaxis()->SetTitle("Solid Angle (sr)");
  SA1o->GetYaxis()->SetTitleSize(0.06);
  SA1o->GetYaxis()->SetTitleOffset(0.65);
  SA1o->SetMarkerStyle(20);
  SA1o->SetMarkerSize(2);
  SA1o->SetLineWidth(2);

  SA0c->SetMarkerStyle(20);
  SA0c->SetMarkerSize(2);
  SA0c->SetMarkerColor(kBlue);
  SA0c->SetLineColor(kBlue);
  SA0c->SetLineWidth(2);

  SA0o->SetMarkerStyle(20);
  SA0o->SetMarkerSize(2);
  SA0o->SetMarkerColor(kBlue);
  SA0o->SetLineColor(kBlue);
  SA0o->SetLineWidth(2);

  TLegend* leg = new TLegend(0.68,0.7,0.9,0.9);
  leg->AddEntry(SA1c,"Downstream","P");
  leg->AddEntry(SA0c,"Upstream","P");
  
  new TCanvas();
  SA1c->Draw("APL");
  SA0c->Draw("samePL");
  leg->Draw();
  
  SA1c->GetYaxis()->SetRangeUser(0.045,0.1);
  SA1c->GetXaxis()->SetRangeUser(0,25);
  
  new TCanvas();
  SA1o->Draw("APL");
  SA0o->Draw("samePL");
  leg->Draw();

  SA1o->GetYaxis()->SetRangeUser(0.045,0.1);
  SA1o->GetXaxis()->SetRangeUser(0,25);
  
  return;
}

void PrintSA(int Ri = 1, int Rf = 24) {

  FillGraphs();

  double tot_SAo = 0.0;
  double tot_SAc = 0.0;
  
  std::cout << "\nSource offset: " << TMath::Sqrt(sX*sX + sY*sY) << " mm\n";
  
  std::cout << "Upstream Detector (z = " << US_Offset << " mm)\n\t Centered\tOffset\n";
  for(int i=Ri-1;i<Rf;i++) {

    double sac = SA0c->GetY()[i];
    double sao = SA0o->GetY()[i];

    tot_SAc += sac;
    tot_SAo += sao;
    
    std::cout << "  Ring" << i+1 << ": " << sac << " sr, " << sao << " sr" << std::endl;
    
  }
  std::cout << "Total Upstream Solid Angle:\n  " << tot_SAc << " sr (" << 100*tot_SAc/(4*TMath::Pi())
  	    << "% of 4 Pi) Centered\n  " << tot_SAo << " sr (" << 100*tot_SAo/(4*TMath::Pi())
  	    << "% of 4 Pi) Offset\n\n";

  tot_SAo = 0.0;
  tot_SAc = 0.0;
  
  std::cout << "Downstream Detector (z = " << DS_Offset << " mm)\n\t Centered\tOffset\n";
  for(int i=Ri-1;i<Rf;i++) {

    double sac = SA1c->GetY()[i];
    double sao = SA1o->GetY()[i];

    tot_SAc += sac;
    tot_SAo += sao;
    
    std::cout << "  Ring" << i+1 << ": " << sac << " sr, " << sao << " sr" << std::endl;
    
  }
  std::cout << "Total Downstream Solid Angle:\n  " << tot_SAc << " sr (" << 100*tot_SAc/(4*TMath::Pi())
  	    << "% of 4 Pi) Centered\n  " << tot_SAo << " sr (" << 100*tot_SAo/(4*TMath::Pi())
  	    << "% of 4 Pi) Offset\n\n";
  
  return;
}
