#
# $Id: Makefile,v 1.25 2010/08/18 21:47:29 thomasw Exp $
#
# General build makefile.  (Main)

## These variable MUST be accurate for the installation to succeed.
export KDIR = /lib/modules/$(shell uname -r)/build
export MODDIR = /lib/modules/$(shell uname -r)/kernel/drivers/serial

## Change these if you wish to change default installation directories.
export INCLUDEDIR = /usr/local/include
export SUAPPDIR = /sbin
export APPDIR = /usr/bin

## Change these if your system is configured differently.
export CP 	= /bin/cp
export RM 	= /bin/rm
export MKDIR 	= /bin/mkdir
export CHMOD    = /bin/chmod
export CC 	= /usr/bin/gcc

export INCLUDE = $(shell pwd)/include
## ============================================================================
## The following values are for internal package managment and should only
## be changed if you know what you are doing.
PRODUCT = Route56
VERSION = 2.0.10

#CVS project name, used for install scripts
PROJECT = route56
SUBDIRS = include driver scripts utilities examples
CVS = `find -iname \CVS`

TAR 	= /bin/tar
GZIP 	= /bin/gzip

## ============================================================================
all: subdirs

objs:
	@echo "----------------------------------------------------------------"
	@echo "Building all objects."
	@echo "----------------------------------------------------------------"
	@echo
	@for n in $(SUBDIRS); do $(MAKE) -s -C $$n objs || exit 1; done
	@echo "----------------------------------------------------------------"
	@echo "Done building objects."
	@echo "----------------------------------------------------------------"

subdirs:
	@echo "----------------------------------------------------------------"
	@echo "Building all."
	@echo "----------------------------------------------------------------"
	@echo
	@for n in $(SUBDIRS); do $(MAKE) -s -C $$n all || exit 1; done
	@echo "----------------------------------------------------------------"
	@echo "Done building all sources."
	@echo "----------------------------------------------------------------"

clean:
	@echo "----------------------------------------------------------------"
	@echo "Removing all build files."
	@echo "----------------------------------------------------------------"
	@echo
	@for n in $(SUBDIRS); do $(MAKE) -s -C $$n clean || exit 1; done
	@echo "----------------------------------------------------------------"
	@echo "Done removing all build files."
	@echo "----------------------------------------------------------------"

install:
	@echo "----------------------------------------------------------------"
	@echo "Installing $(PRODUCT) suite."
	@echo "----------------------------------------------------------------"
	@echo
	@for n in $(SUBDIRS); do $(MAKE) -s -C $$n install || exit 1; done
	@echo "----------------------------------------------------------------"
	@echo "Done installing $(PRODUCT) suite."
	@echo "----------------------------------------------------------------"

uninstall:
	@echo "----------------------------------------------------------------"
	@echo "Uninstalling $(PRODUCT) suite."
	@echo "----------------------------------------------------------------"
	@echo
	@for n in $(SUBDIRS); do $(MAKE) -s -C $$n uninstall || exit 1; done
	@echo "----------------------------------------------------------------"
	@echo "Done uninstalling $(PRODUCT) suite."
	@echo "----------------------------------------------------------------"

cleaninstall: clean install

purge: uninstall clean

distro: clean
	@echo "----------------------------------------------------------------"
	@echo "Building $(PRODUCT) distribution package!"
	@echo "----------------------------------------------------------------"
	@$(RM) -rf $(PRODUCT)_$(VERSION).tar.gz
	@$(RM) -rf $(CVS)
	@$(RM) -rf ./TESTING
	@$(TAR) -cf $(PRODUCT)_$(VERSION).tar ../${PROJECT}
	@$(GZIP) $(PRODUCT)_$(VERSION).tar
	@echo "----------------------------------------------------------------"
	@echo "Done!"
	@echo "----------------------------------------------------------------"
