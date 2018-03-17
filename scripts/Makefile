
PWD:=$(shell pwd)
SCRIPTS_DIR:=$(dir $(lastword $(MAKEFILE_LIST)))
SCRIPTS_DIR:=$(shell cd $(SCRIPTS_DIR); pwd)
OPEN_PS_DIR:=$(shell cd $(SCRIPTS_DIR)/.. ; pwd)
PACKAGES_DIR := $(OPEN_PS_DIR)/packages


uname_o := $(shell uname -o)

ifeq (Msys,$(uname_o))
IS_WIN := true
IS_MSYS := true
else
IS_WIN := false
IS_MSYS := false
endif

ifeq (true,$(IS_WIN))
DLIBEXT := .dll
EXEEXT := .exe
PLATFORM := win64
else
DLIBEXT := .so
EXEEXT :=
PLATFORM := linux_x86_64
endif

ifeq ($(PWD),$(SCRIPTS_DIR))
	OPEN_PS_BUILD_DIR:=$(OPEN_PS_DIR)/build
else 
	OPEN_PS_BUILD_DIR:=$(PWD)
endif

OPEN_PS_RLS_DIR = $(OPEN_PS_BUILD_DIR)/ops-$(version)

MK_INCLUDES += $(OPEN_PS_DIR)/parser/src/pss-parser.mk
MK_INCLUDES += $(OPEN_PS_DIR)/ops/ops.mk
MK_INCLUDES += $(OPEN_PS_DIR)/etc/open-ps.info
MK_INCLUDES += $(OPEN_PS_DIR)/model/model.mk
MK_INCLUDES += $(OPEN_PS_DIR)/processor/processor.mk
#MK_INCLUDES += $(OPEN_PS_DIR)/qpssc/qpssc.mk
#MK_INCLUDES += $(OPEN_PS_DIR)/qpsrt/qpsrt.mk
#MK_INCLUDES += $(OPEN_PS_DIR)/pss2psi/pss2psi.mk
#MK_INCLUDES += $(OPEN_PS_DIR)/qpsskw/qpsskw.mk

ifeq (true,$(VERBOSE))
UNZIP:=unzip -o
WGET:=wget
else 
UNZIP:=unzip -oq
WGET:=wget
Q:=@
endif

ifeq (true,$(BUILD))	
include $(MK_INCLUDES)
else # not build
ifeq (,$(J))
NUM_CORES:=$(shell cat /proc/cpuinfo | grep 'processor.*:' | wc -l)
else
NUM_CORES:=$(J)
endif

ifeq (,$(MAKEFLAGS))
MAKEFLAGS += -j$(NUM_CORES)
endif
endif # end not build

RULES := 1

vpath %.cpp $(SRC_DIRS)
CXXFLAGS += $(foreach d,$(SRC_DIRS),-I$(d)) -std=c++11 -fPIC -g

ifeq (true,$(BUILD))	
include $(MK_INCLUDES)
endif


all :
	$(Q)echo "PWD=$(PWD) SCRIPTS_DIR=$(SCRIPTS_DIR)"
	$(Q)if test "x$(PWD)" = "x$(SCRIPTS_DIR)"; then \
		echo "Running from scripts dir"; \
		if test ! -d $(OPEN_PS_BUILD_DIR); then mkdir -p $(OPEN_PS_BUILD_DIR); fi ; \
		$(MAKE) BUILD=true -C $(OPEN_PS_BUILD_DIR) -f $(SCRIPTS_DIR)/Makefile __release ; \
	else \
		echo "Running from remote build dir"; \
		$(MAKE) BUILD=true -f $(SCRIPTS_DIR)/Makefile __release ; \
	fi 
	
clean :
	$(Q)if test "x$(PWD)" = "x$(SCRIPTS_DIR)"; then \
		echo "Running clean in scripts dir"; \
		rm -rf $(OPEN_PS_BUILD_DIR); \
	else \
		echo "Running clean from build dir"; \
		rm -rf *.o *.a $(LIB_TARGETS) $(EXE_TARGETS) *.gen *.unpack; \
	fi \

ifeq (true,$(BUILD))	
__build : $(LIB_TARGETS) $(EXE_TARGETS)
	echo "__build: $(LIB_TARGETS) $(EXE_TARGETS) $(MK_INCLUDES)"

__release :	$(RELEASE_TARGETS)
	
endif


