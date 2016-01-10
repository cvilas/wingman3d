#======================================================================== 
# Project: Logitech Wingman USB HID Joystick Client
# ---------------------------------------------------------------------- 
# Package: WingmanClient 
# Authors: Vilas Kumar Chitrakaran  
# Start Date: Thu Mar 04 11:01:10 EST 2004
# ----------------------------------------------------------------------  
# File: makefile (makefile of examples)
#========================================================================  

include $(QRTS)/makefiles/endusertop

TESTS = WingmanClient.t 

tests:	$(TESTS)

LOCALCLEAN = rm -rf $(TESTS)


# ----- WingmanClient -----

WingmanClient.t:	WingmanClient.t.o
	$(LD) -o WingmanClient.t WingmanClient.t.o -lWingmanClient \
	      -lSharedMemory -lClientServer -lStatus


include $(QRTS)/makefiles/enduserbottom
