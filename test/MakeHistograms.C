///////////////////////////////////////////////////////////////////////////////
// Macro to produce histograms from the GlobalDigisProducer
//
// root -b -q MakeHistograms.C
///////////////////////////////////////////////////////////////////////////////
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"

void MakeHistograms(TString srcname="GlobalDigis.root",
		    TString filename="GlobalDigisHistograms")
{
  gROOT->Reset();
  //http://root.cern.ch/root/html/src/TStyle.cxx.html#TStyle:SetOptStat
  gStyle->SetOptStat("emruo");

  // setup names
  //TString srcname = "GlobalHits.root";
  TString treename = "Events";
  TString brnchname = "PGlobalDigi_globaldigis_GlobalDigis_GLOBAL.obj";

  // clear memory of file name
  delete gROOT->GetListOfFiles()->FindObject(srcname);

  // open source file
  TFile *srcfile = new TFile(srcname);
  
  // get tree from file
  TTree *srcevts = dynamic_cast<TTree*>(srcfile->Get(treename));
  assert(srcevts != 0);
  
  // get branch from tree
  TBranch *srcbrnch = srcevts->GetBranch(brnchname);
  assert(srcbrnch != 0);

  // get number of events
  Int_t nsrcevts = srcevts->GetEntries();

  // set access to branch as proper object
  PGlobalDigi srcGlobalDigis;
  srcbrnch->SetAddress(&srcGlobalDigis);

  // create canvas
  Int_t cWidth = 928, cHeight = 1218;
  TCanvas *myCanvas = new TCanvas("globaldigis","globaldigis",cWidth,cHeight);
  
  // open output ps file
  //TString filename = "GlobalHitsHistograms";
  TString psfile = filename+".ps";
  TString psfileopen = filename+".ps[";
  TString psfileclose = filename+".ps]";
  myCanvas->Print(psfileopen);

  // open output root file
  TString rootfile = filename+".root";
  TFile *f1 = new TFile(rootfile,"recreate");
  if (f1->IsZombie()) {
    cout << "***Unable to open " << outroot << endl;
    return;
  }

  // create label
  TLatex *label = new TLatex();
  label->SetNDC();
  TString labeltitle;

  // create attributes
  Int_t srccolor = kBlue;
  Int_t linewidth = 2;

  // create histograms
  // ecal
  TH1F *hEcaln[2];
  hEcaln[0] = 
    new TH1F("hEcaln_EE","EECal digis",100,0.,500.);
  hEcaln[1] = 
    new TH1F("hEcaln_EB","EBCal digis",300,1000.,4000.);
  TH1F *hEcalAEE[2];
  hEcalAEE[0] = 
    new TH1F("hEcalAEE_EE","EECal AEE",100,0.,1.);
  hEcalAEE[1] = 
    new TH1F("hEcalAEE_EB","EBCal AEE",100,0.,1.);
  TH1F *hEcalSHE[2];
  hEcalSHE[0] = 
    new TH1F("hEcalSHE_EE","EECal SHE",50,0.,5.);
  hEcalSHE[1] = 
    new TH1F("hEcalSHE_EB","EBCal SHE",20,0.,2.);
  TH1F *hEcalMaxPos[2];
  hEcalMaxPos[0] = 
    new TH1F("hEcalMaxPos_EE","EECal MaxPos",10,0.,10.);
  hEcalMaxPos[1] = 
    new TH1F("hEcalMaxPos_EB","EBCal MaxPos",10,0.,10.);
  TProfile *hEcalMultvAEE[2];
  hEcalMultvAEE[0] =
    new TProfile("hEcalMultvAEE_EE","EECal Mult vs. AEE",50,0.,10.,0.,500.);
  hEcalMultvAEE[1] =
    new TProfile("hEcalMultvAEE_EB","EBCal Mult vs. AEE",50,0.,10.,0.,4000.);
  TProfile *hEcalSHEvAEESHE[2];
  hEcalSHEvAEESHE[0] =
    new TProfile("hEcalSHEvAEESHE_EE","EECal SHE vs. AEE/SHE",
		 50,0.,10.,0.,10.);
  hEcalSHEvAEESHE[1] =
    new TProfile("hEcalSHEvAEESHE_EB","EBCal SHE vs. AEE/SHE",
		 50,0.,10.,0.,10.);
  for (Int_t i = 0; i < 2; ++i) {
    hEcaln[i]->GetXaxis()->SetTitle("Number of Digis");
    hEcaln[i]->GetYaxis()->SetTitle("Count");
    hEcaln[i]->SetLineColor(srccolor);
    hEcaln[i]->SetLineWidth(linewidth);
    hEcalAEE[i]->GetXaxis()->SetTitle("Analog Equivalent Energy");
    hEcalAEE[i]->GetYaxis()->SetTitle("Count");
    hEcalAEE[i]->SetLineColor(srccolor);
    hEcalAEE[i]->SetLineWidth(linewidth);
    hEcalSHE[i]->GetXaxis()->SetTitle("Simulated Hit Energy");
    hEcalSHE[i]->GetYaxis()->SetTitle("Count");
    hEcalSHE[i]->SetLineColor(srccolor);
    hEcalSHE[i]->SetLineWidth(linewidth);
    hEcalMaxPos[i]->GetXaxis()->SetTitle("Maximum Position");
    hEcalMaxPos[i]->GetYaxis()->SetTitle("Count");
    hEcalMaxPos[i]->SetLineColor(srccolor);
    hEcalMaxPos[i]->SetLineWidth(linewidth);   
    hEcalMultvAEE[i]->GetXaxis()->SetTitle("Analog Equivalent Energy");
    hEcalMultvAEE[i]->GetYaxis()->SetTitle("Number of Digis");
    hEcalMultvAEE[i]->SetLineColor(srccolor);
    hEcalMultvAEE[i]->SetLineWidth(linewidth);
    hEcalSHEvAEESHE[i]->GetXaxis()->SetTitle("AEE / SHE");
    hEcalSHEvAEESHE[i]->GetYaxis()->SetTitle("SHE");
    hEcalSHEvAEESHE[i]->SetLineColor(srccolor);
    hEcalSHEvAEESHE[i]->SetLineWidth(linewidth);
  }

  TH1F *hEScaln;
  hEScaln = 
    new TH1F("hEcaln_ES","ESCal digis",100,0.,500.);
  TH1F *hEScalADC0;
  hEScalADC0 = 
    new TH1F("hEcalADC0_ES","ESCal ADC0",150,950.,1050.);
  TH1F *hEScalADC1;
  hEScalADC1 = 
    new TH1F("hEcalADC1_ES","ESCal ADC1",150,950.,1050.);
  TH1F *hEScalADC2;
  hEScalADC2 = 
    new TH1F("hEcalADC2_ES","ESCal ADC2",150,950.,1050.);
  hEScaln->GetXaxis()->SetTitle("Number of Digis");
  hEScaln->GetYaxis()->SetTitle("Count");
  hEScaln->SetLineColor(srccolor);
  hEScaln->SetLineWidth(linewidth);
  hEScalADC0->GetXaxis()->SetTitle("ADC0");
  hEScalADC0->GetYaxis()->SetTitle("Count");
  hEScalADC0->SetLineColor(srccolor);
  hEScalADC0->SetLineWidth(linewidth);
  hEScalADC1->GetXaxis()->SetTitle("ADC1");
  hEScalADC1->GetYaxis()->SetTitle("Count");
  hEScalADC1->SetLineColor(srccolor);
  hEScalADC1->SetLineWidth(linewidth);
  hEScalADC2->GetXaxis()->SetTitle("ADC2");
  hEScalADC2->GetYaxis()->SetTitle("Count");
  hEScalADC2->SetLineColor(srccolor);
  hEScalADC2->SetLineWidth(linewidth);

  // hcal
  TH1F *hHcaln[4];
  hHcaln[0] = 
    new TH1F("hHcaln_HB","HBCal digis",1000,2000.,3000.);
  hHcaln[1] = 
    new TH1F("hHcaln_HE","HECal digis",1000,2000.,3000.);
  hHcaln[2] = 
    new TH1F("hHcaln_HO","HOCal digis",1000,2000.,3000.);
  hHcaln[3] = 
    new TH1F("hHcaln_HF","HFCal digis",1000,1000.,2000.);
  TH1F *hHcalAEE[4];
  hHcalAEE[0] = 
    new TH1F("hHcalAEE_HB","HBCal AEE",60,-10.,50.);
  hHcalAEE[1] = 
    new TH1F("hHcalAEE_HE","HECal AEE",60,-10.,50.);
  hHcalAEE[2] = 
    new TH1F("hHcalAEE_HO","HOCal AEE",60,-10.,50.);
  hHcalAEE[3] = 
    new TH1F("hHcalAEE_HF","HFCal AEE",60,-10.,50.);
  TH1F *hHcalSHE[4];
  hHcalSHE[0] = 
    new TH1F("hHcalSHE_HB","HBCal SHE",100,0.0,0.05);
  hHcalSHE[1] = 
    new TH1F("hHcalSHE_HE","HECal SHE",100,0.0,0.05);
  hHcalSHE[2] = 
    new TH1F("hHcalSHE_HO","HOCal SHE",100,0.0,0.05);
  hHcalSHE[3] = 
    new TH1F("hHcalSHE_HF","HFCal SHE",100,0.0,20.);
  TH1F *hHcalAEESHE[4];
  hHcalAEESHE[0] = 
    new TH1F("hHcalAEESHE_HB","HBCal AEE/SHE",200,-5000.,5000.);
  hHcalAEESHE[1] = 
    new TH1F("hHcalAEESHE_HE","HECal AEE/SHE",200,-5000.,5000.);
  hHcalAEESHE[2] = 
    new TH1F("hHcalAEESHE_HO","HOCal AEE/SHE",200,-5000.,5000.);
  hHcalAEESHE[3] = 
    new TH1F("hHcalAEESHE_HF","HFCal AEE/SHE",40,-20.,20.);
  TProfile *hHcalSHEvAEE[4];
  hHcalSHEvAEE[0] =
    new TProfile("hHcalSHEvAEE_HB","HBCal SHE vs. AEE",
		 60,-10.,50.,0.,1.);
  hHcalSHEvAEE[1] =
    new TProfile("hHcalSHEvAEE_HE","HECal SHE vs. AEE",
		 60,-10.,50.,0.,1.);
  hHcalSHEvAEE[2] =
    new TProfile("hHcalSHEvAEE_HO","HOCal SHE vs. AEE",
		 60,-10.,50.,0.,1.);
  hHcalSHEvAEE[3] =
    new TProfile("hHcalSHEvAEE_HF","HFCal SHE vs. AEE",
		 60,-10.,50.,0.,20.);
  for (Int_t i = 0; i < 4; ++i) {
    hHcaln[i]->GetXaxis()->SetTitle("Number of Digis");
    hHcaln[i]->GetYaxis()->SetTitle("Count");
    hHcaln[i]->SetLineColor(srccolor);
    hHcaln[i]->SetLineWidth(linewidth);
    hHcalAEE[i]->GetXaxis()->SetTitle("ADC (fC)");
    hHcalAEE[i]->GetYaxis()->SetTitle("Count");
    hHcalAEE[i]->SetLineColor(srccolor);
    hHcalAEE[i]->SetLineWidth(linewidth);  
    hHcalSHE[i]->GetXaxis()->SetTitle("Simulated Hit Energy");
    hHcalSHE[i]->GetYaxis()->SetTitle("Count");
    hHcalSHE[i]->SetLineColor(srccolor);
    hHcalSHE[i]->SetLineWidth(linewidth);
    hHcalAEESHE[i]->GetXaxis()->SetTitle("ADC / SHE");
    hHcalAEESHE[i]->GetYaxis()->SetTitle("Count");
    hHcalAEESHE[i]->SetLineColor(srccolor);
    hHcalAEESHE[i]->SetLineWidth(linewidth);
    hHcalSHEvAEE[i]->GetXaxis()->SetTitle("ADC");
    hHcalSHEvAEE[i]->GetYaxis()->SetTitle("SHE");
    hHcalSHEvAEE[i]->SetLineColor(srccolor);
    hHcalSHEvAEE[i]->SetLineWidth(linewidth);
  }

  // SiStrip
  TH1F *hSiStripn[19];
  hSiStripn[0] = 
    new TH1F("hSiStripn_TIBL1","TIBL1 digis",250,0.,500.);
  hSiStripn[1] = 
    new TH1F("hSiStripn_TIBL2","TIBL2 digis",250,0.,500.);
  hSiStripn[2] = 
    new TH1F("hSiStripn_TIBL3","TIBL3 digis",250,0.,500.);
  hSiStripn[3] = 
    new TH1F("hSiStripn_TIBL4","TIBL4 digis",250,0.,500.);
  hSiStripn[4] = 
    new TH1F("hSiStripn_TOBL1","TOBL1 digis",250,0.,500.);
  hSiStripn[5] = 
    new TH1F("hSiStripn_TOBL2","TOBL2 digis",250,0.,500.);
  hSiStripn[6] = 
    new TH1F("hSiStripn_TOBL3","TOBL3 digis",250,0.,500.);
  hSiStripn[7] = 
    new TH1F("hSiStripn_TOBL4","TOBL4 digis",250,0.,500.);
  hSiStripn[8] = 
    new TH1F("hSiStripn_TIDW1","TIDW1 digis",50,0.,100.);
  hSiStripn[9] = 
    new TH1F("hSiStripn_TIDW2","TIDW2 digis",50,0.,100.);
  hSiStripn[10] = 
    new TH1F("hSiStripn_TIDW3","TIDW3 digis",50,0.,100.);
  hSiStripn[11] = 
    new TH1F("hSiStripn_TECW1","TECW1 digis",50,0.,100.);
  hSiStripn[12] = 
    new TH1F("hSiStripn_TECW2","TECW2 digis",50,0.,100.);
  hSiStripn[13] = 
    new TH1F("hSiStripn_TECW3","TECW3 digis",50,0.,100.);
  hSiStripn[14] = 
    new TH1F("hSiStripn_TECW4","TECW4 digis",50,0.,100.);
  hSiStripn[15] = 
    new TH1F("hSiStripn_TECW5","TECW5 digis",50,0.,100.);
  hSiStripn[16] = 
    new TH1F("hSiStripn_TECW6","TECW6 digis",50,0.,100.);
  hSiStripn[17] = 
    new TH1F("hSiStripn_TECW7","TECW7 digis",50,0.,100.);
  hSiStripn[18] = 
    new TH1F("hSiStripn_TECW8","TECW8 digis",50,0.,100.);
  TH1F *hSiStripADC[19];
  hSiStripADC[0] = 
    new TH1F("hSiStripADC_TIBL1","TIBL1 ADC",150,0.,300.);
  hSiStripADC[1] = 
    new TH1F("hSiStripADC_TIBL2","TIBL2 ADC",150,0.,300.);
  hSiStripADC[2] = 
    new TH1F("hSiStripADC_TIBL3","TIBL3 ADC",150,0.,300.);
  hSiStripADC[3] = 
    new TH1F("hSiStripADC_TIBL4","TIBL4 ADC",150,0.,300.);
  hSiStripADC[4] = 
    new TH1F("hSiStripADC_TOBL1","TOBL1 ADC",150,0.,300.);
  hSiStripADC[5] = 
    new TH1F("hSiStripADC_TOBL2","TOBL2 ADC",150,0.,300.);
  hSiStripADC[6] = 
    new TH1F("hSiStripADC_TOBL3","TOBL3 ADC",150,0.,300.);
  hSiStripADC[7] = 
    new TH1F("hSiStripADC_TOBL4","TOBL4 ADC",150,0.,300.);
  hSiStripADC[8] = 
    new TH1F("hSiStripADC_TIDW1","TIDW1 ADC",150,0.,300.);
  hSiStripADC[9] = 
    new TH1F("hSiStripADC_TIDW2","TIDW2 ADC",150,0.,300.);
  hSiStripADC[10] = 
    new TH1F("hSiStripADC_TIDW3","TIDW3 ADC",150,0.,300.);
  hSiStripADC[11] = 
    new TH1F("hSiStripADC_TECW1","TECW1 ADC",150,0.,300.);
  hSiStripADC[12] = 
    new TH1F("hSiStripADC_TECW2","TECW2 ADC",150,0.,300.);
  hSiStripADC[13] = 
    new TH1F("hSiStripADC_TECW3","TECW3 ADC",150,0.,300.);
  hSiStripADC[14] = 
    new TH1F("hSiStripADC_TECW4","TECW4 ADC",150,0.,300.);
  hSiStripADC[15] = 
    new TH1F("hSiStripADC_TECW5","TECW5 ADC",150,0.,300.);
  hSiStripADC[16] = 
    new TH1F("hSiStripADC_TECW6","TECW6 ADC",150,0.,300.);
  hSiStripADC[17] = 
    new TH1F("hSiStripADC_TECW7","TECW7 ADC",150,0.,300.);
  hSiStripADC[18] = 
    new TH1F("hSiStripADC_TECW8","TECW8 ADC",150,0.,300.);
  TH1F *hSiStripStrip[19];
  hSiStripStrip[0] = 
    new TH1F("hSiStripStrip_TIBL1","TIBL1 Strip",200,0.,800.);
  hSiStripStrip[1] = 
    new TH1F("hSiStripStrip_TIBL2","TIBL2 Strip",200,0.,800.);
  hSiStripStrip[2] = 
    new TH1F("hSiStripStrip_TIBL3","TIBL3 Strip",200,0.,800.);
  hSiStripStrip[3] = 
    new TH1F("hSiStripStrip_TIBL4","TIBL4 Strip",200,0.,800.);
  hSiStripStrip[4] = 
    new TH1F("hSiStripStrip_TOBL1","TOBL1 Strip",200,0.,800.);
  hSiStripStrip[5] = 
    new TH1F("hSiStripStrip_TOBL2","TOBL2 Strip",200,0.,800.);
  hSiStripStrip[6] = 
    new TH1F("hSiStripStrip_TOBL3","TOBL3 Strip",200,0.,800.);
  hSiStripStrip[7] = 
    new TH1F("hSiStripStrip_TOBL4","TOBL4 Strip",200,0.,800.);
  hSiStripStrip[8] = 
    new TH1F("hSiStripStrip_TIDW1","TIDW1 Strip",200,0.,800.);
  hSiStripStrip[9] = 
    new TH1F("hSiStripStrip_TIDW2","TIDW2 Strip",200,0.,800.);
  hSiStripStrip[10] = 
    new TH1F("hSiStripStrip_TIDW3","TIDW3 Strip",200,0.,800.);
  hSiStripStrip[11] = 
    new TH1F("hSiStripStrip_TECW1","TECW1 Strip",200,0.,800.);
  hSiStripStrip[12] = 
    new TH1F("hSiStripStrip_TECW2","TECW2 Strip",200,0.,800.);
  hSiStripStrip[13] = 
    new TH1F("hSiStripStrip_TECW3","TECW3 Strip",200,0.,800.);
  hSiStripStrip[14] = 
    new TH1F("hSiStripStrip_TECW4","TECW4 Strip",200,0.,800.);
  hSiStripStrip[15] = 
    new TH1F("hSiStripStrip_TECW5","TECW5 Strip",200,0.,800.);
  hSiStripStrip[16] = 
    new TH1F("hSiStripStrip_TECW6","TECW6 Strip",200,0.,800.);
  hSiStripStrip[17] = 
    new TH1F("hSiStripStrip_TECW7","TECW7 Strip",200,0.,800.);
  hSiStripStrip[18] = 
    new TH1F("hSiStripStrip_TECW8","TECW8 Strip",200,0.,800.);
  for (Int_t i = 0; i < 19; ++i) {
    hSiStripn[i]->GetXaxis()->SetTitle("Number of Digis");
    hSiStripn[i]->GetYaxis()->SetTitle("Count");
    hSiStripn[i]->SetLineColor(srccolor);
    hSiStripn[i]->SetLineWidth(linewidth);
    hSiStripADC[i]->GetXaxis()->SetTitle("ADC");
    hSiStripADC[i]->GetYaxis()->SetTitle("Count");
    hSiStripADC[i]->SetLineColor(srccolor);
    hSiStripADC[i]->SetLineWidth(linewidth);
    hSiStripStrip[i]->GetXaxis()->SetTitle("Strip Number");
    hSiStripStrip[i]->GetYaxis()->SetTitle("Count");
    hSiStripStrip[i]->SetLineColor(srccolor);
    hSiStripStrip[i]->SetLineWidth(linewidth);
  }

  // get siPixel
  TH1F *hSiPixeln[7];
  hSiPixeln[0] = 
    new TH1F("hSiPixeln_BRL1","BRL1 digis",50,0.,100.);
  hSiPixeln[1] = 
    new TH1F("hSiPixeln_BRL2","BRL2 digis",50,0.,100.);
  hSiPixeln[2] = 
    new TH1F("hSiPixeln_BRL3","BRL3 digis",50,0.,100.);
  hSiPixeln[3] = 
    new TH1F("hSiPixeln_FWD1p","FWD1p digis",25,0.,50.);
  hSiPixeln[4] = 
    new TH1F("hSiPixeln_FWD1n","FWD1n digis",25,0.,50.);
  hSiPixeln[5] = 
    new TH1F("hSiPixeln_FWD2p","FWD2p digis",25,0.,50.);
  hSiPixeln[6] = 
    new TH1F("hSiPixeln_FWD2n","FWD2n digis",25,0.,50.);
  TH1F *hSiPixelADC[7];
  hSiPixelADC[0] = 
    new TH1F("hSiPixelADC_BRL1","BRL1 ADC",150,0.,300.);
  hSiPixelADC[1] = 
    new TH1F("hSiPixelADC_BRL2","BRL2 ADC",150,0.,300.);
  hSiPixelADC[2] = 
    new TH1F("hSiPixelADC_BRL3","BRL3 ADC",150,0.,300.);
  hSiPixelADC[3] = 
    new TH1F("hSiPixelADC_FWD1p","FWD1p ADC",150,0.,300.);
  hSiPixelADC[4] = 
    new TH1F("hSiPixelADC_FWD1n","FWD1n ADC",150,0.,300.);
  hSiPixelADC[5] = 
    new TH1F("hSiPixelADC_FWD2p","FWD2p ADC",150,0.,300.);
  hSiPixelADC[6] = 
    new TH1F("hSiPixelADC_FWD2n","FWD2n ADC",150,0.,300.);
  TH1F *hSiPixelRow[7];
  hSiPixelRow[0] = 
    new TH1F("hSiPixelRow_BRL1","BRL1 Row",100,0.,200.);
  hSiPixelRow[1] = 
    new TH1F("hSiPixelRow_BRL2","BRL2 Row",100,0.,200.);
  hSiPixelRow[2] = 
    new TH1F("hSiPixelRow_BRL3","BRL3 Row",100,0.,200.);
  hSiPixelRow[3] = 
    new TH1F("hSiPixelRow_FWD1p","FWD1p Row",100,0.,200.);
  hSiPixelRow[4] = 
    new TH1F("hSiPixelRow_FWD1n","FWD1n Row",100,0.,200.);
  hSiPixelRow[5] = 
    new TH1F("hSiPixelRow_FWD2p","FWD2p Row",100,0.,200.);
  hSiPixelRow[6] = 
    new TH1F("hSiPixelRow_FWD2n","FWD2n Row",100,0.,200.);
  TH1F *hSiPixelCol[7];
  hSiPixelCol[0] = 
    new TH1F("hSiPixelCol_BRL1","BRL1 Column",200,0.,500.);
  hSiPixelCol[1] = 
    new TH1F("hSiPixelCol_BRL2","BRL2 Column",200,0.,500.);
  hSiPixelCol[2] = 
    new TH1F("hSiPixelCol_BRL3","BRL3 Column",200,0.,500.);
  hSiPixelCol[3] = 
    new TH1F("hSiPixelCol_FWD1p","FWD1p Column",200,0.,500.);
  hSiPixelCol[4] = 
    new TH1F("hSiPixelCol_FWD1n","FWD1n Column",200,0.,500.);
  hSiPixelCol[5] = 
    new TH1F("hSiPixelCol_FWD2p","FWD2p Column",200,0.,500.);
  hSiPixelCol[6] = 
    new TH1F("hSiPixelCol_FWD2n","FWD2n Column",200,0.,500.);
  for (Int_t i = 0; i < 7; ++i) {
    hSiPixeln[i]->GetXaxis()->SetTitle("Number of Digis");
    hSiPixeln[i]->GetYaxis()->SetTitle("Count");
    hSiPixeln[i]->SetLineColor(srccolor);
    hSiPixeln[i]->SetLineWidth(linewidth);
    hSiPixelADC[i]->GetXaxis()->SetTitle("ADC");
    hSiPixelADC[i]->GetYaxis()->SetTitle("Count");
    hSiPixelADC[i]->SetLineColor(srccolor);
    hSiPixelADC[i]->SetLineWidth(linewidth);
    hSiPixelRow[i]->GetXaxis()->SetTitle("Row Number");
    hSiPixelRow[i]->GetYaxis()->SetTitle("Count");
    hSiPixelRow[i]->SetLineColor(srccolor);
    hSiPixelRow[i]->SetLineWidth(linewidth);
    hSiPixelCol[i]->GetXaxis()->SetTitle("Column Number");
    hSiPixelCol[i]->GetYaxis()->SetTitle("Count");
    hSiPixelCol[i]->SetLineColor(srccolor);
    hSiPixelCol[i]->SetLineWidth(linewidth);
  }

  TH1F *hDtMuonn[4];
  hDtMuonn[0] = 
    new TH1F("hDtMuonn_MB1","MB1 digis",25,0.,50.);
  hDtMuonn[1] = 
    new TH1F("hDtMuonn_MB2","MB2 digis",25,0.,50.);
  hDtMuonn[2] = 
    new TH1F("hDtMuonn_MB3","MB3 digis",25,0.,50.);
  hDtMuonn[3] = 
    new TH1F("hDtMuonn_MB4","MB4 digis",25,0.,50.);
  TH1F *hDtMuonLayer[4];
  hDtMuonLayer[0] = 
    new TH1F("hDtMuonLayer_MB1","MB1 Layer",12,1.,13.);
  hDtMuonLayer[1] = 
    new TH1F("hDtMuonLayer_MB2","MB2 Layer",12,1.,13.);
  hDtMuonLayer[2] = 
    new TH1F("hDtMuonLayer_MB3","MB3 Layer",12,1.,13.);
  hDtMuonLayer[3] = 
    new TH1F("hDtMuonLayer_MB4","MB4 Layer",12,1.,13.);
  TH1F *hDtMuonTime[4];
  hDtMuonTime[0] = 
    new TH1F("hDtMuonTime_MB1","MB1 Time",300,400.,1000.);
  hDtMuonTime[1] = 
    new TH1F("hDtMuonTime_MB2","MB2 Time",300,400.,1000.);
  hDtMuonTime[2] = 
    new TH1F("hDtMuonTime_MB3","MB3 Time",300,400.,1000.);
  hDtMuonTime[3] = 
    new TH1F("hDtMuonTime_MB4","MB4 Time",300,400.,1000.);
  TProfile *hDtMuonTimevLayer[4];
  hDtMuonTimevLayer[0] = 
    new TProfile("hDtMuonTimevLayer_MB1","MB1 Time vs. Layer",
	     12,1.,13.,500.,1000.);
  hDtMuonTimevLayer[1] = 
    new TProfile("hDtMuonTimevLayer_MB2","MB2 Time vs. Layer",12,1.,13.,
	     500.,1000.);
  hDtMuonTimevLayer[2] = 
    new TProfile("hDtMuonTimevLayer_MB3","MB3 Time vs. Layer",12,1.,13.,
	     500.,1000.);
  hDtMuonTimevLayer[3] = 
    new TProfile("hDtMuonTimevLayer_MB4","MB4 Time vs. Layer",12,1.,13.,
	     500.,1000.);
  for (Int_t i = 0; i < 4; ++i) {
    hDtMuonn[i]->GetXaxis()->SetTitle("Number of Digis");
    hDtMuonn[i]->GetYaxis()->SetTitle("Count");
    hDtMuonn[i]->SetLineColor(srccolor);
    hDtMuonn[i]->SetLineWidth(linewidth);
    hDtMuonLayer[i]->GetXaxis()->SetTitle("4 * (SuperLayer - 1) + Layer");
    hDtMuonLayer[i]->GetYaxis()->SetTitle("Count");
    hDtMuonLayer[i]->SetLineColor(srccolor);
    hDtMuonLayer[i]->SetLineWidth(linewidth);
    hDtMuonTime[i]->GetXaxis()->SetTitle("Time");
    hDtMuonTime[i]->GetYaxis()->SetTitle("Count");
    hDtMuonTime[i]->SetLineColor(srccolor);
    hDtMuonTime[i]->SetLineWidth(linewidth);
    hDtMuonTimevLayer[i]->GetXaxis()->SetTitle("4 * (SuperLayer - 1) + Layer");
    hDtMuonTimevLayer[i]->GetYaxis()->SetTitle("Time");
    hDtMuonTimevLayer[i]->SetLineColor(srccolor);
    hDtMuonTimevLayer[i]->SetLineWidth(linewidth);
  }

  TH1F *hCSCStripn;
  hCSCStripn = 
    new TH1F("hCSCStripn","CSC Strip digis",25,0.,50.);
  TH1F *hCSCStripADC;
  hCSCStripADC = 
    new TH1F("hCSCStripADC","CSC Strip ADC",110,0.,1100);
  hCSCStripn->GetXaxis()->SetTitle("Number of Digis");
  hCSCStripn->GetYaxis()->SetTitle("Count");
  hCSCStripn->SetLineColor(srccolor);
  hCSCStripn->SetLineWidth(linewidth);
  hCSCStripADC->GetXaxis()->SetTitle("ADC");
  hCSCStripADC->GetYaxis()->SetTitle("Count");
  hCSCStripADC->SetLineColor(srccolor);
  hCSCStripADC->SetLineWidth(linewidth);

  TH1F *hCSCWiren;
  hCSCWiren = 
    new TH1F("hCSCWiren","CSC Wire digis",25,0.,50.);
  TH1F *hCSCWireTime;
  hCSCWireTime = 
    new TH1F("hCSCWireTime","CSC Wire Time",10,0.,10);
  hCSCWiren->GetXaxis()->SetTitle("Number of Digis");
  hCSCWiren->GetYaxis()->SetTitle("Count");
  hCSCWiren->SetLineColor(srccolor);
  hCSCWiren->SetLineWidth(linewidth);
  hCSCWireTime->GetXaxis()->SetTitle("Time");
  hCSCWireTime->GetYaxis()->SetTitle("Count");
  hCSCWireTime->SetLineColor(srccolor);
  hCSCWireTime->SetLineWidth(linewidth);

  // cycle through events in tree
  for (Int_t evt = 0; evt < nsrcevts; ++evt) {
    Int_t nevt = evt+1;

    if (nevt%100 == 0 || nevt == 1) {
      cout << "     Processing event " << nevt << endl;
    }

    srcbrnch->GetEntry(evt);

    int nEBCalDigis = srcGlobalDigis.getnEBCalDigis();
    int nEECalDigis = srcGlobalDigis.getnEECalDigis();
    int nESCalDigis = srcGlobalDigis.getnESCalDigis();

    int nHBCalDigis = srcGlobalDigis.getnHBCalDigis();
    int nHECalDigis = srcGlobalDigis.getnHECalDigis();
    int nHOCalDigis = srcGlobalDigis.getnHOCalDigis();
    int nHFCalDigis = srcGlobalDigis.getnHFCalDigis();        

    int nTIBL1Digis = srcGlobalDigis.getnTIBL1Digis();    
    int nTIBL2Digis = srcGlobalDigis.getnTIBL2Digis();    
    int nTIBL3Digis = srcGlobalDigis.getnTIBL3Digis();    
    int nTIBL4Digis = srcGlobalDigis.getnTIBL4Digis();    
    int nTOBL1Digis = srcGlobalDigis.getnTOBL1Digis();    
    int nTOBL2Digis = srcGlobalDigis.getnTOBL2Digis();    
    int nTOBL3Digis = srcGlobalDigis.getnTOBL3Digis();    
    int nTOBL4Digis = srcGlobalDigis.getnTOBL4Digis();    
    int nTIDW1Digis = srcGlobalDigis.getnTIDW1Digis();    
    int nTIDW2Digis = srcGlobalDigis.getnTIDW2Digis();    
    int nTIDW3Digis = srcGlobalDigis.getnTIDW3Digis();    
    int nTECW1Digis = srcGlobalDigis.getnTECW1Digis();    
    int nTECW2Digis = srcGlobalDigis.getnTECW2Digis();    
    int nTECW3Digis = srcGlobalDigis.getnTECW3Digis();  
    int nTECW4Digis = srcGlobalDigis.getnTECW4Digis();    
    int nTECW5Digis = srcGlobalDigis.getnTECW5Digis();    
    int nTECW6Digis = srcGlobalDigis.getnTECW6Digis();  
    int nTECW7Digis = srcGlobalDigis.getnTECW7Digis();    
    int nTECW8Digis = srcGlobalDigis.getnTECW8Digis();

    int nBRL1Digis = srcGlobalDigis.getnBRL1Digis();    
    int nBRL2Digis = srcGlobalDigis.getnBRL2Digis();    
    int nBRL3Digis = srcGlobalDigis.getnBRL3Digis();       
    int nFWD1nDigis = srcGlobalDigis.getnFWD1nDigis();
    int nFWD1pDigis = srcGlobalDigis.getnFWD1pDigis();    
    int nFWD2nDigis = srcGlobalDigis.getnFWD2nDigis();    
    int nFWD2pDigis = srcGlobalDigis.getnFWD2pDigis(); 
  
    int nMB1Digis = srcGlobalDigis.getnMB1Digis();  
    int nMB2Digis = srcGlobalDigis.getnMB2Digis();  
    int nMB3Digis = srcGlobalDigis.getnMB3Digis();  
    int nMB4Digis = srcGlobalDigis.getnMB4Digis();  

    int nCSCstripDigis = srcGlobalDigis.getnCSCstripDigis();

    int nCSCwireDigis = srcGlobalDigis.getnCSCwireDigis();


    // get Ecal info
    std::vector<PGlobalDigi::ECalDigi> EECalDigis = 
      srcGlobalDigis.getEECalDigis();
    hEcaln[0]->Fill((float)nEECalDigis);
    for (Int_t i = 0; i < EECalDigis.size(); ++i) {
      hEcalAEE[0]->Fill(EECalDigis[i].AEE);
      hEcalMaxPos[0]->Fill(EECalDigis[i].maxPos);
      hEcalMultvAEE[0]->Fill(EECalDigis[i].AEE,(float)nEECalDigis,1);
      if (EECalDigis[i].SHE != 0.) {
	hEcalSHE[0]->Fill(EECalDigis[i].SHE);
	hEcalSHEvAEESHE[0]->
	  Fill(EECalDigis[i].AEE/EECalDigis[i].SHE,EECalDigis[i].SHE,1);
      }
    }
    
    std::vector<PGlobalDigi::ECalDigi> EBCalDigis = 
      srcGlobalDigis.getEBCalDigis();
    hEcaln[1]->Fill((float)nEBCalDigis);
    for (Int_t i = 0; i < EBCalDigis.size(); ++i) {
      hEcalAEE[1]->Fill(EBCalDigis[i].AEE);
      hEcalMaxPos[1]->Fill(EBCalDigis[i].maxPos);
      hEcalMultvAEE[1]->Fill(EBCalDigis[i].AEE,(float)nEBCalDigis,1);
      if (EBCalDigis[i].SHE != 0.) {
	hEcalSHE[1]->Fill(EBCalDigis[i].SHE);
	hEcalSHEvAEESHE[1]->
	  Fill(EBCalDigis[i].AEE/EBCalDigis[i].SHE,EBCalDigis[i].SHE,1);
      }
    }
    
    std::vector<PGlobalDigi::ESCalDigi> ESCalDigis = 
      srcGlobalDigis.getESCalDigis();   
    hEScaln->Fill((float)nESCalDigis);
    for (Int_t i = 0; i < ESCalDigis.size(); ++i) {
      hEScalADC0->Fill(ESCalDigis[i].ADC0);
      hEScalADC1->Fill(ESCalDigis[i].ADC1);
      hEScalADC2->Fill(ESCalDigis[i].ADC2);
    }
    
    // Get HCal info
    std::vector<PGlobalDigi::HCalDigi> HBCalDigis = 
      srcGlobalDigis.getHBCalDigis();
    hHcaln[0]->Fill((float)nHBCalDigis);
    for (Int_t i = 0; i < HBCalDigis.size(); ++i) {
      hHcalAEE[0]->Fill(HBCalDigis[i].AEE);
      if (HBCalDigis[i].SHE != 0.) {
	hHcalSHE[0]->Fill(HBCalDigis[i].SHE);
	hHcalAEESHE[0]->Fill(HBCalDigis[i].AEE/HBCalDigis[i].SHE);
	hHcalSHEvAEE[0]->
	  Fill(HBCalDigis[i].AEE,HBCalDigis[i].SHE,1);
      }
    }
    std::vector<PGlobalDigi::HCalDigi> HECalDigis = 
      srcGlobalDigis.getHECalDigis();
    hHcaln[1]->Fill((float)nHECalDigis);
    for (Int_t i = 0; i < HECalDigis.size(); ++i) {
      hHcalAEE[1]->Fill(HECalDigis[i].AEE);
      if (HECalDigis[i].SHE != 0.) {
	hHcalSHE[1]->Fill(HECalDigis[i].SHE);
	hHcalAEESHE[1]->Fill(HECalDigis[i].AEE/HECalDigis[i].SHE);
	hHcalSHEvAEE[1]->
	  Fill(HECalDigis[i].AEE,HECalDigis[i].SHE,1);
      }
    }

    std::vector<PGlobalDigi::HCalDigi> HOCalDigis = 
      srcGlobalDigis.getHOCalDigis();
    hHcaln[2]->Fill((float)nHOCalDigis);
    for (Int_t i = 0; i < HOCalDigis.size(); ++i) {
      hHcalAEE[2]->Fill(HOCalDigis[i].AEE);
      if (HOCalDigis[i].SHE != 0.) {
	hHcalSHE[2]->Fill(HOCalDigis[i].SHE);
	hHcalAEESHE[2]->Fill(HOCalDigis[i].AEE/HOCalDigis[i].SHE);
	hHcalSHEvAEE[2]->
	  Fill(HOCalDigis[i].AEE,HOCalDigis[i].SHE,1);
      }
    }

    std::vector<PGlobalDigi::HCalDigi> HFCalDigis = 
      srcGlobalDigis.getHFCalDigis();
    hHcaln[3]->Fill((float)nHFCalDigis);
    for (Int_t i = 0; i < HFCalDigis.size(); ++i) {
      hHcalAEE[3]->Fill(HFCalDigis[i].AEE);
      if (HFCalDigis[i].SHE != 0.) {
	hHcalSHE[3]->Fill(HFCalDigis[i].SHE);
	hHcalAEESHE[3]->Fill(HFCalDigis[i].AEE/HFCalDigis[i].SHE);
	hHcalSHEvAEE[3]->
	  Fill(HFCalDigis[i].AEE,HFCalDigis[i].SHE,1);
      }
    }

    // get SiStrip info
    std::vector<PGlobalDigi::SiStripDigi> TIBL1Digis =
      srcGlobalDigis.getTIBL1Digis();      
    hSiStripn[0]->Fill((float)nTIBL1Digis);
    for (Int_t i = 0; i < TIBL1Digis.size(); ++i) {
      hSiStripADC[0]->Fill(TIBL1Digis[i].ADC);
      hSiStripStrip[0]->Fill(TIBL1Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TIBL2Digis =
      srcGlobalDigis.getTIBL2Digis();      
    hSiStripn[1]->Fill((float)nTIBL2Digis);
    for (Int_t i = 0; i < TIBL2Digis.size(); ++i) {
      hSiStripADC[1]->Fill(TIBL2Digis[i].ADC);
      hSiStripStrip[1]->Fill(TIBL2Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TIBL3Digis =
      srcGlobalDigis.getTIBL3Digis();      
    hSiStripn[2]->Fill((float)nTIBL3Digis);
    for (Int_t i = 0; i < TIBL3Digis.size(); ++i) {
      hSiStripADC[2]->Fill(TIBL3Digis[i].ADC);
      hSiStripStrip[2]->Fill(TIBL3Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TIBL4Digis =
      srcGlobalDigis.getTIBL4Digis();      
    hSiStripn[3]->Fill((float)nTIBL4Digis);
    for (Int_t i = 0; i < TIBL4Digis.size(); ++i) {
      hSiStripADC[3]->Fill(TIBL4Digis[i].ADC);
      hSiStripStrip[3]->Fill(TIBL4Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TOBL1Digis =
      srcGlobalDigis.getTOBL1Digis();      
    hSiStripn[4]->Fill((float)nTOBL1Digis);
    for (Int_t i = 0; i < TOBL1Digis.size(); ++i) {
      hSiStripADC[4]->Fill(TOBL1Digis[i].ADC);
      hSiStripStrip[4]->Fill(TOBL1Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TOBL2Digis =
      srcGlobalDigis.getTOBL2Digis();      
    hSiStripn[5]->Fill((float)nTOBL2Digis);
    for (Int_t i = 0; i < TOBL2Digis.size(); ++i) {
      hSiStripADC[5]->Fill(TOBL2Digis[i].ADC);
      hSiStripStrip[5]->Fill(TOBL2Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TOBL3Digis =
      srcGlobalDigis.getTOBL3Digis();      
    hSiStripn[6]->Fill((float)nTOBL3Digis);
    for (Int_t i = 0; i < TOBL3Digis.size(); ++i) {
      hSiStripADC[6]->Fill(TOBL3Digis[i].ADC);
      hSiStripStrip[6]->Fill(TOBL3Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TOBL4Digis =
      srcGlobalDigis.getTOBL4Digis();      
    hSiStripn[7]->Fill((float)nTOBL4Digis);
    for (Int_t i = 0; i < TOBL4Digis.size(); ++i) {
      hSiStripADC[7]->Fill(TOBL4Digis[i].ADC);
      hSiStripStrip[7]->Fill(TOBL4Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TIDW1Digis =
      srcGlobalDigis.getTIDW1Digis();      
    hSiStripn[8]->Fill((float)nTIDW1Digis);
    for (Int_t i = 0; i < TIDW1Digis.size(); ++i) {
      hSiStripADC[8]->Fill(TIDW1Digis[i].ADC);
      hSiStripStrip[8]->Fill(TIDW1Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TIDW2Digis =
      srcGlobalDigis.getTIDW2Digis();      
    hSiStripn[9]->Fill((float)nTIDW2Digis);
    for (Int_t i = 0; i < TIDW2Digis.size(); ++i) {
      hSiStripADC[9]->Fill(TIDW2Digis[i].ADC);
      hSiStripStrip[9]->Fill(TIDW2Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TIDW3Digis =
      srcGlobalDigis.getTIDW3Digis();      
    hSiStripn[10]->Fill((float)nTIDW3Digis);
    for (Int_t i = 0; i < TIDW3Digis.size(); ++i) {
      hSiStripADC[10]->Fill(TIDW3Digis[i].ADC);
      hSiStripStrip[10]->Fill(TIDW3Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TECW1Digis =
      srcGlobalDigis.getTECW1Digis();      
    hSiStripn[11]->Fill((float)nTECW1Digis);
    for (Int_t i = 0; i < TECW1Digis.size(); ++i) {
      hSiStripADC[11]->Fill(TECW1Digis[i].ADC);
      hSiStripStrip[11]->Fill(TECW1Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TECW2Digis =
      srcGlobalDigis.getTECW2Digis();      
    hSiStripn[12]->Fill((float)nTECW2Digis);
    for (Int_t i = 0; i < TECW2Digis.size(); ++i) {
      hSiStripADC[12]->Fill(TECW2Digis[i].ADC);
      hSiStripStrip[12]->Fill(TECW2Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TECW3Digis =
      srcGlobalDigis.getTECW3Digis();      
    hSiStripn[13]->Fill((float)nTECW3Digis);
    for (Int_t i = 0; i < TECW3Digis.size(); ++i) {
      hSiStripADC[13]->Fill(TECW3Digis[i].ADC);
      hSiStripStrip[13]->Fill(TECW3Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TECW4Digis =
      srcGlobalDigis.getTECW4Digis();      
    hSiStripn[14]->Fill((float)nTECW4Digis);
    for (Int_t i = 0; i < TECW4Digis.size(); ++i) {
      hSiStripADC[14]->Fill(TECW4Digis[i].ADC);
      hSiStripStrip[14]->Fill(TECW4Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TECW5Digis =
      srcGlobalDigis.getTECW5Digis();      
    hSiStripn[15]->Fill((float)nTECW5Digis);
    for (Int_t i = 0; i < TECW5Digis.size(); ++i) {
      hSiStripADC[15]->Fill(TECW5Digis[i].ADC);
      hSiStripStrip[15]->Fill(TECW5Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TECW6Digis =
      srcGlobalDigis.getTECW6Digis();      
    hSiStripn[16]->Fill((float)nTECW6Digis);
    for (Int_t i = 0; i < TECW6Digis.size(); ++i) {
      hSiStripADC[16]->Fill(TECW6Digis[i].ADC);
      hSiStripStrip[16]->Fill(TECW6Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TECW7Digis =
      srcGlobalDigis.getTECW7Digis();      
    hSiStripn[17]->Fill((float)nTECW7Digis);
    for (Int_t i = 0; i < TECW7Digis.size(); ++i) {
      hSiStripADC[17]->Fill(TECW7Digis[i].ADC);
      hSiStripStrip[17]->Fill(TECW7Digis[i].STRIP);
    }

    std::vector<PGlobalDigi::SiStripDigi> TECW8Digis =
      srcGlobalDigis.getTECW8Digis();      
    hSiStripn[18]->Fill((float)nTECW8Digis);
    for (Int_t i = 0; i < TECW8Digis.size(); ++i) {
      hSiStripADC[18]->Fill(TECW8Digis[i].ADC);
      hSiStripStrip[18]->Fill(TECW8Digis[i].STRIP);
    }

    // get SiPixel info
    std::vector<PGlobalDigi::SiPixelDigi> BRL1Digis =
      srcGlobalDigis.getBRL1Digis();      
    hSiPixeln[0]->Fill((float)nBRL1Digis);
    for (Int_t i = 0; i < BRL1Digis.size(); ++i) {
      hSiPixelADC[0]->Fill(BRL1Digis[i].ADC);
      hSiPixelRow[0]->Fill(BRL1Digis[i].ROW);
      hSiPixelCol[0]->Fill(BRL1Digis[i].COLUMN);
    }

    std::vector<PGlobalDigi::SiPixelDigi> BRL2Digis =
      srcGlobalDigis.getBRL2Digis();      
    hSiPixeln[1]->Fill((float)nBRL2Digis);
    for (Int_t i = 0; i < BRL2Digis.size(); ++i) {
      hSiPixelADC[1]->Fill(BRL2Digis[i].ADC);
      hSiPixelRow[1]->Fill(BRL2Digis[i].ROW);
      hSiPixelCol[1]->Fill(BRL2Digis[i].COLUMN);
    }

    std::vector<PGlobalDigi::SiPixelDigi> BRL3Digis =
      srcGlobalDigis.getBRL3Digis();      
    hSiPixeln[2]->Fill((float)nBRL3Digis);
    for (Int_t i = 0; i < BRL3Digis.size(); ++i) {
      hSiPixelADC[2]->Fill(BRL3Digis[i].ADC);
      hSiPixelRow[2]->Fill(BRL3Digis[i].ROW);
      hSiPixelCol[2]->Fill(BRL3Digis[i].COLUMN); 
   }

    std::vector<PGlobalDigi::SiPixelDigi> FWD1pDigis =
      srcGlobalDigis.getFWD1pDigis();      
    hSiPixeln[3]->Fill((float)nFWD1pDigis);
    for (Int_t i = 0; i < FWD1pDigis.size(); ++i) {
      hSiPixelADC[3]->Fill(FWD1pDigis[i].ADC);
      hSiPixelRow[3]->Fill(FWD1pDigis[i].ROW);
      hSiPixelCol[3]->Fill(FWD1pDigis[i].COLUMN);
    }

    std::vector<PGlobalDigi::SiPixelDigi> FWD1nDigis =
      srcGlobalDigis.getFWD1nDigis();      
    hSiPixeln[4]->Fill((float)nFWD1nDigis);
    for (Int_t i = 0; i < FWD1nDigis.size(); ++i) {
      hSiPixelADC[4]->Fill(FWD1nDigis[i].ADC);
      hSiPixelRow[4]->Fill(FWD1nDigis[i].ROW);
      hSiPixelCol[4]->Fill(FWD1nDigis[i].COLUMN);
    }

    std::vector<PGlobalDigi::SiPixelDigi> FWD2pDigis =
      srcGlobalDigis.getFWD2pDigis();      
    hSiPixeln[5]->Fill((float)nFWD2pDigis);
    for (Int_t i = 0; i < FWD2pDigis.size(); ++i) {
      hSiPixelADC[5]->Fill(FWD2pDigis[i].ADC);
      hSiPixelRow[5]->Fill(FWD2pDigis[i].ROW);
      hSiPixelCol[5]->Fill(FWD2pDigis[i].COLUMN);
    }

    std::vector<PGlobalDigi::SiPixelDigi> FWD2nDigis =
      srcGlobalDigis.getFWD2nDigis();      
    hSiPixeln[6]->Fill((float)nFWD2nDigis);
    for (Int_t i = 0; i < FWD2nDigis.size(); ++i) {
      hSiPixelADC[6]->Fill(FWD2nDigis[i].ADC);
      hSiPixelRow[6]->Fill(FWD2nDigis[i].ROW);
      hSiPixelCol[6]->Fill(FWD2nDigis[i].COLUMN);
    }

    // get DtMuon info
    std::vector<PGlobalDigi::DTDigi> MB1Digis =
      srcGlobalDigis.getMB1Digis();      
    hDtMuonn[0]->Fill((float)nMB1Digis);
    for (Int_t i = 0; i < MB1Digis.size(); ++i) {
      float layer = 4.0 * (MB1Digis[i].SLAYER - 1.0) + MB1Digis[i].LAYER;
      hDtMuonLayer[0]->Fill(layer);
      hDtMuonTime[0]->Fill(MB1Digis[i].TIME);
      hDtMuonTimevLayer[0]->Fill(layer,MB1Digis[i].TIME,1);
    }

    std::vector<PGlobalDigi::DTDigi> MB2Digis =
      srcGlobalDigis.getMB2Digis();      
    hDtMuonn[1]->Fill((float)nMB2Digis);
    for (Int_t i = 0; i < MB2Digis.size(); ++i) {
      float layer = 4.0 * (MB2Digis[i].SLAYER - 1.0) + MB2Digis[i].LAYER;
      hDtMuonLayer[1]->Fill(layer);
      hDtMuonTime[1]->Fill(MB2Digis[i].TIME);
      hDtMuonTimevLayer[1]->Fill(layer,MB2Digis[i].TIME,1);
    }

    std::vector<PGlobalDigi::DTDigi> MB3Digis =
      srcGlobalDigis.getMB3Digis();      
    hDtMuonn[2]->Fill((float)nMB3Digis);
    for (Int_t i = 0; i < MB3Digis.size(); ++i) {
      float layer = 4.0 * (MB3Digis[i].SLAYER - 1.0) + MB3Digis[i].LAYER;
      hDtMuonLayer[2]->Fill(layer);
      hDtMuonTime[2]->Fill(MB3Digis[i].TIME);
      hDtMuonTimevLayer[2]->Fill(layer,MB3Digis[i].TIME,1);
    }

    std::vector<PGlobalDigi::DTDigi> MB4Digis =
      srcGlobalDigis.getMB4Digis();      
    hDtMuonn[3]->Fill((float)nMB4Digis);
    for (Int_t i = 0; i < MB4Digis.size(); ++i) {
      float layer = 4.0 * (MB4Digis[i].SLAYER - 1.0) + MB4Digis[i].LAYER;
      hDtMuonLayer[3]->Fill(layer);
      hDtMuonTime[3]->Fill(MB4Digis[i].TIME);
      hDtMuonTimevLayer[3]->Fill(layer,MB4Digis[i].TIME,1);
    }

    // get CSC Strip info
    std::vector<PGlobalDigi::CSCstripDigi> CSCstripDigis =
      srcGlobalDigis.getCSCstripDigis();      
    hCSCStripn->Fill((float)nCSCstripDigis);
    for (Int_t i = 0; i < CSCstripDigis.size(); ++i) {
      hCSCStripADC->Fill(CSCstripDigis[i].ADC);
    }

    // get CSC Wire info
    std::vector<PGlobalDigi::CSCwireDigi> CSCwireDigis =
      srcGlobalDigis.getCSCwireDigis();      
    hCSCWiren->Fill((float)nCSCwireDigis);
    for (Int_t i = 0; i < CSCwireDigis.size(); ++i) {
      hCSCWireTime->Fill(CSCwireDigis[i].TIME);
    }

  } // end loop through events

  // output to root file
  f1->Write();

  // draw plots

  //ECal info
  TCanvas *Canvas1 = new TCanvas("ECalDigis","ECalDigis",cWidth,cHeight);
  Canvas1->Divide(2,3);
  myCanvas = Canvas1;
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(1);
    gPad->SetLogy();
    hEcaln[i]->Draw();
    myCanvas->cd(2);
    //gPad->SetLogy();
    hEcalAEE[i]->Draw();
    myCanvas->cd(3);
    gPad->SetLogy();
    hEcalSHE[i]->Draw();
    myCanvas->cd(4);
    //gPad->SetLogy();  
    hEcalMaxPos[i]->Draw();
    myCanvas->cd(5);
    //gPad->SetLogy();  
    hEcalMultvAEE[i]->Draw();
    myCanvas->cd(6);
    //gPad->SetLogy();  
    hEcalSHEvAEESHE[i]->Draw();
    myCanvas->Print(psfile);    
  }

  TCanvas *Canvas2 = new TCanvas("ESCalDigis","ESCalDigis",cWidth,cWidth);
  Canvas2->Divide(2,2);
  myCanvas = Canvas2; 
  myCanvas->cd(1);
  gPad->SetLogy();
  hEScaln->Draw();
  myCanvas->cd(2);
  //gPad->SetLogy();
  hEScalADC0->Draw();
  myCanvas->cd(3);
  //gPad->SetLogy();
  hEScalADC1->Draw();
  myCanvas->cd(4);
  //gPad->SetLogy();
  hEScalADC2->Draw();
  myCanvas->Print(psfile);

  //HCal info
  TCanvas *Canvas3 = new TCanvas("HCalDigis","HCalDigis",cWidth,cHeight);
  Canvas3->Divide(2,3);
  myCanvas = Canvas3;
  for (Int_t i = 0; i < 4; ++i) {
    myCanvas->cd(1);
    gPad->SetLogy();
    hHcaln[i]->Draw();
    myCanvas->cd(2);
    //gPad->SetLogy();
    hHcalAEE[i]->Draw();
    myCanvas->cd(3);
    gPad->SetLogy();
    hHcalSHE[i]->Draw();
    myCanvas->cd(4);
    //gPad->SetLogy();  
    hHcalAEESHE[i]->Draw();
    myCanvas->cd(5);
    //gPad->SetLogy();  
    hHcalSHEvAEE[i]->Draw();
    myCanvas->Print(psfile);    
  }

  //SiStrip info
  TCanvas *Canvas4 = new TCanvas("SiStripDigis","SiStripDigis",cWidth,cWidth);
  Canvas4->Divide(2,2);
  myCanvas = Canvas4;
  for (Int_t i = 0; i < 19; ++i) {
    myCanvas->cd(1);
    gPad->SetLogy();
    hSiStripn[i]->Draw();
    myCanvas->cd(2);
    //gPad->SetLogy();
    hSiStripADC[i]->Draw();
    myCanvas->cd(3);
    //gPad->SetLogy();
    hSiStripStrip[i]->Draw();
    myCanvas->Print(psfile);    
  }
 
  //SiPixel info
  TCanvas *Canvas5 = new TCanvas("SiPixelDigis","SiPixelDigis",cWidth,cWidth);
  Canvas5->Divide(2,2);
  myCanvas = Canvas5;
  for (Int_t i = 0; i < 7; ++i) {
    myCanvas->cd(1);
    gPad->SetLogy();
    hSiPixeln[i]->Draw();
    myCanvas->cd(2);
    gPad->SetLogy();
    hSiPixelADC[i]->Draw();
    myCanvas->cd(3);
    //gPad->SetLogy();
    hSiPixelRow[i]->Draw();
    myCanvas->cd(4);
    //gPad->SetLogy();
    hSiPixelCol[i]->Draw();
    myCanvas->Print(psfile);    
  }

  //DtMuon info
  TCanvas *Canvas6 = new TCanvas("DtMuonDigis","DtMuonDigis",cWidth,cWidth);
  Canvas6->Divide(2,2);
  myCanvas = Canvas6;
  for (Int_t i = 0; i < 4; ++i) {
    myCanvas->cd(1);
    gPad->SetLogy();
    hDtMuonn[i]->Draw();
    myCanvas->cd(2);
    //gPad->SetLogy();
    hDtMuonLayer[i]->Draw();
    myCanvas->cd(3);
    //gPad->SetLogy();
    hDtMuonTime[i]->Draw();
    myCanvas->cd(4);
    //gPad->SetLogy();
    hDtMuonTimevLayer[i]->Draw();
    myCanvas->Print(psfile);    
  }

  //CSCstripMuon info
  TCanvas *Canvas7 = new TCanvas("CSCstripDigis","CSCstripDigis",cWidth,
				 cWidth);
  Canvas7->Divide(2,2);
  myCanvas = Canvas7;
  myCanvas->cd(1);
  gPad->SetLogy();
  hCSCStripn->Draw();
  myCanvas->cd(2);
  //gPad->SetLogy();
  hCSCStripADC->Draw();
  myCanvas->Print(psfile);    

  //CSCwireMuon info
  TCanvas *Canvas8 = new TCanvas("CSCwireDigis","CSCwireDigis",cWidth,
				 cWidth);
  Canvas8->Divide(2,2);
  myCanvas = Canvas8;
  myCanvas->cd(1);
  gPad->SetLogy();
  hCSCWiren->Draw();
  myCanvas->cd(2);
  //gPad->SetLogy();
  hCSCWireTime->Draw();
  myCanvas->Print(psfile);    

  // close output ps file
  myCanvas->Print(psfileclose);
  
  // close root files
  f1->Close();
  srcfile->Close();

  return;
}
