#========================================================================
# Logitech Wingman Extreme 3D Digital HID Joystick Interface    
# ----------------------------------------------------------------------  
# File: makefile
#========================================================================  

LDFLAGS = g++ -Wall -O3 -o
INCLUDEHEADERS = -I ../ -I /usr/qrts/include
INCLUDELIB = -L ../ -lWingman3D -lhiddi
TARGETS = Wingman3D.t

targets:	$(TARGETS)
CLEAN = rm -rf $(TARGETS) *.dat *.o


# ----- Wingman3D -----
Wingman3D.t : Wingman3D.t.cpp
	$(LDFLAGS) $@ $? $(INCLUDEHEADERS) $(INCLUDELIB)

clean:
	$(CLEAN)

