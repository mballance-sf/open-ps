
OPS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
OPS_SRC_DIR := $(OPS_DIR)/src

ifneq (1, $(RULES))

OPS_SRC_FULL = $(wildcard $(OPS_SRC_DIR)/*.cpp)
OPS_SRC = $(notdir $(OPS_SRC_FULL))

SRC_DIRS += $(OPS_SRC_DIR)
EXE_TARGETS := ops$(EXEEXT)

RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/bin/ops
RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/$(PLATFORM)/bin/ops$(EXEEXT)

ifeq (true,$(IS_WIN))
RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/bin/ops.bat
# Mingw64 support libraries
RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/$(PLATFORM)/bin/libwinpthread-1.dll
RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/$(PLATFORM)/bin/libgcc_s_seh-1.dll
RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/$(PLATFORM)/bin/libstdc++-6.dll
endif

OPS_EXE_DEP_COMMON = $(OPS_SRC:.cpp=.o) libpss_parser.a libops_model.a
OPS_EXE_DEPS = $(OPS_EXE_DEP_COMMON) $(ANTLR4_RUNTIME_DEPS)
OPS_EXE_LINK = $(OPS_EXE_DEP_COMMON) $(ANTLR4_RUNTIME_LINK)

else # Rules

$(OPS_SRC_FULL) : pss-grammar.gen

ops$(EXEEXT) : $(OPS_EXE_DEPS)
	$(Q)$(CXX) -o $@ $(OPS_EXE_LINK)
	
$(OPEN_PS_RLS_DIR)/$(PLATFORM)/bin/ops$(EXEEXT) : ops$(EXEEXT)
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)cp $^ $@
ifneq (true,$(IS_WIN))
	$(Q)chmod +x $@
endif

$(OPEN_PS_RLS_DIR)/bin/ops : $(OPS_SRC_DIR)/ops
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)cp $^ $@
ifneq (true,$(IS_WIN))
	$(Q)chmod +x $@
endif

$(OPEN_PS_RLS_DIR)/bin/ops.bat : $(OPS_SRC_DIR)/ops.bat
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)cp $^ $@
	
$(OPEN_PS_RLS_DIR)/$(PLATFORM)/bin/%.dll : /mingw64/bin/%.dll
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)cp $^ $@

endif # End Rules

