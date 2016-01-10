SUBDIRS = ../Wingman3D ../WingmanServer ../WingmanClient
PKG=Wingman

normal:
	for i in ${SUBDIRS}; do ( cd $$i; make release=$(release) shared=$(shared)) ; done

all:
	for i in ${SUBDIRS}; do ( cd $$i; make release=$(release) shared=$(shared) all ) ; done

install:
	for i in ${SUBDIRS}; do ( cd $$i; make release=$(release) shared=$(shared) install ) ; done

installheaders:
	for i in ${SUBDIRS}; do ( cd $$i; make release=$(release) shared=$(shared) installheaders ) ; done

uninstall:
	for i in ${SUBDIRS}; do ( cd $$i; make release=$(release) shared=$(shared) uninstall ) ; done

fresh:
	make clean
	for i in ${SUBDIRS}; do ( cd $$i; make release=$(release) shared=$(shared) install ) ; done

installproject:
	for i in ${SUBDIRS}; do ( cd $$i; make release=$(release) shared=$(shared) installproject ) ; done

installprojectheaders:
	for i in ${SUBDIRS}; do ( cd $$i; make release=$(release) shared=$(shared) installprojectheaders ) ; done

installprojectlibs:
	for i in ${SUBDIRS}; do ( cd $$i; cp lib* /rp/lib ) ; done

installsource:
	for i in ${SUBDIRS}; do ( cd $$i; make release=$(release) shared=$(shared) installsource ) ; done

clean:
	for i in ${SUBDIRS}; do ( cd $$i; make release=$(release) shared=$(shared) clean ) ; done
	rm -rf $(PKG).tar.gz $(PKG).tar $(PKG)
	
version:	clean
	rm -rf ../$(PKG).tar.gz
	(cd ..; tar -cvf $(PKG).tar $(PKG))
	gzip ../$(PKG).tar
	mv ../versions.$(PKG)/*.tar.gz ../versions.$(PKG).old
	mv ../$(PKG).tar.gz ../versions.$(PKG)/$(PKG)_`date -t`.tar.gz

distrib:
	make clean
	#make
	make release=release all
	sh createdistrib
