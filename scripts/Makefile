
PWD:=$(shell pwd)
SCRIPTS_DIR:=$(abspath $(dir $(lastword $(MAKEFILE_LIST))))
OPEN_PS_DIR:=$(abspath $(SCRIPTS_DIR)/..)
# SCRIPTS_DIR:=$(shell cd $(SCRIPTS_DIR); pwd)

ifeq ($(PWD),$(SCRIPTS_DIR))
	BUILD_DIR:=$(OPEN_PS_DIR)/build
else 
	BUILD_DIR:=$(PWD)
endif

include $(SCRIPTS_DIR)/common.mk

MK_INCLUDES += $(OPEN_PS_DIR)/compiler/compiler.mk
MK_INCLUDES += $(OPEN_PS_DIR)/opsc/opsc.mk
#MK_INCLUDES += $(OPEN_PS_DIR)/opsg/opsg.mk
MK_INCLUDES += $(OPEN_PS_DIR)/model/model.mk
MK_INCLUDES += $(OPEN_PS_DIR)/processor/processor.mk
MK_INCLUDES += $(OPEN_PS_DIR)/xml/xml.mk
#MK_INCLUDES += $(OPEN_PS_DIR)/qpssc/qpssc.mk
#MK_INCLUDES += $(OPEN_PS_DIR)/qpsrt/qpsrt.mk
#MK_INCLUDES += $(OPEN_PS_DIR)/pss2psi/pss2psi.mk
#MK_INCLUDES += $(OPEN_PS_DIR)/qpsskw/qpsskw.mk

ifeq (true,$(BUILD))	
include $(MK_INCLUDES)
else # not build
ifeq (,$(J))
NUM_CORES:=$(shell cat /proc/cpuinfo | grep 'processor.*:' | wc -l)
else
NUM_CORES:=$(J)
endif

#ifeq (,$(MAKEFLAGS))
#MAKEFLAGS += -j$(NUM_CORES)
#endif
endif # end not build

RULES := 1

vpath %.cpp $(SRC_DIRS)

ifeq (true,$(BUILD))	
include $(MK_INCLUDES)
endif


all :
	$(Q)echo "PWD=$(PWD) SCRIPTS_DIR=$(SCRIPTS_DIR)"
	$(Q)if test "x$(PWD)" = "x$(SCRIPTS_DIR)"; then \
		echo "Running from scripts dir"; \
		if test ! -d $(BUILD_DIR); then mkdir -p $(BUILD_DIR); fi ; \
		$(MAKE) BUILD=true -C $(BUILD_DIR) -f $(SCRIPTS_DIR)/Makefile __unpack ; \
		$(MAKE) BUILD=true -j$(NUM_CORES) -C $(BUILD_DIR) -f $(SCRIPTS_DIR)/Makefile __build ; \
	else \
		echo "Running from remote build dir"; \
		$(MAKE) BUILD=true -f $(SCRIPTS_DIR)/Makefile __release ; \
	fi 

ifeq (true,$(BUILD))	
fetch-packages : $(PACKAGES)
	$(Q)echo "PACKAGES: $(PACKAGES)"
else
fetch-packages : 
	$(MAKE) BUILD=true -f $(SCRIPTS_DIR)/Makefile fetch-packages
endif

clean-packages :
	$(Q)rm -rf $(PACKAGES_DIR)
	
clean :
	$(Q)if test "x$(PWD)" = "x$(SCRIPTS_DIR)"; then \
		echo "Running clean in scripts dir"; \
		rm -rf $(BUILD_DIR); \
	else \
		echo "Running clean from build dir"; \
		rm -rf *.o *.a $(LIB_TARGETS) $(EXE_TARGETS) *.gen *.unpack; \
	fi \

include $(SCRIPTS_DIR)/common.mk

ifeq (true,$(BUILD))	
__unpack : $(UNPACK_TARGETS)

__build : $(LIB_TARGETS) $(EXE_TARGETS)
	echo "__build: $(LIB_TARGETS) $(EXE_TARGETS) $(MK_INCLUDES)"

__release :	$(RELEASE_TARGETS)
	

endif


