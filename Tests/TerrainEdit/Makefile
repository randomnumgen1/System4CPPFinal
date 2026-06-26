
CXX = g++
INCLUDES = -I../../Headers -I/usr/include/GL
LIBS = -lGL -lwayland-client -lwayland-egl -lEGL -ldl

STANDARD = -std=c++17

VPATH = ../../Sources/System:../../Sources/System/Internal:../../Sources/System/Internal/Audio:../../Sources/System/Internal/Physics:../../Sources/System/Components

SRCS = TerrainEdit.cpp \
       Terrain.cpp \
       Vector3.cpp \
       Quaternion.cpp \
       Matrix4x4.cpp \
       Mathf.cpp \
       Vector4.cpp \
       Vector2.cpp \
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
       Input.cpp \
       Color.cpp \
       Color32.cpp \
       Bounds.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = TerrainEdit_debug

COMMON_FLAGS = $(INCLUDES) $(STANDARD) -march=native -DSYSTEM_GRAPHICS_OPENGL
CXXFLAGS = $(COMMON_FLAGS) -g -DDEBUG

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
