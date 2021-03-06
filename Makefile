#************************************************
#*                                              *
#*             (c) 2019 J. FABRIZIO             *
#*                                              *
#*                               LRDE EPITA     *
#*                                              *
#************************************************

CC = g++

CPP_FILES = src/matrix4.cpp
CPP_FILES += src/OBJloader.cpp
CPP_FILES += src/scene.cpp
CPP_FILES += src/display.cpp
CPP_FILES += src/renderer.cpp
CPP_FILES += src/shader.cpp
HXX_FILES = src/bunny.hh
HXX_FILES += src/matrix4.hh
HXX_FILES += src/OBJloader.hh
HXX_FILES += src/scene.hh
HXX_FILES += src/display.hh
HXX_FILES += src/renderer.hh
HXX_FILES += src/shader.hh
OBJ_FILES = $(CPP_FILES:.cpp=.o)

CXX_FLAGS += -Wall -Wextra -O3 -g -std=c++11
CXX_FLAGS +=
CXX_FLAGS += -m64 -march=native
CXX_FLAGS += -fopt-info-vec-optimized #-fopt-info-vec-missed -ftree-vectorize
LDXX_FLAGS = -lGL  -lGLEW -lglut -lGLU -lpthread

MAIN_FILE = src/main.cpp
DIST = main

SKEL_DIST_DIR = pogl_skel_tp
SKEL_FILES = $(CPP_FILES) $(HXX_FILES) $(MAIN_FILE) Makefile src/vertex.shd src/fragment.shd


#For gcc 4.9
#CXXFLAGS+=-fdiagnostics-color=auto
export GCC_COLORS=1

define color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then \
	    tput setaf $(1); \
        fi;\
    fi
endef

define default_color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then  tput sgr0 ; fi; \
    fi
endef


all: post-build

pre-build:
	@$(call color,4)
	@echo "******** Starting Compilation ************"
	@$(call default_color)

post-build:
	@make --no-print-directory main-build ; \
	sta=$$?;	  \
	$(call color,4); \
	echo "*********** End Compilation **************"; \
	$(call default_color); \
	exit $$sta;

main-build: pre-build build

build: $(OBJ_FILES)
	$(CC) $(MAIN_FILE) -o $(DIST) $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS)


%.o: %.cpp %.hh
	@$(call color,2)
	@echo "[$@] $(CXX_FLAGS)"
	@$(call default_color)
	@$(CC) -c -o $@ $< $(CXX_FLAGS) ; \
	sta=$$?;	  \
	if [ $$sta -eq 0 ]; then  \
	  $(call color,2) ; \
	  echo "[$@ succes]" ; \
	  $(call default_color) ; \
	else  \
	  $(call color,1) ; \
	  echo "[$@ failure]" ; \
	  $(call default_color) ; \
	fi ;\
	exit $$sta

.PHONY: all clean pre-build post-build main-build build skel

clean:
	rm -f $(OBJ_FILES)
	rm -f $(DIST)
	rm -rf $(SKEL_DIST_DIR).tar.bz2


skel:
	rm -rf $(SKEL_DIST_DIR)
	mkdir $(SKEL_DIST_DIR)
	cp $(SKEL_FILES) $(SKEL_DIST_DIR)
	tar -cjvf $(SKEL_DIST_DIR).tar.bz2 $(SKEL_DIST_DIR)
	rm -rf $(SKEL_DIST_DIR)
