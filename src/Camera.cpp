// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Class that provides an abstraction for openGL textures.

#include <stdlib.h>
#include <err.h>
#include <GL/glew.h>
#include <unistd.h>
#include <cglm/cglm.h>
#include <cstring>
#include "Camera.h"




// =======================================================================================
// Useful static variables

vec3 zAxis = {0.0f, 0.0f, 1.0f};

// =======================================================================================
// Initialize a new camera view distance down the Z axis, with y axis as the up direction

Camera::Camera(Shader& S, float distance, float viewAngleDegrees):
                  shader(S),
                  speed(200.0f),
                  rotationalSpeed(30.0f),
                  mouseRotation(180.0f),
                  viewAngle(viewAngleDegrees),
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
  viewLoc = shader.getUniformLocation("view");
  projLoc = shader.getUniformLocation("projection");
  if(checkGLError(stderr, "Camera::Camera"))
    exit(-1);
  setProjectionMatrix();
  updateViewMatrix();
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
  // Compute the vector to the left side of the way the camera is currently oriented
  vec3 sideways;
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
  float aspectRatio = (float)(viewportParams[2]) / (float)(viewportParams[3]);
  glm_perspective(glm_rad(viewAngle), aspectRatio, near, far, projection);
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
// Provide a diagnostic page about this camera

bool Camera::diagnosticHTML(HttpDebug* serv)
{
  serv->startResponsePage("Camera");
    
  serv->addResponseData("<center>\n");
  serv->startTable();
  serv->addResponseData("<tr><th></th><th>X</th><th>Y</th><th>Y</th></tr>\n");

  //Position
  serv->addResponseData("<tr><td>Position</td><td>");
  serv->respPtr += sprintf(serv->respPtr, "%.1f</td><td>%.1f</td><td>%.1f</td></tr>\n",
                             pos[0], pos[1], pos[2]);

  //Front
  serv->addResponseData("<tr><td>Front</td><td>");
  serv->respPtr += sprintf(serv->respPtr, "%.4f</td><td>%.4f</td><td>%.4f</td></tr>\n",
                           front[0], front[1], front[2]);

  //Up
  serv->addResponseData("<tr><td>Up</td><td>");
  serv->respPtr += sprintf(serv->respPtr, "%.4f</td><td>%.4f</td><td>%.4f</td></tr>\n",
                           up[0], up[1], up[2]);

  serv->addResponseData("</table></center><hr>\n");
    

  serv->addResponseData("<b>viewAngle:</b> ");
  serv->respPtr += sprintf(serv->respPtr, "%.1f<br>\n",viewAngle);

  
  serv->endResponsePage();
  return true;
}


// =======================================================================================
