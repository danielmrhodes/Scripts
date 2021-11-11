double WSJ(int two_ja, int two_jb, int two_jc, int two_jd, int two_je, int two_jf) {
  return ROOT::Math::wigner_6j(two_ja,two_jb,two_jc,two_jd,two_je,two_jf);
}

double Q2() {

  double m12 = 0.652; //2
  double m14 = 0.169; //3
  
  double Is = 0.0;
  int Isi2 = int(2.0*Is + 0.001);
  double wsj = WSJ(4,4,0,Isi2,Isi2,4);
  
  double fac = TMath::Power(-1.0,2.0*Is)*TMath::Sqrt(5.0/(2.0*Is + 1.0))*wsj;
  
  double t1 = m12*m12;
  double t2 = m14*m14;

  double q2 = fac*(t1 + t2);

  std::cout << "Q^2:\n Term 1: " << t1 << "\n Term 2: " << t2 << "\n<Q2>: " << q2 << std::endl;
  
  return q2;
}

void Centroids() {

  double m12 = 0.652; //2
  double m14 = 0.169; //3
  double m22 = -0.25; //4
  double m24 = 0.415; //6
  double m44 = 1.33; //12
  
  double Is = 0.0;
  int Isi2 = int(2.0*Is + 0.001);
  double wsj = WSJ(4,4,4,Isi2,4,4);
  
  double fac = -TMath::Sqrt(35.0/2.0)/(2.0*Is + 1.0)*wsj;
  
  double t1 = m12*m22*m12;
  double t2 = 2.0*m12*m24*m14;
  double t3 = m14*m44*m14;
  
  double q3c3d = fac*(t1 + t2 + t3);
  double c3d = q3c3d/TMath::Power(Q2(),3.0/2.0);
  
  std::cout << "\nQ^3 Cos(3D):\n Term 1: " << t1 << "\n Term 2: " << t2 << "\n Term 3: " << t3
  	    << "\n<Q^3 Cos(3D)>: " << q3c3d << "\n<Cos(3D)>: " << c3d << std::endl;
  
  return;
}

double Q2(double m12, double m14) {

  double Is = 0.0;
  int Isi2 = int(2.0*Is + 0.001);
  double wsj = WSJ(4,4,0,Isi2,Isi2,4);
  
  double fac = TMath::Power(-1.0,2.0*Is)*TMath::Sqrt(5.0/(2.0*Is + 1.0))*wsj;
  
  double t1 = m12*m12;
  double t2 = m14*m14;

  double Q2 = fac*(t1 + t2);
  
  return Q2;
}

double Q3COS3D(double m12, double m14, double m22, double m24, double m44) {

  double Is = 0.0;
  int Isi2 = int(2.0*Is + 0.001);
  double wsj = WSJ(4,4,4,Isi2,4,4);
  
  double fac = -TMath::Sqrt(35.0/2.0)/(2.0*Is + 1.0)*wsj;
  
  double t1 = m12*m22*m12;
  double t2 = 2.0*m12*m24*m14;
  double t3 = m14*m44*m14;
  
  double Q3C3D = fac*(t1 + t2 + t3);
  //double C3D = Q3C3D/TMath::Power(Q2(m12,m14),3.0/2.0);
  
  return Q3C3D;
}

void Uncertainties() {

  double m12 = 0.652;
  double e12 = 0.011;

  double m14 = 0.169;
  double e14 = 0.004;

  double m22 = -0.25;
  double e22 = 0.05;
  
  double m24 = 0.415;
  double e24 = 0.015;
  
  double m44 = 1.33;
  double e44 = 0.06;

  GH1* hq = new GH1D("Q2","Q^{2}",2000,0,1);
  GH1* h3 = new GH1D("Q3C3","Q^{3}cos3#delta",1000,-0.5,0.5);
  GH1* hc = new GH1D("C3","cos3#delta",1000,-0.5,0.5);
  
  for(int i=0;i<1000000;i++) {

    double t12 = gRandom->Gaus(m12,e12);
    double t14 = gRandom->Gaus(m14,e14);
    double t22 = gRandom->Gaus(m22,e22);
    double t24 = gRandom->Gaus(m24,e24);
    double t44 = gRandom->Gaus(m44,e44);

    double q2 = Q2(t12,t14);
    hq->Fill(q2);

    double q3c3d = Q3COS3D(t12,t14,t22,t24,t44);
    h3->Fill(q3c3d);

    double c3 = q3c3d/TMath::Power(q2,3.0/2.0);
    hc->Fill(c3);
    
  }

  new GCanvas();
  hq->Draw("hist");

  new GCanvas();
  h3->Draw("hist");
  
  new GCanvas();
  hc->Draw("hist");
  
  return;
}

void RotInv() {
  
  Centroids();
  Uncertainties();
  
  return;
}
