
MODEL_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

MODEL_EXPORT_HEADERS=$(notdir $(wildcard $(MODEL_DIR)/src/I*.h))
MODEL_SRC=$(notdir $(wildcard $(MODEL_DIR)/src/*.cpp))

SRC_DIRS += $(MODEL_DIR)/src

MODEL_DEPS := $(PLATFORM_LIB_DIR)/$(DLIBPREF)pss_model$(DLIBEXT)

ifeq (gcc,$(COMPILER))
MODEL_LINK := -L$(PLATFORM_LIB_DIR) -lpss_model
else
MODEL_LINK := -L$(PLATFORM_LIB_DIR) -lpss_model
endif

else # Rules

$(PLATFORM_LIB_DIR)/$(DLIBPREF)pss_model$(DLIBEXT) : $(MODEL_SRC:.cpp=.o)
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi	
	$(Q)$(LINK_DLIB)

endif
