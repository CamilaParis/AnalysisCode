/*                             __
                           .--()°'.'
 Author: Nathalia Cardona '|, . ,'
                           !_-(_\
*/

#include "PhenoAnalyzer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <bits/stdc++.h>
#include "Analysis/LeptonCounter.h"
#include "Analysis/Cuts.h"

using namespace std;

int main(int argc, char *argv[])
{
  cout << "Starting phenoanalyzer..." << endl;

  // standardize print to 2 dp
  cout << fixed;
  cout << setprecision(2);

  // Importing Delphes data
  TChain chain("Delphes");
  chain.Add(argv[1]);
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);

  vector<int> ns = {15, 20, 30, 40, 50};

  // output file manager
  TFile *HistoOutputFile = new TFile(argv[2], "RECREATE");

  // directory to store the histograms
  TDirectory *nLeptonsDirectory = HistoOutputFile->mkdir("nLeptons");
  TDirectory *VBF_CutsDirectory = HistoOutputFile->mkdir("VBF_Cuts");
  TDirectory *CutsDirectory = HistoOutputFile->mkdir("Extra_Cuts");
  TDirectory *single_e = HistoOutputFile->mkdir("single_e");
  TDirectory *single_mu = HistoOutputFile->mkdir("single_mu");
  TDirectory *single_tau = HistoOutputFile->mkdir("single_tau");

  cout << "processing.." << endl;

  // get tree info
  vector<string> branches = {
      "Electron",
      "Muon",
      "Jet",
      "MissingET"};

  map<string, TClonesArray *> branchDict;

  // create a dictionary with the branches
  for (int i = 0; (unsigned)i < branches.size(); i++)
  {
    TClonesArray *branch = treeReader->UseBranch(branches[i].c_str());
    branchDict[branches[i]] = branch;
  }

  /*
    File Structure:
    - Test.root
      - nLeptons
        - 8 < pt < 50 
        - 8 < pt < 40
            .
            .
            .
  */

  // boolean mask to avoid over computation
  vector<bool> vbfCutsArr;
  vector<bool> cutsArr;

  for(int i = 0 ; (unsigned) i < treeReader->GetEntries(); i ++)
  {
    vbfCutsArr.push_back(false);
    cutsArr.push_back(false);
  }

  // open output file
  HistoOutputFile->cd();

  nLeptonsDirectory->cd();
  cout << "nLeptons" << endl;
  drawLeptonCount(treeReader, ns, branchDict, vbfCutsArr, cutsArr,noFilter);
  ptEtaPhiMjjMt(treeReader, branchDict, vbfCutsArr, cutsArr,noFilter);
  cout << "nLeptons done." << endl;

  CutsDirectory->cd();
  cout << "Extra_cuts" << endl;
  drawLeptonCount(treeReader, ns, branchDict, vbfCutsArr, cutsArr, cuts);
  ptEtaPhiMjjMt(treeReader, branchDict, vbfCutsArr, cutsArr, cuts);
  cout << "Extra cuts done." << endl;
  
  VBF_CutsDirectory->cd();
  cout << "VBF_Cuts" << endl;
  drawLeptonCount(treeReader, ns, branchDict, vbfCutsArr, cutsArr, vbfCut);
  ptEtaPhiMjjMt(treeReader, branchDict, vbfCutsArr, cutsArr, vbfCut);
  cout << "VBF_Cuts done." << endl;

  single_e->cd();
  cout << "single_e" << endl;
  drawLeptonCount(treeReader, ns, branchDict, vbfCutsArr, cutsArr, cut_e);
  ptEtaPhiMjjMt(treeReader, branchDict, vbfCutsArr, cutsArr, cut_e);
  cout << "single_e done." << endl;

  single_mu->cd();
  cout << "single_mu" << endl;
  drawLeptonCount(treeReader, ns, branchDict, vbfCutsArr, cutsArr,cut_mu);
  ptEtaPhiMjjMt(treeReader, branchDict, vbfCutsArr, cutsArr,cut_mu);
  cout << "single_mu done." << endl;

  single_tau->cd();
  cout << "single_tau" << endl;
  drawLeptonCount(treeReader, ns, branchDict, vbfCutsArr, cutsArr, cut_tau);
  ptEtaPhiMjjMt(treeReader, branchDict, vbfCutsArr, cutsArr, cut_tau);
  cout << "single_tau done." << endl;

  // close output file
  cout << "closing output file" << endl;
  HistoOutputFile->Close();

  //write to file as log
  cout << "Writing to log file" << endl;
  ofstream myfile;
  myfile.open("finishedProcesses.dat", ios::app);
  myfile << argv[1] << "\n";
  myfile.close();

  cout << "DONE." << endl;
}
