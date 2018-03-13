
PROCESSOR_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

Z3_VERSION=4.6.0
Z3_URL:=https://github.com/Z3Prover/z3/releases/download/z3-$(Z3_VERSION)

ifeq (true,$(IS_WIN))
Z3_ZIP:=z3-$(Z3_VERSION)-x64-win.zip
else
Z3_ZIP:=z3-$(Z3_VERSION)-x64-ubuntu-14.04.zip
endif

ifneq (1,$(RULES))

LIB_TARGETS += $(PACKAGES_DIR)/$(Z3_ZIP)

PROCESSOR_SRC = $(notdir $(wildcard $(PROCESSOR_DIR)/src/*.cpp))
SRC_DIRS += $(PROCESSOR_DIR)/src


else # Rules

libpss_processor.a : $(PROCESSOR_SRC:.cpp=.o)
	$(Q)rm -f $@
	$(Q)$(AR) vcq $@ $^

$(PACKAGES_DIR)/$(Z3_ZIP) : 
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(WGET) -O $@ $(Z3_URL)/$(Z3_ZIP)

endif

