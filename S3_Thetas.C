void S3_Thetas(double zoff = 2.6) {

  for(int det=0;det<2;det++) {
    std::cout << "Det " << det << " (z = " << zoff << " cm)" << std::endl;
    for(int ring=1;ring<25;ring++) {

      const double PI = TMath::Pi();

      double outer_radius = 3.5;
      double inner_radius = 1.1;

      double rad_slope = (outer_radius - inner_radius)/24.0;
      double rad_offset = inner_radius;

      TVector3 position1(1.,1.,1.);  // Not (0,0,0), because otherwise SetPerp and SetPhi fail.
      position1.SetPerp((ring-1)*rad_slope + rad_offset);
      position1.SetPhi(1.5*PI);
      position1.SetZ(zoff);
      
      TVector3 position2(1.0,1.0,1.0);
      position2.SetPerp(ring*rad_slope + rad_offset);
      position2.SetPhi(1.5*PI);
      position2.SetZ(zoff);

      double tmin = position1.Theta()*TMath::RadToDeg();
      double tmax = position2.Theta()*TMath::RadToDeg();
      if(det==0) {
        position1.RotateY(PI);
        position2.RotateY(PI);

	tmin = position2.Theta()*TMath::RadToDeg();
	tmax = position1.Theta()*TMath::RadToDeg();
      }

      std::cout << "Ring " << ring << ": Theta_min = " << tmin << ", Theta_max = " << tmax
		<< ", Theta_ave = " << (tmax+tmin)/2.0 << ", Diff = " << tmax-tmin << std::endl;
      
    }

    std::cout << "\n" << std::endl;
  }

  return;

}
