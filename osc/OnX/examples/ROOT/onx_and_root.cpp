
///////////////////////////////////////////////
/// CERN-ROOT /////////////////////////////////
///////////////////////////////////////////////
//
#define HAVE_ROOT

#ifdef HAVE_ROOT
#include <TApplication.h>
#include <TRandom.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TSystem.h>
#endif

///////////////////////////////////////////////
/// osc_vis ///////////////////////////////////
///////////////////////////////////////////////
//
#define HAVE_OSC_VIS

#ifdef HAVE_OSC_VIS
#include <Slash/Core/ISession.h>
#include <Slash/Tools/UI.h>
#include <Slash/Tools/Managers.h>

#include <inlib/osc_env> //For TTFPATH

#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoRegion.h>
#include <HEPVis/nodekits/SoDetectorExample.h>
#endif

///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////

#include <cstdlib>
#include <iostream>

int main(int,char**) {

#ifdef HAVE_OSC_VIS
  ///////////////////////////////////////////////
  /// osc_vis ///////////////////////////////////
  ///////////////////////////////////////////////
  if(!inlib::osc::set_env(std::cout)) {
    std::cout << "onx_root :"
              << " unable to set OSC environment." << std::endl;
    return EXIT_FAILURE;
  }

  int _argc = 1;
  static char* _argv[] = {
    (char*)"-onx_file=detector.onx"
    //,(char*)"-thread"
  };  

  Slash::Core::ISession* session = 
    Slash_create_session(inlib::to(_argc,_argv));
  if(!session) {
    std::cout << "onx_root :"
              << " session not found." << std::endl;
    return EXIT_FAILURE;
  }

  Slash::UI::ISceneHandler* sh = Slash::sceneHandler(*session);
  if(!sh) {
    std::cout << "onx_root :"
              << " scene handler not found." 
              << std::endl;
  } else {
    SoDetectorExample* det = new SoDetectorExample();
    det->ref();
    sh->setViewer("@current@");
    sh->addScene("staticScene",det);
    det->unref();
  }

  Slash::UI::IUI* ui = Slash::find_UI(*session);     
  if(!ui) {
    std::cout << "onx_root :"
              << "Slash::UI::IUI not found."
              << std::endl;
  }
#endif

#ifdef HAVE_ROOT
  ///////////////////////////////////////////////
  /// CERN-ROOT /////////////////////////////////
  ///////////////////////////////////////////////
  TApplication app("onx_root",0,0);

  TH1* histogram = new TH1D("hGauss","Random gauss",100,-5,5);
  for(unsigned int count=0;count<10000;count++) 
    histogram->Fill(gRandom->Gaus(0,1));

  histogram->Draw();
#endif

  ///////////////////////////////////////////////
  /// steering //////////////////////////////////
  ///////////////////////////////////////////////
#ifdef HAVE_OSC_VIS
  if(ui) {
#ifdef HAVE_ROOT
    while(true) {
      ui->synchronize();

      app.StartIdleing();
      gSystem->ProcessEvents();
      app.StopIdleing();
    }
#else
    ui->steer();
#endif
  }
  delete session;
#else

#ifdef HAVE_ROOT
  app.Run();
#endif

#endif

  return EXIT_SUCCESS;
}
