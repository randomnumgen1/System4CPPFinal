CXX = g++
CXXFLAGS = -std=c++11 -Wall -I../../../
LDFLAGS =
SOURCES = main.cpp
EXECUTABLE = client

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)
