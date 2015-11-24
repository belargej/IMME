// COMPILE : g++ `root-config --glibs --cflags` goIMME_ord3.cxx -o goIMMEOrd3
// NOTE -> Everything with a "Gade/gade" attached to it in here is what I used to make 
//         sure that the method is working.  I compared my results to those obtained in 
//         the paper:
//                      PRC <bold>76</bold>,024317 (2007)
//--------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TF1.h>

using namespace std;

int main(int argc, char** argv){

  if(argc!=3){ 
    cout << " *** Usage : goIMME <inputfile> <outputfile>" << endl; 
    cout << " *** Inputfile should have format : " << endl;
    cout << " ***\tTz\tME\tError" << endl;
    return 1;
  }

  map<float,float> MassExc;
  map<float,float> MassExc_Error;
  float Tz = -100;
  float ME = 0;
  float Er = 0;
  ifstream inFile;
  inFile.open(argv[1]);
  
  // Read everything in.
  while(!(inFile.eof())){
    inFile >> Tz >> ME >> Er;
    MassExc[Tz] = ME;
    MassExc_Error[Tz] = Er;
  }
  
  TFile *rootFile = new TFile(argv[2],"RECREATE");
  TGraphErrors *g = new TGraphErrors(MassExc.size());
  TGraphErrors *resid = new TGraphErrors(MassExc.size());
  TGraphErrors *residGade = new TGraphErrors(MassExc.size());
  g->SetName("IMME_graph1");
  resid->SetName("IMME_Resid_graph1");
  residGade->SetName("IMME_ResidGade_graph1");
  int counter1 = 0;
  float Low,High;

  // Set Mass excess vs Tz points:
  for(map<float,float>::iterator it = MassExc.begin();it != MassExc.end(); it++){
    g->SetPoint(counter1,it->first,it->second);
    if(counter1 == 0) Low = it->first;
    counter1++;
  }
  
  // Set errors for Mass excess vs Tz points:
  counter1 = 0;
  for(map<float,float>::iterator it = MassExc_Error.begin();it != MassExc_Error.end(); it++){
    g->SetPointError(counter1,0,it->second);
    counter1++;
  }
  
  High = sqrt(Low*Low)-1;
  Low = Low+1;
  TF1 *IMME_fit = new TF1("IMME_fit","[0]+[1]*x+[2]*x*x",Low,High);
  TF1 *Gade_fit = new TF1("Gade_fit","9692.5-3438.9*x+245.7*x*x",-2,2);
  g->Fit("IMME_fit","M");
  
  counter1 = 0;
  vector<float> resid_vec;
  vector<float> resid_gade;
  for(map<float,float>::iterator it = MassExc.begin();it != MassExc.end(); it++){
    float diff  = it->second - IMME_fit->Eval(it->first);
    float diff_gade = it->second - Gade_fit->Eval(it->first);
    //    cout << " diff " << counter1 << " : " << it->second << " - " << IMME_fit->Eval(it->first) << " = " << diff << endl;
    resid->SetPoint(counter1,it->first,diff);
    residGade->SetPoint(counter1,it->first,diff_gade);
    resid_vec.push_back(diff);
    resid_gade.push_back(diff_gade);
    counter1++;
  }

  counter1 = 0;
  vector<float> error_vec;
  for(map<float,float>::iterator it = MassExc_Error.begin();it != MassExc_Error.end(); it++){
    resid->SetPointError(counter1,0,it->second);
    residGade->SetPointError(counter1,0,it->second);
    error_vec.push_back(it->second);
    counter1++;
  }


  // Calc Chi2 by hand:
  float Chi2 = 0;
  float Chi2Gade = 0;
  for(int i = 0; i < counter1;i++){
    //    cout << " Current Chi2 : " << Chi2 << endl;
    //cout << "  Resid : " << resid_vec.at(i) << endl;
    //cout << "  Error : " << error_vec.at(i) << endl;
    Chi2+=pow(resid_vec.at(i),2.0)/pow(error_vec.at(i),2.0);
    Chi2Gade+=pow(resid_gade.at(i),2.0)/pow(error_vec.at(i),2.0);
  }

  cout << " Final Chi2 : " << Chi2 << endl;
  //cout << " Gade Chi2  : " << Chi2Gade << endl;
  
  //Gade_fit->Write();
  IMME_fit->Write();
  resid->Write();
  //residGade->Write();
  g->Write();
  rootFile->Close();

  inFile.close();
  return 0;
}
