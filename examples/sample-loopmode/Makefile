#
# Makefile for Route56 HDLC LoopMode Samples
#
# Written by Ernie Zapata ez@microgate.com
# for Sealevel Corporation
#
# Initial release ??/??/??
#
# Sealevel and Route56 are registered trademarks
# of Sealevel Corporation
#

# Uncomment this to add debugging information to programs
DEBUG=1

ifdef DEBUG
DEBUG_FLAGS=-g -ggdb  -O
else
DEBUG_FLAGS=-O2
endif

##################################################################
# define common build flags for all programs

CFLAGS = $(DEBUG_FLAGS) -Wall -Wstrict-prototypes 

all: primary secondary

# Primary station sample

primary : primary.o common.o profile.o 
	$(CC) $(CFLAGS) -o $@ $^


# Secondary station sample

secondary : secondary.o common.o profile.o
	$(CC) $(CFLAGS) -o $@ $^
	
clean:
	rm -f *.o *~ core primary secondary
