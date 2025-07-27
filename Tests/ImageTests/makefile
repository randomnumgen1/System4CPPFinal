CXX = g++
INCLUDES = -I../../Headers
STANDARD = -std=c++17
SRCS = $(shell find ../../Sources -name "*.cpp")  ImageTests.cpp   
OBJS = $(SRCS:.cpp=.o)

# Targets
DEBUG_TARGET = image_tests_debug
RELEASE_TARGET = image_tests_release



# Flags
DEBUG_FLAGS = $(INCLUDES) $(STANDARD) -g -DDEBUG $(CXXFLAGS)
RELEASE_FLAGS = $(INCLUDES) $(STANDARD) -O3 -DNDEBUG $(CXXFLAGS)


LDFLAGS = -lgtest -lpthread -lgtest_main

# Default target
all: debug

arm: CXXFLAGS += -march=armv8-a
arm: debug

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
	
	