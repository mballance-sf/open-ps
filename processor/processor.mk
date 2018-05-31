
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
PROCESSOR_DEPS = $(PLATFORM_LIB_DIR)/$(DLIBPREF)pss_processor$(DLIBEXT)
UNPACK_TARGETS += $(BUILD_DIR)/z3.unpack

PROCESSOR_SRC = $(notdir $(wildcard $(PROCESSOR_DIR)/src/*.cpp))
PROCESSOR_SRC_FILES := $(wildcard $(PROCESSOR_DIR)/src/*.cpp)
SRC_DIRS += $(PROCESSOR_DIR)/src $(Z3_DIR)/include

ifeq (gcc,$(COMPILER))
PROCESSOR_LINK = -L$(PLATFORM_LIB_DIR) -lpss_processor
else
PROCESSOR_LINK = -L$(PLATFORM_LIB_DIR) -lpss_processor
endif

Z3_DEPS = $(PLATFORM_LIB_DIR)/$(DLIBPREF)z3$(DLIBEXT)

# Z3_LINK = -L$(Z3_DIR)/bin -lz3


else # Rules

$(PLATFORM_LIB_DIR)/$(DLIBPREF)pss_processor$(DLIBEXT) : \
	$(PROCESSOR_SRC:.cpp=.o) \
	$(PSS_PARSER_DEPS) \
	$(MODEL_DEPS) \
	$(ANTLR4_DEPS) \
	$(Z3_DEPS)
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi	
	$(Q)$(LINK_DLIB)
	
$(BUILD_DIR)/z3.unpack : $(PACKAGES_DIR)/$(Z3_ZIP)
	$(Q)if test ! -d $(BUILD_DIR); then mkdir -p $(BUILD_DIR); fi
	$(Q)cd $(BUILD_DIR) ; $(UNZIP) $^
	$(Q)touch $@

$(PLATFORM_LIB_DIR)/$(DLIBPREF)z3$(DLIBEXT) : z3.unpack
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)cp $(Z3_DIR)/bin/$(DLIBPREF)z3$(DLIBEXT) $@

$(PACKAGES_DIR)/z3-$(Z3_VERSION)-x64-win.zip :
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(WGET) -O $@ $(Z3_URL)/z3-$(Z3_VERSION)-x64-win.zip
	
$(PACKAGES_DIR)/z3-$(Z3_VERSION)-x64-ubuntu-14.04.zip :
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(WGET) -O $@ $(Z3_URL)/z3-$(Z3_VERSION)-x64-ubuntu-14.04.zip

endif

