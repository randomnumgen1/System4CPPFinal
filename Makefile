CXX = g++
CXXFLAGS = -IHeaders -std=c++14
LDFLAGS = -lgtest -lpthread

SRCS = Sources/System/Tools/SoftwareCanvas.cpp Tests/SoftwareCanvasTests.cpp Sources/System/Vector2.cpp Sources/System/Matrix3x3.cpp Sources/System/Mathf.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = test_software_canvas

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) -lgtest_main

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
