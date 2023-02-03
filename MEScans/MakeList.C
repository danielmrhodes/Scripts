void MakeList() {

  double ll = -0.96;
  double ul = 1.0;
  int st = 81;
  
  std::ofstream file;
  file.open("list.txt",std::ios::out);

  double dm = (ul-ll)/(double)(st-1);
  for(int i=0;i<st;i++) {
    file << ll + i*dm << "\n";
  }
  file.close();
  
  return;
}
