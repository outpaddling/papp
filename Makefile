############################################################################
#
#              Another Programmer's Editor Makefile Template
#
# This is a template Makefile for a simple C or C++ program.
# It is meant to serve as a starting point for creating a portable
# Makefile, suitable for use under ports systems like *BSD ports,
# MacPorts, Gentoo Portage, etc.  It contains examples of most common
# Makefile components you will need.  
#
# For most small projects, you should be able to create a usable
# Makefile from this template by assigning the appropriate values
# to variables like BIN1, LIB1, etc., and removing everything you
# don't need.
#
# For more complex projects, start with the advanced Makefile.
#
# Variables that are conditionally assigned (with ?=) can be overridden
# on the command line as follows:
#
#       make VAR=value
#
# They can also inheret values from parent Makefiles (as in *BSD ports).
# This allows different systems to use the Makefile without modifications.
# For example, MacPorts installs to /opt/local instead of /usr/local,
# and hence might use the following:
# 
#       make PREFIX=/opt/local
#
# Lastly, they can be overridden by the environment.  I like to add -Wall
# to my CFLAGS for development, without having to put it in the Makefile,
# since it's a gcc specific flag.  Hence, I have the following in my
# .cshrc on gcc-based systems (BSD, Linux, Mac OS X):
# 
#       setenv CFLAGS "-O -Wall -pipe"
#
# Different systems may also use different compilers and keep libraries in
# different locations:
#
#       make CC=gcc CFLAGS=-O2 LFLAGS1="-L/usr/X11R6 -lX11"
#
# Variables can also be appended in the Makefile (with +=), so that
# flags specified on the command line can be combined with those in
# the Makefile.
#
# Author: J. Bacon
############################################################################

############################################################################
# Executable

BIN     = papp
MAN     = papp.1

############################################################################
# List object files that comprise BIN.

OBJS    = papp.o source_file.o macro.o mactable.o statement.o error.o

############################################################################
# Compile, link, and install options

# Install in /usr/local, unless defined by the parent Makefile, the
# environment, or a command line option such as PREFIX=/opt/local.
PREFIX      ?= /usr/local
MANPREFIX   ?= ${PREFIX}
DATADIR     ?= ${PREFIX}/share/papp

# Where to find local libraries and headers.  For MacPorts, override
# with "make LOCALBASE=/opt/local"
LOCALBASE   ?= ${PREFIX}

############################################################################
# Build flags
# Override with "make CC=gcc", "make CC=icc", etc.
# Do not add non-portable options (such as -Wall) using +=

# Portable defaults.  Can be overridden by mk.conf or command line.
CXX         ?= g++
CXXFLAGS    ?= -Wall -g
CXXFLAGS    += -DPREFIX="\"${PREFIX}\""
CXXFLAGS    += ${INCLUDES}

LD          = ${CXX}

INCLUDES    += -I${LOCALBASE}/include
LFLAGS      += -L${LOCALBASE}/lib

############################################################################
# Assume first command in PATH.  Override with full pathnames if necessary.
# E.g. "make INSTALL=/usr/local/bin/ginstall"
# Do not place flags here (e.g. RM = rm -f).  Just provide the command
# and let each usage dictate the flags.

INSTALL ?= install
LN      ?= ln
RM      ?= rm
AR      ?= ar
MKDIR   ?= mkdir
CP      ?= /bin/cp

############################################################################
# Standard targets required by ports

all:    ${BIN}

# Link rules
${BIN}: ${OBJS}
	${LD} -o ${BIN} ${OBJS} ${LFLAGS1}

############################################################################
# Include dependencies generated by "make depend", if they exist.
# These rules explicitly list dependencies for each object file.
# See "depend" target below.  If Makefile.depend does not exist, use
# generic source compile rules.  These have some limitations, so you
# may prefer to create explicit rules for each target file.  This can
# be done automatically using "cpp -M" or "cpp -MM".  Run "man cpp"
# for more information, or see the "depend" target below.

# Rules generated by "make depend"
include Makefile.depend

############################################################################
# Self-generate dependencies the old-fashioned way

depend:
	rm -f Makefile.depend
	for file in *.cc; do \
	    g++ ${INCLUDES} -MM $${file} >> Makefile.depend; \
	    printf "\t\$${CXX} -c \$${CXXFLAGS} $${file}\n" >> Makefile.depend; \
	    echo "" >> Makefile.depend; \
	done

############################################################################
# Remove generated files (objs and nroff output from man pages)

clean:
	rm -f ${OBJS} ${BIN} *.nr

# Keep backup files during normal clean, but provide an option to remove them
realclean: clean
	rm -f .*.bak *.bak *.BAK *.gmon core *.core


############################################################################
# Install all target files (binaries, libraries, docs, etc.)

install: all
	${MKDIR} -p ${PREFIX}/bin ${PREFIX}/man/man1 ${DATADIR}/include
	${INSTALL} -s -m 0555 ${BIN} ${PREFIX}/bin
	${INSTALL} -m 0444 ${MAN} ${MANPREFIX}/man/man1
	${CP} -R Include/* ${DATADIR}/include

############################################################################
# Remove all installed files

uninstall:
	${RM} ${PREFIX}/bin/${BIN}
	${RM} ${MANPREFIX}/man/man1/${MAN}
	${RM} ${DATADIR}/include

test:
	${BIN} < main.tal
