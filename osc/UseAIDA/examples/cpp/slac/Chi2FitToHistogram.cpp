
// C++ conversion of the java/Chi2FitToHistogram.java example.

#include <AIDA/AIDA.h>

#include "Random.h"

using namespace AIDA;

#include <stdlib.h>

int main(int argc,char* argv[]) {

  IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) return EXIT_FAILURE;

  ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  IHistogramFactory* hf = af->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;
  IHistogram1D* h1d = hf->createHistogram1D("Gaussian Distribution",100,-5,5);
  delete hf;
  if(!h1d) return EXIT_FAILURE;
      
  Random r;
  for (int i=0; i<5000; i++) {
    h1d->fill(r.nextGaussian());
  }
      
  // Do Fit
  IFitFactory* ff = af->createFitFactory(); 
  if(!ff) return EXIT_FAILURE;
  IFitter* fitter = ff->createFitter("chi2");
  delete ff;
  if(!fitter) return EXIT_FAILURE;

  IFitResult* result = fitter->fit(*h1d,"g");
  delete fitter;
  if(!result) return EXIT_FAILURE;
      
  IFunction& function = result->fittedFunction();

  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    // Show results
    plotter->createRegions(1,1,0);
    plotter->region(0)->plot(*h1d);
    plotter->region(0)->plot(function);
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  delete result;

  delete af;

  return EXIT_SUCCESS;
}
