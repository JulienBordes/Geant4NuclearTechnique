#include "TGraph.h"
#include "TMath.h"
#include "TRandom3.h"

void EstimatorOfPi(int samples, int update) {
  int nEvents = samples;
  int n = 0;
  int in = 0;
  double x, y;
  TRandom3 rndNum;
  TGraph *TG_Pi = new TGraph();

  TGraph *TG_Inside = new TGraph();
  TGraph *TG_Outside = new TGraph();

  while (n < nEvents) {
    x = 1 - 2 * rndNum.Rndm();
    y = 1 - 2 * rndNum.Rndm();

    double radius = TMath::Sqrt(TMath::Power(x, 2) + TMath::Power(y, 2));

    if (radius < 1) {
      TG_Inside->Set(TG_Inside->GetN() + 1);
      TG_Inside->SetPoint(TG_Inside->GetN() - 1, x, y);
      in++;
    }

    else {
      TG_Outside->Set(TG_Outside->GetN() + 1);
      TG_Outside->SetPoint(TG_Outside->GetN() - 1, x, y);
    }

    n++;

    if (n % update == 0) {
      double pivalue = 4. * in / n;
      TG_Pi->Set(TG_Pi->GetN() + 1);
      TG_Pi->SetPoint(TG_Pi->GetN() - 1, n, pivalue);
    }
  }

  cout << endl;

  TG_Outside->SetFillColor(kBlue);
  TG_Outside->SetFillStyle(3001);
  TG_Outside->SetMarkerStyle(8);
  TG_Outside->SetMarkerColor(kBlue);

  TG_Inside->SetFillColor(kRed);
  TG_Inside->SetFillStyle(3001);
  TG_Inside->SetMarkerStyle(8);
  TG_Inside->SetMarkerColor(kRed);

  TG_Pi->SetFillColor(kBlue);
  TG_Pi->SetFillStyle(3001);
  TG_Pi->SetMarkerStyle(8);
  TG_Pi->SetMarkerColor(kBlue);

  TCanvas *c1a = new TCanvas("c1a", "Pi", 1200, 600);
  c1a->Divide(2, 1);
  c1a->cd(1);
  TH1F *ha = gPad->DrawFrame(-1, -1, 1, 1);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  ha->SetXTitle("x (arbitrary unit)");
  ha->SetYTitle("y (arbitrary unit)");
  ha->GetXaxis()->CenterTitle();
  ha->GetYaxis()->CenterTitle();
  ha->GetYaxis()->SetTitleOffset(1.5);
  ha->GetXaxis()->SetNdivisions(105);
  ha->GetYaxis()->SetNdivisions(105);
  TG_Outside->Draw("P");
  TG_Inside->Draw("P");

  c1a->cd(2);
  TLine *nline = new TLine(0, TMath::Pi(), nEvents, TMath::Pi());
  nline->SetLineColor(kRed);
  nline->SetLineStyle(5);
  nline->SetLineWidth(2);
  TH1F *hb = gPad->DrawFrame(0, 3, nEvents + 10, 3.5);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  hb->SetXTitle("Number of darts");
  hb->SetYTitle("Estimated #pi");
  hb->GetXaxis()->CenterTitle();
  hb->GetYaxis()->CenterTitle();
  hb->GetYaxis()->SetTitleOffset(1.5);

  TG_Pi->Draw("P");
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  nline->Draw("same");
}