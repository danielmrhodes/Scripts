/////
//These are only for the first 2+ moment and transition to ground state
double Q2P(double dme) {

  return (sqrt(16.0*TMath::Pi()/5.0)*sqrt(2.0/7.0)/sqrt(5.0))*dme;
  
}

double DME(double q2p) {

  return q2p/(sqrt(16.0*TMath::Pi()/5.0)*sqrt(2.0/7.0)/sqrt(5.0));
  
}

double BE2(double tme, bool up) {
  
  if(up)
    return tme*tme;
  
  return tme*tme/5.0;
  
}

double TME(double be2, bool up) {
  
  if(up)
    return sqrt(be2);
 
  return sqrt(5.0*be2);
  
}
/////

double BE1(double t12, double egam) {

  return 0.435/(TMath::Power(egam,3.0)*t12*1000.0*100.);
  
}

double BE2(double t12, double egam) {

  return 564.0/(TMath::Power(egam,5.0)*t12*10000.0);
  
}

double BE3(double t12, double egam) {

  return 1212.0/(TMath::Power(egam,7.0)*t12*1000000.0/1000.0);
  
}

double BM1(double t12, double egam) {
  return 39.4/(TMath::Power(egam,3.0)*t12*1000.0);
}

double Q4P(double dme) {

  return (sqrt(16.0*TMath::Pi()/5.0)*2.0*sqrt(7.0/55.0)/sqrt(9.0))*dme;
  
}

double DME_4P(double q4p) {

  return q4p/(sqrt(16.0*TMath::Pi()/5.0)*2.0*sqrt(7.0/55.0)/sqrt(9.0));
  
}

double Q(double dme, double spin, int mult) {

  int isp2 = int(2.0*spin + 0.001);
  double w3j = ROOT::Math::wigner_3j(isp2,2*mult,isp2,-isp2,0,isp2);

  return TMath::Sqrt(16.0*TMath::Pi()/5.0)*w3j*dme;
  
}

double DME(double Q, double spin, int mult) {

  int isp2 = int(2.0*spin + 0.001);
  double w3j = ROOT::Math::wigner_3j(isp2,2*mult,isp2,-isp2,0,isp2);
  double denom = TMath::Sqrt(16.0*TMath::Pi()/5.0)*w3j;
  
  return Q/denom;

}

double TME(double bsl, double spin) {

  return TMath::Sqrt((2.0*spin + 1.0)*bsl);
  
}

double BSL(double tme, double spin) {
  
  return tme*tme/(2.0*spin + 1.0);

}

double tauE2(double tme, double egam, double spin) {

  return 816./(10000.0*BSL(tme,spin)*TMath::Power(egam,5.0));
  
}

double q(double tme, double dme) {

  double fac = 64.0*TMath::Pi()/49.0;
  return Q2P(dme)/TMath::Sqrt(fac*BE2(tme,false));
  
}

double q(double gamma_deg) {

  double d2r = TMath::DegToRad();
  
  double t1 = 3.0*TMath::Cos(3.0*gamma_deg*d2r);
  double t2 = 9.0 - 8.0*TMath::Sin(3.0*gamma_deg*d2r)*TMath::Sin(3.0*gamma_deg*d2r);

  return t1/TMath::Sqrt(t2);
  
}

double c3dLO(double tme, double dme) {

  double fac = -1.0/((2.0/7.0)*TMath::Sqrt(16.0*TMath::Pi()/5.0));
  double ratio = Q2P(dme)/TMath::Sqrt(BE2(tme,true));
  
  return fac*ratio;
}

void q_err(double tme, double dtme, double dme, double ddme) {

  double be2 = tme*tme/5.0;
  double db2 = 2.0*(dtme/tme)*be2;
  double q2p = Q2P(dme);
  double dqp = (ddme/dme)*q2p;

  double fac = 64.0*TMath::Pi()/49.0;
  double val = q2p/TMath::Sqrt(fac*be2);
  double err = TMath::Sqrt(TMath::Power(dqp/TMath::Sqrt(fac*be2),2) +
			     TMath::Power(0.5*q2p*TMath::Power(fac*be2,-3.0/2.0)*fac*db2,2));

  std::cout << "q = " << val << " +- " << err << std::endl;
  
}

double r(double tme, double dme) {

  double fac = 64.0*TMath::Pi()/49.0;
  return fac*BE2(tme,false)/(TMath::Power(Q2P(dme),2.0));
  
}
