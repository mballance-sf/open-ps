
SCRIPTS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
OPEN_PS_DIR:=$(abspath $(SCRIPTS_DIR)/..)
PACKAGES_DIR := $(OPEN_PS_DIR)/packages

uname_o := $(shell uname -o)

ifneq (1,$(RULES))

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

ifeq (true,$(IS_WIN))
	ifeq (,$(COMPILER))
#		COMPILER := cl
		COMPILER := gcc
	endif
else
	COMPILER := gcc
endif

ifeq (cl, $(COMPILER))
else
endif

CXXFLAGS += $(foreach d,$(SRC_DIRS),-I $(d))

ifeq (cl,$(COMPILER))
	CXX=cl
	CC=cl
	CXXFLAGS += -std:c++14 -nologo -EHsc
	LINK_EXE=link -nologo -out:$@
	DLIBPREF := 
endif

ifeq (gcc,$(COMPILER))
	GCC_VERSION := $(shell gcc --version | grep gcc | sed -e 's/^.*\([0-9]\.[0-9]\.[0-9]\).*$$/\1/g')
	GCC_VERSION_MAJOR := $(shell echo $(GCC_VERSION) | sed -e 's/^\([0-9]\).*$$/\1/g')
	GCC_VERSION_GE_7 := $(shell test $(GCC_VERSION_MAJOR) -ge 7; echo $$?)

ifeq (0,$(GCC_VERSION_GE_7))
	CXXFLAGS += -Wno-attributes -fpermissive
endif

	DLIBPREF := lib
	CXXFLAGS += -std=c++11 -fPIC -g
	LINK_EXE=$(CXX) -o $@
define LINK_DLIB
	$(Q)$(CXX) -shared -o $@ $(filter %.o,$(^)) \
		$(foreach lib,$(filter %$(DLIBEXT),$(^)),-L$(dir $(lib)) -l$(subst $(DLIBPREF),,$(subst $(DLIBEXT),,$(notdir $(lib)))))
endef
define LINK_EXE
	$(Q)$(CXX) -o $@ $(filter %.o,$(^)) \
		$(foreach lib,$(filter %$(DLIBEXT),$(^)),-L$(dir $(lib)) -l$(subst $(DLIBPREF),,$(subst $(DLIBEXT),,$(notdir $(lib)))))
endef
endif

ifeq (true,$(VERBOSE))
  UNZIP:=unzip -o
  UNTARGZ:=tar xvzf
  WGET:=wget
else 
  UNZIP:=unzip -oq
  UNTARGZ:=tar xzf
  WGET:=wget
  Q:=@
endif

else # Rules

ifeq (cl,$(COMPILER))
%.o : %.cpp
	$(Q)$(CXX) -c -Fo$(@) $(CXXFLAGS) $^
endif

ifeq (gcc,$(COMPILER))
ifeq (true,$(VERBOSE))
%.o : %.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $^
	
%.o : %.cc
	$(CXX) -c -o $@ $(CXXFLAGS) $^
else
%.o : %.cpp
	@echo "CXX: $(notdir $^)"
	@$(CXX) -c -o $@ $(CXXFLAGS) $^
	
%.o : %.cc
	@echo "CXX: $(notdir $^)"
	@$(CXX) -c -o $@ $(CXXFLAGS) $^
endif
endif

endif # Rules

