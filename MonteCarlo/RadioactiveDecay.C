
#include "TCanvas.h"
#include "TColor.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TRandom3.h"
#include "TStyle.h"

#include <cmath>
#include <iostream>
#include <vector>

void RadioactiveDecay(int N0 = 1000, double halfLife = 5.0, double dt = 0.5,
                      int nSteps = 40, int nSims = 100) {

  if (N0 <= 0 || halfLife <= 0 || dt <= 0 || nSteps <= 0 || nSims < 2) {
    std::cerr << "Invalid parameters (nSims must be >= 2)" << std::endl;
    return;
  }

  TRandom3 rnd(0);

  const double lambda = std::log(2.0) / halfLife;
  const double pDecay = 1.0 - std::exp(-lambda * dt);

  // Statistics across independent simulations
  std::vector<double> mean(nSteps + 1, 0.0);
  std::vector<double> m2(nSteps + 1, 0.0);

  // --------------------------------------------------
  // Monte Carlo simulations
  // --------------------------------------------------
  for (int sim = 0; sim < nSims; ++sim) {

    int remaining = N0;

    for (int step = 0; step <= nSteps; ++step) {

      // Update mean and variance (Welford's method)
      double delta = remaining - mean[step];
      mean[step] += delta / (sim + 1);
      m2[step] += delta * (remaining - mean[step]);

      if (step == nSteps)
        break;

      int decayed = 0;

      for (int i = 0; i < remaining; ++i) {
        if (rnd.Rndm() < pDecay)
          decayed++;
      }

      remaining -= decayed;
    }
  }

  // --------------------------------------------------
  // Build graphs
  // --------------------------------------------------
  TGraphErrors *mc = new TGraphErrors();
  TGraph *theory = new TGraph();

  for (int step = 0; step <= nSteps; ++step) {

    double t = step * dt;
    double expected = N0 * std::exp(-lambda * t);

    // Sample standard deviation across simulations
    double sigmaMC = std::sqrt(m2[step] / (nSims - 1));

    // Statistical uncertainty on the MC mean
    double sigmaMean = sigmaMC / std::sqrt(nSims);

    mc->SetPoint(step, t, mean[step]);
    mc->SetPointError(step, 0.0, sigmaMean);

    theory->SetPoint(step, t, expected);
  }

  // --------------------------------------------------
  // Print statistical comparison
  // --------------------------------------------------
  int reportStep = static_cast<int>(std::round(halfLife / dt));

  if (reportStep > nSteps)
    reportStep = nSteps;

  double t = reportStep * dt;
  double expected = N0 * std::exp(-lambda * t);

  double sigmaMC = std::sqrt(m2[reportStep] / (nSims - 1));

  double sigmaMean = sigmaMC / std::sqrt(nSims);

  std::cout << "\nNumber of simulations: " << nSims << "\n";
  std::cout << "At t = " << t << " s:\n";
  std::cout << "  Monte Carlo mean       : " << mean[reportStep] << "\n";
  std::cout << "  Theoretical mean       : " << expected << "\n";
  std::cout << "  MC standard deviation  : " << sigmaMC << "\n";
  std::cout << "  MC standard error      : " << sigmaMean << "\n";

  // --------------------------------------------------
  // Modern colours
  // --------------------------------------------------
  int mcColor = TColor::GetColor("#153EAD");
  int theoryColor = TColor::GetColor("#D56A40");

  // --------------------------------------------------
  // Canvas
  // --------------------------------------------------
  gStyle->SetOptStat(0);
  gStyle->SetGridColor(TColor::GetColor("#E1E5E9"));
  gStyle->SetGridStyle(3);
  gStyle->SetEndErrorSize(3);

  TCanvas *c = new TCanvas("cDecay", "Radioactive decay", 1100, 650);

  c->SetFillColor(kWhite);
  c->SetLeftMargin(0.12);
  c->SetRightMargin(0.05);
  c->SetBottomMargin(0.13);
  c->SetTopMargin(0.06);
  c->SetTicks(1, 1);
  c->SetGridy();

  TH1F *frame = c->DrawFrame(0, 0, nSteps * dt, N0 * 1.08);

  frame->SetTitle(";Time (s);Number of undecayed nuclei");

  frame->GetXaxis()->SetTitleSize(0.05);
  frame->GetYaxis()->SetTitleSize(0.05);
  frame->GetXaxis()->SetLabelSize(0.04);
  frame->GetYaxis()->SetLabelSize(0.04);
  frame->GetYaxis()->SetTitleOffset(1.25);
  frame->GetXaxis()->CenterTitle(true);
  frame->GetYaxis()->CenterTitle(true);

  // --------------------------------------------------
  // Theory
  // --------------------------------------------------
  theory->SetLineColor(theoryColor);
  theory->SetLineWidth(3);
  theory->SetLineStyle(2);
  theory->Draw("L SAME");

  // --------------------------------------------------
  // Monte Carlo mean with standard error bars
  // --------------------------------------------------
  mc->SetLineColor(mcColor);
  mc->SetLineWidth(3);
  mc->SetMarkerStyle(20);
  mc->SetMarkerSize(0.65);
  mc->SetMarkerColor(mcColor);
  mc->Draw("LP SAME");

  // --------------------------------------------------
  // Legend
  // --------------------------------------------------
  TLegend *leg = new TLegend(0.58, 0.78, 0.93, 0.90);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.032);

  leg->AddEntry(mc, "Monte Carlo mean #pm SE", "lep");
  leg->AddEntry(theory, "Theory", "l");
  leg->Draw();

  c->Modified();
  c->Update();

  // c->SaveAs("RadioactiveDecay.pdf");
}
