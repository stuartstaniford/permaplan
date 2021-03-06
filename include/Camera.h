// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Class that manages the current location, direction, view angle, etc, of the camera

#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include "Shader.h"
#include "HttpDebug.h"
#include "GlobalMacros.h"


// Define flags used for passing multiple camera ops in parallel.  Intended to go
// in an unsigned 32 bit opFlags variable.

#define CAM_PITCH_UP          0x00000001  // Rotate camera direction upwards at rotationalSpeed
#define CAM_PITCH_DOWN        0x00000002  // Rotate camera direction downwards at rotationalSpeed
#define CAM_YAW_LEFT          0x00000004  // Rotate camera direction left at rotationalSpeed
#define CAM_YAW_RIGHT         0x00000008  // Rotate camera direction right at rotationalSpeed
#define CAM_ROLL_LEFT         0x00000010  // Roll camera to the left at rotationalSpeed
#define CAM_ROLL_RIGHT        0x00000020  // Roll camera to the right at rotationalSpeed
#define CAM_MOVE_FORWARD      0x00000040  // Move camera position forward at speed
#define CAM_MOVE_BACK         0x00000080  // Move camera position back at speed
#define CAM_MOVE_UP           0x00000100  // Move camera position up at speed
#define CAM_MOVE_DOWN         0x00000200  // Move camera position back at speed
#define CAM_MOVE_LEFT         0x00000400  // Move camera position left at speed
#define CAM_MOVE_RIGHT        0x00000800  // Move camera position right at speed
#define CAM_INTERFACE_ACTION  0x00001000  // Some kind of interface action (ScriptController.h)


// =======================================================================================
// Class variable initialization


class Camera
{
 public:

  // Instance variables - public
  
  // Member functions - public
  Camera(float distance, float viewAngleDegrees);
  ~Camera(void);
  void  adjust(unsigned opFlags, float timeLapseUsec);
  void  adjustWithPivot(unsigned opFlags, float timeLapseUsec);
  void  mouseDrag(float xDelta, float yDelta);
  void  copyDirection(vec3& position, vec3& direction);
  void  rayFromScreenLocation(vec3& position, vec3& direction, float clipX, float clipY);
  void  invertView(mat4& model, mat4& invertMatrix);
  bool  diagnosticHTML(HttpDebug* serv);
  inline void teleportUp(float distance) {adjust(CAM_MOVE_UP, distance/speed*1.0e6);}
  inline void  setPivotLocation(float* location) {pivotLocation = location;}

 private:
  
  // Instance variables - private
  vec3      pos;
  vec3      front;
  vec3      up;
  vec3      sideways;
  float     speed;             // feet/sec
  float     percentageSpeed;   // percent/sec
  float     rotationalSpeed;  // degrees/second
  float     mouseRotation;  // degrees/window size
  float     viewAngle;  // degrees
  float     aspectRatio;
  float*    pivotLocation;
  float     near;
  float     far;
  unsigned  viewLoc;
  unsigned  projLoc;
public:
  mat4      view;
  mat4      projection;
private:
  // Member functions - private
  void updateViewMatrix(void);
  void setProjectionMatrix(void);
  Camera(const Camera&);                 // Prevent copy-construction
  Camera& operator=(const Camera&);      // Prevent assignment

};

#endif


