CXX = g++
INCLUDES = -I../../Headers
STANDARD = -std=c++17

# VPATH specifies a list of directories to search for prerequisites.
VPATH = ../../Sources/System ../../Sources/System/Internal ../../Sources/System/Internal/Audio ../../Sources/System/Internal/Physics

# SRCS
SRCS = scene_render.cpp \
       Scene.cpp \
       GameObject.cpp \
       Camera.cpp \
       Shader.cpp \
       Texture2d.cpp \
       Transform.cpp \
       Vector3.cpp \
	   DummyPhysicsEngine.cpp \
       Vector4.cpp \
       Quaternion.cpp \
       Matrix4x4.cpp \
       Mathf.cpp \
       Mesh.cpp \
       Image.cpp \
       InternalGLloader.cpp \
       Color.cpp \
       DummyAudioEngine.cpp

OBJS = $(SRCS:.cpp=.o)

# Targets
DEBUG_TARGET = scene_render_debug
RELEASE_TARGET = scene_render_release

# Flags
DEBUG_FLAGS = $(INCLUDES) $(STANDARD) -g -DDEBUG -DSYSTEM_GRAPHICS_OPENGL
RELEASE_FLAGS = $(INCLUDES) $(STANDARD) -O3 -DNDEBUG -DSYSTEM_GRAPHICS_OPENGL
LDFLAGS = -lGL -ldl

# Default target
all: debug

debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: $(DEBUG_TARGET)

release: CXXFLAGS = $(RELEASE_FLAGS)
release: $(RELEASE_TARGET)

$(DEBUG_TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(RELEASE_TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(DEBUG_TARGET) $(RELEASE_TARGET)

.PHONY: all debug release clean
