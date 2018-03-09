
MODEL_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

MODEL_EXPORT_HEADERS=$(notdir $(wildcard $(MODEL_DIR)/src/I*.h))
MODEL_SRC=$(notdir $(wildcard $(MODEL_DIR)/src/*.cpp))

SRC_DIRS += $(MODEL_DIR)/src

else # Rules

libops_model.a : $(MODEL_SRC:.cpp=.o)
	$(Q)rm -f $@
	$(Q)$(AR) vcq $@ $^

endif
