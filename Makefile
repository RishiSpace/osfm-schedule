CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread
INCLUDES = 
LIBS = 

# If you installed nlohmann-json3-dev, no extra include needed.
# If you use a local json.hpp, add: INCLUDES = -I/path/to/json

SRCS = server.cpp client.cpp network_utils.cpp
OBJS = $(SRCS:.cpp=.o)

all: server client

server: server.o network_utils.o
	$(CXX) $(CXXFLAGS) -o server server.o network_utils.o $(LIBS)

client: client.o network_utils.o
	$(CXX) $(CXXFLAGS) -o client client.o network_utils.o $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o server client

.PHONY: all clean