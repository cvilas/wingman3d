//======================================================================== 
// Project: Logitech Wingman Joystick Client   
// ---------------------------------------------------------------------- 
// Package          : WingmanClient
// Author           : Vilas Kumar Chitrakaran <cvilas@ces.clemson.edu>     
// Date             : July 2004
// Compiler         : GNU GCC 2.95.3qnx-nto
// Operating System : QNX Momentics 6.2.1                                
// ----------------------------------------------------------------------  
// File: WingmanClient.cpp
// Implementation of the class WingmanClient.
// See WingmanClient.hpp for more details.
//========================================================================  
 

// ----- Standard Libaries -----
#include <iostream.h>
#include <stdio.h>

// ----- QRTS Libraries -----

// ----- Project Includes -----
#include "WingmanClient.hpp"

 
//========================================================================
// WingmanClient::WingmanClient()
//========================================================================
WingmanClient::WingmanClient (int devNum, char *serverName) 
 : Client(serverName)
{
 d_jsData = NULL;
 
 d_deviceNumber = devNum;
 
 if(d_status.isStatusError())
  return;	
  
 if(attachSharedMemory() != 0)
  d_status.setStatusError();

 d_status.setStatusOk();
}


//========================================================================
// WingmanClient::~WingmanClient()
//========================================================================
WingmanClient::~WingmanClient ()
{
 d_shm.close();
}


//========================================================================
// WingmanClient::isJoystickPresent()
//========================================================================
bool WingmanClient::isJoystickPresent()
{
 d_jsData[d_deviceNumber].jstk_is_present;
}


//========================================================================
// WingmanClient::getX()
//========================================================================
int WingmanClient::getX() const
{
 if(d_jsData)
  return d_jsData[d_deviceNumber].x;
}


//========================================================================
// WingmanClient::getMaxX()
//========================================================================
int WingmanClient::getMaxX() const
{
 if(d_jsData)
  return d_jsData[d_deviceNumber].max_x;
 return 0;
}


//========================================================================
// WingmanClient::getY()
//========================================================================
int WingmanClient::getY() const
{
 if(d_jsData)
  return d_jsData[d_deviceNumber].y;
 return 0;
}


//========================================================================
// WingmanClient::getMaxY()
//========================================================================
int WingmanClient::getMaxY() const
{
 if(d_jsData)
  return d_jsData[d_deviceNumber].max_y;
 return 0;
}


//========================================================================
// WingmanClient::getTwist()
//========================================================================
int WingmanClient::getTwist() const
{
 if(d_jsData)
  return d_jsData[d_deviceNumber].twist;
 return 0;
}


//========================================================================
// WingmanClient::getMaxTwist()
//========================================================================
int WingmanClient::getMaxTwist() const
{
 if(d_jsData)
  return d_jsData[d_deviceNumber].max_twist;
 return 0;
}


//========================================================================
// WingmanClient::isButtonPressed
//========================================================================
bool WingmanClient::isButtonPressed(int button) const
{
 if(!d_jsData)
  return false;
  
 if( button < 1 || button > d_jsData[d_deviceNumber].num_buttons)
  return false;
  
 if( (1 << (button-1)) & d_jsData[d_deviceNumber].buttons)
  return true;
 
 return false;
}


//========================================================================
// WingmanClient::getNumButtons()
//========================================================================
int WingmanClient::getNumButtons() const
{
 if(d_jsData)
  return d_jsData[d_deviceNumber].num_buttons;
 return 0;
}


//========================================================================
// WingmanClient::getSliderValue()
//========================================================================
int WingmanClient::getSliderValue() const
{
 if(d_jsData)
  return d_jsData[d_deviceNumber].slider;
 return 0;
}


//========================================================================
// WingmanClient::getMaxSliderValue()
//========================================================================
int WingmanClient::getMaxSliderValue() const
{
 if(d_jsData)
  return d_jsData[d_deviceNumber].max_slider;
 return 0;
}


//========================================================================
// WingmanClient::getHatSwitchStatus
//========================================================================
int WingmanClient::getHatSwitchStatus() const
{
 if(!d_jsData)
  return 0;
  
 if(d_jsData[d_deviceNumber].hat_switch == 0x8)
  return 0;
 return (d_jsData[d_deviceNumber].hat_switch+1);
}


//========================================================================
// WingmanClient::isStatusOk()
//========================================================================
bool WingmanClient::isStatusOk()
{
 if(d_jsData)
  if(d_jsData[d_deviceNumber].status_ok && d_status.isStatusOk() )
   return true;
 return false;
}


//========================================================================
// WingmanClient::getStatusMessage()
//========================================================================
const char * WingmanClient::getStatusMessage()
{
 if(d_jsData)
  return d_jsData[d_deviceNumber].status_message;
 return d_status.getMessageText();
}



//========================================================================
// WingmanClient::attachSharedMemory()
//========================================================================
int WingmanClient::attachSharedMemory()
{
 char shmName[256];

 sprintf(shmName, "QRTS_WingmanServer_%s", getServerName());

	cout << shmName << endl;
 d_jsData = (WingmanData_t *)d_shm.attach(shmName, 2 * sizeof(WingmanData_t));
 if (!d_shm.d_status.isStatusOk())
 {
  d_status.setStatusError()
    << "[WingmanClient::attachSharedMemory] " 
    << d_shm.d_status.getMessageText() << endl;
  return -1;
 }
 return 0;
}




