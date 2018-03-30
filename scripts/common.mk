
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
#	ifeq (,$(COMPILER))
#		COMPILER := cl
#	endif
	COMPILER := gcc
else
	COMPILER := gcc
endif

CXXFLAGS += $(foreach d,$(SRC_DIRS),-I $(d))

ifeq (cl,$(COMPILER))
	CXX=cl
	CC=cl
	CXXFLAGS += -std:c++14 -nologo
endif

ifeq (gcc,$(COMPILER))
	CXXFLAGS += -std=c++11 -fPIC -g
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
	$(Q)$(CXX) -Fo$(@) $(CXXFLAGS) $^
endif

endif

