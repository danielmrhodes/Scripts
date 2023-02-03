double ReadChiSqFromFile() {

  ifstream g2out;
  g2out.open("ge80.out",std::ios::in);	
  if( !g2out.is_open() ) {	
    std::cout << "Unable to open ge80.out" << std::endl;
    return 999;
  }
  
  bool flag = false;
  string line, tmp;
  string qry = "     *** CHISQ=";
  stringstream gosia_chisq (stringstream::in | stringstream::out);
  double chisq = 999;
 
  while (std::getline(g2out,line)) {
    
    if(!line.compare(0,qry.size(),qry)) {
      gosia_chisq.str("");
      gosia_chisq.clear();
      gosia_chisq << line.substr(qry.size(),qry.size()+15);
      gosia_chisq >> chisq;
      flag = true;
    }
    
  }
	
  g2out.close();
	
  if(!flag) {
    std::cout << "Couldn't find chisq value in ge80.out" << std::endl;
  }
	
  return chisq;

}

void WriteOutput() {
  
  std::ifstream list;
  list.open("list.txt",std::ios::in);
  
  std::ofstream tmp_list;
  tmp_list.open("tmp_list.txt",std::ios::out);

  std::string line;
  std::getline(list,line);
  
  while(std::getline(list,line)) {
    tmp_list << line << "\n";
  }
  list.close();
  tmp_list.close();

  std::remove("list.txt");
  std::rename("tmp_list.txt","list.txt");

  std::ofstream outfile;
  outfile.open("output.txt",std::ios::app);

  std::ifstream mefile;
  mefile.open("ge80.bst",std::ios::in);

  double chi2 = ReadChiSqFromFile();
  
  outfile << chi2;
  while(std::getline(mefile,line)) {
    outfile << " " << line;
  }
  outfile << "\n";
  outfile.close();

  return;
  
}

void WriteOutput_old() {

  std::ofstream outfile;
  outfile.open("output.txt",std::ios::app);
  
  std::ifstream list;
  list.open("list.txt",std::ios::in);

  std::string line;
  std::getline(list,line);

  std::stringstream ss(line);
  double me;
  ss >> me;

  double chi2 = ReadChiSqFromFile();

  outfile << me << " " << " " << chi2 << "\n";
  outfile.close();
  
  std::ofstream tmp_list;
  tmp_list.open("tmp_list.txt",std::ios::out);

  while(std::getline(list,line)) {
    tmp_list << line << "\n";
  }
  list.close();
  tmp_list.close();

  std::remove("list.txt");
  std::rename("tmp_list.txt","list.txt");

  return;
  
}
