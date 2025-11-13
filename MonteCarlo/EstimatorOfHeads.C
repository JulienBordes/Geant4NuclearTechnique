#include "TGraph.h"
#include "TMath.h"
#include "TRandom3.h"

// void EstimatorOfHeads(int samples, int throws, vector<int> headsinThrows)
void EstimatorOfHeads(int samples, int throws, int k, int *headsinThrow) {
  vector<int> headsinThrows;

  for (int j = 0; j < k; j++) {
    headsinThrows.push_back(headsinThrow[j]);
  }

  TRandom3 rndNum;
  int bins = (samples < 300) ? samples : 300;
  TH1F *hist = new TH1F("hist", ";Probability;counts;", bins, 0, 1);

  constexpr double probhead = 0.5;

  // loop over experiments
  for (int ii = 0; ii < 500; ii++) {
    int Tally = 0;

    // loop over samples per experiment
    for (int i = 0; i < samples; i++) {
      int numheads = 0;

      // loop over coin throws
      for (int j = 0; j < throws; j++) {
        // count number of heads
        numheads = (rndNum.Rndm() < probhead) ? numheads + 1 : numheads;
      }

      // Check if number of heads in throws equals any of required number of
      // heads and increament Tally
      for (int j = 0; j < headsinThrows.size(); j++) {
        if (numheads == headsinThrows[j])
          Tally++;
      }
    }

    // calculate probability
    double prob = Tally * 1.0 / samples;
    hist->Fill(prob);
  }

  // Calculation of true probability based on Binomial prob.
  double trueVal = 0;

  for (int j = 0; j < headsinThrows.size(); j++) {
    trueVal += TMath::Binomial(throws, headsinThrows[j]) *
               TMath::Power(probhead, headsinThrows[j]) *
               TMath::Power(probhead, throws - headsinThrows[j]);
  }

  TF1 *mygaus =
      new TF1("mygaus", "[0]*TMath::Exp(-0.5*(x-[1])/[2]*(x-[1])/[2])", 0, 1.0);
  mygaus->SetParName(0, "Amplitude");
  mygaus->SetParName(1, "#mu");
  mygaus->SetParName(2, "#sigma");

  mygaus->SetParameters(hist->GetMaximum(), hist->GetMean(), hist->GetRMS());
  mygaus->SetNpx(1000);
  mygaus->SetLineWidth(4);
  mygaus->SetLineColor(kBlue);
  hist->SetFillColor(kYellow);
  TLine *nline = new TLine(trueVal, 0, trueVal, hist->GetMaximum());
  nline->SetLineColor(kRed);
  nline->SetLineStyle(5);
  nline->SetLineWidth(2);
  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0);

  TCanvas *c1a = new TCanvas("c1a", "Coin Flip Probability", 1200, 600);
  hist->Draw();
  hist->Fit("mygaus", "R");
  nline->Draw("same");
}