#========================================================================
# Logitech Wingman USB HID Joystick Server    
# ----------------------------------------------------------------------  
# File: makefile
#========================================================================  


include $(QRTS)/makefiles/generictop

# Name of the package (tar file will be $(PKG).tar)
PKG = Wingman Server

# Libraries, headers, and binaries that will be installed to the INSTALLLIB,
# INSTALLINCLUDE, and INSTALLBIN directories.
LIBS = $(PL)WingmanServer.$(EL) 
HDRS = WingmanServer.hpp 
BINS = WingmanServer

# Commands specific to this package that will be executed in addition to the
# default commands.
LOCALCLEAN = rm -rf $(LIBS) $(BINS) *.o
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
targets: $(BINS)

all: $(BINS)

# ----- WingmanServer -----
WingmanServer:	WingmanServer.o WingmanServer.t.o WingmanServer.use
	$(LD) -o WingmanServer WingmanServer.t.o WingmanServer.o \
	-Bstatic -lSharedMemory -lStatus -lClientServer -lCmdLineArgs -lhiddi
	usemsg WingmanServer WingmanServer.use

WingmanServer.o: WingmanServer.cpp WingmanServer.hpp


install:
	cp $(HDRS) $(QRTS)/include
	chown 0 $(BINS)
	chmod a+s $(BINS)
	cp $(BINS) $(QRTS)/bin

clean:
	rm -rf $(BINS) *.o

