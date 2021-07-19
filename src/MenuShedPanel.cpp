// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class handles the UI panel for inserting a shed.

#include "MenuShedPanel.h"
#include "Global.h"
#include "PmodDesign.h"
#include "MenuInterface.h"
#include "MenuErrorPanel.h"


// =======================================================================================
// Constants, static variable initializations, etc

bool  MenuShedPanel::staticsInitialized = false; 
char  MenuShedPanel::heightLabel[16];
char  MenuShedPanel::lengthLabel[16];
char  MenuShedPanel::widthLabel[16];
char  MenuShedPanel::rOverhangLabel[16];
char  MenuShedPanel::lOverhangLabel[16];
char  MenuShedPanel::fOverhangLabel[16];
char  MenuShedPanel::bOverhangLabel[16];


// =======================================================================================
/// @brief Constructor
///
/// The constructor is where a variety of default values for the inserted shed are set. 
/// It also initializes a number of static variables if they weren't done before.
///
/// @param menu Pointer to the overall MenuInterface so we can call back into it. 
/// @param S A pointer to the scene.


MenuShedPanel::MenuShedPanel(MenuInterface* menu, Scene* S):
                              MenuPanel(menu, S),
                              height(8.0f),
                              length(12.0f),
                              width(6.0f),
                              roofAngle(15.0f),
                              rightOverhang(1.0f),
                              leftOverhang(1.0f),
                              frontOverhang(1.0f),
                              backOverhang(1.0f)
{
  unless(staticsInitialized)
   {
    staticsInitialized = true;
    sprintf(heightLabel, "(%s)##h", spaceUnitName);
    sprintf(lengthLabel, "(%s)##l", spaceUnitName);
    sprintf(widthLabel, "(%s)##w", spaceUnitName);
    sprintf(rOverhangLabel, "(%s)##r", spaceUnitName);
    sprintf(lOverhangLabel, "(%s)##L", spaceUnitName); // capitalized to avoid conflict with length
    sprintf(fOverhangLabel, "(%s)##f", spaceUnitName);
    sprintf(bOverhangLabel, "(%s)##b", spaceUnitName);
   }
}


// =======================================================================================
/// @brief Destructor
///
/// Presently empty/default.

MenuShedPanel::~MenuShedPanel(void)
{
}


// =======================================================================================
/// @brief Verifies the validity of the panel state.
/// 
/// This function verifies that all state variables of the MenuShedPanel are consistent
/// and complete.  If not, it provides a diagnostic error string.  Note in some cases it
/// relies on a static buffer and thus is not thread-safe.
///
/// @return A char* pointer to a diagnostic error string.


char* MenuShedPanel::errorInFields(void)
{
  static char errBuf[48];
  char* errString;
  
  if((errString = errorInFloat(height)))
   {
    sprintf(errBuf, "Bad height value: %s", errString);
    return errBuf;
   }

  if((errString = errorInFloat(length)))
   {
    sprintf(errBuf, "Bad length value: %s", errString);
    return errBuf;
   }

  if((errString = errorInFloat(width)))
   {
    sprintf(errBuf, "Bad width value: %s", errString);
    return errBuf;
   }
            
  return NULL;
}


// =======================================================================================
/// @brief Display the panel.
///
/// Calls all the various ImGui calls to actually put the panel together.

void MenuShedPanel::imGuiDisplay(void)
{
  if(!displayVisible)
    return;
  ImGui::Begin("Insert a Shed", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);

  // Text fields for data entry
  ImGui::Text("Length:");
  ImGui::SameLine();
  ImGui::InputFloat(lengthLabel, &length, 1.0f, 5.0f, "%.2f");

  ImGui::Text("Width:");
  ImGui::SameLine();
  ImGui::InputFloat(widthLabel, &width, 1.0f, 5.0f, "%.2f");

  ImGui::Text("Height");
  ImGui::SameLine();
  ImGui::InputFloat(heightLabel, &height, 1.0f, 5.0f, "%.2f");

  float maxAngle = atanf(height/width)*180.0f/M_PI;
  ImGui::Text("Roof Angle:");
  ImGui::SameLine();
  ImGui::SliderFloat("degrees", &roofAngle, 0.0f, maxAngle);

  ImGui::Text("R. Overhang");
  ImGui::SameLine();
  ImGui::InputFloat(rOverhangLabel, &rightOverhang, 0.1f, 1.0f, "%.2f");

  ImGui::Text("L. Overhang");
  ImGui::SameLine();
  ImGui::InputFloat(lOverhangLabel, &leftOverhang, 0.1f, 1.0f, "%.2f");

  ImGui::Text("F. Overhang");
  ImGui::SameLine();
  ImGui::InputFloat(fOverhangLabel, &frontOverhang, 0.1f, 1.0f, "%.2f");

  ImGui::Text("B Overhang");
  ImGui::SameLine();
  ImGui::InputFloat(bOverhangLabel, &backOverhang, 0.1f, 1.0f, "%.2f");

  // Bottom row of buttons to cancel/enter.
  if(ImGui::Button("Cancel"))
   {

   }
  ImGui::SameLine();
  if(ImGui::Button("Create Shed"))
   {
    char* errString;
    if((errString = errorInFields()))
     {
      errPanel = new MenuErrorPanel(errString);
     }
    else
     {
      ImGui::End();
      return;
     }
   }

  if(errPanel)
    if(errPanel->displayDone())
      errPanel = NULL;
  
  ImGui::End();
}


// =======================================================================================

void MenuShedPanel::shedEntered(void)
{
  
}


// =======================================================================================
/// @brief Interface for the method to be called in the main thread to process an action
/// for shed creation coming from the HTTP interface.
/// 
/// Note the action will have originally arisen in the HTTP server and have been 
/// processed by handleHTTPRequest.  
/// @returns The ActionType of the action we processed.
/// @param action The InterfaceAction that needs to be handled.
/// @todo presently defaults to Strawbale all the time

ActionType MenuShedPanel::processAction(InterfaceAction* action)
{
  switch(action->actionType)
   {
    case ShedEntered:
      LogPseudoActions("Shed entered.\n");
      shedEntered();
      return ShedEntered;
   
    default:
      return NoAction;
   }
}


// =======================================================================================
/// @brief Function to handle block entry requests from the HTTP diagnostic server.  
/// 
/// Note that this will *not* be called on the main thread, but rather one
/// of the HTTP server threads.  
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuShedPanel::handleHTTPRequest(HttpDebug* serv, char* path)
{
  return true;
}


// =======================================================================================
