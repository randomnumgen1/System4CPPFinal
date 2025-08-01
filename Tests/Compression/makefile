CXX = g++
INCLUDES = -I../../Headers
STANDARD = -std=c++17
SRCS = $(shell find ../../Sources -name "*.cpp") DeflateTests.cpp ZLibTests.cpp
OBJS = $(SRCS:.cpp=.o)

# Targets
DEBUG_TARGET = compression_tests_debug
RELEASE_TARGET = compression_tests_release

# Flags
DEBUG_FLAGS = $(INCLUDES) $(STANDARD) -g -DDEBUG -march=native
RELEASE_FLAGS = $(INCLUDES) $(STANDARD) -O3 -DNDEBUG -march=native

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
