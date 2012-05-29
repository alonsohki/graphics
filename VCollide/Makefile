############################################################################
#
# Top Level Makefile for V-Collide.
#
#
############################################################################

#Choose the C++ compiler you want
CC = g++

# If you want the demos to be compiled with a graphical display of 
# the simulation, then uncomment the following lines and provide
# the correct locations of the libraries and include files.

#GRAPHICS_CFLAGS   = -DOGL_GRAPHICS
#GRAPHICS_INCLUDES = -I/afs/unc/project/walk/include
#GRAPHICS_LDFLAGS  = -L/afs/unc/project/walk/lib/`hw_os`
#GRAPHICS_LIBS     = -lglut -lGLU -lGL -lX11 -lXmu


all : 
	cd RAPID; \
	$(MAKE) CC="$(CC)"
	cd src; \
	$(MAKE) CC="$(CC)" install

clean : 
	cd RAPID; \
	$(MAKE) clean
	cd src; \
	$(MAKE) clean
	cd lib; \
	rm -f libVCollide.a

distclean: clean
