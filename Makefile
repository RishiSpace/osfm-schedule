CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread
INCLUDES = 
LIBS = 

SRCS = server.cpp client.cpp network_utils.cpp
OBJS = $(SRCS:.cpp=./Binaries/%.o)

BIN_DIR = Binaries

all: $(BIN_DIR)/server $(BIN_DIR)/client

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/server: $(BIN_DIR)/server.o $(BIN_DIR)/network_utils.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/server $(BIN_DIR)/server.o $(BIN_DIR)/network_utils.o $(LIBS)

$(BIN_DIR)/client: $(BIN_DIR)/client.o $(BIN_DIR)/network_utils.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/client $(BIN_DIR)/client.o $(BIN_DIR)/network_utils.o $(LIBS)

$(BIN_DIR)/%.o: %.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean