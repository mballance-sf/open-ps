
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
ifeq (true,$(BUILD))
MK_INCLUDES += $(TESTS_PARSER_DIR)/../../parser/src/pss-parser.mk
endif

PARSER_TESTS_SRC = $(notdir $(wildcard $(TESTS_PARSER_DIR)/src/*.cpp))
SRC_DIRS += $(TESTS_PARSER_DIR)/src

CFLAGS += $(foreach dir,$(SRC_DIRS),-I$(dir))
CXXFLAGS += $(foreach dir,$(SRC_DIRS),-I$(dir))
CXXFLAGS += -std=c++11
CXXFLAGS += -DTESTS_DATA_DIR='"$(TESTS_DATA_DIR)"'

include $(MK_INCLUDES)

RULES := 1

vpath %.cpp $(SRC_DIRS)
vpath %.cc $(SRC_DIRS)
vpath %.c $(SRC_DIRS)

all : $(BUILD_DIR)/googletest.unpack
	$(MAKE) -C $(BUILD_DIR) -f $(TESTS_PARSER_DIR)/Makefile BUILD=true \
		-j$(NUM_CORES) TESTS_PARSER_DIR=$(TESTS_PARSER_DIR) __run

__run: unittest$(EXEEXT)
	./unittest$(EXEEXT)

unittest$(EXEEXT) : $(PARSER_TESTS_SRC:.cpp=.o) \
		libpss_parser.a libantlr_runtime.a \
		gtest-all.o gtest_main.o
	$(Q)$(CXX) -o $@ $^ -lpthread -static-libstdc++

clean :
	rm -rf $(BUILD_DIR)

include $(MK_INCLUDES)


