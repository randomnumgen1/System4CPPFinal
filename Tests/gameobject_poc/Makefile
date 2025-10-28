CXX = g++
INCLUDES = -IHeaders
STANDARD = -std=c++17
SRCS = Sources/System/Tools/SoftwareCanvas.cpp Tests/SoftwareCanvasTests.cpp Sources/System/Vector2.cpp Sources/System/Matrix3x3.cpp Sources/System/Mathf.cpp Sources/System/Color32.cpp
OBJS = $(SRCS:.cpp=.o)

# POC
POC_SRCS = Sources/gameobject_poc.cpp Sources/System/Vector3.cpp Sources/System/Quaternion.cpp Sources/System/Matrix4x4.cpp Sources/System/Mathf.cpp Sources/System/Vector4.cpp Sources/System/GameObject.cpp Sources/System/Scene.cpp Sources/System/Transform.cpp
POC_OBJS = $(POC_SRCS:.cpp=.o)
POC_TARGET = gameobject_poc

# Targets
DEBUG_TARGET = test_software_canvas_debug
RELEASE_TARGET = test_software_canvas_release

# Flags
DEBUG_FLAGS = $(INCLUDES) $(STANDARD) -g -DDEBUG -DSYSTEM_PROFILER_ENABLED
RELEASE_FLAGS = $(INCLUDES) $(STANDARD) -O3 -DNDEBUG

LDFLAGS = -lgtest -lpthread -lgtest_main

# Default target
all: debug

debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: $(DEBUG_TARGET)

release: CXXFLAGS = $(RELEASE_FLAGS)
release: $(RELEASE_TARGET)

poc: CXXFLAGS = $(DEBUG_FLAGS)
poc: $(POC_TARGET)

$(DEBUG_TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(RELEASE_TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(POC_TARGET): $(POC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(DEBUG_TARGET) $(RELEASE_TARGET) $(POC_OBJS) $(POC_TARGET)

.PHONY: all debug release clean poc
