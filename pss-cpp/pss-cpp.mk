
PSS_CPP_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
PORTABLE_STIMULUS_TGZ := $(PSS_CPP_DIR)/../packages/portable-stimulus.tar.gz

ifneq (1,$(RULES))

PSS_CPP_SRC += $(wildcard $(PSS_CPP_DIR)/src/impl/*.cpp) 

PSS_CPP_OBJS = $(notdir $(PSS_CPP_SRC:.cpp=.o))

PSS_CPP_SRC_DEPS += $(wildcard $(PSS_CPP_DIR)/src/impl/*.cpp)
PSS_CPP_SRC_DEPS += $(wildcard $(PSS_CPP_DIR)/src/impl/*.h)

SRC_DIRS += $(PSS_CPP_DIR)/src
SRC_DIRS += $(PSS_CPP_DIR)/src/impl
CXXFLAGS += -Ipss-headers

LIB_TARGETS += $(DLIBPREF)pss-cpp$(DLIBEXT) pss_sys_test.o

PSS_SYS_TEST_SRC = $(PSS_CPP_DIR)/src/pss_sys_test.cpp

else # Rules

$(PSS_SYS_TEST_SRC) : pss-headers.d $(PSS_CPP_SRC_DEPS)

$(PSS_CPP_SRC) : pss-headers.d

$(DLIBPREF)pss-cpp$(DLIBEXT) : $(PSS_CPP_OBJS)
	$(LINK_DLIB)

portable-stimulus.d : $(PORTABLE_STIMULUS_TGZ)
	$(UNTARGZ) $^
	$(Q)touch $@

pss-headers.d : portable-stimulus.d
	$(Q)rm -rf pss-headers
	$(Q)mkdir -p pss-headers/pss
	$(Q)cd pss-headers/pss ; for header in ../../portable-stimulus/pss-cpp/include/pss/*.h; do \
		perl $(PSS_CPP_DIR)/cpp-filter.pl $$header `basename $$header`; \
	done
	$(Q)cp portable-stimulus/pss-cpp/include/pss.h pss-headers
	$(Q)rm -rf portable-stimulus
	$(Q)touch $@

endif
