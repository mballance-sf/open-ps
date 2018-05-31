
OPSC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
OPSC_SRC_DIR := $(OPSC_DIR)/src

ifneq (1, $(RULES))

OPSC_SRC_FULL = $(wildcard $(OPSC_SRC_DIR)/*.cpp)
OPSC_SRC = $(notdir $(OPSC_SRC_FULL))

SRC_DIRS += $(OPSC_SRC_DIR)
EXE_TARGETS += $(PLATFORM_BIN_DIR)/opsc$(EXEEXT)

RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/bin/opsc
RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/$(PLATFORM)/bin/opsc$(EXEEXT)

ifeq (true,$(IS_WIN))
RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/bin/opsc.bat
# Mingw64 support libraries
RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/$(PLATFORM)/bin/libwinpthread-1.dll
RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/$(PLATFORM)/bin/libgcc_s_seh-1.dll
#RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/$(PLATFORM)/bin/libstdc++-6.dll
OPS_EXE_LINK += -static-libstdc++
else
#LIBSTDCPP_A = $(shell $(CXX) --print-file-name=libstdc++.a)
OPS_EXE_LINK += -static-libstdc++
#LIBSTDCPP_PATH = $(shell g++ --print-file-name=libstdc++.so)
#LIBSTDCPP6_PATH = $(shell g++ --print-file-name=libstdc++.so.6)
#LIBSTDCPP = $(notdir $(LIBSTDCPP_PATH))
#LIBSTDCPP6 = $(notdir $(LIBSTDCPP6_PATH))
#LIBSTDCPP_DIR = $(dir $(LIBSTDCPP_PATH))

#RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/$(PLATFORM)/lib/$(LIBSTDCPP)
#RELEASE_TARGETS += $(OPEN_PS_RLS_DIR)/$(PLATFORM)/lib/$(LIBSTDCPP6)
#RELEASE_TARGETS += print
endif

OPSC_DEPS = $(OPSC_SRC:.cpp=.o)
OPSC_EXE_DEP_COMMON = $(OPSC_DEPS) libpss_parser.a libops_model.a
OPSC_EXE_DEPS = $(OPS_EXE_DEP_COMMON) $(ANTLR4_RUNTIME_DEPS)
OPSC_EXE_LINK += $(OPS_EXE_DEP_COMMON) $(ANTLR4_RUNTIME_LINK)

else # Rules

print :
	echo "LIBSTDCPP=$(LIBSTDCPP)"
	echo "LIBSTDCPP_DIR=$(LIBSTDCPP_DIR)"

$(OPS_SRC_FULL) : pss-grammar.gen

$(PLATFORM_BIN_DIR)/opsc$(EXEEXT) : \
		$(OPSC_SRC:.cpp=.o) \
		$(PSS_COMPILER_DEPS) \
		$(PROCESSOR_DEPS) \
		$(XML_DEPS) \
		$(Z3_DEPS) \
		$(MODEL_DEPS) \
		$(ANTLR4_DEPS)
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(LINK_EXE) 
	
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
	
$(OPEN_PSS_RLS_DIR)/$(PLATFORM)/lib/%.so : $(LIBSTDCPP_DIR)/%.so
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)cp $^ $@
	
$(OPEN_PSS_RLS_DIR)/$(PLATFORM)/lib/%.so.6 : $(LIBSTDCPP_DIR)/%.so.6
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)cp $^ $@

endif # End Rules

