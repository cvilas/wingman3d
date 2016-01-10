//======================================================================== 
// Project: Logitech Wingman Joystick Client 
// ---------------------------------------------------------------------- 
// Package: WingmanClient
// Authors: Vilas Kumar Chitrakaran
// Start Date: Thu Mar 04 11:01:10 EST 2004
// ----------------------------------------------------------------------  
// File: WingmanClient.t.cpp
// Example program for the class WingmanClient.
//========================================================================  
 

#include "WingmanClient.hpp"
#include <stdio.h>
#include <stdlib.h>

 
//======================================================================== 
// main 
// -----------------------------------------------------------------------
// Note that WingmanServer must be running for this program to work. The
// server can be started as follows:
//
// 1. First ensure that the USB manager (devu-uhci or devu-ohci) and manager 
// for HID devices (io-hid) are running. Here are
// the commands (you may put these in /etc/rc.d/rc.local)
//
// /sbin/devu-uhci & (or /sbin/devu-ohci &)
// /sbin/io-hid &
// mount -Tio-hid devh-usb.so &
// 
// 2. Now start a server named "usbjs" as follows :
// WingmanServer -n qrts/usbjs &
//
// After starting the server, activate any of the degrees of freedom of
// the joystick once so that data structures are initialized to proper
// values.
//========================================================================  
 
int main()
{
 WingmanClient *joystick;
 
 joystick = new WingmanClient(0, "qrts/usbjs");
 if( !joystick->isStatusOk() ) {
  fprintf(stderr, "%s\n", joystick->getStatusMessage());
  return -1;
 }

 while(1)
 {
  fprintf(stdout, "\nx: %03d y: %03d z: %03d slide: %03d hat: %03d buttons: ", 
   joystick->getX(), joystick->getY(), joystick->getTwist(),
   joystick->getSliderValue(), joystick->getHatSwitchStatus());
 
  for(int i = 1; i < joystick->getNumButtons(); i++)
   if( joystick->isButtonPressed(i) )
    fprintf(stdout, "%01d ", i);

  if( !joystick->isStatusOk() )
   return -1;
  
 }
 
 delete(joystick);
 
 return 0;
}

