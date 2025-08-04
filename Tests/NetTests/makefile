CXX = g++
INCLUDES = -I../../Headers
STANDARD = -std=c++17
SRCS = $(shell find ../../Sources -name "*.cpp") NetTests.cpp
OBJS = $(SRCS:.cpp=.o)

# Targets
DEBUG_TARGET = net_tests_debug
RELEASE_TARGET = net_tests_release

# Flags
COMMON_FLAGS = -DSYSTEM_SOCKETS_ENABLED
DEBUG_FLAGS = $(INCLUDES) $(STANDARD) -g -DDEBUG -march=native $(COMMON_FLAGS)
RELEASE_FLAGS = $(INCLUDES) $(STANDARD) -O3 -DNDEBUG -march=native $(COMMON_FLAGS)

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
