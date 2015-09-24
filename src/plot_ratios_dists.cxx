#include <iostream>
#include <vector>

#include "TString.h"

#include "utilities_macros_rohan.cpp"

using namespace std;

int main(){ 

  TString ntuple_date = "2015_09_14";
  
  //inc tt
  double ratio1[8] = {0.3, 0.3, 0.3, 0.3, 0.2, 0.6, 0.55, 1.1};
  double ratio3[12] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.5, 0.6, 1.2, 0.25, 0.3, 0.55};
  
  //1l tt
  bool do_1ltt(false);
  //  double ratio1[8] = {0.1, 0.1, 0.1, 0.1, 0.5, 1, 1, 1.3};
  //  double ratio3[12] = {0.15, 0.15, 0.14, 0.14, 0.14, 0.08, 1.5, 1.5, 3, 1, 1.5, 2};
  
  //2l tt
  bool do_2ltt(false);
  //  double ratio1[8] = {2, 2, 2, 2, 0.25, 0.6, 0.55, 1.1};
  //  double ratio3[12] = {1.2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1};
  
  TString tag = "";
  if(do_1ltt) tag = "1ltt";
  if(do_2ltt) tag = "2ltt";

  TString folder="/cms5r0/rohan/archive/"+ntuple_date+"/skim/";
  TString folder_1l="/cms5r0/rohan/archive/"+ntuple_date+"/skim_1l/";
  TString folder_2l="/cms5r0/rohan/archive/"+ntuple_date+"/skim_2l/";
  TString folder_genht="/cms5r0/rohan/archive/"+ntuple_date+"/skim_genht/";

  vector<TString> s_tt; 
  if(do_1ltt) {
    s_tt.push_back(folder+"*_TTJets*SingleLept*");  
    s_tt.push_back(folder_1l+"*_TTJets*HT*");
  }
  else if(do_2ltt){
    s_tt.push_back(folder+"*_TTJets*DiLept*");  
    s_tt.push_back(folder_2l+"*_TTJets*HT*");
  }
  else{
    s_tt.push_back(folder+"*_TTJets*HT*");
    s_tt.push_back(folder_genht+"*_TTJets*Lept*");
  }

  // Reading ntuples
  vector<sfeats> Samples, Samples_m3;
  Samples.push_back(sfeats(s_tt, "t#bar{t}, mt > 140", 1,0, "mt>140")); Samples.back().isData = true; 
  Samples.push_back(sfeats(s_tt, "t#bar{t}, mt #leq 140", 31,1,"mt<=140"));

  Samples.push_back(sfeats(s_tt, "t#bar{t}, mj > 600", 1,0, "mj>600")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt, "t#bar{t}, mj #leq 600", 31,1,"mj<=600"));

  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mt > 140", 1,0, "mt>140&&mj>400&&njets<=8")); Samples_m3.back().isData = true; 
  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mt > 140", 1,0, "mt>140&&mj>400&&njets>=9")); Samples_m3.back().isData = true; 
  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mt #leq 140", 31,1,"mt<=140&&mj>400&&njets<=8"));  
  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mt #leq 140", 31,1,"mt<=140&&mj>400&&njets>=9"));  
  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mt > 140", 1,0, "mt>140&&mj<=400")); Samples_m3.back().isData = true; 
  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mt #leq 140", 31,1,"mt<=140&&mj<=400"));  

  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mj > 400", 1,0, "mj>400&&mt>140&&njets<=8")); Samples_m3.back().isData = true; 
  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mj > 400", 1,0, "mj>400&&mt>140&&njets>=9")); Samples_m3.back().isData = true; 
  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mj #leq 400", 31,1, "mj<=400&&mt>140"));
  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mj > 400", 1,0, "mj>400&&mt<=140&&njets<=8")); Samples_m3.back().isData = true;  
  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mj > 400", 1,0, "mj>400&&mt<=140&&njets>=9")); Samples_m3.back().isData = true;  
  Samples_m3.push_back(sfeats(s_tt, "t#bar{t}, mj #leq 400", 31,1,"mj<=400&&mt<=140"));

  vector<int> mt140, mj600, mt140_m3_1, mt140_m3_2, mj400_m3_1, mj400_m3_2;
  mt140.push_back(0);
  mt140.push_back(1);

  mj600.push_back(2);
  mj600.push_back(3);

  mt140_m3_1.push_back(0);
  mt140_m3_1.push_back(2);
  mt140_m3_1.push_back(4);
  mt140_m3_1.push_back(5);

  mt140_m3_2.push_back(1);
  mt140_m3_2.push_back(3);
  mt140_m3_2.push_back(4);
  mt140_m3_2.push_back(5);

  mj400_m3_1.push_back(6);
  mj400_m3_1.push_back(8);
  mj400_m3_1.push_back(9);
  mj400_m3_1.push_back(11);

  mj400_m3_2.push_back(7);
  mj400_m3_2.push_back(8);
  mj400_m3_2.push_back(10);
  mj400_m3_2.push_back(11);

  TString noCuts = "1";
  TString baseline = "ht>500&&met>200&&njets>=7&&nbm>=2&&nleps==1";
  TString baselineNb1 = "ht>500&&met>200&&nbm>=1&&njets>=7&&nleps==1";

  vector<hfeats> vars, vars3;

  // METHOD 1
  // RMT
  vars.push_back(hfeats("mj",36,0,1800, mt140, "MJ",baseline+"&&njets<=8&&met<=400",600,tag)); vars.back().maxRatio = ratio1[0];
  vars.push_back(hfeats("mj",36,0,1800, mt140, "MJ",baseline+"&&njets<=8&&met>400",600,tag));  vars.back().maxRatio = ratio1[1];
  vars.push_back(hfeats("mj",36,0,1800, mt140, "MJ",baseline+"&&njets>=9&&met<=400",600,tag));  vars.back().maxRatio = ratio1[2];
  vars.push_back(hfeats("mj",36,0,1800, mt140, "MJ",baseline+"&&njets>=9&&met>400",600,tag));  vars.back().maxRatio = ratio1[3];

  // RMJ
  vars.push_back(hfeats("mt",33,0,660, mj600, "mT",baseline+"&&met<=400&&njets<=8",140,tag)); vars.back().maxRatio = ratio1[4];
  vars.push_back(hfeats("mt",33,0,660, mj600, "mT",baseline+"&&met>400&&njets<=8",140,tag)); vars.back().maxRatio = ratio1[5];
  vars.push_back(hfeats("mt",33,0,660, mj600, "mT",baseline+"&&met<=400&&njets>=9",140,tag)); vars.back().maxRatio = ratio1[6];
  vars.push_back(hfeats("mt",33,0,660, mj600, "mT",baseline+"&&met>400&&njets>=9",140,tag)); vars.back().maxRatio = ratio1[7];
  
  //  plot_distributions(Samples, vars, "10", ".pdf", "RA4","",true);

  // METHOD 3 **TURN ON ROHANHACK IN UTILITIES_MACRO
  // RMT
  vars3.push_back(hfeats("mj",36,0,1800, mt140_m3_1, "MJ",baseline+"&&met<=400&&nbm==2",400,tag)); vars3.back().maxRatio = ratio3[0];
  vars3.push_back(hfeats("mj",36,0,1800, mt140_m3_1, "MJ",baseline+"&&met<=400&&nbm>=3",400,tag)); vars3.back().maxRatio = ratio3[1];
  vars3.push_back(hfeats("mj",36,0,1800, mt140_m3_1, "MJ",baseline+"&&met>400",400,tag)); vars3.back().maxRatio = ratio3[2];

  vars3.push_back(hfeats("mj",36,0,1800, mt140_m3_2, "MJ",baseline+"&&met<=400&&nbm==2&&1",400,tag)); vars3.back().maxRatio = ratio3[3];
  vars3.push_back(hfeats("mj",36,0,1800, mt140_m3_2, "MJ",baseline+"&&met<=400&&nbm>=3&&1",400,tag)); vars3.back().maxRatio = ratio3[4];
  vars3.push_back(hfeats("mj",36,0,1800, mt140_m3_2, "MJ",baseline+"&&met>400&&1",400,tag)); vars3.back().maxRatio = ratio3[5];
  
  // RMJ
  vars3.push_back(hfeats("mt",33,0,660, mj400_m3_1, "mT",baseline+"&&met<=400&&nbm==2",140,tag)); vars3.back().maxRatio = ratio3[6];
  vars3.push_back(hfeats("mt",33,0,660, mj400_m3_1, "mT",baseline+"&&met<=400&&nbm>=3",140,tag)); vars3.back().maxRatio = ratio3[7];
  vars3.push_back(hfeats("mt",33,0,660, mj400_m3_1, "mT",baseline+"&&met>400",140,tag)); vars3.back().maxRatio = ratio3[8];

  vars3.push_back(hfeats("mt",33,0,660, mj400_m3_2, "mT",baseline+"&&met<=400&&nbm==2&&1",140,tag)); vars3.back().maxRatio = ratio3[9];
  vars3.push_back(hfeats("mt",33,0,660, mj400_m3_2, "mT",baseline+"&&met<=400&&nbm>=3&&1",140,tag)); vars3.back().maxRatio = ratio3[10];
  vars3.push_back(hfeats("mt",33,0,660, mj400_m3_2, "mT",baseline+"&&met>400&&1",140,tag)); vars3.back().maxRatio = ratio3[11];

  plot_distributions(Samples_m3, vars3, "10", ".pdf", "RA4","",true);
}
