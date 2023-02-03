void MakeList3D() {

  const double x_ll = 0.52;
  const double x_ul = 0.80;
  const int x_st = 5;
  const int np = 8;

  const double y_ll = -0.57;
  const double y_ul = -0.23;
  const int y_st = 69;
  
  const double z_ll = 0.055;
  const double z_ul = 0.085;
  const int z_st = 41;

  double dx = (x_ul-x_ll)/(double)(x_st*np);
  double dy = (y_ul-y_ll)/(double)(y_st-1);
  double dz = (z_ul-z_ll)/(double)(z_st-1);

  int num = x_st*y_st*z_st;
  int num1 = num + y_st*z_st;
  
  std::cout << "\nTotal Scan: " << np-1 << " " << x_st << "x" << y_st << "x" << z_st
	    << " cubes (" << num << " points), 1 " << x_st + 1 << "x" << y_st << "x" << z_st
	    << " cube (" << num1 << " points)\n"
	    << "X axis: [" << x_ll << "," << x_ul << "] in " << x_st*np + 1 << " steps (dx = "
	    << dx << ")\n"
	    << "Y axis: [" << y_ll << "," << y_ul << "] in " << y_st << " steps (dy = "
	    << dy << ")\n"
	    << "Z axis: [" << z_ll << "," << z_ul << "] in " << z_st << " steps (dz = "
	    << dz << ")\n"
	    << std::endl;
    
  std::ofstream file;
  for(int k=0;k<np;k++) {

    int xn = x_st;
    if(k==np-1) {
      xn += 1;
    }
    
    file.open(Form("list%i.txt",k+1),std::ios::out);
    for(int i=0;i<xn;i++) {
      for(int j=0;j<y_st;j++) {
	for(int l=0;l<z_st;l++) {
	  file << x_ll + (i + k*x_st)*dx << " " << y_ll + j*dy << " " << z_ll + l*dz << "\n";
	}
      }
    }
    file.close();
    
  }
  
  return;
}
