CXX = g++
INCLUDES = -I../../Headers
STANDARD = -std=c++17

# VPATH specifies a list of directories to search for prerequisites.
VPATH = ../../Sources/System

# POC
POC_SRCS = gameobject_poc.cpp MonsterAI.cpp Vector3.cpp Quaternion.cpp Matrix4x4.cpp Mathf.cpp Vector4.cpp GameObject.cpp Scene.cpp Transform.cpp
POC_OBJS = $(POC_SRCS:.cpp=.o)

# Targets
DEBUG_TARGET = gameobject_poc_debug
RELEASE_TARGET = gameobject_poc_release

# Flags
DEBUG_FLAGS = $(INCLUDES) $(STANDARD) -g -DDEBUG -DSYSTEM_PROFILER_ENABLED
RELEASE_FLAGS = $(INCLUDES) $(STANDARD) -O3 -DNDEBUG

# Default target
all: debug

debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: $(DEBUG_TARGET)

release: CXXFLAGS = $(RELEASE_FLAGS)
release: $(RELEASE_TARGET)

$(DEBUG_TARGET): $(POC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(RELEASE_TARGET): $(POC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(POC_OBJS) $(DEBUG_TARGET) $(RELEASE_TARGET)

.PHONY: all debug release clean
