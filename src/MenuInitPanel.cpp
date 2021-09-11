// Copyright Staniford Systems.  All Rights Reserved.  September 2021 -
// This class is a special case of MenuPanel used only during startup when we need to 
// interact with the user about something (often something that has gone wrong).  It 
// is only called from Window3D::initPanel, which has it's own special rendering loop 
// because this is for use before the main rendering loop has started.

#include "MenuInitPanel.h"


// =======================================================================================
/// @brief Constructor

MenuInitPanel::MenuInitPanel(void)
{
}


// =======================================================================================
/// @brief Destructor

MenuInitPanel::~MenuInitPanel(void)
{
}


// =======================================================================================
