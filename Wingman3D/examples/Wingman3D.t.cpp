//========================================================================
// Wingman3D.t.cpp  : Example program for Logitech Wingman HID Joystick     
//
// Author           : Vilas Kumar Chitrakaran <cvilas@ces.clemson.edu>     
// Date             : March 2, 2004
// Compiler         : GNU GCC 2.95.3qnx-nto
// Operating System : QNX Momentics 6.2.1                                
//========================================================================

#include "Wingman3D.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
 Wingman3D joystick(0);

 if( !joystick.isStatusOk() ) {
  fprintf(stderr, "%s\n", joystick.getStatusMessage());
  return EXIT_FAILURE;
 }
 sleep(1);
 joystick.printDeviceInfo(3);

 while(1)
 {
  fprintf(stdout, "\nx: %03d y: %03d z: %03d slide: %03d hat: %03d buttons: ", 
   joystick.getX(), joystick.getY(), joystick.getTwist(),
   joystick.getSliderValue(), joystick.getHatSwitchStatus());
 
  for(int i = 1; i < joystick.getNumButtons(); i++)
   if( joystick.isButtonPressed(i) )
    fprintf(stdout, "%01d ", i);

  if( !joystick.isStatusOk() )
   return -1;
  
 }

 return EXIT_SUCCESS;
}