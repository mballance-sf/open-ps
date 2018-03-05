
PWD:=$(shell pwd)
SCRIPTS_DIR:=$(dir $(lastword $(MAKEFILE_LIST)))
SCRIPTS_DIR:=$(shell cd $(SCRIPTS_DIR); pwd)
OPEN_PS_DIR:=$(shell cd $(SCRIPTS_DIR)/.. ; pwd)
PACKAGES_DIR := $(OPEN_PS_DIR)/packages

uname_o := $(shell uname -o)

ifeq (Msys,$(uname_o))
IS_WIN := true
else
IS_WIN := false
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
	QPSSKW_BUILD_DIR:=$(OPEN_PS_DIR)/build
else 
	QPSSKW_BUILD_DIR:=$(PWD)
endif

MK_INCLUDES += $(OPEN_PS_DIR)/parser/src/pss-parser.mk
MK_INCLUDES += $(OPEN_PS_DIR)/ops/ops.mk
MK_INCLUDES += $(OPEN_PS_DIR)/etc/open-ps.info
#MK_INCLUDES += $(OPEN_PS_DIR)/psi/psi.mk
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
LIBSTDCPP = $(shell g++ --print-file-name=libstdc++.so)
LIBSTDCPP6 = $(shell g++ --print-file-name=libstdc++.so.6)
else # not build
NUM_CORES:=$(shell cat /proc/cpuinfo | grep 'processor.*:' | wc -l)

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
		if test ! -d $(QPSSKW_BUILD_DIR); then mkdir -p $(QPSSKW_BUILD_DIR); fi ; \
		$(MAKE) BUILD=true -C $(QPSSKW_BUILD_DIR) -f $(SCRIPTS_DIR)/Makefile __release ; \
	else \
		echo "Running from remote build dir"; \
		$(MAKE) BUILD=true -f $(SCRIPTS_DIR)/Makefile __release ; \
	fi 
	
clean :
	$(Q)if test "x$(PWD)" = "x$(SCRIPTS_DIR)"; then \
		echo "Running clean in scripts dir"; \
		rm -rf $(QPSSKW_BUILD_DIR); \
	else \
		echo "Running clean from build dir"; \
		rm -rf *.o *.a $(LIB_TARGETS) $(EXE_TARGETS) *.gen *.unpack; \
	fi \

ifeq (true,$(BUILD))	
__build : $(LIB_TARGETS) $(EXE_TARGETS)
	echo "__build: $(LIB_TARGETS) $(EXE_TARGETS) $(MK_INCLUDES)"

__release :	__build
	$(Q)rm -rf $(QPSSKW_BUILD_DIR)/ops-$(version)
	$(Q)mkdir -p $(QPSSKW_BUILD_DIR)/ops-$(version)
	$(Q)mkdir -p $(QPSSKW_BUILD_DIR)/ops-$(version)/bin
	$(Q)mkdir -p $(QPSSKW_BUILD_DIR)/ops-$(version)/$(PLATFORM)/bin
	$(Q)mkdir -p $(QPSSKW_BUILD_DIR)/ops-$(version)/$(PLATFORM)/lib
	$(Q)cp $(QPSSKW_BUILD_DIR)/ops$(EXEEXT) $(QPSSKW_BUILD_DIR)/ops-$(version)/$(PLATFORM)/bin
ifeq (false,$(IS_WIN))
	$(Q)cp $(SCRIPTS_DIR)/ops $(QPSSKW_BUILD_DIR)/ops-$(version)/bin
	$(Q)chmod +x $(QPSSKW_BUILD_DIR)/ops-$(version)/bin/ops
	$(Q)cp $(LIBSTDCPP) $(LIBSTDCPP6) $(QPSSKW_BUILD_DIR)/ops-$(version)/linux_x86_64/lib
endif
	
endif


