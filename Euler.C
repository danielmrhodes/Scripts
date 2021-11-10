#include "new_tensor_reader.C"

typedef std::vector< std::vector<std::complex<double>> > POLAR;

double WignerLittleD(int twoJ, int twoM, int twoN, double cosTheta) {

  if(twoM < -twoJ || twoM > twoJ || twoN < -twoJ || twoN > twoJ ||
     ((twoM % 2) != (twoJ % 2)) || ((twoN % 2) != (twoJ % 2))) {

    return 0;
  }

  if(cosTheta == 1.0) {
    return double(twoM == twoN);
  }

  int kMin = 0;
  if(twoM > twoN) {
    kMin = (twoM-twoN)/2;
  }

  int kMax = (twoJ + twoM)/2;
  if((twoJ-twoN)/2 < kMax) {
    kMax = (twoJ-twoN)/2;
  }
  
  double lnCosHalfTheta = TMath::Log((cosTheta+1.)*0.5)*0.5;
  double lnSinHalfTheta = TMath::Log((1.-cosTheta)*0.5)*0.5;

  double d = 0;
  for(int k=kMin;k<=kMax;k++) {
    
    double logSum = 0.5*(TMath::Log(TMath::Factorial((twoJ+twoM)/2)) +
			 TMath::Log(TMath::Factorial((twoJ-twoM)/2)) +
			 TMath::Log(TMath::Factorial((twoJ+twoN)/2)) +
			 TMath::Log(TMath::Factorial((twoJ-twoN)/2)));
    
    logSum += -TMath::Log(TMath::Factorial((twoJ+twoM)/2 - k)) -
               TMath::Log(TMath::Factorial((twoJ-twoN)/2 - k)) -
               TMath::Log(TMath::Factorial(k)) - 
               TMath::Log(TMath::Factorial(k+(twoN-twoM)/2));
    
    logSum += (twoJ+(twoM-twoN)/2 - 2*k)*lnCosHalfTheta + 
              (2*k + (twoN-twoM)/2)*lnSinHalfTheta;
    
    double sign = (k % 2) ? -1 : 1;
    d += sign * TMath::Exp(logSum);
    
  }
  
  return d;
}

std::complex<double> WignerBigD(int twoJ, int twoM, int twoN, double cosTheta, double alpha, double gamma) {


  std::complex<double> ima(0.0,0.5*twoM*alpha);
  std::complex<double> ing(0.0,0.5*twoN*gamma);
  std::complex<double> d(WignerLittleD(twoJ,twoM,twoN,cosTheta),0.0);
  
  return std::exp(ima)*d*std::exp(ing);
  
}

void PrintP(const POLAR polar) {

  std::cout << " P = [ {";
  size_t kk = polar.size();
  for(size_t k=0; k<kk; ++k) {
    if(k>0) { std::cout << "       {"; }
    size_t kpmax = (polar[k]).size();
    for(size_t kappa=0; kappa<kpmax; ++kappa) {
      if(kappa > 0) { std::cout << "}  {"; }
      std::cout << polar[k][kappa].real() << " + " 
	  << polar[k][kappa].imag() << "*i";
    }
    if(k+1 < kk) { std::cout << "}" << std::endl; }
  }
  std::cout << "}\n     ]" << std::endl;

  return;
}


POLAR RotatedP(const POLAR polar,double alpha, double beta, double gamma) {

  POLAR rot_p;
  double cosTheta = TMath::Cos(beta);

  const std::complex<double> ni(0.0,-1.0);
  
  const int kk = polar.size();
  for(int k=0;k<=kk;k++) {

    rot_p.emplace_back();
    
    //std::cout << "k: " << k << std::endl;
    if(k%2) {
      continue;
    }   
    
    for(int kp=0;kp<=k;kp++) {

      //std::cout << " kp: " << kp << std::endl;
      std::complex<double> val(0.0,0.0);
      for(int kpp=0;kpp<=k;kpp++) {
	
	//if(!kpp) {
	//std::cout << "  kpp: ";
	//}

	std::complex<double> tmp_val = polar[k][kpp]*WignerBigD(2*k,2*kpp,2*kp,cosTheta,alpha,gamma);
	if(kpp%2) {
	  tmp_val *= ni; //I don't know why this is needed
	}
	//std::cout << " " << kpp << " " << tmp_val;

	val += tmp_val;
	//val += polar[k][kpp]*WignerBigD(2*k,2*kpp,2*kp,cosTheta,alpha,gamma);
	if(kpp) {
	  
	  int ph = TMath::Power(-1,k);
	  std::complex<double> phase(ph,0.0);

	  std::complex<double> tmp_val1 = phase*polar[k][kpp]*WignerBigD(2*k,-2*kpp,2*kp,cosTheta,alpha,gamma);

	  if(kpp%2) {
	    tmp_val1 *= ni; //I don't know why this is needed
	  }
	  //std::cout << " " << -kpp << " " << tmp_val1;
	  
	  val += tmp_val1;
	  //val += phase*polar[k][kpp]*WignerBigD(2*k,-2*kpp,2*kp,cosTheta,alpha,gamma);
	}
	
      }
      // std::cout << std::endl;
      rot_p[k].push_back(val);
    }
  }

  return rot_p;
  
}

POLAR RotatedP_1(const POLAR polar, double beta) {

  POLAR rot_p;
  double cosTheta = TMath::Cos(beta);
  
  const int kk = polar.size();
  for(int k=0;k<=kk;k++) {
    
    rot_p.emplace_back();

    //std::cout << "k: " << k << std::endl;
    if(k%2) {
      continue;
    }
    
    for(int kp=0;kp<=k;kp++) {

      //std::cout << " kp: " << kp << std::endl;
      std::complex<double> val(0.0,0.0);
      for(int kpp=0;kpp<=k;kpp++) {
	
	int ph = TMath::Power(-1,kp + kpp/2);
	std::complex<double> phase(ph,0.0);
	
	double d = WignerLittleD(2*k,2*kpp,2*kp,cosTheta);
	std::complex<double> wld(d,0.0);

	std::complex<double> tmp_val = polar[k][kpp]*phase*wld;
	//if(!kpp) {
	//std::cout << "  kpp: ";
	//}
	//std::cout << " " << kpp << " " << tmp_val;

	val += tmp_val;
	//val += polar[k][kpp]*phase*wld;
	if(kpp) {
	  
	  int ph1 = TMath::Power(-1,kp + (kpp+1)/2);
	  std::complex<double> phase1(ph1,0.0);
	  
	  double d1 = WignerLittleD(2*k,-2*kpp,2*kp,cosTheta);
	  std::complex<double> wld1(d1,0.0);

	  std::complex<double> tmp_val1 = polar[k][kpp]*phase1*wld1;
	  
	  //std::cout << " " << -kpp << " " << tmp_val1;
	  
	  val += tmp_val1;
	  //val += polar[k][kpp]*phase1*wld1;
	}	
	
      }
      //std::cout << std::endl;
      rot_p[k].push_back(val);
    }
  }

  return rot_p;
  
}

void Euler(int index = 0) {

  Data infoB = NewReadFile("tensorsB.ten");
  State stB = infoB.fStates.at(index);

  POLAR polB;
  for(int k=0;k<=stB.fMaxK;k++) {
    polB.emplace_back();

    if(k%2) {
      continue;
    }
    
    for(int kp=0;kp<=k;kp++) {
      
      TensorComp comp = stB.fTensor.at(stB.Index(k,kp));
      std::complex<double> val(comp.fVal.at(0),0.0);
      
      polB[k].push_back(val);
    }
  }

  Data infoC = NewReadFile("tensorsC.ten");
  State stC = infoC.fStates.at(index);

  POLAR polC;
  for(int k=0;k<=stC.fMaxK;k++) {
    polC.emplace_back();

    if(k%2) {
      continue;
    }
    
    for(int kp=0;kp<=k;kp++) {
      
      TensorComp comp = stC.fTensor.at(stC.Index(k,kp));
      std::complex<double> val(comp.fVal.at(0),0.0);
      
      polC[k].push_back(val);
    }
  }

  
  std::cout << "Excitation Frame:" << std::endl;
  PrintP(polB);
  std::cout << std::endl;

  std::cout << "LAB Frame:" << std::endl;
  PrintP(polC);
  std::cout << std::endl;
  
  
  double thetaCM = 65.0*TMath::DegToRad();
  double alpha = TMath::Pi()/2.0;
  double beta = (TMath::Pi() + thetaCM)/2.0;
  double gam = TMath::Pi();

  POLAR test = RotatedP(polB,alpha,beta,gam);
  //POLAR test1 = RotatedP_1(polB,beta);

  std::cout << "LAB Frame:" << std::endl;
  PrintP(test);
  std::cout << std::endl;
  
  return;

}
