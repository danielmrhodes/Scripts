void WriteME() {

  const int index = 4; //Change this accordingly
  
  std::ifstream litfile;
  litfile.open("ge80.bst.bak",std::ios::in);

  std::ofstream mefile;
  mefile.open("ge80.bst",std::ios::out);

  std::ifstream list;
  list.open("list.txt",std::ios::in);

  std::string line;
  std::getline(list,line);
  list.close();
   
  std::stringstream ss(line);
  double me;
  ss >> me;
  
  int line_num = 1;
  while(std::getline(litfile,line)) {

    if(line_num == index) {
      mefile << me << "\n";
    }
    else {
      mefile << line << "\n";
    }

    line_num++;
  }

  mefile.close();
  litfile.close();
  
  return;
}
