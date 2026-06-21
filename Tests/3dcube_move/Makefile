
CXX = g++
INCLUDES = -I../../Headers -I/usr/include/GL
LIBS = -lwayland-client -lwayland-egl -lEGL -lGL
 
STANDARD = -std=c++17

# VPATH specifies a list of directories to search for prerequisites

VPATH = ../../Sources/System:../../Sources/System/Internal:../../Sources/System/Internal/Audio:../../Sources/System/Internal/Physics:../../Sources/System/Environment:../../Sources/System/IO:../../Sources/System/Tools:../../Sources/System/Windows

# POC sources
POC_SRCS = 3dcube_move.cpp \
           Vector3.cpp \
           Quaternion.cpp \
           Matrix4x4.cpp \
           Mathf.cpp \
           Vector4.cpp \
           GameObject.cpp \
           Scene.cpp \
           Transform.cpp \
           Component.cpp \
           Light.cpp \
           Camera.cpp \
           Time.cpp \
           InternalGLloader.cpp \
           DummyAudioEngine.cpp \
           DummyPhysicsEngine.cpp \
           Mesh.cpp \
           Image.cpp \
           Shader.cpp \
           Vector2.cpp \
           Color.cpp \
           Color32.cpp \
           Environment.cpp \
           Input.cpp

POC_OBJS = $(POC_SRCS:.cpp=.o)

# Targets
DEBUG_TARGET = 3dcube_render_debug
RELEASE_TARGET = 3dcube_render_release

# Flags
COMMON_FLAGS = $(INCLUDES) $(STANDARD) -march=native -DSYSTEM_GRAPHICS_OPENGL
DEBUG_FLAGS = $(COMMON_FLAGS) -g -DDEBUG -DSYSTEM_PROFILER_ENABLED
RELEASE_FLAGS = $(COMMON_FLAGS) -O3 -DNDEBUG

# Default target
all: debug

debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: $(DEBUG_TARGET)

release: CXXFLAGS = $(RELEASE_FLAGS)
release: $(RELEASE_TARGET)

$(DEBUG_TARGET): $(POC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(RELEASE_TARGET): $(POC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(POC_OBJS) $(DEBUG_TARGET) $(RELEASE_TARGET)

.PHONY: all debug release clean
