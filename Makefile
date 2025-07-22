CXX = g++
INCLUDES = -IHeaders
STANDARD = -std=c++14
SRCS = Sources/System/Tools/SoftwareCanvas.cpp Tests/SoftwareCanvasTests.cpp Sources/System/Vector2.cpp Sources/System/Matrix3x3.cpp Sources/System/Mathf.cpp Sources/System/Color32.cpp
OBJS = $(SRCS:.cpp=.o)

# Targets
DEBUG_TARGET = test_software_canvas_debug
RELEASE_TARGET = test_software_canvas_release

# Flags
DEBUG_FLAGS = $(INCLUDES) $(STANDARD) -g -DDEBUG
RELEASE_FLAGS = $(INCLUDES) $(STANDARD) -O3 -DNDEBUG

LDFLAGS = -lgtest -lpthread -lgtest_main

# Default target
all: debug

debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: $(DEBUG_TARGET)

release: CXXFLAGS = $(RELEASE_FLAGS)
release: $(RELEASE_TARGET)

$(DEBUG_TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(RELEASE_TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(DEBUG_TARGET) $(RELEASE_TARGET)