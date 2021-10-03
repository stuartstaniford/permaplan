// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Class that provides an abstraction for openGL textures.

#include "Camera.h"
#include "Logging.h"
#include "Scene.h"
#include "Shader.h"
#include "HttpDebug.h"
#include <stdlib.h>
#include <err.h>
#include <GL/glew.h>
#include <unistd.h>
#include <cglm/cglm.h>
#include <cstring>


// =======================================================================================
// Useful static variables

vec3 zAxis = {0.0f, 0.0f, 1.0f};


// =======================================================================================
///@ brief Constructor.
///
/// Initialize a new camera view distance down the Z axis, with y axis as the up 
/// direction.  Cameras are generally associated with a Window3D or one of it's 
/// subclasses and represent the view onto a Scene in that particular window.  
/// See also Camera::focusOnObject(), which is used in modifying the setup
/// here in the case of cameras in an ObjectInspectorWindow.
/// @param distance The distance up the Z axis that the camera is
/// @param viewAngleDegrees The width of the field of view in degrees.

Camera::Camera(float distance, float viewAngleDegrees):
                  speed(200.0f),
                  percentageSpeed(75.0f),
                  rotationalSpeed(60.0f),
                  mouseRotation(180.0f),
                  viewAngle(viewAngleDegrees),
                  aspectRatio(0.0f),
                  pivotLocation(NULL),
                  near(distance/100.0f),
                  far(distance*10.0f),
                  viewLoc(0xffffffff),
                  projLoc(0xffffffff)
{
  pos[0]    = 0.0f,
  pos[1]    = 0.0f,
  pos[2]    = distance;
  front[0]  = 0.0f;
  front[1]  = 0.0f;
  front[2]  = -1.0f;
  up[0]     = 0.0f;
  up[1]     = 1.0f;
  up[2]     = 0.0f;
  glm_vec3_cross(up, front, sideways);
}


// =======================================================================================
/// @brief Destructor

Camera::~Camera(void)
{
  
}


// =======================================================================================
/// @brief Set parameters to center a particular object in the view.
/// 
/// We are given the bounding box of the object as well as the front vector of a camera
/// from another window from which the object was selected.  We set our parameters up 
/// to face in the same direction as that vector in the x,y plane, we face towards the 
/// centroid of the bounding box from a height above the bounding box by 20% of it's 
/// vertical extent, and we back off the object such that it mostly but not altogether
/// fills the view.  We need to work with both tall narrow objects (eg conifers), and
/// low sprawling objects (eg ranch houses).
/// Note this function should be read in conjunction with the constructor, as it 
/// assumes that the Camera is in it's immediate post construction state.
/// @param bbox The pointer to the bounding box of the object.
/// @param camFront A reference to a vec3 representing the camera direction of the winow
/// that spawned a new window in which we are setting up this camera.

void Camera::focusOnObject(BoundingBox* bbox, vec3& camFront,  int pixWidth, int pixHeight)
{
  // Set ourselves as a camera pointing in the same direction as caller argument, but
  // with our up direction closest to the Z axis.
  glm_vec3_copy(camFront, front);
  getCrossVectors(front, up, sideways, 1.0f);

  // Get the center of the box, as corners will be relative to that.
  vec3 boxCentroid;
  bbox->getCentroid(boxCentroid);

  // Variables for keeping track of max distances away from centroid when projected
  // into the plane normal to the camera direction.
  float upMax   = -HUGE_VALF;
  float upMin   = HUGE_VALF;
  float sideMax = -HUGE_VALF; 
  float sideMin = HUGE_VALF;

  for(unsigned i=0; i<8;i++) // for each corner of the box
   {
    vec3 corner, inPlanePerp, inPlaneProj;
    float renormFactor, upComp, sideComp;
    
    // Get corner, make relative to centroid
    bbox->getVertexVectorByIndex(i, corner);
    glm_vec3_sub(corner, boxCentroid, corner); 
    
    // We need to compute the projection of the corner-centroid vector into the plane
    // normal to the camera direction.  
    
    // Start by getting a vector in the plane at right angles to both corner, and
    // camFront.
    glm_vec3_cross(camFront, corner, inPlanePerp);
    if(fabs(glm_vec3_norm(inPlanePerp)) < EPSILON)
      continue; // corner is pretty much colinear with camFront

    // Now this will give us a vector in the right direction in the plane
    glm_vec3_cross(inPlanePerp, camFront, inPlaneProj);
    
    // But we need to fix the size.
    renormFactor = glm_vec3_dot(inPlaneProj, corner)/glm_vec3_norm(inPlaneProj);
    glm_vec3_scale_as(inPlaneProj, renormFactor, inPlaneProj);
    
    // Ok, now project the inPlaneProj vector onto the camera basis of up and sideways
    upComp = glm_vec3_dot(inPlaneProj, up);
    sideComp = glm_vec3_dot(inPlaneProj, sideways);

    // Update our idea of the min and max up and sideways values of corners.
    if(upComp < upMin)
      upMin = upComp;
    if(upComp > upMax)
      upMax = upComp;
    if(sideComp < sideMin)
      sideMin = sideComp;
    if(sideComp > sideMax)
      sideMax = sideComp;
   }
  
  // Now need to compute how far away we have to be so that all the bounding box corners
  // will appear within a reasonable subset of the window.  For now, we just make sure
  // it's within 90% of the window size, but later we'll want to make some allowance for
  // control panels within the window.
  // https://glm.g-truc.net/0.9.9/api/a00243.html#ga747c8cf99458663dd7ad1bb3a2f07787
  
  // Allowances for margin around the edge of the window
  upMin   *= 1.1f;
  upMax   *= 1.1f;
  sideMin *= 1.1f;
  sideMax *= 1.1f;

  // Deal with the vertical direction
  float tanHalfYAngle = tanf(glm_rad(viewAngle/2.0f));
  float maxDist = upMax/tanHalfYAngle;
  float downDist = upMin/tanHalfYAngle;
  if(downDist > maxDist)
    maxDist = downDist;
  
  // Deal with the horizontal direction
  float tanHalfXAngle = tanHalfYAngle*pixWidth/pixHeight;
  float leftDist = sideMin/tanHalfXAngle;
  if(leftDist > maxDist)
    maxDist = leftDist;
  float rightDist = sideMax/tanHalfXAngle;
  if(rightDist > maxDist)
    maxDist = rightDist;
  
  // Ok, maxDist is now the distance of the camera from the centroid, so implement that.
  near = maxDist/100.0f;
  far = maxDist*10.0f;
  glm_vec3_scale_as(camFront, maxDist, camFront);
  glm_vec3_sub(boxCentroid, camFront, pos);
}


// =======================================================================================
/// @brief Log the main camera variables

void Camera::logFrames(void)
{
#ifdef LOG_CAMERA_FRAMES
  LogCameraFrames("Pos: [%.1f, %.1f, %.1f], Front: [%.1f, %.1f, %.1f], "
                      "Up: [%.1f, %.1f, %.1f]\n", pos[0], pos[1], pos[2],
                      front[0], front[1], front[2], up[0], up[1], up[2]);
#endif
}


// =======================================================================================
/// @brief Instantiate this camera into the shader

void Camera::makeActive(void)
{
  Shader& shader = Shader::getMainShader();
  viewLoc = shader.getUniformLocation("view");
  projLoc = shader.getUniformLocation("projection");
  if(checkGLError(stderr, "Camera::Camera"))
    exit(-1);
  updateAfterMove();
  if(checkGLError(stderr, "after updateViewMatrix"))
    exit(-1);
}


// =======================================================================================
/// @brief Implement all the supported keyboard camera operations.
/// 
/// These are expressed as bit flags like CAM_DELTA_UP, CAM_DELTA_DOWN, etc - see 
/// Camera.h for documentation.
/// @param opFlags A 32 bit unsigned with the flags to be implemented.
/// @param timeLapseUsec How many microseconds have lapsed (this is multiplied by 
/// Camera::speed or Camera::rotationalSpeed to dictate how much things change in response
/// to the keyboard input.

void Camera::adjust(unsigned opFlags, float timeLapseUsec)
{
  if(pivotLocation)
    return adjustWithPivot(opFlags, timeLapseUsec);
  
  // Compute the vector to the left side of the way the camera is currently oriented
  glm_vec3_cross(up, front, sideways);
  
  // move forward/back in the front direction but parallel to the x-y plane
  vec3 forward;
  glm_vec3_cross(sideways, zAxis, forward);

  // The amount of angle for the various rotational commands
  float rotAngle = glm_rad(rotationalSpeed*timeLapseUsec/1.0e6);
  
  // The amount of movement for the forward/back, etc commands
  float moveDist = speed*timeLapseUsec/1.0e6;
  vec3 forwardDelta, upDelta, sideDelta;
  glm_vec3_scale(forward,   moveDist, forwardDelta);
  glm_vec3_scale(sideways,  moveDist, sideDelta);
  glm_vec3_scale(zAxis,     moveDist, upDelta);

  // Pitch camera direction upwards at rotationalSpeed
  if(CAM_PITCH_UP & opFlags)
   {
    glm_vec3_rotate(front, -rotAngle, sideways);
    glm_vec3_rotate(up, -rotAngle, sideways);
   }
  
  // Pitch camera direction downwards at rotationalSpeed
  else if(CAM_PITCH_DOWN & opFlags)  // Note if up *and* down are set, up wins
   {
    glm_vec3_rotate(front, rotAngle, sideways);
    glm_vec3_rotate(up, rotAngle, sideways);
   }
  
  // Yaw camera direction left at rotationalSpeed
  if(CAM_YAW_LEFT & opFlags)
   {
    glm_vec3_rotate(front, rotAngle, zAxis);
    glm_vec3_rotate(up, rotAngle, zAxis);
   }
  
  // Yaw camera direction right at rotationalSpeed
  else if(CAM_YAW_RIGHT & opFlags)  // Note if left *and* right are set, left wins
   {
    glm_vec3_rotate(front, -rotAngle, zAxis);
    glm_vec3_rotate(up, -rotAngle, zAxis);
   }
  
  // Roll camera direction left at rotationalSpeed
  if(CAM_ROLL_LEFT & opFlags)
    glm_vec3_rotate(up, -rotAngle, front);
  
  // Roll camera direction right at rotationalSpeed
  else if(CAM_ROLL_RIGHT & opFlags)  // Note if forward *and* right are set, left wins
    glm_vec3_rotate(up, rotAngle, front);
  
  // Move camera forward at speed
  if(CAM_MOVE_FORWARD & opFlags)
    glm_vec3_add(pos, forwardDelta, pos);
  
  // Move camera back at speed
  else if(CAM_MOVE_BACK & opFlags)  // Note if forward *and* back are set, forward wins
    glm_vec3_sub(pos, forwardDelta, pos);

  // Move camera up at speed
  if(CAM_MOVE_UP & opFlags)
    glm_vec3_add(pos, upDelta, pos);
  
  // Move camera down at speed
  else if(CAM_MOVE_DOWN & opFlags)
    glm_vec3_sub(pos, upDelta, pos);
  
  // Move camera left at speed
  if(CAM_MOVE_LEFT & opFlags)
    glm_vec3_add(pos, sideDelta, pos);
  
  // Move camera right at speed
  else if(CAM_MOVE_RIGHT & opFlags)
    glm_vec3_sub(pos, sideDelta, pos); // Note if left *and* right are set, left wins
  
  updateAfterMove();
}


// =======================================================================================
/// @brief Implement the supported keyboard camera operations when a pivot location has 
/// been established and now we are going to move relative to that location.
/// @param opFlags A 32 bit unsigned with the flags to be implemented.
/// @param timeLapseUsec How many microseconds have lapsed (this is multiplied by 
/// Camera::speed or Camera::rotationalSpeed to dictate how much things change in response
/// to the keyboard input.
/// @todo This mode is really just annoying and probably needs to be replaced with 
/// dedicated inspector windows.

void Camera::adjustWithPivot(unsigned opFlags, float timeLapseUsec)
{
  // The amount of angle for the various rotational commands
  float rotAngle = glm_rad(rotationalSpeed*timeLapseUsec/1.0e6f);

  // The distance ratio for percentage based changes.
  float distRatio = 1.0f + percentageSpeed/100.0f*timeLapseUsec/1.0e6f;
  
  // The vector from the pivot location to the camera
  vec3 relativeCam;
  glm_vec3_sub(pos, pivotLocation, relativeCam);
  vec3 newRelativeCam;
  
  // Vector at right angles to the above and the zAxis
  if(glm_vec3_dot(relativeCam, zAxis) > EPSILON)
    glm_vec3_cross(relativeCam, zAxis, sideways);
  else
    glm_vec3_cross(relativeCam, up, sideways);

  // Rotate camera left around the pivot
  if(CAM_MOVE_LEFT & opFlags || CAM_YAW_LEFT & opFlags)
   {
    glm_vec3_rotate(front, -rotAngle, zAxis);
    glm_vec3_rotate(up, -rotAngle, zAxis);
    glm_vec3_rotate(relativeCam, -rotAngle, zAxis);
    glm_vec3_add(pivotLocation, relativeCam, pos);
   }
  
  // Rotate camera right around the pivot
  // Note the else - if left *and* right are set, left wins
  else if(CAM_MOVE_RIGHT & opFlags || CAM_YAW_RIGHT & opFlags)
   {
    glm_vec3_rotate(front, rotAngle, zAxis);
    glm_vec3_rotate(up, rotAngle, zAxis);
    glm_vec3_rotate(relativeCam, rotAngle, zAxis);
    glm_vec3_add(pivotLocation, relativeCam, pos);
   }

  // Move camera towards the pivot point
  if(CAM_MOVE_FORWARD & opFlags)
   {
    glm_vec3_scale(relativeCam, 1.0f/distRatio, newRelativeCam);
    glm_vec3_add(pivotLocation, newRelativeCam, pos);
   }
    
  // Move camera away from the pivot point
  // Note the else if forward *and* back are set, forward wins
  else if(CAM_MOVE_BACK & opFlags)
   {
    glm_vec3_scale(relativeCam, distRatio, newRelativeCam);
    glm_vec3_add(pivotLocation, newRelativeCam, pos);
   }

  // Rotate camera above the pivot point
  if(CAM_PITCH_UP & opFlags || CAM_MOVE_UP & opFlags)
   {
    glm_vec3_rotate(front, rotAngle, sideways);
    glm_vec3_rotate(up, rotAngle, sideways);
    glm_vec3_rotate(relativeCam, rotAngle, sideways);
    glm_vec3_add(pivotLocation, relativeCam, pos);
   }
    
  // Rotate camera to the horizontal plane from the pivot point
  // Note the else if up and down are set, up wins
  else if(CAM_PITCH_DOWN & opFlags || CAM_MOVE_DOWN & opFlags)
   {
    glm_vec3_rotate(front, -rotAngle, sideways);
    glm_vec3_rotate(up, -rotAngle, sideways);
    glm_vec3_rotate(relativeCam, -rotAngle, sideways);
    glm_vec3_add(pivotLocation, relativeCam, pos);
   }

  updateAfterMove();
}


// =======================================================================================
/// @brief Function to Update the camera matrices in the shader

void Camera::updateAfterMove(void)
{
  setProjectionMatrix();
  updateViewMatrix();  
}


// =======================================================================================
/// @brief Function to implement the effects of dragging the mouse in the window.  
/// @param xDelta float in [-1.0, 1.0] for how much to move in x-direction.
/// @param yDelta float in [-1.0, 1.0] for how much to move in y-direction.

void Camera::mouseDrag(float xDelta, float yDelta)
{
  // Compute the vector to the left side of the way the camera is currently oriented
  vec3 sideways;
  glm_vec3_cross(up, front, sideways);
  
  float upAngle = -glm_rad(mouseRotation*yDelta);
  glm_vec3_rotate(front, upAngle, sideways);
  glm_vec3_rotate(up, upAngle, sideways);

  float sideAngle = glm_rad(mouseRotation*xDelta);
  glm_vec3_rotate(front, sideAngle, zAxis);
  glm_vec3_rotate(up, sideAngle, zAxis);

  // Update the camera matrices in the shader
  setProjectionMatrix();
  updateViewMatrix();
}


// =======================================================================================
/// @brief Get a copy (ie read only) of the camera direction

void Camera::copyDirection(vec3& position, vec3& direction)
{
  memcpy(position, pos, sizeof(vec3));
  memcpy(direction, front, sizeof(vec3));
}


// =======================================================================================
/// @brief Function to set up and apply the projection matrix - assumed to be 
/// called 'projection'

void Camera::setProjectionMatrix(void)
{
  int viewportParams[4];
  glGetIntegerv(GL_VIEWPORT, viewportParams);
  aspectRatio = (float)(viewportParams[2]) / (float)(viewportParams[3]);
  glm_perspective(glm_rad(viewAngle), aspectRatio, near, far, projection);
  Shader& shader = Shader::getMainShader();
  shader.setUniform(projLoc, projection);
  if(checkGLError(stderr, "Camera::setProjectionMatrix"))
    exit(-1);
}


// =======================================================================================
/// @brief Function to set up and apply the view matrix - assumed to be 
/// called 'view'

void Camera::updateViewMatrix(void)
{
  vec3 target;
  
  glm_vec3_add(pos, front, target);
  glm_lookat(pos, target, up, view);
  Shader& shader = Shader::getMainShader();
  shader.setUniform(viewLoc, view);
  if(checkGLError(stderr, "Camera::updateViewMatrix"))
    exit(-1);
}


// =======================================================================================
/// @brief Invert the projection/view/model transformation.
/// 
/// Given the model matrix (from the scene) this will provide an inverse transformation 
/// from normal device co-ordinates to the model co-ordinates.  Useful in translating
/// on screen coordinates to points in the model.  Providing inverse of
/// projection*view*model (which is generally done in the vertex shader).
/// @param model Reference to the mat4 model matrix.
/// @param invertMatrix Reference to the matrix where the invert result will be stored.

void Camera::invertView(mat4& model, mat4& invertMatrix)
{
  mat4 product, product2;
  glm_mat4_mul(projection, view, product);
  glm_mat4_mul(product, model, product2);
  glm_mat4_inv(product2, invertMatrix);
}


// =======================================================================================
/// @brief Find a path in worldspace corresponding to the current screen coordinates.
/// Return a line (as a point and a vector in the line direction), given two coordinates
/// in the window (but normalized to [-1,1] as in clip space).  This approach inspired 
/// by http://schabby.de/picking-opengl-ray-tracing/
/// @param position The output ray position.
/// @param direction The output ray direction.
/// @param clipX The input X coordinate in clip space.
/// @param clipY The input Y coordinate in clip space

void Camera::rayFromScreenLocation(vec3& position, vec3& direction, float clipX, float clipY)
{
  vec3 viewX, viewY, viewZ;  // point in the X,Y,-Z direction in the view plane in world space

  glm_vec3_copy(pos, position);

  glm_vec3_cross(front, up, viewX);
  
  float yLength = tanf( glm_rad(viewAngle) / 2 )*near;
  float xLength = yLength * aspectRatio;
  
  xLength *= clipX;
  yLength *= clipY;

  glm_vec3_scale_as(viewX, xLength, viewX);
  glm_vec3_scale_as(up, yLength, viewY);
  glm_vec3_scale_as(front, near, viewZ);
  glm_vec3_add(viewX, viewY, direction);
  glm_vec3_add(direction, viewZ, direction);
}


// =======================================================================================
/// @brief Implement the API for setting camera variables (used extensively by test 
/// scripts).
/// @returns True if the desired data was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server
/// @param path A C-string for the remainder of the URL after the /camera/ substring

bool Camera::setApi(HttpDebug* serv, char* path, Scene& scene)
{
  vec3 trial;
  unless(strncmp(path, "set/", 4)== 0)
   {
    LogRequestErrors("Camera::setApi can't handle %s\n", path);
    return false;
   }
  path += 4;
  
  if(strncmp(path, "pos/", 4)== 0)
   {
    if(extractColonVec3(path+4, trial))
     {
      glm_vec3_copy(trial, pos);
      LogWindowOperations("Camera::setApi, camera position set to (%.1f, %.1f, %.1f).\n", 
                                                            pos[0], pos[1], pos[2]);
      goto goodExit;
     }
    LogRequestErrors("Camera::setApi couldn't extract vector from %s\n", path+4);
    return false;
   }

  if(strncmp(path, "front/", 6)== 0)
   {
    if(extractColonVec3(path+6, trial))
     {
      glm_vec3_copy(trial, front);
      glm_vec3_scale_as(front, 1.0f, front);
      LogWindowOperations("Camera::setApi, camera direction set to (%.1f, %.1f, %.1f).\n", 
                                                            front[0], front[1], front[2]);
      goto goodExit;
     }
    LogRequestErrors("Camera::setApi couldn't extract vector from %s\n", path+6);
    return false;
   }
  
  if(strncmp(path, "up/", 3)== 0)
   {
    if(extractColonVec3(path+3, trial))
     {
      glm_vec3_copy(trial, up);
      glm_vec3_scale_as(up, 1.0f, up);
      LogWindowOperations("Camera::setApi, camera up vector set to (%.1f, %.1f, %.1f).\n", 
                                                                  up[0], up[1], up[2]);
      goto goodExit;
     }
    LogRequestErrors("Camera::setApi couldn't extract vector from %s\n", path+3);
    return false;
   }

  LogRequestErrors("Camera::setApi unknown directive %s\n", path);
  return false;

goodExit:  
  
  InterfaceAction* action = new InterfaceAction(CameraMoved, NULL);
  scene.actions.push_back(action);
  httPrintf("OK\n");
  return true;
}


// =======================================================================================
/// @brief Provide a diagnostic page about this camera
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server
/// @param path A C-string for the remainder of the URL after the /camera/ substring.  If
/// there is anything, we will send the request to Camera::setApi to handle.

bool Camera::diagnosticHTML(HttpDebug* serv, char* path, Scene& scene)
{
  if(strlen(path) > 0)
    return setApi(serv, path, scene);
  
  serv->startResponsePage("Camera");
    
  httPrintf("<center>\n");
  serv->startTable((char*)"camSummary");
  httPrintf("<tr><th>Label</th><th>X</th><th>Y</th><th>Y</th></tr>\n");

  //Position
  httPrintf("<tr><td>Position</td><td>");
  httPrintf("%.1f</td><td>%.1f</td><td>%.1f</td></tr>\n", pos[0], pos[1], pos[2]);

  //Front
  httPrintf("<tr><td>Front</td><td>");
  httPrintf("%.4f</td><td>%.4f</td><td>%.4f</td></tr>\n", front[0], front[1], front[2]);

  //Up
  httPrintf("<tr><td>Up</td><td>");
  httPrintf("%.4f</td><td>%.4f</td><td>%.4f</td></tr>\n", up[0], up[1], up[2]);

  httPrintf("</table></center><hr>\n");

  httPrintf("<b>viewAngle:</b> ");
  httPrintf("%.1f<br>\n", viewAngle);
  
  serv->endResponsePage();
  return true;
}


// =======================================================================================
