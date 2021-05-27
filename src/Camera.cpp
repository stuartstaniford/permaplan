// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Class that provides an abstraction for openGL textures.

#include <stdlib.h>
#include <err.h>
#include <GL/glew.h>
#include <unistd.h>
#include <cglm/cglm.h>
#include <cstring>
#include "Camera.h"
#include "Logging.h"




// =======================================================================================
// Useful static variables

vec3 zAxis = {0.0f, 0.0f, 1.0f};

// =======================================================================================
// Initialize a new camera view distance down the Z axis, with y axis as the up direction

Camera::Camera(float distance, float viewAngleDegrees):
                  speed(200.0f),
                  percentageSpeed(75.0f),
                  rotationalSpeed(60.0f),
                  mouseRotation(180.0f),
                  viewAngle(viewAngleDegrees),
                  aspectRatio(0.0f),
                  near(distance/100.0f),
                  far(distance*10.0f)
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

  Shader& shader = Shader::getMainShader();
  viewLoc = shader.getUniformLocation("view");
  projLoc = shader.getUniformLocation("projection");
  if(checkGLError(stderr, "Camera::Camera"))
    exit(-1);
  setProjectionMatrix();
  updateViewMatrix();
  if(checkGLError(stderr, "after updateViewMatrix"))
    exit(-1);
}


// =======================================================================================
// Destructor

Camera::~Camera(void)
{
  
}


// =======================================================================================
// Function to implement all the supported keyboard camera operations (expressed as bit
// flags like CAM_DELTA_UP, CAM_DELTA_DOWN, etc - see Camera.h for documentation.

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
  
  // Update the camera matrices in the shader
  setProjectionMatrix();
  updateViewMatrix();
}


// =======================================================================================
// Function to implement all the supported keyboard camera operations when a pivot
// location has been established and now we are going to move relative to that location.

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

  // Update the camera matrices in the shader
  setProjectionMatrix();
  updateViewMatrix();
}


// =======================================================================================
// Function to implement the effects of dragging the mouse in the window.  Parameters
// range from -1.0 to 1.0.

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
// Used to get a copy (ie read only) the camera direction

void Camera::copyDirection(vec3& position, vec3& direction)
{
  memcpy(position, pos, sizeof(vec3));
  memcpy(direction, front, sizeof(vec3));
}


// =======================================================================================
// Function to set up and apply the projection matrix - assumed to be called 'projection'

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
// Bind a particular texture to a given active texture unit and name in the shader

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
// Given the model matrix (from the scene) this will provide an inverse transformation
// from normal device co-ordinates to the model co-ordinates.  Useful in translating
// on screen coordinates to points in the model.  Providing inverse of
// projection*view*model (which is generally done in the vertex shader).

void Camera::invertView(mat4& model, mat4& invertMatrix)
{
  mat4 product, product2;
  glm_mat4_mul(projection, view, product);
  glm_mat4_mul(product, model, product2);
  glm_mat4_inv(product2, invertMatrix);
}


// =======================================================================================
// Return a line (as a point and a vector in the line direction), given two coordinates
// in the window (but normalized to [-1,1] as in clip space).  This approach inspired by
// http://schabby.de/picking-opengl-ray-tracing/

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
// Implement the API for setting camera variables (used extensively by test scripts).

bool Camera::setApi(HttpDebug* serv, char* path)
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
      httPrintf("OK\n");
      return true;
     }
    LogRequestErrors("Camera::setApi couldn't extract vector from %s\n", path+4);
    return false;
   }

  if(strncmp(path, "front/", 6)== 0)
   {
    if(extractColonVec3(path+6, trial))
     {
      glm_vec3_copy(trial, front);
      LogWindowOperations("Camera::setApi, camera direction set to (%.1f, %.1f, %.1f).\n", 
                                                            front[0], front[1], front[2]);
      httPrintf("OK\n");
      return true;
     }
    LogRequestErrors("Camera::setApi couldn't extract vector from %s\n", path+6);
    return false;
   }
  
  LogRequestErrors("Camera::setApi unknown directive %s\n", path);
  return false;
}


// =======================================================================================
// Provide a diagnostic page about this camera

bool Camera::diagnosticHTML(HttpDebug* serv, char* path)
{
  if(strlen(path) > 0)
    return setApi(serv, path);
  
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
