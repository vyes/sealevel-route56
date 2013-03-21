#
# $Id: Makefile,v 1.2 2007/11/14 20:32:38 thomasw Exp $
#
# General examples build makefile.


INCLUDE ?= $(shell pwd)/../include
## ============================================================================
SUBDIRS = sample-async sample-hdlc sample-raw


## ============================================================================
all: subdirs

subdirs:
	@echo " --Building example applications--"
	@echo
	@for n in $(SUBDIRS); do $(MAKE) -s -C $$n all || exit 1; done
	@echo " --Done building example applications--"
	@echo

clean:
	@echo " --Removing example build code--"
	@echo
	@for n in $(SUBDIRS); do $(MAKE) -s -C $$n clean || exit 1; done
	@echo " --Example build code removed--"
	@echo

objs:
	@echo " --Building example objs--"
	@echo
	@for n in $(SUBDIRS); do $(MAKE) -s -C $$n objs || exit 1; done
	@echo " --Done building example objs--"
	@echo


install:
	@echo " --Installing example applications--"
	@echo
	@for n in $(SUBDIRS); do $(MAKE) -s -C $$n install || exit 1; done
	@echo " --Done installing example applications--"
	@echo

uninstall:
	@echo " --Uninstalling example applications--"
	@echo
	@for n in $(SUBDIRS); do $(MAKE) -s -C $$n uninstall || exit 1; done
	@echo " --Example applications removed--"
	@echo
