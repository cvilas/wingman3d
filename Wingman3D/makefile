#========================================================================
# Logitech Wingman Extreme 3D Digital HID Joystick Interface    
# ----------------------------------------------------------------------  
# File: makefile
#========================================================================  


#Name of the package
PKG = Wingman3D

# ----- Directories -----

INSTALLDIR = /usr/local
INSTALLHEADER= $(INSTALLDIR)/include/
INSTALLLIB= $(INSTALLDIR)/lib/
INSTALLBIN =
INSTALLSRC = 

# ----- Doxygen documentation parameters -----
DOCNAME = Logitach Wingman Extreme 3D Digital Joystick Driver
DOCSOURCE = *.hpp
DOCTARGET = 

# Libraries, headers, and binaries that will be installed.
LIBS = lib$(PKG).so lib$(PKG).a
HDRS = Wingman3D.hpp 
SRC = *.cpp

# ---- compiler options ----
CC = g++
LD = g++
CFLAGS = -W -Wall -fexceptions -fno-builtin -O2 -fpic -D_REENTRANT
LDFLAGS =
INCLUDEHEADERS =
INCLUDELIBS =
OBJ = Wingman3D.o

TARGET = $(LIBS)
INCLUDEHEADERS =
INCLUDELIBS =
CLEAN = rm -rf *.o lib* *.dat docs/


# ========== Targets ==========
targets: $(TARGET)
all: $(TARGET)

lib$(PKG).a: $(OBJ)
	ar crv $@ $(OBJ)
	ranlib $@

lib$(PKG).so: $(OBJ)
	$(LD) -shared -o $@ $(OBJ)
	
# ----- obj -----
.cpp.o:
	$(CC) $(CFLAGS) -c $< $(INCLUDEHEADERS)


# ---- make rules ----
clean:
	@echo
	@echo ----- Package $(PKG), Cleaning -----
	@echo
	$(CLEAN)
	if (test -d examples) ; then (cd examples; make clean);fi

install:
	@echo
	@echo ----- Package $(PKG), Installing to $(INSTALLDIR) -----
	@echo
	if !(test -d $(INSTALLLIB)); then (mkdir -p $(INSTALLLIB)); fi
	for i in ${LIBS}; do (cp $$i $(INSTALLLIB)); done
	if !(test -d $(INSTALLHEADER)); then (mkdir -p $(INSTALLHEADER)); fi
	for i in ${HDRS}; do (cp $$i $(INSTALLHEADER)); done

