struct TensorComp {

  int fK;
  int fKappa;  
  std::vector<double> fVal;
  
};

struct State {

  int fStateIndex;
  int fMaxK;
  
  std::vector<TensorComp> fTensor;

  bool HasK(int k) {

    for(int i=0;i<fTensor.size();i++) {
      if(fTensor.at(i).fK == k) {
	return true;
      }
    }

    return false;
  }

  bool HasKKappa(int k, int kappa) {

    for(int i=0;i<fTensor.size();i++) {
      if(fTensor.at(i).fK == k && fTensor.at(i).fKappa == kappa) {
	return true;
      }
    }

    return false;
  }

  int Index(int k, int kappa) {

    for(int i=0;i<fTensor.size();i++) {
      if(fTensor.at(i).fK == k && fTensor.at(i).fKappa == kappa) {
	return i;
      }
    }

    return -1;
    
  }
  
};

struct Data {

  std::vector<double> fThetas;
  std::vector<State> fStates;
  
};

Data NewReadFile(std::string input) {

  std::ifstream file;
  file.open(input.c_str(),std::ios::in);

  std::vector<State> states;
  std::vector<double> thetas;

  std::string line, word;
  while(std::getline(file,line)) {

    std::stringstream ls(line);
    ls >> word;
    ls >> word;
    ls >> word;

    double theta;
    std::stringstream ss(word);
    ss >> theta;

    thetas.push_back(theta);
    //std::cout << "Theta: " << theta << std::endl;
    
    std::getline(file,line);
    std::getline(file,line);
    while(std::getline(file,line)) {

      if(line.empty()) {
	break;
      }
	
      std::stringstream ls1(line);

      ls1 >> word; //index
      unsigned int index;
      std::stringstream ss1(word);
      ss1 >> index;
	  
      if(states.size() < index) {
	    
	states.emplace_back();
	states.back().fStateIndex = (int)index;
	states.back().fMaxK = 0;
	   
	states.back().fTensor.emplace_back();
	states.back().fTensor.back().fK = 0;
	states.back().fTensor.back().fKappa = 0;
	   
      }

      ls1 >> word; //k
      int k;
      std::stringstream ss2(word);
      ss2 >> k;

      if(!(states.at(index-1).HasK(k))) { //new k value

	states.at(index-1).fMaxK = k;
    
	states.at(index-1).fTensor.emplace_back();
	states.at(index-1).fTensor.back().fK = k;
	states.at(index-1).fTensor.back().fKappa = 0;
	
      }

      ls1 >> word; //kappa
      int kappa;
      std::stringstream ss3(word);
      ss3 >> kappa;

      if(!(states.at(index-1).HasKKappa(k,kappa))) { //new kappa value (for current k)
    
	states.at(index-1).fTensor.emplace_back();
	states.at(index-1).fTensor.back().fK = k;
	states.at(index-1).fTensor.back().fKappa = kappa;
	
      }

      ls1 >> word; //value
      double val;
      std::stringstream ss4(word);
      ss4 >> val;

      int iindex = states.at(index-1).Index(k,kappa);
      states.at(index-1).fTensor.at(iindex).fVal.push_back(val);

      //std::cout << " " << index << "\t" << k << "\t" << kappa << "\t" << val << std::endl;
	
    } //End second while(getline()) (1 ThetaCM Block)

    //std::cout << std::endl;
    
  } //End first while(getline()) (All ThetaCM Blocks)

  Data info;
  info.fThetas = thetas;
  info.fStates = states;
  
  return info;
}

void new_tensor_reader(int state_index = 1, bool norm = false, std::string fn = "kr80_on_pt196.ten") {

  //106Cd
  std::map<int,std::string> myMap = {{0,"0_{1}^{+}"},{1,"2_{1}^{+}"},{2,"4_{1}^{+}"},{3,"2_{2}^{+}"},
  				     {4,"0_{2}^{+}"},{5,"4_{2}^{+}"},{6,"3_{1}^{-}"},{7,"6_{1}^{+}"},
  				     {8,"5_{1}^{-}"},{9,"8_{1}^{+}"},{10,"2_{3}^{+}"},{11,"6_{2}^{+}"}};

  //48Ti
  //std::map<int,std::string> myMap = {{0,"0_{1}^{+}"},{1,"2_{1}^{+}"},{2,"4_{1}^{+}"},{3,"2_{2}^{+}"},
  //				     {4,"6_{1}^{+}"},{5,"4_{2}^{+}"}};

  //80Ge
  //std::map<int,std::string> myMap = {{0,"0_{1}^{+}"},{1,"2_{1}^{+}"},{2,"2_{2}^{+}"},{3,"4_{1}^{+}"},
  //				     {4,"4_{2}^{+}"},{5,"6_{1}^{+}"}};

  //80Kr
  //std::map<int,std::string> myMap = {{0,"0_{1}^{+}"},{1,"2_{1}^{+}"},{2,"2_{2}^{+}"},{3,"0_{1}^{+}"},
  //				     {4,"4_{1}^{+}"},{5,"3_{1}^{+}"},{6,"4_{2}^{+}"},{7,"6_{1}^{+}"},
  //				     {8,"8_{1}^{+}"}};

  //196Pt
  //std::map<int,std::string> myMap = {{0,"0_{1}^{+}"},{1,"2_{1}^{+}"},{2,"2_{2}^{+}"},{3,"4_{1}^{+}"},
  //				     {4,"4_{2}^{+}"},{5,"6_{1}^{+}"},{6,"6_{2}^{+}"},{7,"8_{1}^{+}"}};

  Data info = NewReadFile(fn);

  std::vector<double> thetas = info.fThetas;
  State state = info.fStates.at(state_index-1);

  if(state_index > info.fStates.size()) {
    std::cout << "State Index is too large" << std::endl;
    return;
   }
  
  int np = thetas.size();
  int ng = state.fTensor.size();

  //std::vector<double> G_k = {1.0,0.0,0.80,0.0,0.58,0.0,0.40};
  //std::vector<double> G_k  = {1.0,0.0,0.50,0.0,0.26,0.0,0.14};
  std::vector<double> G_k  = {1.0,1.0,1.0,1.0,1.0,1.0,1.0};
    
  TLegend* leg = new TLegend(0.9,0.1,0.95,0.9);
  std::vector<TGraph*> graphs;
  for(int i=0;i<ng;i++) {
    
    TGraph* g = new TGraph(np);
    g->SetMarkerStyle(20);
    g->SetMarkerSize(2);
    g->SetLineWidth(2);

    if(i<8) {
      g->SetMarkerColor(i+2);
      g->SetLineColor(i+2);
    }
    else {
      g->SetMarkerColor(i+22);
      g->SetLineColor(i+22);
    }
    
    int k = info.fStates.at(state_index-1).fTensor.at(i).fK;
    int kappa = info.fStates.at(state_index-1).fTensor.at(i).fKappa;
    
    leg->AddEntry(g,Form("#rho_{%d%d}",k,kappa),"PL");
    graphs.push_back(g);

  }

  double min = 100.;
  double max = -100.;

  for(int i=0;i<ng;i++) {
    for(int j=0;j<np;j++) {

      double val = state.fTensor.at(i).fVal.at(j)*G_k.at(state.fTensor.at(i).fK);
      if(norm) {
	val /= state.fTensor.at(0).fVal.at(j);
      }
      
      graphs.at(i)->SetPoint(j,thetas.at(j),val);

      if(val < min) {
	min = val;
      }
      if(val > max) {
	max = val;
      }
      
    }
  }

  graphs.at(0)->GetYaxis()->SetTitleSize(0.06);
  graphs.at(0)->GetYaxis()->SetTitleOffset(0.6);
  graphs.at(0)->GetYaxis()->SetTitle("Value");

  graphs.at(0)->GetXaxis()->SetTitleSize(0.05);
  graphs.at(0)->GetXaxis()->SetTitleOffset(0.8);
  graphs.at(0)->GetXaxis()->SetTitle("#theta_{CM} (rad)");

  //std::string nuc = "^{48}Ti ";
  std::string nuc = "^{106}Cd ";
  //std::string nuc = "^{80}Ge ";
  //std::string nuc = "^{196}Pt ";
  std::string title =  nuc + myMap[state_index];
  if(norm) {
    title += " Polarization Tensor";
  }
  else {
    title += " Statistical Tensor";
  }
  graphs.at(0)->SetTitle(title.c_str());
  //graphs.at(0)->SetTitle("");
  graphs.at(0)->Draw("APL");
  graphs.at(0)->GetYaxis()->SetRangeUser(1.1*min,1.1*max);
  graphs.at(0)->GetXaxis()->SetRangeUser(0,3.14);
  
  for(int i=1;i<graphs.size();i++) {
    graphs.at(i)->Draw("samePL");
  }
  leg->Draw();
  
  return;
}
