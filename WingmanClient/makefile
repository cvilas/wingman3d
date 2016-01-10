#======================================================================== 
# Logitech Wingman USB HID Joystick Client 
# ---------------------------------------------------------------------- 
# File: makefile
#========================================================================  

include $(QRTS)/makefiles/generictop

# Name of the package (tar file will be $(PKG).tar)
PKG = WingmanClient 

# Libraries, headers, and binaries that will be installed to the INSTALLLIB,
# INSTALLINCLUDE, and INSTALLBIN directories.
LIBS = $(PL)WingmanClient.$(EL) 
HDRS = WingmanClient.hpp 
BINS = 

# Commands specific to this package that will be executed in addition to the
# default commands.
LOCALCLEAN = 
LOCALINSTALL = 
LOCALUNINSTALL = 

# Files that are used to generate the documentation
DOCSOURCE = *.hpp
DOCTARGET = 
DOCNAME = 

# Path of project directory
PROJECT = 

# Only use the following line if you are creating a library
COMPILETYPE = -shared


# ========== Targets ==========

targets:	$(LIBS) $(BINS)

# ----- WingmanClient -----

WingmanClient.o:	WingmanClient.hpp WingmanClient.cpp

$(PL)WingmanClient.$(EL):	WingmanClient.o
	$(MAKELIB) $(PL)WingmanClient.$(EL) WingmanClient.o


include $(QRTS)/makefiles/genericbottom
