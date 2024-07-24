std::vector<int> emoms = {80, 100};
std::vector<int> thicknesses = {2, 4, 6, 8, 10};
std::vector<int> colors = {kBlack, kRed, kBlue, kGreen, kOrange, kCyan, kPink, kViolet};

void GetHistograms(std::vector<TH1D*>& gamma_specs, std::vector<TH1D*>& gamma_specs_cut, std::vector<TH1D*>& gamma_specs_cut_angle) {
    auto infile = new TFile("output.root", "READ");
    for (auto emom : emoms) {
        auto cdemom = infile->GetDirectory(Form("Emom_%d", emom));
        if (!cdemom) continue;
        for (auto thickness : thicknesses) {
            auto cdthickness = cdemom->GetDirectory(Form("Thickness_%d", thickness));
            if (!cdthickness) continue;
            auto gamma_spec = (TH1D*) cdthickness->Get("gamma_spec");
            auto gamma_spec_cut = (TH1D*) cdthickness->Get("gamma_spec_cut");
            auto gamma_spec_cut_angle = (TH1D*) cdthickness->Get("gamma_spec_cut_angle");
            gamma_specs.push_back(gamma_spec);
            gamma_specs_cut.push_back(gamma_spec_cut);
            gamma_specs_cut_angle.push_back(gamma_spec_cut_angle);
        }
    }
}

void GetProfiles(std::vector<TH2D*>& gamma_specs_cut_profile) {
    auto infile = new TFile("output.root", "READ");
    for (auto emom : emoms) {
        auto cdemom = infile->GetDirectory(Form("Emom_%d", emom));
        if (!cdemom) continue;
        for (auto thickness : thicknesses) {
            if (thickness != 4) continue;
            auto cdthickness = cdemom->GetDirectory(Form("Thickness_%d", thickness));
            if (!cdthickness) continue;
            auto gamma_spec_cut_profile = (TH2D*) cdthickness->Get("gamma_spec_cut_profile");
            gamma_specs_cut_profile.push_back(gamma_spec_cut_profile);
        }
    }
}

void PlotSpectrumAsFunctionOfThickness() {
    std::vector<TH1D*> gamma_specs;
    std::vector<TH1D*> gamma_specs_cut;
    std::vector<TH1D*> gamma_specs_cut_angle;
    GetHistograms(gamma_specs, gamma_specs_cut, gamma_specs_cut_angle);

    auto hstack_emom_100 = new THStack("hstack_emom_100", "Bremsstrahlung Spectra - Ebeam = 100 MeV");
    auto hstack_emom_80 = new THStack("hstack_emom_80", "Bremsstrahlung Spectra - Ebeam = 80 MeV");
    for (int i = 0; i < 10; i++) {
        gamma_specs[i]->Rebin(5);
        gamma_specs[i]->GetYaxis()->SetRangeUser(1, 1E8);
        if (i < 5) {
            gamma_specs[i]->SetLineColorAlpha(colors[i], 0.6);
            gamma_specs[i]->SetLineStyle(kSolid);
            gamma_specs[i]->SetLineWidth(2);
            hstack_emom_80->Add(gamma_specs[i]);
        } else {
            gamma_specs[i]->SetLineColorAlpha(colors[i - 5], 0.6);
            gamma_specs[i]->SetLineStyle(kSolid);
            gamma_specs[i]->SetLineWidth(2);
            hstack_emom_100->Add(gamma_specs[i]);
        }
    }

    TCanvas *c = new TCanvas("c", "c", 3600, 1800);
    c->Divide(2, 1);

    c->cd(1);
    gPad->SetLogy();
    hstack_emom_80->Draw("nostack");
    hstack_emom_80->GetXaxis()->SetTitle("Photon Energy [MeV]");
    hstack_emom_80->GetYaxis()->SetTitle("Counts (/MeV)");
    hstack_emom_80->GetXaxis()->CenterTitle();
    hstack_emom_80->GetYaxis()->CenterTitle();
    auto legend = new TLegend(0.55, 0.5, 0.87, 0.85);
    legend->SetBorderSize(0);
    for (int i = 0; i < 5; i++) {
        legend->AddEntry(gamma_specs[i], Form("W %d-mm (%3.2g)", thicknesses[i], gamma_specs[i]->Integral()), "l");
    }
    legend->Draw();
    gPad->Modified();
    gPad->Update();

    c->cd(2);
    gPad->SetLogy();
    hstack_emom_100->Draw("nostack");
    hstack_emom_100->GetXaxis()->SetTitle("Photon Energy [MeV]");
    hstack_emom_100->GetYaxis()->SetTitle("Counts (/MeV)");
    hstack_emom_100->GetXaxis()->CenterTitle();
    hstack_emom_100->GetYaxis()->CenterTitle();
    auto legend_2 = new TLegend(0.55, 0.5, 0.87, 0.85);
    legend_2->SetBorderSize(0);
    for (int i = 5; i < 10; i++) {
        legend_2->AddEntry(gamma_specs[i], Form("W %d-mm (%3.2g)", thicknesses[i-5], gamma_specs[i]->Integral()), "l");
    }
    legend_2->Draw();
    gPad->Modified();
    gPad->Update();

    c->SaveAs("GammaSpecAsFuncOfThickness.png");
    c->SaveAs("GammaSpecAsFuncOfThickness.pdf");
}

void PlotSpectrumCutoffAsFunctionOfThickness() {
    std::vector<TH1D*> gamma_specs;
    std::vector<TH1D*> gamma_specs_cut;
    std::vector<TH1D*> gamma_specs_cut_angle;
    GetHistograms(gamma_specs, gamma_specs_cut, gamma_specs_cut_angle);

    auto hstack_emom_100 = new THStack("hstack_emom_100", "Bremsstrahlung Spectra - Ebeam = 100 MeV");
    auto hstack_emom_80 = new THStack("hstack_emom_80", "Bremsstrahlung Spectra - Ebeam = 80 MeV");
    for (int i = 0; i < 10; i++) {
        gamma_specs_cut[i]->Rebin(5);
        gamma_specs_cut[i]->GetXaxis()->SetRangeUser(20, 200);
        if (i < 5) {
            gamma_specs_cut[i]->SetLineColorAlpha(colors[i], 0.6);
            gamma_specs_cut[i]->SetLineStyle(kSolid);
            gamma_specs_cut[i]->SetLineWidth(2);
            hstack_emom_80->Add(gamma_specs_cut[i]);
        } else {
            gamma_specs_cut[i]->SetLineColorAlpha(colors[i - 5], 0.6);
            gamma_specs_cut[i]->SetLineStyle(kSolid);
            gamma_specs_cut[i]->SetLineWidth(2);
            hstack_emom_100->Add(gamma_specs_cut[i]);
        }
    }

    TCanvas *c = new TCanvas("c", "c", 3600, 1800);
    c->Divide(2, 1);

    c->cd(1);
    gPad->SetLogy();
    hstack_emom_80->Draw("nostack");
    hstack_emom_80->GetXaxis()->SetRangeUser(20, 200);
    hstack_emom_80->GetXaxis()->SetTitle("Photon Energy [MeV]");
    hstack_emom_80->GetYaxis()->SetTitle("Counts (/MeV)");
    hstack_emom_80->GetXaxis()->CenterTitle();
    hstack_emom_80->GetYaxis()->CenterTitle();
    auto legend = new TLegend(0.55, 0.5, 0.87, 0.85);
    legend->SetBorderSize(0);
    for (int i = 0; i < 5; i++) {
        legend->AddEntry(gamma_specs_cut[i], Form("W %d-mm (%3.2g)", thicknesses[i], gamma_specs_cut[i]->Integral()), "l");
    }
    legend->Draw();
    gPad->Modified();
    gPad->Update();

    c->cd(2);
    gPad->SetLogy();
    hstack_emom_100->Draw("nostack");
    hstack_emom_100->GetXaxis()->SetRangeUser(20, 200);
    hstack_emom_100->GetXaxis()->SetTitle("Photon Energy [MeV]");
    hstack_emom_100->GetYaxis()->SetTitle("Counts (/MeV)");
    hstack_emom_100->GetXaxis()->CenterTitle();
    hstack_emom_100->GetYaxis()->CenterTitle();
    auto legend_2 = new TLegend(0.55, 0.5, 0.87, 0.85);
    legend_2->SetBorderSize(0);
    for (int i = 5; i < 10; i++) {
        legend_2->AddEntry(gamma_specs_cut[i], Form("W %d-mm (%3.2g)", thicknesses[i-5], gamma_specs_cut[i]->Integral()), "l");
    }
    legend_2->Draw();
    gPad->Modified();
    gPad->Update();

    c->SaveAs("GammaSpecCutoffAsFuncOfThickness.png");
    c->SaveAs("GammaSpecCutoffAsFuncOfThickness.pdf");
}

void PlotAngularDistribution() {
    std::vector<TH1D*> gamma_specs;
    std::vector<TH1D*> gamma_specs_cut;
    std::vector<TH1D*> gamma_specs_cut_angle;
    GetHistograms(gamma_specs, gamma_specs_cut, gamma_specs_cut_angle);

    auto c = new TCanvas("c", "c", 3600, 1800);
    c->Divide(2, 1);

    c->cd(1);
    gamma_specs_cut_angle[1]->SetTitle("80 MeV, 4-mm");
    gamma_specs_cut_angle[1]->SetLineColor(kRed);
    gamma_specs_cut_angle[1]->SetLineWidth(2);
    gamma_specs_cut_angle[1]->Draw();
    gamma_specs_cut_angle[1]->GetXaxis()->SetTitle("Scatter Angle (rad)");
    gamma_specs_cut_angle[1]->GetYaxis()->SetTitle("Counts");
    gamma_specs_cut_angle[1]->GetXaxis()->CenterTitle();
    gamma_specs_cut_angle[1]->GetYaxis()->CenterTitle();

    c->cd(2);
    gamma_specs_cut_angle[6]->SetTitle("100 MeV, 4-mm");
    gamma_specs_cut_angle[6]->SetLineColor(kRed);
    gamma_specs_cut_angle[6]->SetLineWidth(2);
    gamma_specs_cut_angle[6]->Draw();
    gamma_specs_cut_angle[6]->GetXaxis()->SetTitle("Scatter Angle (rad)");
    gamma_specs_cut_angle[6]->GetYaxis()->SetTitle("Counts");
    gamma_specs_cut_angle[6]->GetXaxis()->CenterTitle();
    gamma_specs_cut_angle[6]->GetYaxis()->CenterTitle();

    c->SaveAs("GammaSpecCutAngle.png");
    c->SaveAs("GammaSpecCutAngle.pdf");
}

void PlotBeamProfile() {
    std::vector<TH2D*> gamma_specs_cut_profile;
    GetProfiles(gamma_specs_cut_profile);

    auto c = new TCanvas("c", "c", 3600, 1800);
    c->Divide(2, 1);

    c->cd(1);
    gPad->SetLogz();
    gamma_specs_cut_profile[0]->SetTitle("80 MeV, 4-mm");
    gamma_specs_cut_profile[0]->Draw("COLZ");
    gamma_specs_cut_profile[0]->GetXaxis()->SetTitle("x (mm)");
    gamma_specs_cut_profile[0]->GetYaxis()->SetTitle("y (mm)");
    gamma_specs_cut_profile[0]->GetXaxis()->CenterTitle();
    gamma_specs_cut_profile[0]->GetYaxis()->CenterTitle();

    c->cd(2);
    gPad->SetLogz();
    gamma_specs_cut_profile[1]->SetTitle("100 MeV, 4-mm");
    gamma_specs_cut_profile[1]->Draw("COLZ");
    gamma_specs_cut_profile[1]->GetXaxis()->SetTitle("x (mm)");
    gamma_specs_cut_profile[1]->GetYaxis()->SetTitle("y (mm)");
    gamma_specs_cut_profile[1]->GetXaxis()->CenterTitle();
    gamma_specs_cut_profile[1]->GetYaxis()->CenterTitle();

    c->SaveAs("GammaSpecProfile.png");
    c->SaveAs("GammaSpecProfile.pdf");
}

void Plot() {
    gStyle->SetOptStat(0);
    // PlotSpectrumAsFunctionOfThickness();
    // PlotSpectrumCutoffAsFunctionOfThickness();
    PlotAngularDistribution();
    // PlotBeamProfile();
}