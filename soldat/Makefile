# Makefile for Soldat Game Engine

# Compiler
CXX = g++

# Compile flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I./soldat/stdsrc -I./soldat/pascal/client -I./soldat/pascal/shared

# Libraries
LIBS = -lSDL2 -lGL -lGLU -lglfw -lGLEW -lphysfs

# Directories
SOLDAT_DIR = soldat
STDINC_DIR = $(SOLDAT_DIR)/stdsrc
CLIENT_DIR = $(SOLDAT_DIR)/pascal/client
SHARED_DIR = $(SOLDAT_DIR)/pascal/shared
SRC_DIR = src
BIN_DIR = bin

# Objects for client
CLIENT_OBJECTS = \
	$(SRC_DIR)/client_main.o \
	$(SRC_DIR)/main.o \
	$(STDINC_DIR)/Constants.o \
	$(STDINC_DIR)/Util.o \
	$(STDINC_DIR)/Vector.o \
	$(STDINC_DIR)/Calc.o \
	$(STDINC_DIR)/Console.o \
	$(STDINC_DIR)/Cvar.o \
	$(STDINC_DIR)/Command.o \
	$(STDINC_DIR)/LogFile.o \
	$(STDINC_DIR)/Version.o \
	$(STDINC_DIR)/SharedConfig.o \
	$(STDINC_DIR)/Demo.o \
	$(STDINC_DIR)/AI.o \
	$(STDINC_DIR)/Anims.o \
	$(STDINC_DIR)/AutoUpdater.o \
	$(STDINC_DIR)/Game.o \
	$(STDINC_DIR)/MapFile.o \
	$(STDINC_DIR)/Parts.o \
	$(STDINC_DIR)/PolyMap.o \
	$(STDINC_DIR)/TraceLog.o \
	$(STDINC_DIR)/Waypoints.o \
	$(STDINC_DIR)/Weapons.o \
	$(STDINC_DIR)/ClientGame.o \
	$(STDINC_DIR)/Sprites.o

# Objects for server
SERVER_OBJECTS = \
	$(SRC_DIR)/server_main.o \
	$(STDINC_DIR)/Constants.o \
	$(STDINC_DIR)/Util.o \
	$(STDINC_DIR)/Vector.o \
	$(STDINC_DIR)/Calc.o \
	$(STDINC_DIR)/Console.o \
	$(STDINC_DIR)/Cvar.o \
	$(STDINC_DIR)/Command.o \
	$(STDINC_DIR)/LogFile.o \
	$(STDINC_DIR)/Version.o \
	$(STDINC_DIR)/SharedConfig.o \
	$(STDINC_DIR)/Demo.o \
	$(STDINC_DIR)/AI.o \
	$(STDINC_DIR)/Anims.o \
	$(STDINC_DIR)/AutoUpdater.o \
	$(STDINC_DIR)/Game.o \
	$(STDINC_DIR)/MapFile.o \
	$(STDINC_DIR)/Parts.o \
	$(STDINC_DIR)/PolyMap.o \
	$(STDINC_DIR)/TraceLog.o \
	$(STDINC_DIR)/Waypoints.o \
	$(STDINC_DIR)/Weapons.o \
	$(STDINC_DIR)/Server.o

# Targets
all: client server

client: $(BIN_DIR) $(BIN_DIR)/soldat_client

server: $(BIN_DIR) $(BIN_DIR)/soldat_server

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/soldat_client: $(CLIENT_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(BIN_DIR)/soldat_server: $(SERVER_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Compile C++ sources to objects
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Headers dependencies would go here, but are simplified for this makefile
$(STDINC_DIR)/%.o: $(STDINC_DIR)/%.h
	# Headers are directly included in source files

clean:
	rm -f $(BIN_DIR)/soldat_client $(BIN_DIR)/soldat_server $(CLIENT_OBJECTS) $(SERVER_OBJECTS)

install: all
	cp $(BIN_DIR)/soldat_client /usr/local/bin/ || cp $(BIN_DIR)/soldat_client /usr/bin/
	cp $(BIN_DIR)/soldat_server /usr/local/bin/ || cp $(BIN_DIR)/soldat_server /usr/bin/

.PHONY: all client server clean install