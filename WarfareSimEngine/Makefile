# ============================================================
# Compiler / Tools
# ============================================================

CXX := g++
AR  := ar

# ============================================================
# Directories
# ============================================================

OBJ_DIR := Object
BIN_DIR := Bin

ENGINE_DIR  := WarfareSimEngine
MEMCONTROL_DIR  := SharedMemControl
SPATIAL_DIR  := SpatialDataManager
ENTITY_DIR   := EntityManager
EVENT_DIR    := EventManager
MOVEMENT_DIR := MovementSystem
TIME_DIR     := TimeManager
NETWORK_DIR  := NetworkManager

CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -pthread
LDLIBS   := -pthread

INCLUDES := \
	-I$(ENGINE_DIR)/inc \
	-I$(MEMCONTROL_DIR)/inc \
	-I$(SPATIAL_DIR)/inc \
	-I$(ENTITY_DIR)/inc \
	-I$(EVENT_DIR)/inc \
	-I$(MOVEMENT_DIR)/inc \
	-I$(TIME_DIR)/inc \
	-I$(NETWORK_DIR)/inc

# ============================================================
# Source Files
# ============================================================

ENGINE_SRCS := $(wildcard $(ENGINE_DIR)/src/*.cpp)
MEMCONTROL_SRCS := $(wildcard $(MEMCONTROL_DIR)/src/*.cpp)
SPATIAL_SRCS := $(wildcard $(SPATIAL_DIR)/src/*.cpp)
ENTITY_SRCS  := $(wildcard $(ENTITY_DIR)/src/*.cpp)
EVENT_SRCS   := $(wildcard $(EVENT_DIR)/src/*.cpp)
MOVEMENT_SRCS := $(wildcard $(MOVEMENT_DIR)/src/*.cpp)
TIME_SRCS     := $(wildcard $(TIME_DIR)/src/*.cpp)
NETWORK_SRCS  := $(wildcard $(NETWORK_DIR)/src/*.cpp)

# ============================================================
# Object Files
# ============================================================

ENGINE_OBJS := \
	$(patsubst $(ENGINE_DIR)/src/%.cpp,$(OBJ_DIR)/$(ENGINE_DIR)/%.o,$(ENGINE_SRCS))

MEMCONTROL_OBJS := \
	$(patsubst $(MEMCONTROL_DIR)/src/%.cpp,$(OBJ_DIR)/$(MEMCONTROL_DIR)/%.o,$(MEMCONTROL_SRCS))

SPATIAL_OBJS := \
	$(patsubst $(SPATIAL_DIR)/src/%.cpp,$(OBJ_DIR)/$(SPATIAL_DIR)/%.o,$(SPATIAL_SRCS))

ENTITY_OBJS := \
	$(patsubst $(ENTITY_DIR)/src/%.cpp,$(OBJ_DIR)/$(ENTITY_DIR)/%.o,$(ENTITY_SRCS))

EVENT_OBJS := \
	$(patsubst $(EVENT_DIR)/src/%.cpp,$(OBJ_DIR)/$(EVENT_DIR)/%.o,$(EVENT_SRCS))

MOVEMENT_OBJS := \
	$(patsubst $(MOVEMENT_DIR)/src/%.cpp,$(OBJ_DIR)/$(MOVEMENT_DIR)/%.o,$(MOVEMENT_SRCS))

TIME_OBJS := \
	$(patsubst $(TIME_DIR)/src/%.cpp,$(OBJ_DIR)/$(TIME_DIR)/%.o,$(TIME_SRCS))

NETWORK_OBJS := \
	$(patsubst $(NETWORK_DIR)/src/%.cpp,$(OBJ_DIR)/$(NETWORK_DIR)/%.o,$(NETWORK_SRCS))

# main.cpp
MAIN_OBJ := $(OBJ_DIR)/main.o

# ============================================================
# Static Libraries
# ============================================================

ENGINE_LIB  := $(OBJ_DIR)/libWarfareSimEngine.a
MEMCONTROL_LIB  := $(OBJ_DIR)/libSharedMemControl.a
SPATIAL_LIB  := $(OBJ_DIR)/libSpatialDataManager.a
ENTITY_LIB   := $(OBJ_DIR)/libEntityManager.a
EVENT_LIB    := $(OBJ_DIR)/libEventManager.a
MOVEMENT_LIB := $(OBJ_DIR)/libMovementSystem.a
TIME_LIB := $(OBJ_DIR)/libTimeManager.a
NETWORK_LIB := $(OBJ_DIR)/libNetworkManager.a

LIBS := \
	$(ENGINE_LIB) \
	$(MEMCONTROL_LIB) \
	$(SPATIAL_LIB) \
	$(ENTITY_LIB) \
	$(EVENT_LIB) \
	$(MOVEMENT_LIB) \
	$(TIME_LIB) \
	$(NETWORK_LIB)

# ============================================================
# Target
# ============================================================

TARGET := $(BIN_DIR)/WarfareSimEngine

# ============================================================
# Default Target
# ============================================================

.PHONY: all clean

all: $(TARGET)

# ============================================================
# Create Directories
# ============================================================

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR)/$(ENGINE_DIR):
	mkdir -p $@

$(OBJ_DIR)/$(MEMCONTROL_DIR):
	mkdir -p $@

$(OBJ_DIR)/$(SPATIAL_DIR):
	mkdir -p $@

$(OBJ_DIR)/$(ENTITY_DIR):
	mkdir -p $@

$(OBJ_DIR)/$(EVENT_DIR):
	mkdir -p $@

$(OBJ_DIR)/$(MOVEMENT_DIR):
	mkdir -p $@

$(OBJ_DIR)/$(TIME_DIR):
	mkdir -p $@

$(OBJ_DIR)/$(NETWORK_DIR):
	mkdir -p $@

# ============================================================
# Compile WarfareSimEngine
# ============================================================

$(OBJ_DIR)/$(MEMCONTROL_DIR)/%.o: $(MEMCONTROL_DIR)/src/%.cpp | $(OBJ_DIR)/$(MEMCONTROL_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Compile SharedMemControl
# ============================================================

$(OBJ_DIR)/$(MEMCONTROL_DIR)/%.o: $(MEMCONTROL_DIR)/src/%.cpp | $(OBJ_DIR)/$(MEMCONTROL_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Compile SpatialDataManager
# ============================================================

$(OBJ_DIR)/$(SPATIAL_DIR)/%.o: $(SPATIAL_DIR)/src/%.cpp | $(OBJ_DIR)/$(SPATIAL_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Compile EntityManager
# ============================================================

$(OBJ_DIR)/$(ENTITY_DIR)/%.o: $(ENTITY_DIR)/src/%.cpp | $(OBJ_DIR)/$(ENTITY_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Compile EventManager
# ============================================================

$(OBJ_DIR)/$(EVENT_DIR)/%.o: $(EVENT_DIR)/src/%.cpp | $(OBJ_DIR)/$(EVENT_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Compile MovementSystem
# ============================================================

$(OBJ_DIR)/$(MOVEMENT_DIR)/%.o: $(MOVEMENT_DIR)/src/%.cpp | $(OBJ_DIR)/$(MOVEMENT_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Compile TimeManager
# ============================================================

$(OBJ_DIR)/$(TIME_DIR)/%.o: $(TIME_DIR)/src/%.cpp | $(OBJ_DIR)/$(TIME_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Compile NetworkManager
# ============================================================

$(OBJ_DIR)/$(NETWORK_DIR)/%.o: $(NETWORK_DIR)/src/%.cpp | $(OBJ_DIR)/$(NETWORK_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Compile main.cpp
# ============================================================

$(MAIN_OBJ): main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Static Libraries
# ============================================================

$(ENGINE_LIB): $(ENGINE_OBJS)
	$(AR) rcs $@ $^

$(MEMCONTROL_LIB): $(MEMCONTROL_OBJS)
	$(AR) rcs $@ $^

$(SPATIAL_LIB): $(SPATIAL_OBJS)
	$(AR) rcs $@ $^

$(ENTITY_LIB): $(ENTITY_OBJS)
	$(AR) rcs $@ $^

$(EVENT_LIB): $(EVENT_OBJS)
	$(AR) rcs $@ $^

$(MOVEMENT_LIB): $(MOVEMENT_OBJS)
	$(AR) rcs $@ $^

$(TIME_LIB): $(TIME_OBJS)
	$(AR) rcs $@ $^

$(NETWORK_LIB): $(NETWORK_OBJS)
	$(AR) rcs $@ $^

# ============================================================
# Link
# ============================================================

$(TARGET): $(MAIN_OBJ) $(LIBS) | $(BIN_DIR)
	$(CXX) $(MAIN_OBJ) \
		-Wl,--start-group \
		$(LIBS) \
		-Wl,--end-group \
		$(LDLIBS) \
		-o $@

# ============================================================
# Clean
# ============================================================

clean:
	@echo "Cleaning build files..."
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)
	@find $(OBJ_DIR) -mindepth 1 -delete
	@find $(BIN_DIR) -mindepth 1 -delete
	@echo "Clean complete."