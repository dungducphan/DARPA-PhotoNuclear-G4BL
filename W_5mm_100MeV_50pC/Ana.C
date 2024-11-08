#define Ana_cxx
#include "Ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Ana::Loop() {

   auto gamma_spec = new TH1D("gamma_spec", "gamma spectrum", 1000, 0, 200);
   auto gamma_spec_lowE = new TH1D("gamma_spec_lowE", "gamma spectrum low energy", 1000, 0, 1);
   auto gamma_spec_lowE_angle = new TH1D("gamma_spec_lowE_angle", "gamma spectrum low energy angle", 1000, 0, TMath::PiOver2());
   

   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (PDGid != 22) continue;

      double P = TMath::Sqrt(Px*Px + Py*Py + Pz*Pz);
      double rho = TMath::Sqrt(x*x + y*y);
      gamma_spec->Fill(P, Weight);
      if (P <= 1) {
         gamma_spec_lowE->Fill(P, Weight);
         gamma_spec_lowE_angle->Fill(TMath::ATan(rho/50), Weight);
      } 
   }

   auto outfile = new TFile("output.root", "UPDATE");
   gamma_spec->Write();
   gamma_spec_lowE->Write();
   gamma_spec_lowE_angle->Write();
   outfile->Write();
   outfile->Close();
}
