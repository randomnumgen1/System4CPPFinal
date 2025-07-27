CXX = g++
INCLUDES = -I../../Headers
STANDARD = -std=c++17
SRCS = ImageTests.cpp
OBJS = $(SRCS:.cpp=.o)

# Targets
DEBUG_TARGET = image_tests_debug
RELEASE_TARGET = image_tests_release


UNAME_M = arm64
# Flags
ifeq ($(UNAME_M),$(shell uname -m))
  MARCH_FLAG = -march=native
else
  MARCH_FLAG =
endif
DEBUG_FLAGS = $(INCLUDES) $(STANDARD) -g -DDEBUG $(MARCH_FLAG)
RELEASE_FLAGS = $(INCLUDES) $(STANDARD) -O3 -DNDEBUG $(MARCH_FLAG)


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
