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

NETWORK_DIR  := NetworkManager
COMMON_DIR  := Common
MEMCONTROL_DIR  := SharedMemControl

CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -pthread
LDLIBS   := -pthread

INCLUDES := \
	-I$(NETWORK_DIR)/inc \
	-I$(COMMON_DIR)/inc \
	-I$(MEMCONTROL_DIR)/inc

# ============================================================
# Source Files
# ============================================================

NETWORK_SRCS  := $(wildcard $(NETWORK_DIR)/src/*.cpp)
COMMON_SRCS  := $(wildcard $(COMMON_DIR)/src/*.cpp)
MEMCONTROL_SRCS  := $(wildcard $(MEMCONTROL_DIR)/src/*.cpp)

# ============================================================
# Object Files
# ============================================================

NETWORK_OBJS := \
	$(patsubst $(NETWORK_DIR)/src/%.cpp,$(OBJ_DIR)/$(NETWORK_DIR)/%.o,$(NETWORK_SRCS))

COMMON_OBJS := \
	$(patsubst $(COMMON_DIR)/src/%.cpp,$(OBJ_DIR)/$(COMMON_DIR)/%.o,$(COMMON_SRCS))

MEMCONTROL_OBJS := \
	$(patsubst $(MEMCONTROL_DIR)/src/%.cpp,$(OBJ_DIR)/$(MEMCONTROL_DIR)/%.o,$(MEMCONTROL_SRCS))

# main.cpp
MAIN_OBJ := $(OBJ_DIR)/main.o

# ============================================================
# Static Libraries
# ============================================================

NETWORK_LIB := $(OBJ_DIR)/libNetworkManager.a
COMMON_LIB := $(OBJ_DIR)/libCommon.a
MEMCONTROL_LIB := $(OBJ_DIR)/libSharedMemControl.a

LIBS := \
	$(NETWORK_LIB) \
	$(COMMON_LIB) \
	$(MEMCONTROL_LIB)

# ============================================================
# Target
# ============================================================

TARGET := $(BIN_DIR)/DataTransfer

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

$(OBJ_DIR)/$(NETWORK_DIR):
	mkdir -p $@
	
$(OBJ_DIR)/$(COMMON_DIR):
	mkdir -p $@
	
$(OBJ_DIR)/$(MEMCONTROL_DIR):
	mkdir -p $@

# ============================================================
# Compile NetworkManager
# ============================================================

$(OBJ_DIR)/$(NETWORK_DIR)/%.o: $(NETWORK_DIR)/src/%.cpp | $(OBJ_DIR)/$(NETWORK_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Compile Common
# ============================================================

$(OBJ_DIR)/$(COMMON_DIR)/%.o: $(COMMON_DIR)/src/%.cpp | $(OBJ_DIR)/$(COMMON_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	
# ============================================================
# Compile SharedMemControl
# ============================================================

$(OBJ_DIR)/$(MEMCONTROL_DIR)/%.o: $(MEMCONTROL_DIR)/src/%.cpp | $(OBJ_DIR)/$(MEMCONTROL_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Compile main.cpp
# ============================================================

$(MAIN_OBJ): main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# Static Libraries
# ============================================================

$(NETWORK_LIB): $(NETWORK_OBJS)
	$(AR) rcs $@ $^

$(COMMON_LIB): $(COMMON_OBJS)
	$(AR) rcs $@ $^
	
$(MEMCONTROL_LIB): $(MEMCONTROL_OBJS)
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