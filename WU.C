void Convert(double val, int A, int L, bool elec = true) {

  const double PI = TMath::Pi();

  double factor;
  std::string units;
  std::string chara;
  int power;
  
  if(elec) {
    
    factor = (1.0/(4.0*PI))*TMath::Power(3.0/(3.0 + L),2)*TMath::Power(1.2*TMath::Power(A,1.0/3.0),2.0*L);
    units = "e^2";
    chara = "E";
    power = 2*L;
    
  }
  else {
    
    factor = (10.0/PI)*TMath::Power(3.0/(3.0 + L),2)*TMath::Power(1.2*TMath::Power(A,1.0/3.0),2.0*L - 2.0);
    units = "mu_N^2";
    chara = "M";
    power = 2*L - 2;
    
  }

  std::cout << "B(" << chara << L << ") = " << val << " W.u" << std::endl;
  std::cout << "B(" << chara << L << ") = " << factor*val << " " << units << " fm^" << power << std::endl;
  std::cout << "B(" << chara << L << ") = " << factor*val/(TMath::Power(100,power/2)) << " " << units << " b^"
	    << power/2 << std::endl;
  
  return;
}

void ME(double BSL, double spin) {

  std::cout << "ME = " << TMath::Sqrt(BSL*(2.0*spin + 1.0)) << std::endl;

  return;
  
}
