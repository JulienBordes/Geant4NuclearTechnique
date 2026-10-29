#include "TCanvas.h"
#include "TEllipse.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TPad.h"
#include "TRandom3.h"
#include "TStyle.h"

void EstimatorOfPi(int samples = 10000, int update = 10) {
  int nEvents = samples;
  int n = 0;
  int in = 0;
  double x, y;

  TRandom3 rndNum(0);

  TGraph *TG_Pi = new TGraph();
  TGraph *TG_Inside = new TGraph();
  TGraph *TG_Outside = new TGraph();

  // -----------------------------
  // Monte Carlo loop
  // -----------------------------
  while (n < nEvents) {
    x = 1. - 2. * rndNum.Rndm();
    y = 1. - 2. * rndNum.Rndm();

    double radius = TMath::Sqrt(x * x + y * y);

    if (radius < 1.) {
      TG_Inside->SetPoint(TG_Inside->GetN(), x, y);
      in++;
    } else {
      TG_Outside->SetPoint(TG_Outside->GetN(), x, y);
    }

    n++;

    if (n % update == 0) {
      double pivalue = 4. * in / n;
      TG_Pi->SetPoint(TG_Pi->GetN(), n, pivalue);
    }
  }

  std::cout << "Number of darts: " << n << std::endl;
  std::cout << "Darts inside circle: " << in << std::endl;
  std::cout << "Estimated pi: " << 4.0 * in / n << std::endl;
  std::cout << "True pi: " << TMath::Pi() << std::endl;

  // -----------------------------
  // Global style
  // -----------------------------
  gStyle->SetOptStat(0);
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetTextFont(42);
  gStyle->SetTitleSize(0.05, "XYZ");
  gStyle->SetLabelSize(0.04, "XYZ");
  gStyle->SetTitleOffset(1.0, "X");
  gStyle->SetTitleOffset(1.2, "Y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  // Modern-ish colours
  int insideColor = kOrange + 7;
  int outsideColor = kAzure + 2;
  int piColor = kBlue + 2;
  int refColor = kGray + 2;

  // -----------------------------
  // Graph styling
  // -----------------------------
  TG_Inside->SetMarkerStyle(20);
  TG_Inside->SetMarkerSize(0.45);
  TG_Inside->SetMarkerColorAlpha(insideColor, 0.75);

  TG_Outside->SetMarkerStyle(20);
  TG_Outside->SetMarkerSize(0.45);
  TG_Outside->SetMarkerColorAlpha(outsideColor, 0.55);

  TG_Pi->SetLineColor(piColor);
  TG_Pi->SetLineWidth(2);
  TG_Pi->SetMarkerStyle(20);
  TG_Pi->SetMarkerSize(0.55);
  TG_Pi->SetMarkerColor(piColor);

  // -----------------------------
  // Canvas
  // -----------------------------
  TCanvas *c1a = new TCanvas("c1a", "Monte Carlo Estimate of Pi", 1400, 650);
  c1a->Divide(2, 1, 0.02, 0.02);

  // =========================================================
  // Left panel: dart throw
  // =========================================================
  c1a->cd(1);
  gPad->SetMargin(0.12, 0.05, 0.12, 0.08);
  gPad->SetGrid(0, 0);

  TH1F *ha = gPad->DrawFrame(-1.05, -1.05, 1.05, 1.05);
  ha->SetTitle("Monte Carlo dart throw");
  ha->GetXaxis()->SetTitle("x");
  ha->GetYaxis()->SetTitle("y");
  ha->GetXaxis()->CenterTitle();
  ha->GetYaxis()->CenterTitle();

  // Draw unit circle outline
  TEllipse *circle = new TEllipse(0., 0., 1., 1.);
  circle->SetFillStyle(0);
  circle->SetLineColor(refColor);
  circle->SetLineWidth(3);
  circle->Draw("same");

  TG_Outside->Draw("P same");
  TG_Inside->Draw("P same");

  gPad->SetFixedAspectRatio();

  // =========================================================
  // Right panel: convergence of pi estimate
  // =========================================================
  c1a->cd(2);
  gPad->SetMargin(0.12, 0.05, 0.12, 0.08);
  gPad->SetGrid(1, 1);
  gStyle->SetGridColor(kGray);
  gStyle->SetGridStyle(3);

  // Automatically determine the y-axis range
  double ymin = TMath::Pi();
  double ymax = TMath::Pi();

  for (int i = 0; i < TG_Pi->GetN(); ++i) {
    double x, y;
    TG_Pi->GetPoint(i, x, y);

    ymin = std::min(ymin, y);
    ymax = std::max(ymax, y);
  }

  // Add 8% padding above and below
  double padding = std::max(0.02, 0.08 * (ymax - ymin));

  TH1F *hb = gPad->DrawFrame(0, ymin - padding, nEvents, ymax + padding);
  hb->GetXaxis()->SetTitle("Number of darts");
  hb->GetYaxis()->SetTitle("Estimated #pi");
  hb->GetXaxis()->CenterTitle();
  hb->GetYaxis()->CenterTitle();

  TLine *nline = new TLine(0, TMath::Pi(), nEvents, TMath::Pi());
  nline->SetLineColor(kRed + 1);
  nline->SetLineStyle(2);
  nline->SetLineWidth(3);

  TG_Pi->Draw("LP same");
  nline->Draw("same");

  TLegend *leg2 = new TLegend(0.15, 0.78, 0.48, 0.90);
  leg2->SetBorderSize(0);
  leg2->SetFillStyle(0);
  leg2->SetTextSize(0.035);
  leg2->AddEntry(TG_Pi, "Monte Carlo estimate", "lp");
  leg2->AddEntry(nline, "True value of #pi", "l");
  leg2->Draw();

  c1a->Update();
}