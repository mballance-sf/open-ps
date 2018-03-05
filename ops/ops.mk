
OPS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
OPS_SRC_DIR := $(OPS_DIR)/src

ifneq (1, $(RULES))

OPS_SRC_FULL = $(wildcard $(OPS_SRC_DIR)/*.cpp)
OPS_SRC = $(notdir $(OPS_SRC_FULL))

SRC_DIRS += $(OPS_SRC_DIR)
EXE_TARGETS := ops$(EXEEXT)

else # Rules

$(OPS_SRC_FULL) : pss-grammar.gen

ops$(EXEEXT) : $(OPS_SRC:.cpp=.o) libpss_parser.a $(ANTLR4_RUNTIME_DEPS)
	$(Q)$(CXX) -o $@ $(OPS_SRC:.cpp=.o) libpss_parser.a $(ANTLR4_RUNTIME_LINK)

endif # End Rules

