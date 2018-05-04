#/////////////////////////////////////////////////////////////////////////////
# Histo plotting part :
#/////////////////////////////////////////////////////////////////////////////

import AIDA

aida = AIDA.createAnalysisFactory()

tf = aida.createTreeFactory()
tree = tf.create()
del tf

plotterFactory = aida.createPlotterFactory()
histogramFactory = aida.createHistogramFactory(tree)

# Get current plotter :
plotter = plotterFactory.create('')

plotter.createRegions(1,1,0)
plotter.setTitle('Histo & detector')

# Create and plot an histo :
h = histogramFactory.createHistogram1D('h','Rand gauss',100,-5,5)
import random
r = random.Random()
for I in range(0,10000):
  h.fill(r.gauss(0,1),1)
del r

region = plotter.currentRegion()
region.plot(h)
region.setParameter('plotter.gridStyle.visible','FALSE')
region.setParameter('plotter.wallStyle.visible','FALSE')

del region
del h
del plotterFactory
del histogramFactory
del tree

#/////////////////////////////////////////////////////////////////////////////
# Detector part :
#/////////////////////////////////////////////////////////////////////////////
import CoinPython as Inventor
import HEPVis

# Linux : does not work.
#ip = plotter.cast_Slash_UI_IPlotter()
#soNode = ip.nodeOfInterest('region')
#soRegion = HEPVis.cast_SoRegion(soNode)

ui = aida.ui()
soPage = ui.findSoPage('plotter_0')
soRegion = soPage.currentRegion()
soPlotterRegion = soRegion.cast_SoPlotterRegion()

soEtc = soPlotterRegion.getEtcSeparator()

soLight = Inventor.SoDirectionalLight()
soLight.color.setValue(Inventor.SbColor(1,1,1))
soLight.direction.setValue(Inventor.SbVec3f(1,-1,-10))
soEtc.addChild(soLight)
del soLight

soLightModel = Inventor.SoLightModel()
soLightModel.model.setValue(Inventor.SoLightModel.PHONG)
soEtc.addChild(soLightModel)
del soLightModel

soCamera = Inventor.SoOrthographicCamera()
soCamera.viewportMapping.setValue(Inventor.SoCamera.ADJUST_CAMERA)
soCamera.position.setValue(Inventor.SbVec3f(0,0,4))
soCamera.height.setValue(2)
soCamera.nearDistance.setValue(1)
soCamera.farDistance.setValue(100)
soCamera.focalDistance.setValue(4)
soEtc.addChild(soCamera)
del soCamera

soTranslation = Inventor.SoTranslation()
soTranslation.translation.setValue(Inventor.SbVec3f(0.8,0.7,0))
soEtc.addChild(soTranslation)
del soTranslation

soScale = Inventor.SoScale()
soScale.scaleFactor.setValue(0.012,0.012,0.012)
soEtc.addChild(soScale)
del soScale

soRotation = Inventor.SoRotation()
soRotation.rotation.setValue(Inventor.SbRotation(Inventor.SbVec3f(1,1,1),0.8))
soEtc.addChild(soRotation)
del soRotation
    
soDet = HEPVis.SoDetectorExample()
soEtc.addChild(soDet)
del soDet

del soEtc
del soPlotterRegion
del soRegion

plotter.show()
plotter.interact()

del plotter

del aida
