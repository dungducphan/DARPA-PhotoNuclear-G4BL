#define Ana_cxx
#include "Ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Ana::Loop() {

   auto gamma_spec = new TH1D("gamma_spec", "gamma spectrum", 1000, 0, 200);
   auto gamma_spec_cut = new TH1D("gamma_spec_cut", "gamma spectrum cut", 1000, 0, 200);
   auto gamma_spec_cut_angle = new TH1D("gamma_spec_cut_angle", "gamma spectrum cut angle", 200, 0, TMath::PiOver2());
   auto gamma_spec_cut_profile = new TH2D("gamma_spec_cut_profile", "gamma spectrum cut profile", 2000, -500, 500, 2000, -500, 500);

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
      if (P >= 20) {
         gamma_spec_cut->Fill(P, Weight);
         gamma_spec_cut_angle->Fill(TMath::ATan(rho/50), Weight);
         gamma_spec_cut_profile->Fill(x, y, Weight);
      } 
   }

   auto outfile = new TFile("output.root", "UPDATE");
   auto *cdemom = outfile->GetDirectory(Form("Emom_%d", m_emom));
   if (!cdemom) {
      cdemom = outfile->mkdir(Form("Emom_%d", m_emom));
   }
   cdemom->cd();
   auto *cdthickness = cdemom->mkdir(Form("Thickness_%d", m_thickness));
   cdthickness->cd();
   gamma_spec->Write();
   gamma_spec_cut->Write();
   gamma_spec_cut_angle->Write();
   gamma_spec_cut_profile->Write();
   outfile->Write();
   outfile->Close();
}
