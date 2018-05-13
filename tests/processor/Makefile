
ifneq (true,$(BUILD))
TESTS_PARSER_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
TESTS_PARSER_DIR := $(shell cd $(TESTS_PARSER_DIR) ; pwd)
ifneq (,$(J))
NUM_CORES := $(J)
else
NUM_CORES:=$(shell cat /proc/cpuinfo | grep 'processor.*:' | wc -l)
endif

endif

BUILD_DIR := $(TESTS_PARSER_DIR)/build

OPEN_PS_DIR := $(shell cd $(TESTS_PARSER_DIR)/../.. ; pwd)
PACKAGES_DIR := $(OPEN_PS_DIR)/packages

TESTS_DATA_DIR := $(shell cd $(TESTS_PARSER_DIR)/../data ; pwd)

MK_INCLUDES += $(TESTS_PARSER_DIR)/../tests.mk
MK_INCLUDES += $(TESTS_PARSER_DIR)/../../scripts/common.mk
MK_INCLUDES += $(TESTS_PARSER_DIR)/../../model/model.mk
MK_INCLUDES += $(TESTS_PARSER_DIR)/../../parser/src/pss-parser.mk
MK_INCLUDES += $(TESTS_PARSER_DIR)/../../processor/processor.mk
MK_INCLUDES += $(TESTS_PARSER_DIR)/../../pss-cpp/pss-cpp.mk
ifeq (true,$(BUILD))
endif

PARSER_TESTS_SRC = $(notdir $(wildcard $(TESTS_PARSER_DIR)/src/*.cpp))
SRC_DIRS += $(TESTS_PARSER_DIR)/src

CXXFLAGS += -DTESTS_DATA_DIR='"$(TESTS_DATA_DIR)"'
CXXFLAGS += -g
CFLAGS += -g

include $(MK_INCLUDES)

ifeq (true,$(IS_WIN))
	PATH := $(Z3_DIR)/bin:.:./antlr:$(PATH)
	export PATH
else
	GCC_BINDIR := $(dir $(shell which g++))
	LD_LIBRARY_PATH := $(Z3_DIR)/bin:.:./antlr:$(GCC_BINDIR)/../lib64:$(LD_LIBRARY_PATH)
	export LD_LIBRARY_PATH
endif

RULES := 1

vpath %.cpp $(SRC_DIRS)
vpath %.cc $(SRC_DIRS)
vpath %.c $(SRC_DIRS)

all : $(UNPACK_TARGETS)
	$(Q)echo "UNPACK_TARGETS=$(UNPACK_TARGETS)"
	$(MAKE) -C $(BUILD_DIR) -f $(TESTS_PARSER_DIR)/Makefile BUILD=true \
		VERBOSE=$(VERBOSE) \
		-j$(NUM_CORES) TESTS_PARSER_DIR=$(TESTS_PARSER_DIR) __run
		

__run: unittest$(EXEEXT)
	echo "LD_LIBRARY_PATH=$(LD_LIBRARY_PATH)"
	./unittest$(EXEEXT) $(GTEST_ARGS)

# __run: $(DLIBPREF)pss-cpp$(DLIBEXT) pss_sys_test.o

unittest$(EXEEXT) : $(PARSER_TESTS_SRC:.cpp=.o) \
		$(PROCESSOR_DEPS) \
		$(MODEL_DEPS) \
		$(ANTLR4_DEPS) \
		gtest-all.o gtest_main.o
	$(Q)$(LINK_EXE) -lpthread

clean :
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*$(DLIBEXT)
	
clean-all :
	rm -rf $(BUILD_DIR)
	
include $(MK_INCLUDES)


