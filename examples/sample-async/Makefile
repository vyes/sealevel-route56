#
# $Id: Makefile,v 1.2 2007/11/14 20:32:38 thomasw Exp $
#
# General build makefile, only for examples.

INCLUDE ?= $(shell pwd)/../../include
HEADER = $(INCLUDE)/route56.h

CC ?=      /usr/bin/gcc
RM ?=      /bin/rm


## ============================================================================
MODEL = Async Communications Example
OBJS = send-async.o receive-async.o
APPS = send-async receive-async

CFLAGS = -g -Wall -I$(INCLUDE)


## ============================================================================
all: $(APPS)
	@echo "   *$(MODEL) is prepared"
	@echo

$(HEADER):
	@echo
	@echo "  ERROR!  Can't find include file!"
	@echo
	@echo "       I suggest you obtain another copy of this distribution."
	@echo
	@exit 1

$(OBJS): %.o: %.c $(HEADER)
	@echo "     Compiling $(MODEL) source: $*.o"
	@$(CC) $(CFLAGS) -c $< -o $@

$(APPS): %: %.o
	@echo "     Linking $^"
	@$(CC) $(CFLAGS) $^ -o $@

objs: $(OBJS)
	@echo "   *$(MODEL) objects prepared"
	@echo

install: $(APPS)
	@echo "   *${MODEL} is NOT installed, but is built"
	@echo

uninstall:
	@echo "   *${MODEL} was NOT installed"
	@echo 

clean: 
	@echo "     Removing ${MODEL} build files"
	@$(RM) -rf $(OBJS) $(APPS) data
	@echo "   *${MODEL} build files removed"
	@echo
