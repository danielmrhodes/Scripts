void SplinePoints(double min, double max, int npnts = 20) {

  double st = (max - min)/double(npnts - 5);
  
  std::cout << "\n";
  for(int i=0;i<npnts;i++) {

    std::cout << min + st*(i-2) << " ";
  
  }
  std::cout << "\n";

  return;
}
