
PROCESSOR_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

Z3_VERSION=4.6.0
Z3_URL:=https://github.com/Z3Prover/z3/releases/download/z3-$(Z3_VERSION)

ifeq (true,$(IS_WIN))
Z3_ZIP:=z3-$(Z3_VERSION)-x64-win.zip
Z3_DIR:=z3-$(Z3_VERSION)-x64-win
else
Z3_ZIP:=z3-$(Z3_VERSION)-x64-ubuntu-14.04.zip
Z3_DIR:=z3-$(Z3_VERSION)-x64-ubuntu-14.04
endif

ifneq (1,$(RULES))

LIB_TARGETS += $(PACKAGES_DIR)/$(Z3_ZIP)

PROCESSOR_SRC = $(notdir $(wildcard $(PROCESSOR_DIR)/src/*.cpp))
SRC_DIRS += $(PROCESSOR_DIR)/src $(Z3_DIR)/include

Z3_LINK = -L$(Z3_DIR)/bin -lz3


else # Rules

libpss_processor.a : $(PROCESSOR_SRC:.cpp=.o)
	$(Q)rm -f $@
	$(Q)$(AR) vcq $@ $^
	
$(PROCESSOR_SRC) : z3.unpack

z3.unpack : $(PACKAGES_DIR)/$(Z3_ZIP)
	$(Q)$(UNZIP) $^
	$(Q)touch $@

$(PACKAGES_DIR)/$(Z3_ZIP) : 
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(WGET) -O $@ $(Z3_URL)/$(Z3_ZIP)

endif

