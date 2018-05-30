
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

PACKAGES += $(PACKAGES_DIR)/z3-$(Z3_VERSION)-x64-win.zip
PACKAGES += $(PACKAGES_DIR)/z3-$(Z3_VERSION)-x64-ubuntu-14.04.zip

# LIB_TARGETS += $(PACKAGES_DIR)/$(Z3_ZIP)
PROCESSOR_DEPS = $(DLIBPREF)pss_processor$(DLIBEXT)
UNPACK_TARGETS += $(BUILD_DIR)/z3.unpack

PROCESSOR_SRC = $(notdir $(wildcard $(PROCESSOR_DIR)/src/*.cpp))
PROCESSOR_SRC_FILES := $(wildcard $(PROCESSOR_DIR)/src/*.cpp)
SRC_DIRS += $(PROCESSOR_DIR)/src $(Z3_DIR)/include

ifeq (gcc,$(COMPILER))
PROCESSOR_LINK = -L. -lpss_processor
else
PROCESSOR_LINK = -L. -lpss_processor
endif

Z3_LINK = -L$(Z3_DIR)/bin -lz3


else # Rules

$(DLIBPREF)pss_processor$(DLIBEXT) : \
	$(PROCESSOR_SRC:.cpp=.o) \
	$(PSS_PARSER_DEPS) \
	$(MODEL_DEPS) \
	$(ANTLR4_DEPS) 
	$(Q)$(LINK_DLIB) $(Z3_LINK)
	
$(BUILD_DIR)/z3.unpack : $(PACKAGES_DIR)/$(Z3_ZIP)
	$(Q)if test ! -d $(BUILD_DIR); then mkdir -p $(BUILD_DIR); fi
	$(Q)cd $(BUILD_DIR) ; $(UNZIP) $^
	$(Q)touch $@

$(PACKAGES_DIR)/z3-$(Z3_VERSION)-x64-win.zip :
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(WGET) -O $@ $(Z3_URL)/z3-$(Z3_VERSION)-x64-win.zip
	
$(PACKAGES_DIR)/z3-$(Z3_VERSION)-x64-ubuntu-14.04.zip :
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(WGET) -O $@ $(Z3_URL)/z3-$(Z3_VERSION)-x64-ubuntu-14.04.zip

endif

