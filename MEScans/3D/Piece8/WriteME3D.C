void WriteME3D() {

  int ix = 4;
  int iy = 3;
  int iz = 2;
  
  std::ifstream litfile;
  litfile.open("kr80.bst.bak",std::ios::in);

  std::ofstream mefile;
  mefile.open("kr80.bst",std::ios::out);

  std::ifstream list;
  list.open("list.txt",std::ios::in);

  std::string line;
  std::getline(list,line);
  list.close();
   
  std::stringstream ss(line);
  double x,y,z;
  ss >> x;
  ss >> y;
  ss >> z;
  
  int line_num = 1;
  while(std::getline(litfile,line)) {

    if(line_num == ix) {
      mefile << x << "\n";
    }
    else if(line_num == iy) {
      mefile << y << "\n";
    }
    else if(line_num == iz) {
      mefile << z << "\n";
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
