void MakeList() {

  double ll = 0.4;
  double ul = 1.1;
  int st = 21;
  
  std::ofstream file;
  file.open("list.txt",std::ios::out);

  double dm = (ul-ll)/(double)(st-1);
  for(int i=0;i<st;i++) {
    file << ll + i*dm << "\n";
  }
  file.close();
  
  return;
}
