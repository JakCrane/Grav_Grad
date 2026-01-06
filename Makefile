# ---- paths & target ----
SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build
TARGET    := gravity_sim

# Choose compiler profile: intel (default) or amd
COMPILER ?= amd

# ---- toolchain selection ----
ifeq ($(COMPILER),intel)
    CXX      := icpx
    CXXFLAGS := -std=c++23 -Wall -Wextra -O3 -g \
                -xCORE-AVX512 -qopenmp -qopt-zmm-usage=high \
                -I$(INC_DIR) -MMD -MP
    LDLIBS   := -qopenmp
    LDFLAGS  :=
else ifeq ($(COMPILER),amd)
    # Use GCC (or replace with clang++ if preferred)
    CXX      := g++
    CXXFLAGS := -std=c++23 -Wall -Wextra -O3 -g \
                -march=znver2 -fopenmp \
                -I$(INC_DIR) -MMD -MP
    LDLIBS   := -fopenmp
    LDFLAGS  :=
else
    $(error Unknown COMPILER '$(COMPILER)'. Use COMPILER=intel or COMPILER=amd)
endif

# Dimensionality: 2 or 3 (default 2)
DIM ?= 2

# Choose which folder to EXCLUDE (TwoD or ThreeD) based on DIM
ifeq ($(DIM),2)
    EXCLUDE ?= ThreeD
    CXX_DEFINES := -DDIM=2
else ifeq ($(DIM),3)
    EXCLUDE ?= TwoD
    CXX_DEFINES := -DDIM=3
else
    $(error Unknown DIM '$(DIM)'. Use DIM=2 or DIM=3)
endif

# ---- sources/objects ----
SRCS_ALL := $(shell find $(SRC_DIR) -type f -name '*.cpp')
SRCS := $(filter-out $(SRC_DIR)/$(EXCLUDE)/%,$(SRCS_ALL))
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
	rm -rf gperf_results*
	rm -rf grav_sim.*
	rm -rf *.csv

# quick debug: show what Make thinks the sources/objects are
list:
	@printf "SRCS:\n%s\n\n" $(SRCS)
	@printf "OBJS:\n%s\n" $(OBJS)