# ---- paths & target ----
SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build
TARGET    := gravity_sim

# ---- toolchain ----
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O3 -march=native -MMD -MP -I$(INC_DIR)
LDLIBS   := -fopenmp
LDFLAGS  :=

# ---- sources/objects ----
SRCS := $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# ---- build rules ----
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: clean list
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# quick debug: show what Make thinks the sources/objects are
list:
	@printf "SRCS:\n%s\n\n" $(SRCS)
	@printf "OBJS:\n%s\n" $(OBJS)