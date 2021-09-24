// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class ..... is a template for real classes

#include "MenuFocusOverlay.h"
#include "Window3D.h"


// =======================================================================================
/// @brief Constructor
///
/// The constructor is mostly a pass through to MenuPanel, but also is where the 
/// displayVisible is set to true as this should be always on.
/// @param menu Pointer to the overall MenuInterface so we can call back into it. 
/// @param S A pointer to the scene.
/// @param A reference to our window (required for mouse coords etc).

MenuFocusOverlay::MenuFocusOverlay(MenuInterface* menu, Scene* S, Window3D& W):
                                                            MenuPanel(menu, S),
                                                            win3D(W)
{
  displayVisible = true; // focus overlay panel shows by default
}


// =======================================================================================
/// @brief Destructor

MenuFocusOverlay::~MenuFocusOverlay(void)
{
}


// =======================================================================================
// Transparent overlay that displays information about the thing currently in focus

void MenuFocusOverlay::imGuiDisplay(void)
{
  if(!displayVisible)
    return;
  static int corner = 0;
  
  setCorner(corner);
  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
  if (corner != -1)
    window_flags |= ImGuiWindowFlags_NoMove;
  if (ImGui::Begin("Focus Overlay", &displayVisible, window_flags))
   {
    //scene->findCameraObject(camF);
    scene->focusObject = scene->findObjectFromWindowCoords(win3D.camera, 
                    scene->focusObjectLocation,
                    win3D.lastMouseX/win3D.width*2.0f-1.0f,
                    1.0f - win3D.lastMouseY/win3D.height*2.0f);
    if(scene->focusObject)
     {
      ImGui::Text("Object Type: %s\n", scene->focusObject->objectName());
      ImGui::Text("Coords: %.1f' east, %.1f' north\nAltitude: %.1f'\n",
                  scene->focusObjectLocation[0], scene->focusObjectLocation[1],
                  scene->focusObjectLocation[2]);
      mouseOverlayDisplays(scene->focusObjectLocation);
     }
    else
      ImGui::Text("Mouse out of scene->\n");

    ImGui::Text("Camera Height: %.1f'\n", scene->findCameraHeight(win3D.camera));
    ImGui::Text("Frames/Sec: %.0f\n", 1.0f/win3D.appParent.frameTimeAvg);
    ImGui::Separator();
   }
  ImGui::End();
}


// =======================================================================================
/// @brief Stuff that may or may not be turned on in the overlay window concerning the mouse
/// location.
///
/// @todo This needs to be run-time selectable as an "Advanced" part of the panel.

void MenuFocusOverlay::mouseOverlayDisplays(vec3 mouseSceneLoc)
{
  //ImGui::Text("Mouse: %.1f E, %.1f N %.1f altitude\n",
  //            mouseSceneLoc[0], mouseSceneLoc[1], mouseSceneLoc[2]);
  
  // Shows location of mouse in window co-ordinates
  /*ImGui::Text("Raw mouse: %.1f/%d, %.1f/%d\n", lastMouseX, width, lastMouseY, height);
  ImGui::Text("Clip mouse: %.3f [-1,1], %.3f [-1,1]\n",
              lastMouseX/width*2.0f-1.0f, 1.0f - lastMouseY/height*2.0f);
  */
  //Mouse Ray
  /*ImGui::Text("Mouse in world space: %.3f, %.3f, %.3f'\n", scene->lastMouseLocation[0],
              scene->lastMouseLocation[1], scene->lastMouseLocation[2]);
  ImGui::Text("Mouse direction: %.3f, %.3f, %.3f'\n", scene->lastMouseDirection[0],
              scene->lastMouseDirection[1], scene->lastMouseDirection[2]);
  */
  // Projection matrix
  //displayImguiMatrix("Projection Matrix:", scene->camera.projection);
  
  // View matrix
  //displayImguiMatrix("View Matrix:", scene->camera.view);
  
  // Model matrix
  //displayImguiMatrix("Model Matrix:", scene->model);
  
  // Invert matrix
  //displayImguiMatrix("Invert Matrix:", scene->invert);
}


// =======================================================================================
