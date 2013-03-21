#
# $Id: Makefile,v 1.2 2007/11/14 20:32:38 thomasw Exp $
#
# General build makefile, only for examples.

APPDIR ?= /usr/bin
INCLUDE ?= $(shell pwd)/../include
HEADER = $(INCLUDE)/route56.h

CC ?=      /usr/bin/gcc
MKDIR ?=   /bin/mkdir
CP ?=      /bin/cp
RM ?=      /bin/rm
CHMOD ?=   /bin/chmod


## ============================================================================
MODEL = Configuration/Test Utilities
OBJS = r56util.o r56test.o
APPS = r56util r56test

CFLAGS = -Wall -O3 -I$(INCLUDE)


## ============================================================================
all: $(APPS)
	@echo " *$(MODEL) are prepared"
	@echo

$(HEADER):
	@echo
	@echo "  ERROR!  Can't find include file!"
	@echo
	@echo "       I suggest you obtain another copy of this distribution."
	@echo
	@exit 1

$(OBJS): %.o: %.c $(HEADER)
	@echo "   Compiling $(MODEL) source: $*.o"
	@$(CC) $(CFLAGS) -c $< -o $@

$(APPS): %: %.o
	@echo "   Linking $^"
	@$(CC) $(CFLAGS) $^ -o $@

objs: $(OBJS)
	@echo " *$(MODEL) objects prepared"
	@echo

install: $(APPS)
	@echo "   Installing ${MODEL} to ${APPDIR}"
	@for n in $(APPS); do $(CP) -r $$n ${APPDIR}; done
	@for n in $(APPS); do $(CHMOD) 755 ${APPDIR}/$$n; done
	@echo " *${MODEL} installed"
	@echo

uninstall:
	@echo "   Uninstalling ${MODEL} from ${APPDIR}"
	@for n in $(APPS); do $(RM) -f ${APPDIR}/$$n; done
	@echo " *${MODEL} uninstalled"
	@echo 

clean: 
	@echo "   Removing ${MODEL} build files"
	@$(RM) -rf $(OBJS) $(APPS)
	@echo " *${MODEL} build files removed"
	@echo
