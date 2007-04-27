///////////////////////////////////////////////////////////////////////////////
// Macro to produce histograms from the GlobalRecHitsProducer
//
// root -b -q MakeHistograms.C
///////////////////////////////////////////////////////////////////////////////
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"

void MakeHistograms(TString srcname="GlobalRecHits.root",
		    TString filename="GlobalRecHitsHistograms")
{
  gROOT->Reset();
  //http://root.cern.ch/root/html/src/TStyle.cxx.html#TStyle:SetOptStat
  gStyle->SetOptStat("emruo");

  // setup names
  //TString srcname = "GlobalRecHits.root";
  TString treename = "Events";
  TString brnchname = "PGlobalRecHit_globalrechits_GlobalRecHits_GLOBAL.obj";

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
  PGlobalRecHit srcGlobalRecHits;
  srcbrnch->SetAddress(&srcGlobalRecHits);

  // create canvas
  Int_t cWidth = 928, cHeight = 1218;
  TCanvas *myCanvas = new TCanvas("globalrechits","globalrechits",cWidth,cHeight);
  
  // open output ps file
  //TString filename = "GlobalRecHitsHistograms";
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
  TH1F *hEcaln[3];
  hEcaln[0] = 
    new TH1F("hEcaln_EE","EECal rechits",50,100.,300.);
  hEcaln[1] = 
    new TH1F("hEcaln_EB","EBCal rechits",200,2000.,3500.);
  hEcaln[2] =
    new TH1F("hEcaln_ES","ESCal rechits",50,100.,300.);
  TH1F *hEcalRes[3];
  hEcalRes[0] = 
    new TH1F("hEcalRes_EE","EECal rechit resolution",200,-1.,1.);
  hEcalRes[1] = 
    new TH1F("hEcalRes_EB","EBCal rechit resolution",200,-0.3.,0.3);
  hEcalRes[2] =
    new TH1F("hEcalRes_ES","ESCal rechit resolution",200,-0.0002,0.0002);

  for (Int_t i = 0; i < 3; ++i) {
    hEcaln[i]->GetXaxis()->SetTitle("Number of RecHits");
    hEcaln[i]->GetYaxis()->SetTitle("Count");
    hEcaln[i]->SetLineColor(srccolor);
    hEcaln[i]->SetLineWidth(linewidth);
    hEcalRes[i]->GetXaxis()->SetTitle("RecHit E - SimHit E");
    hEcalRes[i]->GetYaxis()->SetTitle("Count");
    hEcalRes[i]->SetLineColor(srccolor);
    hEcalRes[i]->SetLineWidth(linewidth);
  }

  // hcal
  TH1F *hHcaln[4];
  hHcaln[0] = 
    new TH1F("hHcaln_HB","HBCal rechits",500,2000.,3000.);
  hHcaln[1] = 
    new TH1F("hHcaln_HE","HECal rechits",500,2000.,3000.);
  hHcaln[2] = 
    new TH1F("hHcaln_HO","HOCal rechits",500,2000.,3000.);
  hHcaln[3] = 
    new TH1F("hHcaln_HF","HFCal rechits",500,1000.,2000.);
  TH1F *hHcalRes[4];
  hHcalRes[0] = 
    new TH1F("hHcalRes_HB","HBCal rechit resolution",25,-2.,2.);
  hHcalRes[1] = 
    new TH1F("hHcalRes_HE","HECal rechit resolution",25,-2.,2.);
  hHcalRes[2] = 
    new TH1F("hHcalRes_HO","HOCal rechit resolution",25,-2.,2.);
  hHcalRes[3] = 
    new TH1F("hHcalRes_HF","HFCal rechit resolution",25,-2.,2.);
  //float HCalRHCone[4] = 0.0;
  //float HCalSH[4] = 0.0;
  for (Int_t i = 0; i < 4; ++i) {
    hHcaln[i]->GetXaxis()->SetTitle("Number of RecHits");
    hHcaln[i]->GetYaxis()->SetTitle("Count");
    hHcaln[i]->SetLineColor(srccolor);
    hHcaln[i]->SetLineWidth(linewidth);
    hHcalRes[i]->GetXaxis()->SetTitle("RecHit E - SimHit E");
    hHcalRes[i]->GetYaxis()->SetTitle("Count");
    hHcalRes[i]->SetLineColor(srccolor);
    hHcalRes[i]->SetLineWidth(linewidth);  
  }

  // SiStrip
  TH1F *hSiStripn[19];
  hSiStripn[0] = 
    new TH1F("hSiStripn_TIBL1","TIBL1 rechits",25,0.,50.);
  hSiStripn[1] = 
    new TH1F("hSiStripn_TIBL2","TIBL2 rechits",25,0.,50.);
  hSiStripn[2] = 
    new TH1F("hSiStripn_TIBL3","TIBL3 rechits",25,0.,50.);
  hSiStripn[3] = 
    new TH1F("hSiStripn_TIBL4","TIBL4 rechits",25,0.,50.);
  hSiStripn[4] = 
    new TH1F("hSiStripn_TOBL1","TOBL1 rechits",25,0.,50.);
  hSiStripn[5] = 
    new TH1F("hSiStripn_TOBL2","TOBL2 rechits",25,0.,50.);
  hSiStripn[6] = 
    new TH1F("hSiStripn_TOBL3","TOBL3 rechits",25,0.,50.);
  hSiStripn[7] = 
    new TH1F("hSiStripn_TOBL4","TOBL4 rechits",25,0.,50.);
  hSiStripn[8] = 
    new TH1F("hSiStripn_TIDW1","TIDW1 rechits",10,0.,20.);
  hSiStripn[9] = 
    new TH1F("hSiStripn_TIDW2","TIDW2 rechits",10,0.,20.);
  hSiStripn[10] = 
    new TH1F("hSiStripn_TIDW3","TIDW3 rechits",10,0.,20.);
  hSiStripn[11] = 
    new TH1F("hSiStripn_TECW1","TECW1 rechits",10,0.,20.);
  hSiStripn[12] = 
    new TH1F("hSiStripn_TECW2","TECW2 rechits",10,0.,20.);
  hSiStripn[13] = 
    new TH1F("hSiStripn_TECW3","TECW3 rechits",10,0.,20.);
  hSiStripn[14] = 
    new TH1F("hSiStripn_TECW4","TECW4 rechits",10,0.,20.);
  hSiStripn[15] = 
    new TH1F("hSiStripn_TECW5","TECW5 rechits",10,0.,20.);
  hSiStripn[16] = 
    new TH1F("hSiStripn_TECW6","TECW6 rechits",10,0.,20.);
  hSiStripn[17] = 
    new TH1F("hSiStripn_TECW7","TECW7 rechits",10,0.,20.);
  hSiStripn[18] = 
    new TH1F("hSiStripn_TECW8","TECW8 rechits",10,0.,20.);
  TH1F *hSiStripResX[19];
  hSiStripResX[0] = 
    new TH1F("hSiStripResX_TIBL1","TIBL1 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[1] = 
    new TH1F("hSiStripResX_TIBL2","TIBL2 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[2] = 
    new TH1F("hSiStripResX_TIBL3","TIBL3 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[3] = 
    new TH1F("hSiStripResX_TIBL4","TIBL4 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[4] = 
    new TH1F("hSiStripResX_TOBL1","TOBL1 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[5] = 
    new TH1F("hSiStripResX_TOBL2","TOBL2 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[6] = 
    new TH1F("hSiStripResX_TOBL3","TOBL3 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[7] = 
    new TH1F("hSiStripResX_TOBL4","TOBL4 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[8] = 
    new TH1F("hSiStripResX_TIDW1","TIDW1 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[9] = 
    new TH1F("hSiStripResX_TIDW2","TIDW2 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[10] = 
    new TH1F("hSiStripResX_TIDW3","TIDW3 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[11] = 
    new TH1F("hSiStripResX_TECW1","TECW1 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[12] = 
    new TH1F("hSiStripResX_TECW2","TECW2 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[13] = 
    new TH1F("hSiStripResX_TECW3","TECW3 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[14] = 
    new TH1F("hSiStripResX_TECW4","TECW4 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[15] = 
    new TH1F("hSiStripResX_TECW5","TECW5 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[16] = 
    new TH1F("hSiStripResX_TECW6","TECW6 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[17] = 
    new TH1F("hSiStripResX_TECW7","TECW7 rechit x resolution",50,-0.05,0.05);
  hSiStripResX[18] = 
    new TH1F("hSiStripResX_TECW8","TECW8 rechit x resolution",50,-0.05,0.05);
  TH1F *hSiStripResY[19];
  hSiStripResY[0] = 
    new TH1F("hSiStripResY_TIBL1","TIBL1 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[1] = 
    new TH1F("hSiStripResY_TIBL2","TIBL2 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[2] = 
    new TH1F("hSiStripResY_TIBL3","TIBL3 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[3] = 
    new TH1F("hSiStripResY_TIBL4","TIBL4 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[4] = 
    new TH1F("hSiStripResY_TOBL1","TOBL1 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[5] = 
    new TH1F("hSiStripResY_TOBL2","TOBL2 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[6] = 
    new TH1F("hSiStripResY_TOBL3","TOBL3 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[7] = 
    new TH1F("hSiStripResY_TOBL4","TOBL4 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[8] = 
    new TH1F("hSiStripResY_TIDW1","TIDW1 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[9] = 
    new TH1F("hSiStripResY_TIDW2","TIDW2 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[10] = 
    new TH1F("hSiStripResY_TIDW3","TIDW3 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[11] = 
    new TH1F("hSiStripResY_TECW1","TECW1 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[12] = 
    new TH1F("hSiStripResY_TECW2","TECW2 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[13] = 
    new TH1F("hSiStripResY_TECW3","TECW3 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[14] = 
    new TH1F("hSiStripResY_TECW4","TECW4 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[15] = 
    new TH1F("hSiStripResY_TECW5","TECW5 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[16] = 
    new TH1F("hSiStripResY_TECW6","TECW6 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[17] = 
    new TH1F("hSiStripResY_TECW7","TECW7 rechit y resolution",50,-0.5,0.5);
  hSiStripResY[18] = 
    new TH1F("hSiStripResY_TECW8","TECW8 rechit y resolution",50,-0.5,0.5);
  for (Int_t i = 0; i < 19; ++i) {
    hSiStripn[i]->GetXaxis()->SetTitle("Number of RecHits");
    hSiStripn[i]->GetYaxis()->SetTitle("Count");
    hSiStripn[i]->SetLineColor(srccolor);
    hSiStripn[i]->SetLineWidth(linewidth);
    hSiStripResX[i]->GetXaxis()->SetTitle("RecHit X - SimHit X");
    hSiStripResX[i]->GetYaxis()->SetTitle("Count");
    hSiStripResX[i]->SetLineColor(srccolor);
    hSiStripResX[i]->SetLineWidth(linewidth);
    hSiStripResY[i]->GetXaxis()->SetTitle("RecHit Y - SimHit Y");
    hSiStripResY[i]->GetYaxis()->SetTitle("Count");
    hSiStripResY[i]->SetLineColor(srccolor);
    hSiStripResY[i]->SetLineWidth(linewidth);
  }

  // get siPixel
  TH1F *hSiPixeln[7];
  hSiPixeln[0] = 
    new TH1F("hSiPixeln_BRL1","BRL1 rechits",20,0.,20.);
  hSiPixeln[1] = 
    new TH1F("hSiPixeln_BRL2","BRL2 rechits",20,0.,20.);
  hSiPixeln[2] = 
    new TH1F("hSiPixeln_BRL3","BRL3 rechits",20,0.,20.);
  hSiPixeln[3] = 
    new TH1F("hSiPixeln_FWD1p","FWD1p rechits",20,0.,20.);
  hSiPixeln[4] = 
    new TH1F("hSiPixeln_FWD1n","FWD1n rechits",20,0.,20.);
  hSiPixeln[5] = 
    new TH1F("hSiPixeln_FWD2p","FWD2p rechits",20,0.,20.);
  hSiPixeln[6] = 
    new TH1F("hSiPixeln_FWD2n","FWD2n rechits",20,0.,20.);
  TH1F *hSiPixelResX[7];
  hSiPixelResX[0] = 
    new TH1F("hSiPixelResX_BRL1","BRL1 rechit x resolution",200,-0.02,0.02);
  hSiPixelResX[1] = 
    new TH1F("hSiPixelResX_BRL2","BRL2 rechit x resolution",200,-0.02,0.02);
  hSiPixelResX[2] = 
    new TH1F("hSiPixelResX_BRL3","BRL3 rechit x resolution",200,-0.02,0.02);
  hSiPixelResX[3] = 
    new TH1F("hSiPixelResX_FWD1p","FWD1p rechit x resolution",200,-0.02,0.02);
  hSiPixelResX[4] = 
    new TH1F("hSiPixelResX_FWD1n","FWD1n rechit x resolution",200,-0.02,0.02);
  hSiPixelResX[5] = 
    new TH1F("hSiPixelResX_FWD2p","FWD2p rechit x resolution",200,-0.02,0.02);
  hSiPixelResX[6] = 
    new TH1F("hSiPixelResX_FWD2n","FWD2n rechit x resolution",200,-0.02,0.02);
  TH1F *hSiPixelResY[7];
  hSiPixelResY[0] = 
    new TH1F("hSiPixelResY_BRL1","BRL1 rechit y resolution",100,-0.02,0.02);
  hSiPixelResY[1] = 
    new TH1F("hSiPixelResY_BRL2","BRL2 rechit y resolution",100,-0.02,0.02);
  hSiPixelResY[2] = 
    new TH1F("hSiPixelResY_BRL3","BRL3 rechit y resolution",100,-0.02,0.02);
  hSiPixelResY[3] = 
    new TH1F("hSiPixelResY_FWD1p","FWD1p rechit y resolution",100,-0.02,0.02);
  hSiPixelResY[4] = 
    new TH1F("hSiPixelResY_FWD1n","FWD1n rechit y resolution",100,-0.02,0.02);
  hSiPixelResY[5] = 
    new TH1F("hSiPixelResY_FWD2p","FWD2p rechit y resolution",100,-0.02,0.02);
  hSiPixelResY[6] = 
    new TH1F("hSiPixelResY_FWD2n","FWD2n rechit y resolution",100,-0.02,0.02);
  for (Int_t i = 0; i < 7; ++i) {
    hSiPixeln[i]->GetXaxis()->SetTitle("Number of RecHits");
    hSiPixeln[i]->GetYaxis()->SetTitle("Count");
    hSiPixeln[i]->SetLineColor(srccolor);
    hSiPixeln[i]->SetLineWidth(linewidth);
    hSiPixelResX[i]->GetXaxis()->SetTitle("RecHit X - SimHit X");
    hSiPixelResX[i]->GetYaxis()->SetTitle("Count");
    hSiPixelResX[i]->SetLineColor(srccolor);
    hSiPixelResX[i]->SetLineWidth(linewidth);
    hSiPixelResY[i]->GetXaxis()->SetTitle("RecHit Y - SimHit Y");
    hSiPixelResY[i]->GetYaxis()->SetTitle("Count");
    hSiPixelResY[i]->SetLineColor(srccolor);
    hSiPixelResY[i]->SetLineWidth(linewidth);
  }

  TH1F *hDtMuonn;
  hDtMuonn = 
    new TH1F("hDtMuonn","DT rechits",25,0.,50.);
  TH1F *hDtMuonRes;
  hDtMuonRes = 
    new TH1F("hDtMuonRes","DT wire distance resolution",200,-0.2,0.2);
  hDtMuonn->GetXaxis()->SetTitle("Number of RecHits");
  hDtMuonn->GetYaxis()->SetTitle("Count");
  hDtMuonn->SetLineColor(srccolor);
  hDtMuonn->SetLineWidth(linewidth);  
  hDtMuonRes->GetXaxis()->SetTitle("RecHit wire dist - SimHit wire dist");
  hDtMuonRes->GetYaxis()->SetTitle("Count");
  hDtMuonRes->SetLineColor(srccolor);
  hDtMuonRes->SetLineWidth(linewidth);

  TH1F *hCSCn;
  hCSCn = 
    new TH1F("hCSCn","CSC rechits",25,0.,50.);
  TH1F *hCSCResRDPhi;
  hCSCResRDPhi = 
    new TH1F("hCSCResRDPhi","CSC perp*dphi resolution",200,-0.2,0.2);
  hCSCn->GetXaxis()->SetTitle("Number of RecHits");
  hCSCn->GetYaxis()->SetTitle("Count");
  hCSCn->SetLineColor(srccolor);
  hCSCn->SetLineWidth(linewidth);
  hCSCResRDPhi->GetXaxis()->
    SetTitle("RecHit.perp * (RecHit.phi - SimHit.phi)");
  hCSCResRDPhi->GetYaxis()->SetTitle("Count");
  hCSCResRDPhi->SetLineColor(srccolor);
  hCSCResRDPhi->SetLineWidth(linewidth);

  TH1F *hRPCn;
  hRPCn = 
    new TH1F("hRPCn","RPC rechits",25,0.,50.);
  TH1F *hRPCResX;
  hRPCResX = 
    new TH1F("hRPCResX","RPC rechtis x resoltuion",50,-5.,5.);
  hRPCn->GetXaxis()->SetTitle("Number of RecHits");
  hRPCn->GetYaxis()->SetTitle("Count");
  hRPCn->SetLineColor(srccolor);
  hRPCn->SetLineWidth(linewidth);
  hRPCResX->GetXaxis()->SetTitle("RecHit X - SimHit X");
  hRPCResX->GetYaxis()->SetTitle("Count");
  hRPCResX->SetLineColor(srccolor);
  hRPCResX->SetLineWidth(linewidth);

  // cycle through events in tree
  for (Int_t evt = 0; evt < nsrcevts; ++evt) {
    Int_t nevt = evt+1;

    if (nevt%100 == 0 || nevt == 1) {
      cout << "     Processing event " << nevt << endl;
    }

    srcbrnch->GetEntry(evt);

    int nEBCalRecHits = srcGlobalRecHits.getnEBCalRecHits();
    int nEECalRecHits = srcGlobalRecHits.getnEECalRecHits();
    int nESCalRecHits = srcGlobalRecHits.getnESCalRecHits();

    int nHBCalRecHits = srcGlobalRecHits.getnHBCalRecHits();
    int nHECalRecHits = srcGlobalRecHits.getnHECalRecHits();
    int nHOCalRecHits = srcGlobalRecHits.getnHOCalRecHits();
    int nHFCalRecHits = srcGlobalRecHits.getnHFCalRecHits();        

    int nTIBL1RecHits = srcGlobalRecHits.getnTIBL1RecHits();    
    int nTIBL2RecHits = srcGlobalRecHits.getnTIBL2RecHits();    
    int nTIBL3RecHits = srcGlobalRecHits.getnTIBL3RecHits();    
    int nTIBL4RecHits = srcGlobalRecHits.getnTIBL4RecHits();    
    int nTOBL1RecHits = srcGlobalRecHits.getnTOBL1RecHits();    
    int nTOBL2RecHits = srcGlobalRecHits.getnTOBL2RecHits();    
    int nTOBL3RecHits = srcGlobalRecHits.getnTOBL3RecHits();    
    int nTOBL4RecHits = srcGlobalRecHits.getnTOBL4RecHits();    
    int nTIDW1RecHits = srcGlobalRecHits.getnTIDW1RecHits();    
    int nTIDW2RecHits = srcGlobalRecHits.getnTIDW2RecHits();    
    int nTIDW3RecHits = srcGlobalRecHits.getnTIDW3RecHits();    
    int nTECW1RecHits = srcGlobalRecHits.getnTECW1RecHits();    
    int nTECW2RecHits = srcGlobalRecHits.getnTECW2RecHits();    
    int nTECW3RecHits = srcGlobalRecHits.getnTECW3RecHits();  
    int nTECW4RecHits = srcGlobalRecHits.getnTECW4RecHits();    
    int nTECW5RecHits = srcGlobalRecHits.getnTECW5RecHits();    
    int nTECW6RecHits = srcGlobalRecHits.getnTECW6RecHits();  
    int nTECW7RecHits = srcGlobalRecHits.getnTECW7RecHits();    
    int nTECW8RecHits = srcGlobalRecHits.getnTECW8RecHits();

    int nBRL1RecHits = srcGlobalRecHits.getnBRL1RecHits();    
    int nBRL2RecHits = srcGlobalRecHits.getnBRL2RecHits();    
    int nBRL3RecHits = srcGlobalRecHits.getnBRL3RecHits();       
    int nFWD1nRecHits = srcGlobalRecHits.getnFWD1nRecHits();
    int nFWD1pRecHits = srcGlobalRecHits.getnFWD1pRecHits();    
    int nFWD2nRecHits = srcGlobalRecHits.getnFWD2nRecHits();    
    int nFWD2pRecHits = srcGlobalRecHits.getnFWD2pRecHits(); 
  
    int nDTRecHits = srcGlobalRecHits.getnDTRecHits();  

    int nCSCRecHits = srcGlobalRecHits.getnCSCRecHits();

    int nRPCRecHits = srcGlobalRecHits.getnRPCRecHits();

    // get Ecal info
    std::vector<PGlobalRecHit::ECalRecHit> EECalRecHits = 
      srcGlobalRecHits.getEECalRecHits();
    hEcaln[0]->Fill((float)nEECalRecHits);
    for (Int_t i = 0; i < EECalRecHits.size(); ++i) {
      hEcalRes[0]->Fill(EECalRecHits[i].RE - EECalRecHits[i].SHE);
    }
    
    std::vector<PGlobalRecHit::ECalRecHit> EBCalRecHits = 
      srcGlobalRecHits.getEBCalRecHits();
    hEcaln[1]->Fill((float)nEBCalRecHits);
    for (Int_t i = 0; i < EBCalRecHits.size(); ++i) {
      hEcalRes[1]->Fill(EBCalRecHits[i].RE - EBCalRecHits[i].SHE);
    }

    std::vector<PGlobalRecHit::ECalRecHit> ESCalRecHits = 
      srcGlobalRecHits.getESCalRecHits();
    hEcaln[2]->Fill((float)nESCalRecHits);
    for (Int_t i = 0; i < ESCalRecHits.size(); ++i) {
      hEcalRes[2]->Fill(ESCalRecHits[i].RE - ESCalRecHits[i].SHE);
    }

    // Get HCal info
    std::vector<PGlobalRecHit::HCalRecHit> HBCalRecHits = 
      srcGlobalRecHits.getHBCalRecHits();
    hHcaln[0]->Fill((float)nHBCalRecHits);
    for (Int_t i = 0; i < HBCalRecHits.size(); ++i) {
      hHcalRes[0]->Fill(HBCalRecHits[i].REC - HBCalRecHits[i].SHE); 
    }

    std::vector<PGlobalRecHit::HCalRecHit> HECalRecHits = 
      srcGlobalRecHits.getHECalRecHits();
    hHcaln[1]->Fill((float)nHECalRecHits);
    for (Int_t i = 0; i < HECalRecHits.size(); ++i) {
      hHcalRes[1]->Fill(HECalRecHits[i].REC - HECalRecHits[i].SHE); 
    }

    std::vector<PGlobalRecHit::HCalRecHit> HOCalRecHits = 
      srcGlobalRecHits.getHOCalRecHits();
    hHcaln[2]->Fill((float)nHOCalRecHits);
    for (Int_t i = 0; i < HOCalRecHits.size(); ++i) {
      hHcalRes[2]->Fill(HOCalRecHits[i].REC - HOCalRecHits[i].SHE); 
    }

    std::vector<PGlobalRecHit::HCalRecHit> HFCalRecHits = 
      srcGlobalRecHits.getHFCalRecHits();
    hHcaln[3]->Fill((float)nHFCalRecHits);
    for (Int_t i = 0; i < HFCalRecHits.size(); ++i) {
      hHcalRes[3]->Fill(HFCalRecHits[i].REC - HFCalRecHits[i].SHE); 
    }

    // get SiStrip info
    std::vector<PGlobalRecHit::SiStripRecHit> TIBL1RecHits =
      srcGlobalRecHits.getTIBL1RecHits();      
    hSiStripn[0]->Fill((float)nTIBL1RecHits);
    for (Int_t i = 0; i < TIBL1RecHits.size(); ++i) {
      hSiStripResX[0]->Fill(TIBL1RecHits[i].RX - TIBL1RecHits[i].SX);
      hSiStripResY[0]->Fill(TIBL1RecHits[i].RY - TIBL1RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TIBL2RecHits =
      srcGlobalRecHits.getTIBL2RecHits();      
    hSiStripn[1]->Fill((float)nTIBL2RecHits);
    for (Int_t i = 0; i < TIBL2RecHits.size(); ++i) {
      hSiStripResX[1]->Fill(TIBL2RecHits[i].RX - TIBL2RecHits[i].SX);
      hSiStripResY[1]->Fill(TIBL2RecHits[i].RY - TIBL2RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TIBL3RecHits =
      srcGlobalRecHits.getTIBL3RecHits();      
    hSiStripn[2]->Fill((float)nTIBL3RecHits);
    for (Int_t i = 0; i < TIBL3RecHits.size(); ++i) {
      hSiStripResX[2]->Fill(TIBL3RecHits[i].RX - TIBL3RecHits[i].SX);
      hSiStripResY[2]->Fill(TIBL3RecHits[i].RY - TIBL3RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TIBL4RecHits =
      srcGlobalRecHits.getTIBL4RecHits();      
    hSiStripn[3]->Fill((float)nTIBL4RecHits);
    for (Int_t i = 0; i < TIBL4RecHits.size(); ++i) {
      hSiStripResX[3]->Fill(TIBL4RecHits[i].RX - TIBL4RecHits[i].SX);
      hSiStripResY[3]->Fill(TIBL4RecHits[i].RY - TIBL4RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TOBL1RecHits =
      srcGlobalRecHits.getTOBL1RecHits();      
    hSiStripn[4]->Fill((float)nTOBL1RecHits);
    for (Int_t i = 0; i < TOBL1RecHits.size(); ++i) {
      hSiStripResX[4]->Fill(TOBL1RecHits[i].RX - TOBL1RecHits[i].SX);
      hSiStripResY[4]->Fill(TOBL1RecHits[i].RY - TOBL1RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TOBL2RecHits =
      srcGlobalRecHits.getTOBL2RecHits();      
    hSiStripn[5]->Fill((float)nTOBL2RecHits);
    for (Int_t i = 0; i < TOBL2RecHits.size(); ++i) {
      hSiStripResX[5]->Fill(TOBL2RecHits[i].RX - TOBL2RecHits[i].SX);
      hSiStripResY[5]->Fill(TOBL2RecHits[i].RY - TOBL2RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TOBL3RecHits =
      srcGlobalRecHits.getTOBL3RecHits();      
    hSiStripn[6]->Fill((float)nTOBL3RecHits);
    for (Int_t i = 0; i < TOBL3RecHits.size(); ++i) {
      hSiStripResX[6]->Fill(TOBL3RecHits[i].RX - TOBL3RecHits[i].SX);
      hSiStripResY[6]->Fill(TOBL3RecHits[i].RY - TOBL3RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TOBL4RecHits =
      srcGlobalRecHits.getTOBL4RecHits();      
    hSiStripn[7]->Fill((float)nTOBL4RecHits);
    for (Int_t i = 0; i < TOBL4RecHits.size(); ++i) {
      hSiStripResX[7]->Fill(TOBL4RecHits[i].RX - TOBL4RecHits[i].SX);
      hSiStripResY[7]->Fill(TOBL4RecHits[i].RY - TOBL4RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TIDW1RecHits =
      srcGlobalRecHits.getTIDW1RecHits();      
    hSiStripn[8]->Fill((float)nTIDW1RecHits);
    for (Int_t i = 0; i < TIDW1RecHits.size(); ++i) {
      hSiStripResX[8]->Fill(TIDW1RecHits[i].RX - TIDW1RecHits[i].SX);
      hSiStripResY[8]->Fill(TIDW1RecHits[i].RY - TIDW1RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TIDW2RecHits =
      srcGlobalRecHits.getTIDW2RecHits();      
    hSiStripn[9]->Fill((float)nTIDW2RecHits);
    for (Int_t i = 0; i < TIDW2RecHits.size(); ++i) {
      hSiStripResX[9]->Fill(TIDW2RecHits[i].RX - TIDW2RecHits[i].SX);
      hSiStripResY[9]->Fill(TIDW2RecHits[i].RY - TIDW2RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TIDW3RecHits =
      srcGlobalRecHits.getTIDW3RecHits();      
    hSiStripn[10]->Fill((float)nTIDW3RecHits);
    for (Int_t i = 0; i < TIDW3RecHits.size(); ++i) {
      hSiStripResX[10]->Fill(TIDW3RecHits[i].RX - TIDW3RecHits[i].SX);
      hSiStripResY[10]->Fill(TIDW3RecHits[i].RY - TIDW3RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TECW1RecHits =
      srcGlobalRecHits.getTECW1RecHits();      
    hSiStripn[11]->Fill((float)nTECW1RecHits);
    for (Int_t i = 0; i < TECW1RecHits.size(); ++i) {
      hSiStripResX[11]->Fill(TECW1RecHits[i].RX - TECW1RecHits[i].SX);
      hSiStripResY[11]->Fill(TECW1RecHits[i].RY - TECW1RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TECW2RecHits =
      srcGlobalRecHits.getTECW2RecHits();      
    hSiStripn[12]->Fill((float)nTECW2RecHits);
    for (Int_t i = 0; i < TECW2RecHits.size(); ++i) {
      hSiStripResX[12]->Fill(TECW2RecHits[i].RX - TECW2RecHits[i].SX);
      hSiStripResY[12]->Fill(TECW2RecHits[i].RY - TECW2RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TECW3RecHits =
      srcGlobalRecHits.getTECW3RecHits();      
    hSiStripn[13]->Fill((float)nTECW3RecHits);
    for (Int_t i = 0; i < TECW3RecHits.size(); ++i) {
      hSiStripResX[13]->Fill(TECW3RecHits[i].RX - TECW3RecHits[i].SX);
      hSiStripResY[13]->Fill(TECW3RecHits[i].RY - TECW3RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TECW4RecHits =
      srcGlobalRecHits.getTECW4RecHits();      
    hSiStripn[14]->Fill((float)nTECW4RecHits);
    for (Int_t i = 0; i < TECW4RecHits.size(); ++i) {
      hSiStripResX[14]->Fill(TECW4RecHits[i].RX - TECW4RecHits[i].SX);
      hSiStripResY[14]->Fill(TECW4RecHits[i].RY - TECW4RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TECW5RecHits =
      srcGlobalRecHits.getTECW5RecHits();      
    hSiStripn[15]->Fill((float)nTECW5RecHits);
    for (Int_t i = 0; i < TECW5RecHits.size(); ++i) {
      hSiStripResX[15]->Fill(TECW5RecHits[i].RX - TECW5RecHits[i].SX);
      hSiStripResY[15]->Fill(TECW5RecHits[i].RY - TECW5RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TECW6RecHits =
      srcGlobalRecHits.getTECW6RecHits();      
    hSiStripn[16]->Fill((float)nTECW6RecHits);
    for (Int_t i = 0; i < TECW6RecHits.size(); ++i) {
      hSiStripResX[16]->Fill(TECW6RecHits[i].RX - TECW6RecHits[i].SX);
      hSiStripResY[16]->Fill(TECW6RecHits[i].RY - TECW6RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TECW7RecHits =
      srcGlobalRecHits.getTECW7RecHits();      
    hSiStripn[17]->Fill((float)nTECW7RecHits);
    for (Int_t i = 0; i < TECW7RecHits.size(); ++i) {
      hSiStripResX[17]->Fill(TECW7RecHits[i].RX - TECW7RecHits[i].SX);
      hSiStripResY[17]->Fill(TECW7RecHits[i].RY - TECW7RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiStripRecHit> TECW8RecHits =
      srcGlobalRecHits.getTECW8RecHits();      
    hSiStripn[18]->Fill((float)nTECW8RecHits);
    for (Int_t i = 0; i < TECW8RecHits.size(); ++i) {
      hSiStripResX[18]->Fill(TECW8RecHits[i].RX - TECW8RecHits[i].SX);
      hSiStripResY[18]->Fill(TECW8RecHits[i].RY - TECW8RecHits[i].SY);
    }

    // get SiPixel info
    std::vector<PGlobalRecHit::SiPixelRecHit> BRL1RecHits =
      srcGlobalRecHits.getBRL1RecHits();      
    hSiPixeln[0]->Fill((float)nBRL1RecHits);
    for (Int_t i = 0; i < BRL1RecHits.size(); ++i) {
      hSiPixelResX[0]->Fill(BRL1RecHits[i].RX - BRL1RecHits[i].SX);
      hSiPixelResY[0]->Fill(BRL1RecHits[i].RY - BRL1RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiPixelRecHit> BRL2RecHits =
      srcGlobalRecHits.getBRL2RecHits();      
    hSiPixeln[1]->Fill((float)nBRL2RecHits);
    for (Int_t i = 0; i < BRL2RecHits.size(); ++i) {
      hSiPixelResX[1]->Fill(BRL2RecHits[i].RX - BRL2RecHits[i].SX);
      hSiPixelResY[1]->Fill(BRL2RecHits[i].RY - BRL2RecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiPixelRecHit> BRL3RecHits =
      srcGlobalRecHits.getBRL3RecHits();      
    hSiPixeln[2]->Fill((float)nBRL3RecHits);
    for (Int_t i = 0; i < BRL3RecHits.size(); ++i) {
      hSiPixelResX[2]->Fill(BRL3RecHits[i].RX - BRL3RecHits[i].SX);
      hSiPixelResY[2]->Fill(BRL3RecHits[i].RY - BRL3RecHits[i].SY);
   }

    std::vector<PGlobalRecHit::SiPixelRecHit> FWD1pRecHits =
      srcGlobalRecHits.getFWD1pRecHits();      
    hSiPixeln[3]->Fill((float)nFWD1pRecHits);
    for (Int_t i = 0; i < FWD1pRecHits.size(); ++i) {
      hSiPixelResX[3]->Fill(FWD1pRecHits[i].RX - FWD1pRecHits[i].SX);
      hSiPixelResY[3]->Fill(FWD1pRecHits[i].RY - FWD1pRecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiPixelRecHit> FWD1nRecHits =
      srcGlobalRecHits.getFWD1nRecHits();      
    hSiPixeln[4]->Fill((float)nFWD1nRecHits);
    for (Int_t i = 0; i < FWD1nRecHits.size(); ++i) {
      hSiPixelResX[4]->Fill(FWD1nRecHits[i].RX - FWD1nRecHits[i].SX);
      hSiPixelResY[4]->Fill(FWD1nRecHits[i].RY - FWD1nRecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiPixelRecHit> FWD2pRecHits =
      srcGlobalRecHits.getFWD2pRecHits();      
    hSiPixeln[5]->Fill((float)nFWD2pRecHits);
    for (Int_t i = 0; i < FWD2pRecHits.size(); ++i) {
      hSiPixelResX[5]->Fill(FWD2pRecHits[i].RX - FWD2pRecHits[i].SX);
      hSiPixelResY[5]->Fill(FWD2pRecHits[i].RY - FWD2pRecHits[i].SY);
    }

    std::vector<PGlobalRecHit::SiPixelRecHit> FWD2nRecHits =
      srcGlobalRecHits.getFWD2nRecHits();      
    hSiPixeln[6]->Fill((float)nFWD2nRecHits);
    for (Int_t i = 0; i < FWD2nRecHits.size(); ++i) {
      hSiPixelResX[6]->Fill(FWD2nRecHits[i].RX - FWD2nRecHits[i].SX);
      hSiPixelResY[6]->Fill(FWD2nRecHits[i].RY - FWD2nRecHits[i].SY);
    }

    // get DtMuon info
    std::vector<PGlobalRecHit::DTRecHit> DTRecHits =
      srcGlobalRecHits.getDTRecHits();      
    hDtMuonn->Fill((float)nDTRecHits);
    for (Int_t i = 0; i < DTRecHits.size(); ++i) {
      hDtMuonRes->Fill(DTRecHits[i].RHD - DTRecHits[i].SHD);
    }

    // get CSC info
    std::vector<PGlobalRecHit::CSCRecHit> CSCRecHits =
      srcGlobalRecHits.getCSCRecHits();      
    hCSCn->Fill((float)nCSCRecHits);
    for (Int_t i = 0; i < CSCRecHits.size(); ++i) {
      hCSCResRDPhi->Fill(CSCRecHits[i].RHPERP * 
			 (CSCRecHits[i].RHPHI - CSCRecHits[i].SHPHI));
    }

    // get RPC info
    std::vector<PGlobalRecHit::RPCRecHit> RPCRecHits =
      srcGlobalRecHits.getRPCRecHits();      
    hRPCn->Fill((float)nRPCRecHits);
    for (Int_t i = 0; i < RPCRecHits.size(); ++i) {
      hRPCResX->Fill(RPCRecHits[i].RHX - RPCRecHits[i].SHX);
    }

  } // end loop through events

  // output to root file
  f1->Write();

  // draw plots

  //ECal info
  TCanvas *Canvas1 = new TCanvas("ECalRecHits","ECalRecHits",cWidth,cWidth);
  Canvas1->Divide(2,2);
  myCanvas = Canvas1;
  for (Int_t i = 0; i < 3; ++i) {
    myCanvas->cd(1);
    gPad->SetLogy();
    hEcaln[i]->Draw();
    myCanvas->cd(2);
    //gPad->SetLogy();
    hEcalRes[i]->Draw();
    myCanvas->Print(psfile);    
  }

  //HCal info
  TCanvas *Canvas2 = new TCanvas("HCalRecHits","HCalRecHits",cWidth,cWidth);
  Canvas2->Divide(2,2);
  myCanvas = Canvas2;
  for (Int_t i = 0; i < 4; ++i) {
    myCanvas->cd(1);
    gPad->SetLogy();
    hHcaln[i]->Draw();
    myCanvas->cd(2);
    //gPad->SetLogy();
    hHcalRes[i]->Draw();
    myCanvas->Print(psfile);    
  }

  //SiStrip info
  TCanvas *Canvas3 = new TCanvas("SiStripRecHits","SiStripRecHits",cWidth,
				 cWidth);
  Canvas3->Divide(2,2);
  myCanvas = Canvas3;
  for (Int_t i = 0; i < 19; ++i) {
    myCanvas->cd(1);
    gPad->SetLogy();
    hSiStripn[i]->Draw();
    myCanvas->cd(2);
    //gPad->SetLogy();
    hSiStripResX[i]->Draw();
    myCanvas->cd(3);
    //gPad->SetLogy();
    hSiStripResY[i]->Draw();
    myCanvas->Print(psfile);    
  }
 
  //SiPixel info
  TCanvas *Canvas4 = new TCanvas("SiPixelRecHits","SiPixelRecHits",cWidth,
				 cWidth);
  Canvas4->Divide(2,2);
  myCanvas = Canvas4;
  for (Int_t i = 0; i < 7; ++i) {
    myCanvas->cd(1);
    gPad->SetLogy();
    hSiPixeln[i]->Draw();
    myCanvas->cd(2);
    //gPad->SetLogy();
    hSiPixelResX[i]->Draw();
    myCanvas->cd(3);
    //gPad->SetLogy();
    hSiPixelResY[i]->Draw();
    myCanvas->Print(psfile);    
  }

  //DtMuon info
  TCanvas *Canvas5 = new TCanvas("DtMuonRecHits","DtMuonRecHits",cWidth,
				 cWidth);
  Canvas5->Divide(2,2);
  myCanvas = Canvas5;
  myCanvas->cd(1);
  gPad->SetLogy();
  hDtMuonn->Draw();
  myCanvas->cd(2);
  //gPad->SetLogy();
  hDtMuonRes->Draw();
  myCanvas->Print(psfile);    

  //CSC Muon info
  TCanvas *Canvas6 = new TCanvas("CSCRecHits","CSCRecHits",cWidth,
				 cWidth);
  Canvas6->Divide(2,2);
  myCanvas = Canvas6;
  myCanvas->cd(1);
  gPad->SetLogy();
  hCSCn->Draw();
  myCanvas->cd(2);
  //gPad->SetLogy();
  hCSCResRDPhi->Draw();
  myCanvas->Print(psfile);    

  //RPC Muon info
  TCanvas *Canvas7 = new TCanvas("RPCRecHits","RPCRecHits",cWidth,
				 cWidth);
  Canvas7->Divide(2,2);
  myCanvas = Canvas7;
  myCanvas->cd(1);
  gPad->SetLogy();
  hRPCn->Draw();
  myCanvas->cd(2);
  //gPad->SetLogy();
  hRPCResX->Draw();
  myCanvas->Print(psfile);    

  // close output ps file
  myCanvas->Print(psfileclose);
  
  // close root files
  f1->Close();
  srcfile->Close();

  return;
}
