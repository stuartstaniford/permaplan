// Copyright Staniford Systems.  All Rights Reserved.  Aug 2020 -
// A set of control points to manage a cubic bezier patch on some particular
// node in the Quadtree

#include <err.h>
#include "BezierPatch.h"
#include "HeightMarker.h"
#include "Arrow.h"


#define forAllControlIndices(i,j)   for(int i=0; i<4; i++) for(int j=0; j<4; j++)

PatchRayState* copyVer = NULL;
void BezierPatch::assertCopyVer(void)
{
  assert(lastRayMatch == copyVer);
}

// =======================================================================================
// Constructors.

// NB Two constructors!!  Probably update both!

BezierPatch::BezierPatch(Quadtree* qtree, unsigned gridPoints):
                        LandSurfaceRegion(qtree->bbox.lower[0], qtree->bbox.lower[1],
                              qtree->bbox.upper[0] - qtree->bbox.lower[0],
                              qtree->bbox.upper[1] - qtree->bbox.lower[1],
                              0.0f, 0.0f, 1.0f, 1.0f), gridN(gridPoints),
                        currentDelta(1.0f),
                        lastRayMatch(NULL)
#ifdef BEZIER_DUMP_DETAIL
                        , fitIterationCount(0)
#endif
{
}


BezierPatch::BezierPatch(float x, float y, float width, float height,
                        float s, float t, float sWidth, float tHeight, unsigned gridPoints):
                            LandSurfaceRegion(x, y, width, height, s, t, sWidth, tHeight),
                            gridN(gridPoints),
                            currentDelta(1.0f),
                            lastRayMatch(NULL)
#ifdef BEZIER_DUMP_DETAIL
                            , fitIterationCount(0)
#endif
{
}


// =======================================================================================
// Destructor

BezierPatch::~BezierPatch(void)
{
}


// =======================================================================================
// Utility function to compute the powers of u, 1-u, v, and 1-v for use in Bernstein
// polynomials

void BezierPatch::calcPowers(float u, float v)
{
  upow[0] = vpow[0] = u1minpow[0] = v1minpow[0] = 1.0f;
  for(int i=1; i<4; i++)
   {
    upow[i]     = u*upow[i-1];
    vpow[i]     = v*vpow[i-1];
    u1minpow[i] = (1.0f - u)*u1minpow[i-1];
    v1minpow[i] = (1.0f - v)*v1minpow[i-1];
   }
}


// =======================================================================================
// Computes the height of the patch at some particular location in parametric
// space. For background, see:
// https://www.scratchapixel.com/lessons/advanced-rendering/bezier-curve-rendering-utah-teapot/bezier-surface

float bern[4] = {1.0f, 3.0f, 3.0f, 1.0f};

void BezierPatch::surfacePoint(float u, float v, vec3 result)
{
  glm_vec3_zero(result);
  
  calcPowers(u,v);
  for(int i=0; i<4; i++)
    for(int j=0; j<4; j++)
     {
      float scale = bern[i]*upow[i]*u1minpow[3-i]*bern[j]*vpow[j]*v1minpow[3-j];
      vec3 scaledControlPoint;
      glm_vec3_scale(controlPoints[i][j], scale, scaledControlPoint);
      glm_vec3_add(result, scaledControlPoint, result);
     }
}


// =======================================================================================
// Create a new display list with markers for all the fit locations.

vec3  orangeColor  = {0.8f, 0.5f, 0.2f};

DisplayList* BezierPatch::newUVLocationList(void)
{
  DisplayList* D = new DisplayList();
  vec3 location;
  int k, N = fitPointUVVals.size();
  HeightMarker* H;
  
  for(k=0; k<N; k++)
   {
    surfacePoint(fitPointUVVals[k][0], fitPointUVVals[k][1], location);
    H = new HeightMarker(location);
    H->setNoTexColor(orangeColor);
    D->push_back(H);
   }
    
  return D;
}


// =======================================================================================
// Add the control points to a display list.

vec3  darkRedColor  = {0.6f, 0.1f, 0.1f};

void BezierPatch::addControlPointsToDisplayList(DisplayList* D)
{
  HeightMarker* H;
  
  for(int i=0; i<4; i++)
    for(int j=0; j<4; j++)
     {
      H = new HeightMarker(controlPoints[i][j]);
      H->setNoTexColor(darkRedColor);
      D->push_back(H);
     }
}


// =======================================================================================
// Add arrows showing the current direction of the

vec3  lighterRedColor  = {0.9f, 0.1f, 0.1f};

void BezierPatch::addControlGradientsToDisplayList(DisplayList* D)
{
  Arrow* A;
  vec3 direction;
  
  forAllControlIndices(i,j)
   {
    //currentDelta
    glm_vec3_copy(gradientControlPoints[i][j], direction);
    glm_vec3_negate(direction);
    A = new Arrow(controlPoints[i][j], direction);
    A->setNoTexColor(lighterRedColor);
    D->push_back(A);
   }
}


// =======================================================================================
// For anyone who needs to know how much space we would take up in a triangleBuffer.

void BezierPatch::triangleBufferSizes(unsigned& vertexCount, unsigned& indexCount)
{
  vertexCount = (gridN+1)*(gridN+1);
  indexCount = 6*gridN*gridN;
}


// =======================================================================================
// How much space we need in a triangle buffer

void BezierPatch::triangleBufferSize(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void BezierPatch::draw(void)
{
  return;
}


// =======================================================================================
// Buffer our triangles.

#define forAllUVGrid(i,j,u,v, limit, spacing)     int i,j; float u,v; float spacing = 1.0f/(float)gridN;\
                                for (i=0, u=0.0f; i<limit; i++, u+=spacing) \
                                  for (j=0, v=0.0f; j<limit; j++, v+=spacing)


bool BezierPatch::bufferGeometry(TriangleBuffer* T)
{
  // Establish space needs and obtain buffers
  VertexBufElement* vertices;
  unsigned* indices;
  unsigned vOffset;
  unsigned vCount, iCount;
  triangleBufferSizes(vCount, iCount);
  unless(T->requestSpace(&vertices, &indices, vOffset, vCount, iCount))
    return false;

  //Figure out the vertices
  forAllUVGrid(i,j,u,v, gridN+1, spacing)
   {
    VertexBufElement* bufEl = vertices + i*(gridN + 1) + j;
    surfacePoint(u, v, bufEl->pos);
    bufEl->tex[0] = stPos[0] + stExtent[0]*spacing*(float)i;
    bufEl->tex[1] = stPos[1] + stExtent[1]*spacing*(float)j;
    bufEl->accent = 0.0f;
    //printf("u,v: %.3f, %.3f\t", u, v);
    //bufEl->fprint(stdout);
   }

  for (int i=0; i<gridN; i++)
    for (int j=0; j<gridN; j++)
     {
      unsigned base = 6*(i*gridN + j); //Location of the i,j square in indices
      
      // Lower left triangle in i,j gridcell
      indices[base] = i*(gridN + 1) + j;
      indices[base + 1] = (i+1)*(gridN + 1) + j;
      indices[base + 2] = i*(gridN + 1) + j + 1;
      
      // Upper right triangle in i,j gridcell
      indices[base + 3] = indices[base + 1];
      indices[base + 4] = (i+1)*(gridN + 1) + j + 1;
      indices[base + 5] = indices[base + 2];
     }

  return true;
}


// =======================================================================================
// When we have no idea where/whether a ray will match, we check every
// triangle in the tesselation.


bool BezierPatch::matchRayAll(vec3& position, vec3& direction, float& lambda)
{
  if(!lastRayMatch)
   {
    lastRayMatch          = new PatchRayState;
    copyVer               = lastRayMatch;
    lastRayMatch->parent  = this;
   }
  //XXX note the algorithm in here calculates each vertex six times.  If this turns
  // out to be a bottleneck, we could cache the answers in some way to reduce the
  // cpu usage.
  
  forAllUVGrid(i, j, u, v, gridN, spacing)
   {
    // Lower left triangle
    surfacePoint(u, v, lastRayMatch->triangle[0]);
    surfacePoint(u+spacing, v, lastRayMatch->triangle[1]);
    surfacePoint(u, v+spacing, lastRayMatch->triangle[2]);
    if(lastRayMatch->matchRay(position, direction, lambda))
     {
      lastRayMatch->lowerLeft = true;
      goto GOT_HIT;
     }

    // Upper right triangle
    surfacePoint(u+spacing, v, lastRayMatch->triangle[0]);
    surfacePoint(u+spacing, v+spacing, lastRayMatch->triangle[1]);
    surfacePoint(u, v+spacing, lastRayMatch->triangle[2]);
    if(lastRayMatch->matchRay(position, direction, lambda))
     {
      lastRayMatch->lowerLeft = false;
      goto GOT_HIT;
     }
   }

  LogBezierMatchRay("matchRayAll did not match.\n");
  return false;

GOT_HIT:
  lastRayMatch->uv[0]   = u;
  lastRayMatch->uv[1]   = v;
  lastRayMatch->spacing = spacing;
  LogBezierMatchRay("matchRayAll hit at u,v: %.3f, %.3f\n", u,v);
  return true;
}


// =======================================================================================
// Decide whether a given ray intersects with a neighboring triangle or not.

bool PatchRayState::matchNeighbor(vec3 rayPos, vec3 rayDir, float& outT)
{
  vec3 neighbor[3];
  
  // First think about the other square in the grid, which is cheaper to compute
  
  if(lowerLeft)
    getUpperRight(neighbor);
  else
    getLowerLeft(neighbor);
  if(mollerTrumbore(neighbor, rayPos, rayDir, outT))
   {
    memcpy(triangle, neighbor, sizeof(neighbor));
    lowerLeft = ~lowerLeft;
    LogBezierMatchRay("Mating triangle to last match succeeded.\n");
    return true;
   }

  // Now check the neighbors
  float trialU, trialV;
  for(int i = -1; i <= 1; i++)
   {
    trialU = uv[0] + spacing*i;
    if(trialU < 0.0f || trialU + spacing > 1.0f)
      continue;
    for(int j = -1; j <= 1; j++)
     {
      if(i==j==0) // already did that case before the loops
        continue;
      trialV = uv[1] + spacing*j;
      if(trialV < 0.0f || trialV + spacing > 1.0f)
        continue;
      getLowerLeft(neighbor, trialU, trialV);
      if(mollerTrumbore(neighbor, rayPos, rayDir, outT))
       {
        lowerLeft = true;
        goto MATCH_NEIGHBOR_FOUND;
       }
      getUpperRight(neighbor, trialU, trialV);
       {
        lowerLeft = false;
        goto MATCH_NEIGHBOR_FOUND;
       }
     }
   }
  
  LogBezierMatchRay("matchNeighbor failed to find hit.\n");
  return false;

MATCH_NEIGHBOR_FOUND:
  memcpy(triangle, neighbor, sizeof(neighbor));
  uv[0] = trialU;
  uv[1] = trialV;
  LogBezierMatchRay("matchNeighbor hit at u,v: %.3f, %.3f\n", uv[0], uv[1]);
  return true;
}


// =======================================================================================
// Decide whether a given ray intersects with our patch or not.

bool BezierPatch::matchRay(vec3& position, vec3& direction, float& lambda)
{
  assertCopyVer();
  if(!box || !box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the patch itself.
  if(!lastRayMatch)
   {
    LogBezierMatchRay("First time to match all.\n");
    if(matchRayAll(position, direction, lambda))
      return true;
    else
      return false;
   }
  
  // We had a match on a previous invocation
  if(lastRayMatch->matchRay(position, direction, lambda))
   {
    // Awesome, it still works.  We expect this to be the usual case.
    LogBezierMatchRay("Rematch on last ray match succeeded.\n");
    return true;
   }
  if(lastRayMatch->matchNeighbor(position, direction, lambda))
    return true;
     
  // All else has failed, so apply brute force again
  return matchRayAll(position, direction, lambda);
}


// =======================================================================================
// Recompute our bounding box when a caller needs it done.  We rely on the fact
// that a Bezier patch lies entirely in the compact hull of the control points, so
// if we do min/max on all the control points, we will have an axis-aligned
// bounding box (although not necessarily an optimal one as the surface does not
// typically touch the interior control points).

void BezierPatch::updateBoundingBox(void)
{
  vec3 bottomCorner, topCorner;
  
  for(int i=0; i<3;i++)
   {
    bottomCorner[i] = HUGE_VALF;
    topCorner[i] = -HUGE_VALF;
   }
  
  forAllControlIndices(i,j)
    for(int m=0; m<3; m++)
     {
      if(controlPoints[i][j][m] < bottomCorner[m])
        bottomCorner[m] = controlPoints[i][j][m];
      if(controlPoints[i][j][m] > topCorner[m])
        topCorner[m] = controlPoints[i][j][m];
     }
  
  if(!box)
    box = new BoundingBox(bottomCorner, topCorner);
  else
    box->reset(bottomCorner, topCorner);

   return;
}


// =======================================================================================
// This creates a random Bezier patch (which can later be improved).

#define setControlPoints(i,j,x,y,z) controlPoints[i][j][0]=(x);controlPoints[i][j][1]=(y);controlPoints[i][j][2]=(z)
#define randHeight arc4random()/(float)UINT32_MAX*100.0f-50.0f

void BezierPatch::randomFit(std::vector<float*>& locations)
{
  forAllControlIndices(i,j)
   {
    setControlPoints(i, j, xyPos[0] + i/3.0f*extent[0],
                        xyPos[1] + j/3.0f*extent[1], randHeight);
   }
  updateBoundingBox();
}


// =======================================================================================
// This creates a flat Bezier patch at the average height of the locations, which can
// later be improved

void BezierPatch::levelFit(std::vector<float*>& locations)
{
  float average = 0.0f;
  int N = locations.size();
  for(int i=0; i<N; i++)
    average += locations[i][2];
  average /= N;
  
  forAllControlIndices(i,j)
   {
    setControlPoints(i, j, xyPos[0] + i/3.0f*extent[0],
                   xyPos[1] + j/3.0f*extent[1], average);
   }
  updateBoundingBox();
}


// =======================================================================================
// Make an initial guess at the u,v values of fitpoints on the parametric surface, based
// on the assumption that the relative locations of the heighmarkers within the patch are
// a reasonable starting point.  (These will subsequently be iteratively improved).

void BezierPatch::setUpUVVals(std::vector<float*>& locations)
{
  int k, N = locations.size();
  for(k=fitPointUVVals.size(); k<N; k++)
   {
    float* uv = new vec2;
    uv[0] = (locations[k][0] - xyPos[0])/extent[0];
    uv[1] = (locations[k][1] - xyPos[1])/extent[1];
    fitPointUVVals.push_back(uv);
   }
}


// =======================================================================================
// Compute the current fit of the patch to the known locations.  Summed square distance.

float BezierPatch::estimateFit(std::vector<float*>& locations)
{
  float sum = 0.0f;
  int i, N = locations.size();
  vec3 estimatedLocation;
  vec3 distVec;
  
  for(i=0; i<N; i++)
   {
    surfacePoint(fitPointUVVals[i][0], fitPointUVVals[i][1], estimatedLocation);
    glm_vec3_sub(locations[i], estimatedLocation, distVec);
    sum += glm_vec3_norm2(distVec);
   }
  
  return sum;
}


// =======================================================================================
// Make a copy of this structure, allocating new copies of all pointed-to things

void BezierPatch::copyFitPointUVVals(void)
{
  int i, N = fitPointUVVals.size();
  int M = copyOfFitPointUVVals.size();
  
  for(i=0; i<N; i++)
   {
    if(i<M)
      glm_vec3_copy(fitPointUVVals[i], copyOfFitPointUVVals[i]);
    else
     {
      float* uv = new vec2;
      glm_vec3_copy(fitPointUVVals[i], uv);
      copyOfFitPointUVVals.push_back(uv);
     }
   }
}


// =======================================================================================
// Make a copy of the control points structure.

void BezierPatch::copyControlPoints(void)
{
  forAllControlIndices(i,j)
    glm_vec3_copy(controlPoints[i][j], copyOfControlPoints[i][j]);
}


// =======================================================================================
// Compute the direction of maximum increase in the fit distance.  This will be hard
// to understand without reviewing the notes in gradient.pdf

void BezierPatch::computeGradientVector(std::vector<float*>& locations)
{
  // i, j indexes which control point we are talking about.
  // k indexes over the N locations (and their mapped u,v estimated points)
  // m indexes over x,y,z directions.
  int i, j, k, m, N = locations.size();
  
  // First precompute a subexpression S[k][m].  See notes in gradient.pdf
  float* S = new float[3*N];
  
  for(k=0; k<N; k++)
    for(m=0; m<3; m++)
     {
      S[3*k+m] = 0.0f;
      calcPowers(fitPointUVVals[k][0], fitPointUVVals[k][1]);
      for(i=0;i<4;i++)
        for(j=0;j<4;j++)
          S[3*k+m] += controlPoints[i][j][m]*bern[i]*bern[j]
                                  *upow[i]*u1minpow[3-i]*vpow[j]*v1minpow[3-j];
     }
  
  // First deal with the gradient with respect to the control points
  for(i=0;i<4;i++)
    for(j=0;j<4;j++)
      for(m=0;m<3;m++)
       {
        gradientControlPoints[i][j][m] = 0.0f;
        for(k=0;k<N;k++)
         {
          calcPowers(fitPointUVVals[k][0], fitPointUVVals[k][1]);
          gradientControlPoints[i][j][m] += -2.0f*(locations[k][m] - S[3*k+m])*
                              bern[i]*bern[j]*upow[i]*u1minpow[3-i]*vpow[j]*v1minpow[3-j];
         }
       }

#define upow_P(i)     ((i)<0?0.0f:upow[(i)])
#define vpow_P(i)     ((i)<0?0.0f:vpow[(i)])
#define u1minpow_P(i) ((i)<0?0.0f:u1minpow[(i)])
#define v1minpow_P(i) ((i)<0?0.0f:v1minpow[(i)])

  // Then deal with the uvFitPoints
  float sumU, sumV, minisumU, minisumV;
  float commonExpression;
  for(k=0; k<N; k++)
   {
    sumU = sumV = 0.0f; // Track as we sum over m
    calcPowers(fitPointUVVals[k][0], fitPointUVVals[k][1]);
    for(m=0; m<3; m++)
     {
      minisumU = minisumV = 0.0f; // Track as we sum over i,j
      for(i=0;i<4;i++)
        for(j=0;j<4;j++)
         {
          commonExpression = controlPoints[i][j][m]*bern[i]*bern[j];
          minisumU += commonExpression*vpow[j]*v1minpow[3-j]*
                    (u1minpow[3-i]*i*upow_P(i-1) - upow[i]*(3-i)*u1minpow_P(3-i-1));
          minisumV += commonExpression*upow[i]*u1minpow[3-i]*
                    (v1minpow[3-j]*j*vpow_P(j-1) - vpow[j]*(3-j)*v1minpow_P(3-j-1));
         }
      commonExpression = -2.0f*(locations[k][m] - S[3*k+m]);
      minisumU *= commonExpression;
      minisumV *= commonExpression;
      sumU += minisumU;
      sumV += minisumV;
     }
    
    if(gradientFitPointUVVals.size() > k)
     {
      gradientFitPointUVVals[k][0] = sumU;
      gradientFitPointUVVals[k][1] = sumV;
     }
    else
     {
      float* newGradient  = new float[2];
      newGradient[0]    = sumU;
      newGradient[1]    = sumV;
      gradientFitPointUVVals.push_back(newGradient);
     }
   }
  delete[] S;
}


//=======================================================================================
// Move the specified delta in the opposite direction of the gradient vector (ie
// downslope).

void BezierPatch::applyGradientVector(void)
{
  // i, j indexes which control point we are talking about.
  // k indexes over the N locations (and their mapped u,v estimated points)
  // m indexes over x,y,z directions.
  // n indexes over u,v directions.

  // First deal with the control points
  forAllControlIndices(i,j)
    for(int m=0;m<3;m++)
      controlPoints[i][j][m] -= currentDelta*gradientControlPoints[i][j][m];

  // Now the fit points
  /*
  int k, n, N = fitPointUVVals.size();
  for(k=0; k<N; k++)
    for(n=0; n<2; n++)
      fitPointUVVals[k][n] -= currentDelta*gradientFitPointUVVals[k][n];
*/
  
}


//=======================================================================================
// We tried something that didn't work out.  Go back to where we were before.

void BezierPatch::revertGradientVector(void)
{
  // i, j indexes which control point we are talking about.
  // k indexes over the N locations (and their mapped u,v estimated points)
  // m indexes over x,y,z directions.
  // n indexes over u,v directions.
  int i, j, k, m, n, N = fitPointUVVals.size();
  
  // First deal with the control points
  for(i=0;i<4;i++)
    for(j=0;j<4;j++)
      for(m=0;m<3;m++)
        controlPoints[i][j][m] = copyOfControlPoints[i][j][m];
  
  // Now the fit points
  for(k=0; k<N; k++)
    for(n=0; n<2; n++)
      fitPointUVVals[k][n] = copyOfFitPointUVVals[k][n];
}


//=======================================================================================
// Make an incremental improvement in the fit of the patch to the known locations.

#define fitTolerance 1e-5

bool BezierPatch::improveFit(std::vector<float*>& locations)
{
  unless(fitPointUVVals.size() == locations.size())
    setUpUVVals(locations);
  
  float fitDist = estimateFit(locations);
  LogBezierFit("fitDist is %.1f, applying delta %.9f\n", fitDist, currentDelta);
  
  copyFitPointUVVals();
  copyControlPoints();

  computeGradientVector(locations);
  
#ifdef BEZIER_DUMP_DETAIL
  char name[16];
  sprintf(name, "bezDetail%d.txt", fitIterationCount++);
  dumpDetailState(name);
#endif
  
  applyGradientVector();
  float newFitDist = estimateFit(locations);
  LogBezierFit("fitDist now %.3f\n", newFitDist);

  // See if it's an improvement or not
  if(newFitDist >= fitDist && !(currentDelta < fitTolerance))
   {
    // Presumably overshot the mark
    currentDelta /= 2.0f;
    revertGradientVector();
    return true;
   }
  else if( (fitDist - newFitDist)/fitDist < fitTolerance || currentDelta < fitTolerance
           || fitDist < 0.1f)
   {
    // Call it good
    LogBezierFit("Terminating with improvement of %.12f\n", (fitDist - newFitDist)/fitDist);
    updateBoundingBox();
    return false;
   }
  else
   {
    // We improved it, see if we can be more aggressive next time
    currentDelta *= 2.0f;
    updateBoundingBox();
    assertCopyVer();
    return true;
   }
}


// =======================================================================================
// Dump out one array as part of dumpDetailState

void printControlPointArray(FILE* file, char* title, vec3 myArray[4][4])
{
  fprintf(file, "%s.\n\n", title);

  forAllControlIndices(i,j)
   {
    fprintf(file, "%d, %d:", i, j);
    for(int m=0;m<3;m++)
      fprintf(file, "\t%f", myArray[i][j][m]);
    fprintf(file, "\n");
   }
  fprintf(file, "\n");
}


// =======================================================================================
// Dump out one array as part of dumpDetailState

void printUVVector(FILE* file, char* title, std::vector<float*>& myVec)
{
  int k, N = myVec.size();
  
  fprintf(file, "%s.\n\n", title);

  for(k=0; k<N; k++)
    fprintf(file, "%d:\t%f\t%f\n", k, myVec[k][0], myVec[k][1]);

  fprintf(file, "\n");
}


// =======================================================================================
// Dump out lots of data for debugging purposes

void BezierPatch::dumpDetailState(char* fileName)
{
  FILE* file = fopen(fileName, "w");
  if(!file)
    err(-1, "Couldn't open %s for writing BezierPatch detail state.", fileName);
  
  printControlPointArray(file, (char*)"Control Points", controlPoints);
  printControlPointArray(file, (char*)"Gradient of Control Points", gradientControlPoints);

  printUVVector(file, (char*)"fitPointUVVals", fitPointUVVals);
  printUVVector(file, (char*)"gradientFitPointUVVals", gradientFitPointUVVals);

  fclose(file);
}


// =======================================================================================
// Tell callers our name at runtime.

const char* BezierPatch::objectName(void)
{
  static char* name = (char*)"Bezier Patch";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular chunk of land surface.  Since BezierPatch is a bit complex
// we have a little mini-table of all our control points inside the cell.

bool BezierPatch::diagnosticHTML(HttpDebug* serv)
{
  serv->addResponseData("<tr><td>BezierPatch</td><td>");
  serv->addResponseData("<table cellpadding=1 border=1><tr><th>i</th><th>j</th><th>X</th><th>Y</th><th>Z</th></tr>");
  forAllControlIndices(i,j)
    serv->respPtr += sprintf(serv->respPtr,
              "<tr><td>%d</td><td>%d</td><td>%.1f</td><td>%.1f</td><td>%.1f</td></tr>",
              i, j, controlPoints[i][j][0], controlPoints[i][j][1], controlPoints[i][j][2]);
  serv->addResponseData("</table>\n");
#ifdef VISUALIZE_FITTING
  serv->addResponseData("Fit Points<br>");
  serv->addResponseData("<table cellpadding=1 border=1><tr><th>k</th><th>u</th><th>v</th><th>X</th><th>Y</th><th>Z</th></tr>");
  int k, N = fitPointUVVals.size();
  for(k=0; k<N; k++)
   {
    serv->respPtr += sprintf(serv->respPtr, "<tr><td>%d</td><td>%.3f</td><td>%.3f</td>\n",
                            k, fitPointUVVals[k][0], fitPointUVVals[k][1]);
    vec3 xyz;
    surfacePoint(fitPointUVVals[k][0], fitPointUVVals[k][1], xyz);
    serv->respPtr += sprintf(serv->respPtr, "<td>%.1f</td><td>%.1f</td><td>%.1f</td></tr>\n",
                             xyz[0], xyz[1], xyz[2]);
   }
  serv->addResponseData("</table>\n");
#endif
  serv->addResponseData("</td></tr>\n");

  return true;
}


// =======================================================================================
