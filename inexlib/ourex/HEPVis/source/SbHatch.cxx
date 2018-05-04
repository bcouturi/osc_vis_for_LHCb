/* 
 *  SbHatch.cxx
 *  
 *
 *  Created by Laurent Garnier on Fri Jan 30 2004.
 *
 */ 

// this :
#include <HEPVis/SbHatch.h>

// HEPVis :
#include <HEPVis/SbMath.h>
 
// Else :
#include <cfloat> // for FLT_MAX
#include <vector> 

//#define HATCH_DEBUG

SbHatch::SbHatch()
  :fShift (.1f)
  ,fDirAngle (FM_PI/4)
  ,fOffsetValue(.0f)
  ,fOffset (SbVec3f(FLT_MAX,FLT_MAX,FLT_MAX))
  ,fPrecisionFactor (.0001f) // good value to get rid of some errors
  ,fStripWidth(0.0)
  ,fFirstNumHatch(0)
  ,fNumberHatchToDraw(0)
  ,fFirstPolyline (TRUE)
  ,fResolveResult (UNDEFINED)
{ }


SbHatch::~SbHatch() { 
}

void SbHatch::setSpacing(float a) {
  fShift = a;
}
void SbHatch::setAngle(float a) {
  fDirAngle = a;
}
void SbHatch::setOffset(float a) {
  fOffsetValue = a;
}
void SbHatch::setOffsetPoint(SbVec3f a) {
  fOffset = a;
}
void SbHatch::setPrecisionFactor(float a) {
  fPrecisionFactor = a;
}
void SbHatch::setStripWidth(float a) {
  if (a<0 || a>1) {
    printf("SbHatch :: stripWidth must be teween 0-1 !\nIt is now set to default.\n\n");
    fStripWidth = 0;
  } else {
    fStripWidth = a;
  }
}
float SbHatch::getSpacing()const  {
  return fShift;
}
float SbHatch::getAngle()const  {
  return fDirAngle;
}
float SbHatch::getOffset()const  {
  return fOffsetValue;
}
const SbVec3f& SbHatch::getOffsetPoint() {
  return fOffset;
}
float SbHatch::getPrecisionFactor()const  {
  return fPrecisionFactor;
}
float SbHatch::getStripWidth()const  {
  return fStripWidth;
}


unsigned int SbHatch::getNoPoints() const  {
  return fPoints.size();
}
const std::vector<SbVec3f>&  SbHatch::getPoints(){
  return fPoints;
}
unsigned int SbHatch::getNoVertices() const  {
  return fVertices.size();
}
const std::vector<unsigned int>&  SbHatch::getVertices(){
  return fVertices;
}
const SbVec3f& SbHatch::getNormal() {
  return fNormal;
}


//////////////////////////////////////////////////////////////////////////////
// test if the polygone given is correct for hatching
// return FALSE if :
//    - All points are not in the same plan
//    - Number of points <3
//    - Offset point is not in the same plan
//    - There is less than three different points
//    - The vector from point[0],point[1] is colinear to point[0],lastPoint
//////////////////////////////////////////////////////////////////////////////

SbBool SbHatch::checkPolyline(SbVec3f* listPoints,unsigned int aNumber){

  unsigned int firstOffset =0;

  if ( listPoints[0].equals(listPoints[1].getValue(),FLT_EPSILON*FLT_EPSILON*10)) {
    firstOffset =1;
  }

  if ( listPoints[0].equals(listPoints[aNumber-1].getValue(),FLT_EPSILON*FLT_EPSILON*10)) {
    aNumber --;
  }

  if ((int)aNumber-firstOffset <3) {
#ifdef HATCH_DEBUG
    printf("SbHatch :: checkPolyline : ERROR the polygone you give have not enought points!\n\n");
#endif
    return FALSE;
  }


  // use to test the polyline and to build the shift vector. A is the first point,
  // B second and C the last (in fact, the last-1)!
  SbVec3f AB,AC;
  AB.setValue(listPoints[1+firstOffset].getValue()[0]-listPoints[0].getValue()[0],
              listPoints[1+firstOffset].getValue()[1]-listPoints[0].getValue()[1],
              listPoints[1+firstOffset].getValue()[2]-listPoints[0].getValue()[2]); // Vector A->B
  
  
  fResolveResult = COLINEAR;
  unsigned int test = aNumber;
  while ((fResolveResult !=0) && (test>2+firstOffset)) {
    test--;
    AC.setValue(listPoints[test].getValue()[0]-listPoints[0].getValue()[0],
                listPoints[test].getValue()[1]-listPoints[0].getValue()[1],
                listPoints[test].getValue()[2]-listPoints[0].getValue()[2]);
    
    // test if AB != AC*i
    resolveSystem( AB,
                   AC,
                   SbVec3f(.0f,.0f,.0f));
  }
  if (fResolveResult == COLINEAR) {
#ifdef HATCH_DEBUG
    printf("SbHatch :: checkPolyline : ERROR all the point you give are colinear!\n\n");
    for (unsigned int a =0;a<aNumber;a++) {
      printf(" %f %f %f \n",listPoints[a][0],listPoints[a][1],listPoints[a][2]);  }
#endif
    return FALSE;
  }

  ///////////////////////////////////////////////////////////////
  // test if all points of the polyline are on the same plan
  ///////////////////////////////////////////////////////////////

  int falsePoints =0;
  for (unsigned int a=2+firstOffset;a<aNumber;a++) {
    resolveSystem( AB,
                   AC,
                       SbVec3f((listPoints[a].getValue()[0]-listPoints[0].getValue()[0]),
                               (listPoints[a].getValue()[1]-listPoints[0].getValue()[1]),
                               (listPoints[a].getValue()[2]-listPoints[0].getValue()[2])));
    if (fResolveResult != 0){
      falsePoints++;
    }
  }
  
  if (falsePoints !=0) {
#ifdef HATCH_DEBUG
    printf("SbHatch :: checkPolyline : ERROR there is %d points on the polyline witch are not on the same plan!\n\n",falsePoints);
#endif
    return FALSE;
  }

  // test offset
    if (! ((fOffset[0] == FLT_MAX) && (fOffset[1] == FLT_MAX) && (fOffset[2] == FLT_MAX))){
      resolveSystem( AB,
                     AC,
                     fOffset-listPoints[0]);
      if (fResolveResult != 0) {
#ifdef HATCH_DEBUG
        printf("SbHatch :: checkPolyline : ERROR Offset vector has to be on the same plan!\n\n");
#endif
        return FALSE;
      }
    }
    return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
// draw the hatch into the polyline bounding box giving in argument
// return FALSE if :
//    - All points are not in the same plan
//    - There is a precision error on one or more point
// Compute a first sequence of hacth, store results, compute a second sequence
// and match all results to get the correct strip points 
//////////////////////////////////////////////////////////////////////////////
/** Compute stripWidth
 * We have to use the conflictNumHatchLineTab, hatchNumber,listHatchStartPoint tables
 * also the HatchShiftToMacthPoint tab. 
 * and the hatch line just compute below 
 * We try to made a polyline with all points witch are on the current hatch and on the next hacth
 * (distant of stripwidth form current hatch)
 * conflictNumHatchLineTab give us something like this for current and next hatch
 * current   next                                                current    next
 *    4       4              if we consider that                   ,4        ,4
 *    0       5              we know the compute hatch             '0        '5
 *    1       3              lines, we could link                  ,1        ,3
 *    3       2              some of theses line numbers           '3        '2
 *    5                        so ->>                              ,5
 *    2                                                            '2
 *  And we have to add some points when  HatchShiftToMacthPoint(point) is between current
 * and next hatch :  We add a point B on intersection of line 0 and 1
 * current   next                                                current    next
 *    ,4             ,4
 *    '0    B(0,1)   '5
 *    ,1   
 *    '3             ,3
 *    ,5             '2 
 *    '2        
 *
 * Now we have to match a way to traverse all of theses lines. We have 3 solutions to go from
 * one line to another :
 * - go to the next point if there is one between current and next hatch
 * - go to the same line but on another hatch
 * - go to the next tach point
 * If there is no solution, we have to close the polyline strip and go to another point until
 * all are compute  
 */

/** first, we have to match 7 different cases
 * 1- all strip hatch are entirely in the polyline
 * 2- the first strip begin before the polyline and the last end in the polyline
 * 3- the first strip begin before the polyline and the last ends after
 * 4- the first strip is entierly in the polyline and the last ends after
 * 5- the strip has only an intersection with the second hatch sequence (if it has only an intersection
 *    with the first hatch sequence, it is case 2
 * 6- the strip has a full intersection
 * 7- the strip has no intersection !
 */

SbBool SbHatch::computePolyline (SbVec3f* tabPoints,unsigned int aNumber)
{
  std::vector<SbVec3f> firstComputePoints;   // copy first Points in
  std::vector<SbVec3f> secondComputePoints;   // copy first Points in
  std::vector<SbBool> firstComputePointsEnable; // table of already compute points for first hatch
  std::vector<SbBool> secondComputePointsEnable;// table of already compute points for second hatch
  std::vector< std::vector<int> > firstComputeConflictNumHatchLineTab; // copy firstComputeConflictNumHatchLineTab in
  
  int firstComputeFirstNumHatch =0;
  unsigned int firstComputeNumberHatchToDraw =0;
  float firstHatchShiftToMatchFirstPoint = FLT_MAX; // use in one case when there is no intersection points: to test we have to fill all the polygone
  float secondHatchShiftToMatchFirstPoint = FLT_MAX; // use in one case when there is no intersection points: to test we have to fill all the polygone
  //call compute for first set of hatch
  if ( !computeSinglePolyline (tabPoints,aNumber))
    return false;
  if (fStripWidth ==0)
    return true;


  //save values
  for (unsigned int a =0;a<fPoints.size();a++){
    firstComputePoints.push_back(fPoints[a]);
  }

  firstComputeConflictNumHatchLineTab.resize(fConflictNumHatchLineTab.size());
  for (unsigned int a=0;a<fConflictNumHatchLineTab.size();a++){
    firstComputeConflictNumHatchLineTab[a].clear();
    for (unsigned int b=0;b<fConflictNumHatchLineTab[a].size();b++){
      firstComputeConflictNumHatchLineTab[a].push_back(fConflictNumHatchLineTab[a][b]);
    } 
  }
  firstComputeFirstNumHatch = fFirstNumHatch;
  firstComputeNumberHatchToDraw = fNumberHatchToDraw;
  firstHatchShiftToMatchFirstPoint = fHatchShiftToMatchPointVec[0];  
  //change the offset vector
  fOffset = fOffset+fShiftVec*fStripWidth;
  
  //call compute for second set of hatch
  if ( !computeSinglePolyline (tabPoints,aNumber))
    return false;

  //save values
  for (unsigned int a =0;a<fPoints.size();a++){
    secondComputePoints.push_back(fPoints[a]);
  }

  secondHatchShiftToMatchFirstPoint = fHatchShiftToMatchPointVec[0];  

    
  // initialize values
  fPoints.clear();
  
  fVertices.resize(0);
  fVertices.clear();
  
  int specialCase=1;

  //first hatch, case 1
  if ((firstComputeFirstNumHatch == fFirstNumHatch) && (firstComputeNumberHatchToDraw == fNumberHatchToDraw) && (firstComputeNumberHatchToDraw !=0)) {
    specialCase =1;
  }
  //first hatch, case 2
  else if ((firstComputeFirstNumHatch > fFirstNumHatch) && (firstComputeNumberHatchToDraw < fNumberHatchToDraw) && (firstComputeNumberHatchToDraw !=0)) {
    //insert a empty element at the beginning
    firstComputeConflictNumHatchLineTab.insert(firstComputeConflictNumHatchLineTab.begin(), firstComputeConflictNumHatchLineTab.back());
    firstComputeConflictNumHatchLineTab[0].resize(0);
    firstComputeFirstNumHatch--;
    firstComputeNumberHatchToDraw ++;
    firstComputeConflictNumHatchLineTab[0].clear();
    specialCase =2;

  }   //second hatch, case 3
  else   if (((firstComputeFirstNumHatch > fFirstNumHatch) && (firstComputeNumberHatchToDraw == fNumberHatchToDraw)) && (firstComputeNumberHatchToDraw !=0)) {
    //insert a empty element at the beginning
    firstComputeConflictNumHatchLineTab.insert(firstComputeConflictNumHatchLineTab.begin(),firstComputeConflictNumHatchLineTab.back());
    firstComputeConflictNumHatchLineTab[0].resize(0);
    firstComputeConflictNumHatchLineTab[0].clear();
    //insert a empty element at the end
    fConflictNumHatchLineTab.push_back(firstComputeConflictNumHatchLineTab.back());
    fConflictNumHatchLineTab.back().resize(0);
    fConflictNumHatchLineTab.back().clear();
    firstComputeFirstNumHatch--;
    firstComputeNumberHatchToDraw ++;
    specialCase =3;
  }   //second hatch, case 4
  else   if (((firstComputeFirstNumHatch == fFirstNumHatch) && (firstComputeNumberHatchToDraw > fNumberHatchToDraw)) && (firstComputeNumberHatchToDraw !=0)) {
    //insert a empty element at the end
    fConflictNumHatchLineTab.push_back(firstComputeConflictNumHatchLineTab.back());
    fConflictNumHatchLineTab.back().resize(0);
    fConflictNumHatchLineTab.back().clear();
    specialCase =4;

  }   //second hatch, case 5
  else   if ((firstComputeNumberHatchToDraw ==0) && (fNumberHatchToDraw !=0)) {
    //insert a empty element at the beginning
    firstComputeConflictNumHatchLineTab.insert(firstComputeConflictNumHatchLineTab.begin(),firstComputeConflictNumHatchLineTab.back());
    firstComputeConflictNumHatchLineTab[0].resize(0);
    firstComputeConflictNumHatchLineTab[0].clear();
    firstComputeNumberHatchToDraw ++;
    specialCase =5;
    
  }   //second hatch, case 6
  else if (floorf(firstHatchShiftToMatchFirstPoint) != floorf(secondHatchShiftToMatchFirstPoint)) {
    specialCase =6;
    
    //fill all the polygone !
    fVertices.push_back(aNumber);
    for (unsigned int a =0;a<aNumber;a++){
      fPoints.push_back(tabPoints[a]);
    }
    return TRUE;
  }
  else if (floorf(firstHatchShiftToMatchFirstPoint) == floorf(secondHatchShiftToMatchFirstPoint)) {
    specialCase =7;
    return TRUE;
  } else {
#ifdef HATCH_DEBUG 
    printf("SbHatch :: drawStripPolyline : WARNING there is a case witch was not done in the algotithm...possibly some drawing problems.\n\n");
#endif

  }

  
  bool result;
  bool find; // temp variable
  int firstHatchComputePoint = 0; //first point number 
  int secondHatchComputePoint = 0; //first point number 
  unsigned int lineNumber;
  unsigned int firstPointTabInd =0;
  unsigned int secondPointTabInd=0;
  unsigned int currentHatch; // 0 is first, 1 is second, 2 is one or other !!
  unsigned int solution; //default for beginning
  unsigned int indTmp;
  unsigned int oldSolution;
  for (unsigned int indHatch =0;indHatch<firstComputeNumberHatchToDraw;indHatch++) {
    

    currentHatch =0; // 0 is first, 1 is second
    solution =99; //default for beginning
    indTmp = 0;
    lineNumber = 0;
    secondComputePointsEnable.clear();
    firstComputePointsEnable.clear();
    for (unsigned int a=0;a<firstComputeConflictNumHatchLineTab[indHatch].size();a++){
      firstComputePointsEnable.push_back(false);}
    for (unsigned int a=0;a<fConflictNumHatchLineTab[indHatch].size();a++){
      secondComputePointsEnable.push_back(false);}
    
    if ((indHatch == 0) && ((specialCase ==2) || (specialCase ==3) || (specialCase ==5))) {
      for (unsigned int a=0;a<firstComputeConflictNumHatchLineTab[indHatch].size();a++){
        firstComputePointsEnable[a] = true;
      }
    }
    if ((indHatch == (firstComputeNumberHatchToDraw-1)) && ((specialCase ==3) || (specialCase ==4))) {
      for (unsigned int a=0;a<fConflictNumHatchLineTab[indHatch].size();a++){
        secondComputePointsEnable[a] = true;
      }
    }
    
    result = false;
    while (result == false) {
      

      //find a uncompute point for this set of hatch
      result =true;
      unsigned int b=0;
      while ((result == true) && (b<firstComputeConflictNumHatchLineTab[indHatch].size())) {
        if (firstComputePointsEnable[b] == false) {
          result =false; 
          firstHatchComputePoint = b; 
          lineNumber = firstComputeConflictNumHatchLineTab[indHatch][b];
          fPoints.push_back(firstComputePoints[b+firstPointTabInd]);
          fVertices.push_back(1);
          firstComputePointsEnable[b] = true;
          currentHatch = 0;
        }
        b++;
      }
      if (result ==true) {
        //find a uncompute point for this set of hatch
        
        while ((result == true) && (b<fConflictNumHatchLineTab[indHatch].size())) {
          if (secondComputePointsEnable[b] == false) {
            result =false;
            secondHatchComputePoint = b; 
            lineNumber = fConflictNumHatchLineTab[indHatch][b];
            fPoints.push_back(secondComputePoints[b+secondPointTabInd]);
            fVertices.push_back(1);
            secondComputePointsEnable[b] = true;
            currentHatch = 1;
          }
          b++;
        }
      }
      if (result == true) {
      }
        solution =99; // to enter in the while
        while (solution !=0) {
          oldSolution = solution;
          solution =0; //default
          // get the line number for this point
          /** Now we have to match a way to traverse all of theses lines. We have 3 solutions to go from
           * one line to another :
           * - go to the next point if there is one between current and next hatch
           * - go to the same line but on another hatch
           * - go to the next hatch point
           */
          if (currentHatch != 1) {

            if (oldSolution != 3) {                   // could go to first solution
              int index =0;
              if ((firstHatchComputePoint % 2 == 0) && (firstComputePointsEnable[firstHatchComputePoint+1] == false))   index =1;
              else if ((firstHatchComputePoint % 2 != 0) && (firstComputePointsEnable[firstHatchComputePoint-1] == false))  index = -1;
              if (index !=0) {
                solution = 1;
                oldSolution = 0;
                firstHatchComputePoint = firstHatchComputePoint+index;
                fPoints.push_back(firstComputePoints[firstHatchComputePoint+firstPointTabInd]);
                fVertices.back() ++;
                firstComputePointsEnable[firstHatchComputePoint] = true;
                lineNumber = firstComputeConflictNumHatchLineTab[indHatch][firstHatchComputePoint];
              }
            }
            if (solution == 0) {                  // could go to second solution
              indTmp = 0;
              while ((solution == 0) && (indTmp < fConflictNumHatchLineTab[indHatch].size())) {
                if ((fConflictNumHatchLineTab[indHatch][indTmp] == (int)lineNumber) && (secondComputePointsEnable[indTmp] == false)) {
                  solution =2;
                  oldSolution = 0;
                  fPoints.push_back(secondComputePoints[indTmp+secondPointTabInd]);
                  fVertices.back() ++;
                  secondComputePointsEnable[indTmp] = true;
                  lineNumber = fConflictNumHatchLineTab[indHatch][indTmp];
                  secondHatchComputePoint = indTmp;
                  currentHatch =1;
                }
                indTmp ++;
              }
            }
            if (solution == 0) {                    // could go to first solution
              indTmp = 0;
              while ((solution == 0) && (indTmp < aNumber)) {

                if ((fHatchShiftToMatchPointVec[indTmp] > ((float)firstComputeFirstNumHatch+(float)indHatch-fStripWidth))
                    && (fHatchShiftToMatchPointVec[indTmp] < ((float)firstComputeFirstNumHatch+(float)indHatch))
                    && ((indTmp == lineNumber) || (indTmp==lineNumber+1) || ((lineNumber == (aNumber-1)) && (indTmp ==0)))) {
                  find = false;
                  unsigned a =0;
                  while ((a<fVertices.back()) && (find == false)) {
                    if ((tabPoints[indTmp][0] == fPoints[a][0]) && (tabPoints[indTmp][1] == fPoints[a][1]) && (tabPoints[indTmp][2] == fPoints[a][2])) find = true;
                    a++;
                  }
                  if (find == false){
                    solution = 3;
                    oldSolution = 0;
                    currentHatch =2;
                    fPoints.push_back(tabPoints[indTmp]);
                    fVertices.back() ++;
                    if (lineNumber == indTmp) {
                      if (indTmp >0)  lineNumber =  indTmp-1;
                      else lineNumber = aNumber-1;
                    }
                    else {
                      if (indTmp < aNumber-1)  lineNumber =  indTmp;
                      else lineNumber = 0;
                    }
                  }
                }
                indTmp++;
              }
            }
          } // end of current hatch
          
            //test of second hatch if currentHatch is second 
          if ((oldSolution != 0) && (solution !=2) && (currentHatch !=0)) {

            if (oldSolution != 3){                   // could go to first solution
              int index =0;
              if ((secondHatchComputePoint % 2 == 0) && (secondComputePointsEnable[secondHatchComputePoint+1] == false))   index =1;
              else if ((secondHatchComputePoint % 2 != 0) && (secondComputePointsEnable[secondHatchComputePoint-1] == false))  index = -1;
              if (index !=0){
                solution = 1;
                secondHatchComputePoint = secondHatchComputePoint+index;
                fPoints.push_back(secondComputePoints[secondHatchComputePoint+secondPointTabInd]);
                fVertices.back() ++;
                secondComputePointsEnable[secondHatchComputePoint] = true;
                lineNumber = fConflictNumHatchLineTab[indHatch][secondHatchComputePoint];
              }
            }
            if (solution == 0) {                  // could go to second solution
              indTmp = 0;
              while ((solution == 0) && (indTmp < firstComputeConflictNumHatchLineTab[indHatch].size())) {
                if ((firstComputeConflictNumHatchLineTab[indHatch][indTmp] == (int)lineNumber) && (firstComputePointsEnable[indTmp] == false)) {
                  solution =2;
                  fPoints.push_back(firstComputePoints[indTmp+firstPointTabInd]);
                  fVertices.back() ++;
                  firstComputePointsEnable[indTmp] = true;
                  lineNumber = firstComputeConflictNumHatchLineTab[indHatch][indTmp];
                  firstHatchComputePoint = indTmp;
                  currentHatch =0;
                }
                indTmp ++;
              }
            }
            if (solution == 0) {                    // could go to first solution
              indTmp = 0;
              while ((solution == 0) && (indTmp < aNumber)) {

                if ((fHatchShiftToMatchPointVec[indTmp] > ((float)fFirstNumHatch+(float)indHatch-fStripWidth))
                    && (fHatchShiftToMatchPointVec[indTmp] < ((float)fFirstNumHatch+(float)indHatch))
                    && ((indTmp == lineNumber) || (indTmp==lineNumber+1) || ((lineNumber == (aNumber-1)) && (indTmp ==0)))) {
                  find = false;
                  unsigned a =0;
                  while ((a<fVertices.back()) && (find == false)) {
                    if ((tabPoints[indTmp][0] == fPoints[a][0]) && (tabPoints[indTmp][1] == fPoints[a][1]) && (tabPoints[indTmp][2] == fPoints[a][2])) find = true;
                    a++;
                  }
                  if (find == false){
                    currentHatch =2;
                    solution = 3;
                    fPoints.push_back(tabPoints[indTmp]);
                    fVertices.back() ++;
                    if (lineNumber == indTmp) {
                      if (indTmp >0)  lineNumber =  indTmp-1;
                      else lineNumber = aNumber-1;
                    }
                    else {
                      if (indTmp < aNumber-1)  lineNumber =  indTmp;
                      else lineNumber = 0;
                    }
                  }
                }
                indTmp++;
              }
            }
          } // end of current hatch
          if (solution == 0) {
            // the end for this polyline
            // close polyline
            fPoints.push_back(fPoints[fPoints.size()-fVertices.back()]);
            fVertices.back() ++;
            result =true;
          }
        } // while solution !=0
        //      } // if result
    } // while result
    for (unsigned int a =0;a<fVertices.size();a++){
#ifdef HATCH_DEBUG 
      if (fVertices[a] <4) printf("SbHatch :: drawStripPolyline : WARNING A strip polyline has been compute with less than 3 points, it could be an error in the algorithm or a special case.\n\n");
#endif
    }
    
    firstPointTabInd += firstComputeConflictNumHatchLineTab[indHatch].size();
    secondPointTabInd += fConflictNumHatchLineTab[indHatch].size();
  } //end for
  return TRUE;
}




//////////////////////////////////////////////////////////////////////////////
// draw the hatch into the polyline bounding box giving in argument
// return FALSE if :
//    - All points are not in the same plan
//    - There is a precision error on one or more point
//////////////////////////////////////////////////////////////////////////////

SbBool SbHatch::computeSinglePolyline (SbVec3f* tabPoints,unsigned int aNumber)
{
  std::vector<SbVec3f> listNormalVec;
  int numberOfPolylinePoints =0;
  fPoints.resize(0);
  fPoints.clear();
  int precisionError =0;
  unsigned int firstOffset =0;
  fFirstNumHatch =0;
  fNumberHatchToDraw =0;
  fVertices.resize(0);
  fVertices.clear();

  if ( tabPoints[0].equals(tabPoints[1].getValue(),FLT_EPSILON*FLT_EPSILON*10)) {
    firstOffset =1;  }

  SbVec3f* listPoints = new SbVec3f[aNumber+1-firstOffset];

  for (unsigned int i=0;i<aNumber;i++){
    if ((i==0) || (listPoints[i-1] !=tabPoints[i+firstOffset])) {
      listPoints[numberOfPolylinePoints] = tabPoints[i+firstOffset];
      numberOfPolylinePoints++;
    }
  }

  // add the first point on last position to close the line
  if ( ! listPoints[0].equals(listPoints[numberOfPolylinePoints-1].getValue(),FLT_EPSILON*FLT_EPSILON*10)) {
    listPoints[numberOfPolylinePoints]=listPoints[0];
    numberOfPolylinePoints ++;
  }

  // use to test the polyline and to build the shift vector. A is the first point,
  // B second and C the last (in fact, the last-1)!
  SbVec3f AB,AC;
  AB.setValue(listPoints[1].getValue()[0]-listPoints[0].getValue()[0],
              listPoints[1].getValue()[1]-listPoints[0].getValue()[1],
              listPoints[1].getValue()[2]-listPoints[0].getValue()[2]); // Vector A->B

  fResolveResult = COLINEAR;
  unsigned int test = numberOfPolylinePoints-1;
  while ((fResolveResult !=0) && (test>1)) {
    test--;
    AC.setValue(listPoints[test].getValue()[0]-listPoints[0].getValue()[0],
                listPoints[test].getValue()[1]-listPoints[0].getValue()[1],
                listPoints[test].getValue()[2]-listPoints[0].getValue()[2]);
    
    // test if AB != AC*i
    resolveSystem( AB,
                   AC,
                   SbVec3f(.0f,.0f,.0f));
  }
  if (fResolveResult == COLINEAR) {
#ifdef HATCH_DEBUG
    printf("SbHatch :: drawPolyline : ERROR all the point you give are colinear!\n\n");
    for (unsigned int a =0;a<aNumber;a++) {
      printf(" %f %f %f \n",listPoints[a][0],listPoints[a][1],listPoints[a][2]);  }
#endif
    return FALSE;
  }

  ///////////////////////////////////////////////////////////////
  // creation of the dirVec. It is done with the dirAngle field
  // The angle is the one between the first line (point 1-point0)
  // and the dirVec, on the plan delimited by polyline
  // Given in the direct axis ((point1-point0),(lastPoint-point0),normalPlanVec)
  // Normal plane Vector = AB x AC
  ///////////////////////////////////////////////////////////////
  if (fFirstPolyline == TRUE) {
    
    fFirstPolyline =FALSE;
    
    fNormal.setValue(AB[1]*AC[2]-AB[2]*AC[1],
                               AB[2]*AC[0]-AB[0]*AC[2],
                               AB[0]*AC[1]-AB[1]*AC[0]);
    
    
    // ABPerp Vector = normal x AB
    SbVec3f ABPerpVector;
    ABPerpVector.setValue(fNormal[1]*AB[2]-fNormal[2]*AB[1],
                          fNormal[2]*AB[0]-fNormal[0]*AB[2],
                          fNormal[0]*AB[1]-fNormal[1]*AB[0]);
    
    float normAB =(float)sqrt(pow(AB[0],2)+
                        pow(AB[1],2)+
                        pow(AB[2],2));
    float normABPerpVector =(float)sqrt(pow(ABPerpVector[0],2)+
                        pow(ABPerpVector[1],2)+
                        pow(ABPerpVector[2],2));

    float j = tan(fDirAngle)*normAB/normABPerpVector;

    if (normABPerpVector == 0){  // never done (should be test before)
#ifdef HATCH_DEBUG 
      printf("SbHatch :: drawPolyline : ERROR Impossible to compute the dir vector for hatch. Normal for this plan is null (normal for : point[0],point[1],lastPoint) point[0], point[1], last point are probably aligned\n\n");
#endif
      delete [] listPoints;
      return FALSE;
    }

    fDirVec = AB +(float)j*ABPerpVector;
    // normalize vector to unit on X or on Y
    if (fDirVec.getValue()[0] ==0){
      fDirVec[0] = fPrecisionFactor; // to get rid of somes errors
      fDirVec = fDirVec/fDirVec.getValue()[1]; // normalize on Y because X will be a big value
    } else {
      fDirVec = fDirVec/fDirVec.getValue()[0];
    }
    
    ///////////////////////////////////////////////////////////////
    // creation of the shiftVec thanks to the shift field
    ///////////////////////////////////////////////////////////////
    
    SbVec3f dirShiftVector;
    dirShiftVector.setValue(fNormal[1]*fDirVec.getValue()[2]-fNormal[2]*fDirVec.getValue()[1],
                            fNormal[2]*fDirVec.getValue()[0]-fNormal[0]*fDirVec.getValue()[2],
                            fNormal[0]*fDirVec.getValue()[1]-fNormal[1]*fDirVec.getValue()[0]);
    
    // normalize vector to match the shift size
    float param = 1.0f;
    param = (float)sqrt((pow(fShift,2))/(
                                        pow(dirShiftVector[0],2)+
                                        pow(dirShiftVector[1],2)+
                                        pow(dirShiftVector[2],2)));
    fShiftVec = dirShiftVector*param;

    // compute offset only if it was not given
    if ((fOffset[0] == FLT_MAX) && (fOffset[1] == FLT_MAX) && (fOffset[2] == FLT_MAX)){
      fOffset = listPoints[0].getValue()+fShiftVec*fOffsetValue;
    }
  }
 
  
  /////////////////////////////////////////////
  // START to compute 
  // We compute each line one by one to know witch hatch will be draw thrue this line
  // we try to know the result of 
  // (origin_point_of_hatch)+i*(directionVector)+j*(shiftVector) = each_point_of_polyline
  // We will be interest only on j factor for the moment. This factor represent the offset
  // between the Origin point of the hatch and the compute point of the polyline
  // We put results in a float table
  //
  // We also have to memorize the min and max number of the hatch to be draw
  // Point                  0 1 2 3 4 5 6 ...n 1
  // hatchShiftToMatchPoint   5 7 2 6 7 8 5 ...2 5
  // min = 1 max = 8   -> 8 hatch to draw
  ////////////////////////////////////////////
  
  fHatchShiftToMatchPointVec.resize(numberOfPolylinePoints+1);
  float minShiftHatch =FLT_MAX;
  float maxShiftHatch =-FLT_MAX;
  SbVec2f res;

  for (int a=0;a<numberOfPolylinePoints;a++) {
    res = resolveSystem(fDirVec.getValue(),
                        fShiftVec,
                        listPoints[a].getValue()-fOffset);
    // test result
    if (fResolveResult ==0 ) {
         fHatchShiftToMatchPointVec[a] = res[1];
         if (res[1]>maxShiftHatch) {
           maxShiftHatch = res[1];
         }
         if (res[1]<minShiftHatch) {
           minShiftHatch = res[1];
         }
    }
    else {  // never done (should be test before)
#ifdef HATCH_DEBUG
      printf("SbHatch :: drawPolyline : ERROR one or more of your polyline points are not on the same plan ! Testing point %d/%d error:%d\n\n",a,numberOfPolylinePoints,fResolveResult);
#endif
      delete [] listPoints;
      return FALSE;
    }
  }
  // for the first point to close the polyline
  fHatchShiftToMatchPointVec[numberOfPolylinePoints] = fHatchShiftToMatchPointVec[0]; 
  fFirstNumHatch = (int)(ceilf(minShiftHatch));
  fNumberHatchToDraw = (int)(floorf(maxShiftHatch)-fFirstNumHatch+1);
  if ((int)(floorf(maxShiftHatch)-fFirstNumHatch+1) <0) fNumberHatchToDraw =0;

  int moreNumberHatchToDraw = fNumberHatchToDraw+1;
  std::vector<SbVec3f> listHatchStartPoint;
  std::vector<SbVec3f> listHatchEndPoint;
  std::vector<int> numberOfStartEndPointsVec;
  
  fConflictNumHatchLineTab.resize(moreNumberHatchToDraw);

  // initialize tab
    for (int a=0;a<moreNumberHatchToDraw;a++) {
      numberOfStartEndPointsVec.push_back(0);
      listHatchStartPoint.push_back(SbVec3f(.0f,.0f,.0f));
      listHatchEndPoint.push_back(SbVec3f(.0f,.0f,.0f));
      fConflictNumHatchLineTab[a].clear();
    }
    
  /////////////////////////////////////////////
  // Compute the normalize shift vector for all lines
  // the normal Vector for point 3 to 4 will be listNormalvec[2]
  /////////////////////////////////////////////
  
  for (int a=0;a<numberOfPolylinePoints-1;a++) {
    res = resolveSystem(fDirVec.getValue(),
                        SbVec3f(listPoints[a].getValue()[0]-listPoints[a+1].getValue()[0],
                                listPoints[a].getValue()[1]-listPoints[a+1].getValue()[1],
                                listPoints[a].getValue()[2]-listPoints[a+1].getValue()[2]),
                        -fShiftVec);
    if (fResolveResult ==0 ) {
      listNormalVec.push_back(SbVec3f(res[1]*(listPoints[a+1].getValue()[0]-listPoints[a].getValue()[0]),
                                       res[1]*(listPoints[a+1].getValue()[1]-listPoints[a].getValue()[1]),
                                       res[1]*(listPoints[a+1].getValue()[2]-listPoints[a].getValue()[2])
                                       ));
    }
    else  if (fResolveResult ==Z_ERROR ) {  // never done (should be test before)
#ifdef HATCH_DEBUG
      printf("SbHatch :: drawPolyline : ERROR one or more of your polyline points are not on the same plan !\n\n");
#endif
      delete [] listPoints;
      return FALSE;
    }
    else{
      listNormalVec.push_back(SbVec3f(FLT_MAX,FLT_MAX,FLT_MAX));
      //      listNormalVec.append(new SbVec3f(FLT_MAX,FLT_MAX,FLT_MAX));
    }
 }
 
  /////////////////////////////////////////////
  // Compute the hatchShiftToMatchPointVec table to try to get the start
  // and end point of each hatch
  // if there is more than one start/end point, we will resolve it later. For the moment,
  // we put confict points into a table
  // HatchNumber        1     2     3      4      5      6      7     8    9
  // listHatchStartPoint  1,0,0  1,1,0  0,0,1  0,1,0  1,1,0  0,2,0  1,1,4   
  // listHatchEndPoint    .............. 
  // conflictNumHatchLineTab 5 6 7    
  // line Number is 0 for (point[0]->point[1])
  // We put each line number into the conflict table to be sure to get all the lines
  //  in conflict. When we will thest the value of the conflicy table, it should
  // be greater than 2 to have a conflict
  /////////////////////////////////////////////
  
  SbVec3f newPoint;
  int minHatch;
  int maxHatch;   
  int hatchIndice =0;
  
  for (int indPolyline=0;indPolyline<numberOfPolylinePoints-1;indPolyline++) {
    minHatch = (int)(ceilf(fHatchShiftToMatchPointVec[indPolyline]));
    maxHatch = (int)(floorf(fHatchShiftToMatchPointVec[indPolyline+1]));

    if (fHatchShiftToMatchPointVec[indPolyline+1] <fHatchShiftToMatchPointVec[indPolyline]) {
      minHatch =(int)(ceilf(fHatchShiftToMatchPointVec[indPolyline+1]));
      maxHatch = (int)(floorf(fHatchShiftToMatchPointVec[indPolyline]));
    }
    for (int b=minHatch;b<=maxHatch;b++) {  // for all number of hatch fund
      // compute new point
      hatchIndice = b-fFirstNumHatch;

      newPoint.setValue(listPoints[indPolyline].getValue()[0]+
                        listNormalVec[indPolyline][0]*(b-fHatchShiftToMatchPointVec[indPolyline]),
                        listPoints[indPolyline].getValue()[1]+
                        listNormalVec[indPolyline][1]*(b-fHatchShiftToMatchPointVec[indPolyline]),
                        listPoints[indPolyline].getValue()[2]+
                        listNormalVec[indPolyline][2]*(b-fHatchShiftToMatchPointVec[indPolyline]));

      if (numberOfStartEndPointsVec[hatchIndice] == 0) {// it is the first point
        //compute point and save it
        // the start point will be :
        // Point_of_the_line + normalVec *
        //(number_of_hatch_to_compute - number_of_hatch_corresponding_to_first_point_of_line)
        //        
        if ( (listNormalVec[indPolyline][0] != FLT_MAX)
            && (listNormalVec[indPolyline][1] != FLT_MAX)
            && (listNormalVec[indPolyline][2] != FLT_MAX)) {
          listHatchStartPoint[hatchIndice] = SbVec3f(newPoint);
           fConflictNumHatchLineTab[hatchIndice].push_back(indPolyline);
           numberOfStartEndPointsVec[hatchIndice]++;
         }
      } else if (numberOfStartEndPointsVec[hatchIndice] == 1) { // it is the second point
        //compute point and save it (same point as previous )
        // the start point will be :
        // Point_of_the_line + normalVec *
        //  (number_of_hatch_to_compute - number_of_hatch_corresponding_to_first_point_of_line)
        // store only if newPoint is != start
        if ((listNormalVec[indPolyline][0] != FLT_MAX)
            && (listNormalVec[indPolyline][1] != FLT_MAX)
            && (listNormalVec[indPolyline][2] != FLT_MAX)) {
          listHatchEndPoint[hatchIndice] = SbVec3f(newPoint);
          fConflictNumHatchLineTab[hatchIndice].push_back(indPolyline);
          numberOfStartEndPointsVec[hatchIndice]++;
        }
      } else { // there is a conflict, we don't compute anything except for conflicts on points
        // witch are already compute
        // case of the hatch will be draw on a point of the polyline,
        // so it match 2 lines + another
        fConflictNumHatchLineTab[hatchIndice].push_back(indPolyline); // put the line number in conflict table
      }
    }
  }

  /////////////////////////////////////////////
  // Compute the numHatchLine tab and draw correct points
  /////////////////////////////////////////////
  std::vector<float> listCoefDirHatch(fNumberHatchToDraw);
  std::vector<SbVec3f> listConflictPoints(numberOfPolylinePoints);

  SbVec3f ABVec,tempVec;
  int valid =FALSE;
  int drawEnabled = FALSE; // TRUE : we could draw second point, FALSE we wait for the first
  float temp=0;
  int tempInt =0;
  float nextPointConflictHatchNumber = -FLT_MAX;
  float currentPointConflictHatchNumber = -FLT_MAX;
  std::vector<unsigned int> orderConflictLineNumber;
  
  for (unsigned int hatchNumber =0;hatchNumber<fNumberHatchToDraw;hatchNumber++) {
    if ( fConflictNumHatchLineTab[hatchNumber].size() <= 2) {
      if (!listHatchStartPoint[hatchNumber].equals(listHatchEndPoint[hatchNumber],FLT_EPSILON*FLT_EPSILON*10)) {
        fPoints.push_back(listHatchStartPoint[hatchNumber]);
        fPoints.push_back(listHatchEndPoint[hatchNumber]);
        fVertices.push_back(2);
      }
    } else { // there is a conflict
      // We read the conflict table and compute all the conflict lines
      // conflict is on hatch number hatchNumber+ firstNumHatch
      // Compute the equation on the conflict line (called ABVec ):
      // i*dirVec - j*ABVec = A-(offset + shiftVec * numberHatchToDraw)
      // and store the i parameter
      // then we 

      listConflictPoints.clear();
      listCoefDirHatch.clear();
      std::vector <unsigned int> toRemove;
      for (unsigned int conflictLineNumber=0;conflictLineNumber<fConflictNumHatchLineTab[hatchNumber].size();conflictLineNumber++ )
        {

          ABVec.setValue(listPoints[fConflictNumHatchLineTab[hatchNumber][conflictLineNumber]+1].getValue()[0]-listPoints[fConflictNumHatchLineTab[hatchNumber][conflictLineNumber]].getValue()[0],
                         listPoints[fConflictNumHatchLineTab[hatchNumber][conflictLineNumber]+1].getValue()[1]-listPoints[fConflictNumHatchLineTab[hatchNumber][conflictLineNumber]].getValue()[1],
                         listPoints[fConflictNumHatchLineTab[hatchNumber][conflictLineNumber]+1].getValue()[2]-listPoints[fConflictNumHatchLineTab[hatchNumber][conflictLineNumber]].getValue()[2]);

          res = resolveSystem(fDirVec.getValue(),
                              ABVec,
                              SbVec3f(listPoints[fConflictNumHatchLineTab[hatchNumber][conflictLineNumber]].getValue()[0]-fOffset[0]-((float)hatchNumber+(float)fFirstNumHatch)*fShiftVec[0],
                                      listPoints[fConflictNumHatchLineTab[hatchNumber][conflictLineNumber]].getValue()[1]-fOffset[1]-((float)hatchNumber+(float)fFirstNumHatch)*fShiftVec[1],
                                      listPoints[fConflictNumHatchLineTab[hatchNumber][conflictLineNumber]].getValue()[2]-fOffset[2]-((float)hatchNumber+(float)fFirstNumHatch)*fShiftVec[2]));

          if (fResolveResult ==0 ) {
            // we store results
            listCoefDirHatch.push_back(2);
            listCoefDirHatch.pop_back();
            listCoefDirHatch.push_back(res[0]);
            res[1] = -res[1];
            listConflictPoints.push_back(SbVec3f(listPoints[fConflictNumHatchLineTab[hatchNumber][conflictLineNumber]].getValue()+ABVec*res[1]));
          }
          else if (fResolveResult != COLINEAR){
#ifdef HATCH_DEBUG
            printf("SbHatch : Precision error during compute on hatch number%d\n\n",hatchNumber);
#endif
            precisionError++;
          } else {
            toRemove.push_back(conflictLineNumber);
          }
        }

      if (toRemove.size()) {
        for (unsigned int conflictLineNumber=0;conflictLineNumber<fConflictNumHatchLineTab[hatchNumber].size();conflictLineNumber++ ) {
        }
        // remove potential colinear problems
        for (unsigned int aa=0;aa<toRemove.size();aa++) {
          unsigned int ind = 0;
          for (std::vector<int>::iterator it = fConflictNumHatchLineTab[hatchNumber].begin();it !=fConflictNumHatchLineTab[hatchNumber].end();it++) {
            if (ind == toRemove[aa]) {
              fConflictNumHatchLineTab[hatchNumber].erase(it);
              break;
            }
            ind++;
          }
        }
        for (unsigned int conflictLineNumber=0;conflictLineNumber<fConflictNumHatchLineTab[hatchNumber].size();conflictLineNumber++ ) {
        }
      }
      if (listCoefDirHatch.size() != 0) { // all points are resolveSystem errors (COLINEAR or Z_ERROR

        // now, we have to sort all coef dir from minus to max 
        // and at the same time, reorder the conflict ponts and the conflict line number
        // this algorithm is not optimum...
        valid = FALSE;
        while (valid ==FALSE )
          {
            valid = TRUE;
            for (unsigned int sort =0;sort< listCoefDirHatch.size()-1;sort++)
              {
                if (listCoefDirHatch[sort]>listCoefDirHatch[sort+1]) {

                  temp = listCoefDirHatch[sort];
                  listCoefDirHatch[sort] = listCoefDirHatch[sort+1];
                  listCoefDirHatch[sort+1] =temp;
                  tempVec = listConflictPoints[sort];
                  listConflictPoints[sort] = listConflictPoints[sort+1];
                  listConflictPoints[sort+1] = tempVec;
                  tempInt = fConflictNumHatchLineTab[hatchNumber][sort];
                  fConflictNumHatchLineTab[hatchNumber][sort] = fConflictNumHatchLineTab[hatchNumber][sort+1];
                  fConflictNumHatchLineTab[hatchNumber][sort+1] = tempInt;
                  valid= FALSE;
                }
              } 
          }

        // once dir coef have been sort, we could draw lines !!
        //witch line had made a conflict ??? conflictNumHatchLineTab[a]
        unsigned int conflictNumber =0;
        orderConflictLineNumber.clear();

        drawEnabled = FALSE;
        while (conflictNumber < fConflictNumHatchLineTab[hatchNumber].size()) { // while
          if (conflictNumber+1 == fConflictNumHatchLineTab[hatchNumber].size()) {
            if (drawEnabled == TRUE) {
              drawEnabled =  FALSE;
              fPoints.push_back(listConflictPoints[conflictNumber].getValue());
              orderConflictLineNumber.push_back(fConflictNumHatchLineTab[hatchNumber][conflictNumber]);
            }
          }
          else {
            // if the conflict point == next conflict point : that is a end/begin line conflict
            // else, this is not a big problem, we just have to invert the drawEnabled
            // (if we were drawing, we have to finish a line, else, we have to begin a line
            if ( !(listConflictPoints[conflictNumber].equals(listConflictPoints[conflictNumber+1],FLT_EPSILON*FLT_EPSILON*10))) {
              // special case of nextPointline=nextConflict point : hatch//line
              unsigned int follow=conflictNumber+1;
              bool overContour = false;
              while ((follow <fConflictNumHatchLineTab[hatchNumber].size()) &&
                     (listConflictPoints[conflictNumber].equals(listConflictPoints[follow],FLT_EPSILON*FLT_EPSILON*10))) {
                follow++;
              }
              //test if next point is on the contour
              if(follow < fConflictNumHatchLineTab[hatchNumber].size()) {
                if ((listConflictPoints[follow].equals(listPoints[fConflictNumHatchLineTab[hatchNumber][follow]].getValue(),FLT_EPSILON*FLT_EPSILON*10))) {
                  if ((fConflictNumHatchLineTab[hatchNumber][follow] != 0) &&
                      (fConflictNumHatchLineTab[hatchNumber][follow] != numberOfPolylinePoints-1)) {
                    if ((listConflictPoints[conflictNumber].equals(listPoints[fConflictNumHatchLineTab[hatchNumber][follow]-1].getValue(),FLT_EPSILON*FLT_EPSILON*10)) ||
                        (listConflictPoints[conflictNumber].equals(listPoints[fConflictNumHatchLineTab[hatchNumber][follow]+1].getValue(),FLT_EPSILON*FLT_EPSILON*10))) {
                      overContour = true;
                    }
                  }
                }
              }
              int previous=conflictNumber-1;
              while ((previous >=0) &&
                     (listConflictPoints[conflictNumber].equals(listConflictPoints[previous],FLT_EPSILON*FLT_EPSILON*10))) {
                previous--;
              }
              //test if next point is on the contour
              if(previous >= 0) {
                if ((listConflictPoints[conflictNumber].equals(listPoints[fConflictNumHatchLineTab[hatchNumber][conflictNumber]].getValue(),FLT_EPSILON*FLT_EPSILON*10))) {
                  if ((listConflictPoints[previous].equals(listPoints[fConflictNumHatchLineTab[hatchNumber][conflictNumber]-1].getValue(),FLT_EPSILON*FLT_EPSILON*10)) ||
                      (listConflictPoints[previous].equals(listPoints[fConflictNumHatchLineTab[hatchNumber][conflictNumber]+1].getValue(),FLT_EPSILON*FLT_EPSILON*10))) {
                    overContour = true;
                  }
                }
              }
              if (!overContour) { // we are not on a contour, we can draw
                fPoints.push_back(listConflictPoints[conflictNumber].getValue());
                orderConflictLineNumber.push_back(fConflictNumHatchLineTab[hatchNumber][conflictNumber]);
                drawEnabled = drawEnabled == TRUE ? FALSE : TRUE;
                if (drawEnabled == TRUE) {
                  fVertices.push_back(2);
                }
              } else { // else we have to stop drawing
                if (drawEnabled == TRUE) {
                  fPoints.push_back(listConflictPoints[conflictNumber].getValue());
                  orderConflictLineNumber.push_back(fConflictNumHatchLineTab[hatchNumber][conflictNumber]);
                  drawEnabled = FALSE;
                }
              }
            }
            else { // next point == current
              bool currentPointCrossLine = false;
              bool nextPointCrossLine = false;
              // if the conflict is on a line point, we have to look the hatch number
              // of the previous and next point to see if the hatch had to be draw or not
              
              // test if conflictPoint == first line point
              if (listConflictPoints[conflictNumber].equals(listPoints[fConflictNumHatchLineTab[hatchNumber][conflictNumber]].getValue(),FLT_EPSILON*FLT_EPSILON*10)) {
                // we look second point hatchNumber
                currentPointConflictHatchNumber = fHatchShiftToMatchPointVec[fConflictNumHatchLineTab[hatchNumber][conflictNumber]+1];
              }
              else if (listConflictPoints[conflictNumber].equals(listPoints[fConflictNumHatchLineTab[hatchNumber][conflictNumber]+1].getValue(),FLT_EPSILON*FLT_EPSILON*10)) {
                // we look first point hatchNumber
                currentPointConflictHatchNumber = fHatchShiftToMatchPointVec[fConflictNumHatchLineTab[hatchNumber][conflictNumber]];
              }
              else { // case of two lines have intersection point on a hatch
                // it is the same case as a "end of line" and a "begin of line" conflict
                currentPointCrossLine = true;
                currentPointConflictHatchNumber =-1 ;
              }
              // test if conflictPoint == second line point
              if (listConflictPoints[conflictNumber+1].equals(listPoints[fConflictNumHatchLineTab[hatchNumber][conflictNumber+1]].getValue(),FLT_EPSILON*FLT_EPSILON*10)) {
                // we look second point hatchNumber
                nextPointConflictHatchNumber = fHatchShiftToMatchPointVec[fConflictNumHatchLineTab[hatchNumber][conflictNumber+1]+1];
              }
              else if (listConflictPoints[conflictNumber+1].equals(listPoints[fConflictNumHatchLineTab[hatchNumber][conflictNumber+1]+1].getValue(),FLT_EPSILON*FLT_EPSILON*10)) {
                // we look first point hatchNumber
                nextPointConflictHatchNumber = fHatchShiftToMatchPointVec[fConflictNumHatchLineTab[hatchNumber][conflictNumber+1]];
              }
              else { // case of two lines have intersection point on a hatch
                // it is the same case as a "end of line" and a "begin of line" conflict
                nextPointConflictHatchNumber = -1; 
                nextPointCrossLine = true;
              }

              // we have to compute the currentPointConflictHatchNumber and
              // nextPointConflictHatchNumber
              // if they are all the same side of the hatch, we have to ignore points
              // else, we have to draw a line
              if (currentPointCrossLine && nextPointCrossLine) {
                // do not draw anything, this is the case of a hatch crossing
                //  two identical line
              }
              // case of two points on  conflict on a contour point where nothing has to be draw
              else if ((!currentPointCrossLine && !nextPointCrossLine) && (currentPointConflictHatchNumber == nextPointConflictHatchNumber) && (currentPointConflictHatchNumber == fHatchShiftToMatchPointVec[fConflictNumHatchLineTab[hatchNumber][conflictNumber]])) {
                if (drawEnabled == TRUE) {
                  fPoints.push_back(listConflictPoints[conflictNumber].getValue());
                  orderConflictLineNumber.push_back(fConflictNumHatchLineTab[hatchNumber][conflictNumber]);
                  drawEnabled = FALSE;
                }                
              }
              // we draw
              else if( ( (currentPointConflictHatchNumber -
                          fHatchShiftToMatchPointVec[fConflictNumHatchLineTab[hatchNumber][conflictNumber]]) *
                         (nextPointConflictHatchNumber - 
                          fHatchShiftToMatchPointVec[fConflictNumHatchLineTab[hatchNumber][conflictNumber]]))
                        <=FLT_EPSILON) {
                // try to see if we are trying to draw a hatch OVER a contour
                unsigned int follow=conflictNumber+1;
                bool overContour = false;
                while ((follow <fConflictNumHatchLineTab[hatchNumber].size()) &&
                       (listConflictPoints[conflictNumber].equals(listConflictPoints[follow],FLT_EPSILON*FLT_EPSILON*10))) {
                  follow++;
                }
                if(follow < fConflictNumHatchLineTab[hatchNumber].size()) {
                  float alpha = 0;
                  bool findAlpha = true;
                  if (listConflictPoints[follow][0] != listConflictPoints[conflictNumber][0]) {
                    alpha = (listPoints[fConflictNumHatchLineTab[hatchNumber][follow]][0]-listConflictPoints[conflictNumber][0])/(listConflictPoints[follow][0]-listConflictPoints[conflictNumber][0]);
                  }
                  else if (listConflictPoints[follow][1] != listConflictPoints[conflictNumber][1]) {
                    alpha = (listPoints[fConflictNumHatchLineTab[hatchNumber][follow]][1]-listConflictPoints[conflictNumber][1])/(listConflictPoints[follow][1]-listConflictPoints[conflictNumber][1]);
                  }
                  else if (listConflictPoints[follow][2] != listConflictPoints[conflictNumber][2]) {
                    alpha = (listPoints[fConflictNumHatchLineTab[hatchNumber][follow]][2]-listConflictPoints[conflictNumber][2])/(listConflictPoints[follow][2]-listConflictPoints[conflictNumber][2]);
                  }
                  else {
                    findAlpha =false;
                  }
                  if (findAlpha) {
                    if ((alpha*(listConflictPoints[follow]-listConflictPoints[conflictNumber])).equals(listPoints[fConflictNumHatchLineTab[hatchNumber][follow]]-listConflictPoints[conflictNumber],FLT_EPSILON*FLT_EPSILON*10)) {
                      overContour = true;
                    }
                  }
                }
                if (!overContour) { // if we are not on a contour, no problem
                  fPoints.push_back(listConflictPoints[conflictNumber].getValue());
                  orderConflictLineNumber.push_back(fConflictNumHatchLineTab[hatchNumber][conflictNumber]);
                  drawEnabled = drawEnabled == TRUE ? FALSE : TRUE;
                  if (drawEnabled == TRUE) {
                    fVertices.push_back(2);
                  }
                } else { // else we have to stop drawing
                  if (drawEnabled == TRUE) {
                    fPoints.push_back(listConflictPoints[conflictNumber].getValue());
                    orderConflictLineNumber.push_back(fConflictNumHatchLineTab[hatchNumber][conflictNumber]);
                    drawEnabled = FALSE;
                  }
                }
              }
              conflictNumber ++;
            } // end next== current
          }
          conflictNumber ++;
        } // end while
        if (drawEnabled) {
          fPoints.push_back(fPoints[fPoints.size()-1]);
#ifdef HATCH_DEBUG
          printf("SbHatch : Probably a error during conflict resolution on hatch number %d :\nWe have close this line by putting two times the same point.\n\n",hatchNumber);
#endif
        }
        //re put the order conflictNumHatchLineTab witch could be use by stripWidth
        fConflictNumHatchLineTab[hatchNumber].clear();
        for(unsigned int a=0;a<orderConflictLineNumber.size();a++) {
          fConflictNumHatchLineTab[hatchNumber].push_back(orderConflictLineNumber[a]);}
        
        // test if it is correct
      } // end resolve system errors
    }  // end conflict
  }

  if (fPoints.size() >0){

    if (precisionError == 0){
      delete [] listPoints;
      return TRUE;
    }
    else {
#ifdef HATCH_DEBUG
      printf("SbHatch : Exit with %d precision error during compute\n\n",precisionError);
#endif
      delete [] listPoints;
      return FALSE;
    }
  }
  delete [] listPoints;
  return TRUE;
}



//////////////////////////////////////////////////////////////////////////////
// Compute a vector system equation aA+bB=C
// return SbVec2f(0,0) if there is an error
// set the resolveResult variable to the error code :
// COLINEAR if A and B are 
// PRECISION_ERROR if there is a lack of precision in computing
// Z_ERROR if there s no solution for Z
// UNDEFINED never throw
// return a SbVec2f  for result. a is 'x' value and b is 'y' if it is correct
//////////////////////////////////////////////////////////////////////////////

SbVec2f SbHatch :: resolveSystem(const SbVec3f& A,const SbVec3f& B,const SbVec3f& C) {

  fResolveResult = UNDEFINED;
  
  double Ax = A[0];
  double Ay = A[1];
  double Az = A[2];
  double Bx = B[0];
  double By = B[1];
  double Bz = B[2];
  double Cx = C[0];
  double Cy = C[1];
  double Cz = C[2];

  double bDiv = (By*Ax-Ay*Bx);
  if (FFABS(bDiv) <=FLT_EPSILON) {
    // we have to test in a other order
    double tmp;
    tmp = Ax; Ax = Ay; Ay = Az; Az = tmp;
    tmp = Bx; Bx = By; By = Bz; Bz = tmp;
    tmp = Cx; Cx = Cy; Cy = Cz; Cz = tmp;

    bDiv = (By*Ax-Ay*Bx);
    
    if  (FFABS(bDiv) <=FLT_EPSILON) {
      // we have to test in a other order
      tmp = Ax; Ax = Ay; Ay = Az; Az = tmp;
      tmp = Bx; Bx = By; By = Bz; Bz = tmp;
      tmp = Cx; Cx = Cy; Cy = Cz; Cz = tmp;
      
      bDiv = (By*Ax-Ay*Bx);
      if (FFABS(bDiv) <=FLT_EPSILON) {
        fResolveResult = COLINEAR;
        return SbVec2f(0,0);
      }
    }
  }    
  double b= (Cy*Ax-Ay*Cx)/bDiv;
  double a= -(Cy*Bx-By*Cx)/bDiv;  
  double  bid = FFABS(a*Az+b*Bz - Cz);
  
  if (bid <= FLT_EPSILON) {
    fResolveResult = OK;
    return SbVec2f((float)a,(float)b);
  }
  else {
    
    double minBoxValue = 1;
 
    double minXValue =FLT_MAX;
    double minYValue =FLT_MAX;
    double minZValue =FLT_MAX;
    if ((A[0] !=0) && ((A[0]) <minXValue)) minXValue = (A[0]);
    if ((B[0] !=0) && ((B[0]) <minXValue)) minXValue = (B[0]);
    if ((C[0] !=0) && ((C[0]) <minXValue)) minXValue = (C[0]);
    if ((A[1] !=0) && ((A[1]) <minYValue)) minYValue = (A[1]);
    if ((B[1] !=0) && ((B[1]) <minYValue)) minYValue = (B[1]);
    if ((C[1] !=0) && ((C[1]) <minYValue)) minYValue = (C[1]);
    if ((A[2] !=0) && ((A[2]) <minZValue)) minZValue = (A[2]);
    if ((B[2] !=0) && ((B[2]) <minZValue)) minZValue = (B[2]);
    if ((C[2] !=0) && ((C[2]) <minZValue)) minZValue = (C[2]);
    

    double maxXValue =-FLT_MAX;
    double maxYValue =-FLT_MAX;
    double maxZValue =-FLT_MAX;
    if ((A[0] !=0) && ((A[0]) >maxXValue)) maxXValue = (A[0]);
    if ((B[0] !=0) && ((B[0]) >maxXValue)) maxXValue = (B[0]);
    if ((C[0] !=0) && ((C[0]) >maxXValue)) maxXValue = (C[0]);
    if ((A[1] !=0) && ((A[1]) >maxYValue)) maxYValue = (A[1]);
    if ((B[1] !=0) && ((B[1]) >maxYValue)) maxYValue = (B[1]);
    if ((C[1] !=0) && ((C[1]) >maxYValue)) maxYValue = (C[1]);
    if ((A[2] !=0) && ((A[2]) >maxZValue)) maxZValue = (A[2]);
    if ((B[2] !=0) && ((B[2]) >maxZValue)) maxZValue = (B[2]);
    if ((C[2] !=0) && ((C[2]) >maxZValue)) maxZValue = (C[2]);
    
    if (((maxXValue-minXValue) <= (maxYValue-minYValue)) && ((maxXValue-minXValue) <= (maxZValue-minZValue))) { minBoxValue = maxXValue-minXValue; }
    else 
      if (((maxYValue-minYValue) <= (maxXValue-minXValue)) && ((maxYValue-minYValue) <= (maxZValue-minZValue))) { minBoxValue = maxYValue-minYValue; }
      else
        { minBoxValue = maxZValue-minZValue; }
    
    minBoxValue *= fPrecisionFactor;
    
    if (bid <= minBoxValue) {
      fResolveResult = OK;
      return SbVec2f((float)a,(float)b);
    }
    else {
      if (bid>100*minBoxValue) {
#ifdef HATCH_DEBUG
        printf("SbHatch : ***** PRECISON ERROR ON Z  ******* compare %f > %f res :%f %f test %f %f bDiv %e\n\n",bid,100*minBoxValue,a,b,a*Ax+b*Bx-Cx,a*Ay+b*By-Cy,bDiv);
#endif
        fResolveResult = Z_ERROR;
      }
      else
        {
#ifdef HATCH_DEBUG
          printf("SbHatch : ***** PRECISON ERROR  ******* compare %f > %f res :%f %f test %f %f bDiv %e\n\n",bid,100*minBoxValue,a,b,a*Ax+b*Bx-Cx,a*Ay+b*By-Cy,bDiv);
#endif
          fResolveResult = PRECISION_ERROR;
        }
      return SbVec2f(0,0);
    }
  }
  return SbVec2f(0,0);
}
