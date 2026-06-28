
CXX = g++
INCLUDES = -I../../Headers -I/usr/include/GL -I/usr/include/wayland
LIBS = -lwayland-client -lwayland-egl -lEGL -lGL

STANDARD = -std=c++17

# VPATH specifies a list of directories to search for prerequisites
VPATH = ../../Sources/System:../../Sources/System/Internal:../../Sources/System/Internal/Audio:../../Sources/System/Internal/Physics

# Sources
SRCS = cube_proced.cpp \
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
           Input.cpp

OBJS = $(SRCS:.cpp=.o)

# Targets
DEBUG_TARGET = cube_proced_debug
RELEASE_TARGET = cube_proced_release

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

$(DEBUG_TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(RELEASE_TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(DEBUG_TARGET) $(RELEASE_TARGET)

.PHONY: all debug release clean
