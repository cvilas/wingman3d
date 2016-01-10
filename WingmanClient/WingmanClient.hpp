//======================================================================== 
// Project: Logitech Wingman Joystick Client   
// ---------------------------------------------------------------------- 
// Package: WingmanClient
// Authors: Vilas Kumar Chitrakaran
// Start Date: Thu Mar 04 11:01:36 EST 2004
// Compiler: GNU G++
// Operating System: QNX Momentics 6.2.1
// ----------------------------------------------------------------------  
// File: WingmanClient.hpp
// Interface of the class WingmanClient.
//========================================================================  
 

#ifndef _WINGMANCLIENT_HPP_INCLUDED
#define _WINGMANCLIENT_HPP_INCLUDED

#include <sys/types.h>

#include "Client.hpp" 
#include "SharedMemory.hpp"

 
//======================================================================== 
// class WingmanClient
// ----------------------------------------------------------------------
// \brief
// Connects to Logitech Wingman USB HID joystick server.
//
// <b>Example Program:</b>
//
// \include WingmanClient.t.cpp
//========================================================================  
 
class WingmanClient : public Client
{
 public:
  WingmanClient(int devNum, char *serverName = "qrts/usbjs");
   // The constructor. Connects to joystick server
  
  ~WingmanClient();
   // The destructor disconnects from the joystick server
 
  bool isJoystickPresent();
   //  return  true if joystick is connected, else false.
   
  int getX() const;
   //  return  The X position
  
  int getMaxX() const;
   //  return  The maximum output from X axis.
   
  int getY() const;
   //  return  The Y position

  int getMaxY() const;
   //  return  The maximum output from Y axis.

  int getTwist() const;
   //  return  The twist handle position. 
  
  int getMaxTwist() const;
   //  return  The maximum output from twist axis.
  
  int getNumButtons() const;
   //  return  The number of programmable buttons available
    
  bool isButtonPressed(int button) const;
   //  return  true if button \a button is pressed, else false 

  int getSliderValue() const;
   //  return  The slider/throttle position. Range [0 - 255].
  
  int getMaxSliderValue() const;
   //  return  The maximum output from slider.

  int getHatSwitchStatus() const;
   //  return  A value indicating position of the hat switch. Range [1 - 8].
  
  bool isStatusOk();
   //  return  true if joystick/client status is ok, else false.
   
  const char *getStatusMessage();
   //  return  A string carrying current status of the device. This string 
   //          also carries error messages when isStatusOk() returns false.

	// ========== END OF INTERFACE ==========

  typedef struct WingmanData
  {
   bool status_ok;
   bool jstk_is_present;
   char status_message[80];
   int x;
   int max_x;
   int y;
   int max_y;
   int twist;
   int max_twist;
   int buttons;
   int num_buttons;
   int slider;
   int max_slider;
   int hat_switch;
  }WingmanData_t; 

 private:
  int d_deviceNumber;
   // The device we are interested in.
   
  int attachSharedMemory();
   // Attach to server's shared memory region
   //  return 0 on ok, -1 on error
   
  SharedMemory d_shm;
   // Shared memory. 
  
  WingmanData_t *d_jsData;
   // The joystick data pointer in shared memory.
};

#endif // _WINGMANCLIENT_HPP_INCLUDED

