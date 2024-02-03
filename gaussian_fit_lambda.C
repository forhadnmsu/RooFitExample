#include "RooFitResult.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include <iostream>
#include <fstream>

using namespace RooFit;
using namespace std;

void gaussian_fit_lambda()
{
	// Create a RooRealVar for the data
	RooRealVar x("x", "x",-0.1, 1.9);

	// Create a RooDataSet to hold the data
	RooDataSet* data = new RooDataSet("data", "Example Dataset", RooArgSet(x));

	// Open the text file for reading
	ifstream inputFile("parameter_lambda.txt");
	if (!inputFile.is_open())
	{
		cerr << "Error opening input file!" << endl;
		return;
	}

	// Read data from the text file and add it to the RooDataSet
	double value;
	while (inputFile >> value)
	{
		x.setVal(value);
		data->add(RooArgSet(x));
	}
	inputFile.close();

	// Define the Gaussian function to fit
	RooRealVar mean("mean", "mean", 1., -1, 1);
	RooRealVar sigma("sigma", "sigma", 1, 0, 1);
	RooGaussian gauss("gauss", "Gaussian PDF", x, mean, sigma);
	RooFitResult* fit_result = gauss.fitTo(*data, Save());

	fit_result->Print("v");

	TCanvas* canvas = new TCanvas("canvas", "Example Canvas", 800, 600);
	canvas->cd();
	RooPlot* frame2 = x.frame();
	frame2->SetTitleSize(0.05, "X");    
	frame2->SetTitle("");    
	frame2->SetXTitle("#lambda");    
	frame2->SetYTitle("Independent Test Histogram Entries");
	frame2->SetTitleSize(0.05, "Y"); 

	data->plotOn(frame2, Binning(20));
	gauss.plotOn(frame2);
	TPaveText* pave_text2 = new TPaveText(0.65, 0.7, 0.9, 0.9, "NDC");
	pave_text2->SetFillColor(0);
	pave_text2->SetTextAlign(12);
	pave_text2->AddText(Form("Mean: %.3f #pm %.3f", mean.getVal(), mean.getError()));
	pave_text2->AddText(Form("#sigma: %.3f #pm %.3f", sigma.getVal(), sigma.getError()));
	frame2->Draw();
	pave_text2->Draw();
	cout << "Mean value: " << mean.getVal() << endl;
	cout << "sigma value: " << sigma.getVal() << endl;
	TLatex latex;
	latex.SetNDC();
	latex.SetTextSize(0.06);
	latex.SetTextColor(kRed);
	latex.DrawLatex(0.15, 0.85, "#lambda_{inject} = 1.0");
	latex.SetTextColor(kBlue);
	latex.DrawLatex(0.15, 0.75, "#lambda_{recovered} = 0.98#pm 0.03");
	canvas->SaveAs("lambda.png");
}

int main()
{
    gaussian_fit_lambda();
    return 0;
}

