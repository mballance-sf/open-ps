
ifneq (1,$(RULES))

QPSSC_SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))
QPS_DIR := $(shell cd $(QPSSC_SRCDIR)/../.. ; pwd)

PSI_DIR := $(QPS_DIR)/psstools/psi

QPSSC_SRC := \
  $(notdir $(wildcard $(QPSSC_SRCDIR)/*.cpp)) Engine.cpp
  
ifeq (,$(QPSRT_SRCDIR))
QPSRT_SRCDIR := $(QPSSC_SRCDIR)/../qpsrt/src
endif

SRC_DIRS += $(QPSSC_SRCDIR)
SRC_DIRS += $(QPSRT_SRCDIR)

CXXFLAGS += -I$(PSI_DIR)/contrib/api_impl
CXXFLAGS += -I$(PSI_DIR)/contrib/apps
CXXFLAGS += -I$(QPS_SRC)/PSS/src 
CXXFLAGS += -I$(QPS_SRC)/utilities/vectors
CXXFLAGS += -I$(QPS_SRC)/utilities/include
CXXFLAGS += -I$(QPS_SRC)/utilities/heaps
CXXFLAGS += -I$(QPS_SRC)/utilities/assert
CXXFLAGS += -I$(QPS_SRC)/utilities/lists
CXXFLAGS += -I$(QPS_SRC)/utilities/bigUnsignedInt
CXXFLAGS += -I$(QPS_SRC)/utilities/report
CXXFLAGS += -I$(BUILD_DIR)

QPSRT_CXXFLAGS += -I$(INFACT_HOME)/include 
QPSRT_CXXFLAGS += -I$(INFACT_SRC)/automataRunTime/src
QPSRT_CXXFLAGS += -I$(INFACT_SRC)/automataCommon/src
QPSRT_CXXFLAGS += -I$(INFACT_SRC)/sim/src
QPSRT_CXXFLAGS += -I$(INFACT_SRC)/tid/src
QPSRT_CXXFLAGS += $(foreach d,$(filter-out %.in %.S,$(wildcard $(INFACT_SRC)/utilities/*)),-I$(d))
QPSRT_CXXFLAGS += $(foreach d,$(filter-out %.in %.S,$(wildcard $(QPS_SRC)/utilities/*)),-I$(d))
QPSRT_CXXFLAGS += -DNDEBUG

LIB_TARGETS += $(BUILD_DIR)/libpssc.a

else # Rules

$(BUILD_DIR)/libpssc.a : \
	$(foreach o,$(QPSSC_SRC:.cpp=.o),$(BUILD_DIR)/$(o)) \
	$(BUILD_DIR)/PSS.tab.o $(BUILD_DIR)/PSS.flex.o
	$(Q)rm -f $@
	$(Q)$(AR) cq $@ $^
	
$(BUILD_DIR)/%.o : $(QPSSC_SRCDIR)/%.cpp
	$(Q)$(CXX) -c $(CXXFLAGS) -o $@ $^
	
$(BUILD_DIR)/%.o : $(QPSSC_SRCDIR)/../../qpsrt/src/%.cpp
	$(Q)$(CXX) -c $(CXXFLAGS) $(QPSRT_CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o : $(BUILD_DIR)/%.c
	$(Q)$(CXX) -c $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.tab.h : $(BUILD_DIR)/%.tab.c

$(BUILD_DIR)/%.tab.o : $(BUILD_DIR)/%.flex.c

$(BUILD_DIR)/%.flex.h : $(BUILD_DIR)/%.flex.c

$(BUILD_DIR)/%.tab.c : $(QPS_SRC)/PSS/src/%.bison $(BUILD_DIR)/%.flex.c
	$(Q)cd $(BUILD_DIR) ; bison $(QPS_SRC)/PSS/src/$*.bison

$(BUILD_DIR)/%.flex.c : $(QPS_SRC)/PSS/src/%.flex 
	$(Q)cd $(BUILD_DIR) ; flex $(QPS_SRC)/PSS/src/$*.flex 

endif
