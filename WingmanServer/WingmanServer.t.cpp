//======================================================================== 
// Project: Wingman Joystick Server   
// ---------------------------------------------------------------------- 
// Package: WingmanServer
// Authors: Vilas Kumar Chitrakaran
// Start Date: Thu Mar 04 11:01:36 EST 2004
// ----------------------------------------------------------------------  
// File: WingmanServer.t.cpp
// Example program for the class WingmanServer.
//========================================================================  
 
#include <iostream.h>
#include <unistd.h>
#include "CmdLineArgs.hpp"
#include "WingmanServer.hpp"

//======================================================================== 
// main
//========================================================================  
 
int main(int argc, char *argv[])
{
	CmdLineArgs args(argc, argv);
	
	const char *serverName;
	
	serverName = args.getStringOption("n", "qrts/usbjs");
	WingmanServer wingmanServer(serverName);

	if (wingmanServer.isStatusError())
	{
		cerr << endl;
		cerr << wingmanServer.d_status.getMessageText();
		return -1;
	}
	
	delay(10);
	wingmanServer.printDeviceInfo();
	
	wingmanServer.doMessageLoop ();

	if (wingmanServer.isStatusError())
	{
		cerr << endl;
		cerr << wingmanServer.d_status.getMessageText();
		return -1;
	}
	
	return 0;
}
