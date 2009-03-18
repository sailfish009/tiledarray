#MADNESSROOT = $(HOME)/Development/MADNESS/install
MADNESSDIR = $(HOME)/workspace/madness
BOOSTDIR = /usr/local/boost_1_36_0

CXX = g++
#CXXFLAGS = -g -Wall -fmessage-length=0 -I./src -I./Tests -I$(MADNESSROOT)/include -DTA_DLEVEL=3 -DTA_WLEVEL=3
CXXFLAGS = -g -Wall -fmessage-length=0 -I$(MADNESSDIR)/include -I$(BOOSTDIR) -I./src -I./Tests -DTA_DLEVEL=3 -DTA_WLEVEL=3

OBJS = src/tilemap.o src/env.o TiledArrayTest.o Tests/coordinatestest.o Tests/range1test.o  \
  Tests/rangetest.o Tests/shapetest.o Tests/arraytest.o

#LIBDIR = -L$(MADNESSROOT)/lib
#LIBS = -lMADworld

TARGET =	TiledArrayTest

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBDIR) $(LIBS) $(INCDIR) $(DEBUGLEVEL)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
