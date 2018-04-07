
PSS_PARSER_SRC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
PSS_PARSER_SRC_DIR := $(shell cd $(PSS_PARSER_SRC_DIR) ; pwd)
PSS_PARSER_SCRIPTS_DIR := $(PSS_PARSER_SRC_DIR)/../scripts

ANTLR_DOWNLOAD_URL:=http://www.antlr.org/download
ANTLR_VERSION:=4.7.1
# http://www.antlr.org/download/antlr4-cpp-runtime-4.7.1-source.zip
ANTLR_RUNTIME_SRC_ZIP:=antlr4-cpp-runtime-$(ANTLR_VERSION)-source.zip
ANTLR_RUNTIME_SRC_URL:=$(ANTLR_DOWNLOAD_URL)/$(ANTLR_RUNTIME_SRC_ZIP)
ANTLR_RUNTIME_WIN_ZIP:=antlr4-cpp-runtime-4.7.1-vs2015.zip
ANTLR_RUNTIME_WIN_URL:=$(ANTLR_DOWNLOAD_URL)/$(ANTLR_RUNTIME_WIN_ZIP)
ANTLR_JAR:=antlr-$(ANTLR_VERSION)-complete.jar
ANTLR_JAR_URL:=$(ANTLR_DOWNLOAD_URL)/$(ANTLR_JAR)

PSS_PARSER_SRC = $(notdir $(wildcard $(PSS_PARSER_SRC_DIR)/*.cpp))

ifeq (,$(UNZIP))
UNZIP := unzip -o
endif

ifneq (1, $(RULES))

ifeq (true,$(BUILD))
-include grammar/src.mk
-include antlr4-cpp-runtime/src.mk
endif

ANTLR4_CPP_RUNTIME_DIR=antlr4-cpp-runtime/runtime/src
ANTLR4_RUNTIME_LINK=antlr/libantlr_runtime.a
ANTLR4_RUNTIME_DEPS=antlr/libantlr_runtime.a

CXXFLAGS += -DANTLR4CPP_EXPORTS
SRC_DIRS += $(PSS_PARSER_SRC_DIR)
SRC_DIRS += $(ANTLR4_CPP_RUNTIME_DIR) $(ANTLR4_CPP_RUNTIME_DIR)/atn 
SRC_DIRS += $(ANTLR4_CPP_RUNTIME_DIR)/dfa $(ANTLR4_CPP_RUNTIME_DIR)/support 
SRC_DIRS += $(ANTLR4_CPP_RUNTIME_DIR)/tree $(ANTLR4_CPP_RUNTIME_DIR)/misc
SRC_DIRS += $(ANTLR4_CPP_RUNTIME_DIR)/tree/pattern 
SRC_DIRS += $(ANTLR4_CPP_RUNTIME_DIR)/tree/xpath 
SRC_DIRS += grammar

PSS_PARSER_DEPS = $(DLIBPREF)pss_parser$(DLIBEXT) 
ANTLR4_DEPS = antlr/$(DLIBPREF)antlr_runtime$(DLIBEXT)
LIB_TARGETS += $(PSS_PARSER_DEPS) $(ANTLR4_DEPS)
UNPACK_TARGETS += $(BUILD_DIR)/runtime.unpack

ifeq (gcc,$(COMPILER))
ANTLR4_LINK = -Lantlr -lantlr_runtime
PSS_PARSER_LINK = -L. -lpss_parser
else
ANTLR4_LINK = -lantlr/antlr_runtime
PSS_PARSER_LINK = -lpss_parser
endif

else # Rules

$(DLIBPREF)pss_parser$(DLIBEXT) : $(PSS_GRAMMAR_SRC:.cpp=.o) $(ANTLR4_DEPS)
	$(Q)$(LINK_DLIB)

antlr/$(DLIBPREF)antlr_runtime$(DLIBEXT) : $(foreach o,$(ANTLR_RT_SRC:.cpp=.o),antlr/$(o))
	$(Q)$(LINK_DLIB) 

ifeq (cl,$(COMPILER))
antlr/%.o : %.cpp
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(CXX) -Fo$(@) $(CXXFLAGS) $^
else
ifeq (true,$(VERBOSE))
antlr/%.o : %.cpp
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(CXX) -c -o $@ $(CXXFLAGS) $^
else
antlr/%.o : %.cpp
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)echo "CXX $(notdir $^)"
	$(Q)$(CXX) -c -o $@ $(CXXFLAGS) $^
endif
endif


$(BUILD_DIR)/runtime.unpack : $(PACKAGES_DIR)/$(ANTLR_RUNTIME_SRC_ZIP)
	$(Q)if test ! -d $(BUILD_DIR); then mkdir -p $(BUILD_DIR); fi
	$(Q)cd $(BUILD_DIR); rm -rf antlr4-cpp-runtime
	$(Q)cd $(BUILD_DIR); mkdir antlr4-cpp-runtime
	$(Q)cd $(BUILD_DIR)/antlr4-cpp-runtime ; $(UNZIP) $^
	$(Q)cd $(BUILD_DIR)/antlr4-cpp-runtime ; patch -p2 < $(PSS_PARSER_SRC_DIR)/Any.h.patch
	$(Q)touch $@

pss-grammar.gen : $(PSS_PARSER_SRC_DIR)/PSS.g4 $(PACKAGES_DIR)/$(ANTLR_JAR)
	$(Q)mkdir -p grammar
	$(Q)java -jar $(PACKAGES_DIR)/$(ANTLR_JAR) \
		-Dlanguage=Cpp -visitor -o grammar $(PSS_PARSER_SRC_DIR)/PSS.g4
	$(Q)touch $@

grammar/src.mk : pss-grammar.gen 
	$(Q)echo 'PSS_GRAMMAR_SRC += $$(notdir $$(wildcard grammar/*.cpp))' > $@
	
antlr4-cpp-runtime/src.mk : runtime.unpack
	$(Q)echo 'ANTLR_RT_SRC += $$(notdir $$(wildcard antlr4-cpp-runtime/runtime/src/*.cpp))' > $@
	$(Q)echo 'ANTLR_RT_SRC += $$(notdir $$(wildcard antlr4-cpp-runtime/runtime/src/atn/*.cpp))' >> $@
	$(Q)echo 'ANTLR_RT_SRC += $$(notdir $$(wildcard antlr4-cpp-runtime/runtime/src/dfa/*.cpp))' >> $@
	$(Q)echo 'ANTLR_RT_SRC += $$(notdir $$(wildcard antlr4-cpp-runtime/runtime/src/support/*.cpp))' >> $@
	$(Q)echo 'ANTLR_RT_SRC += $$(notdir $$(wildcard antlr4-cpp-runtime/runtime/src/tree/*.cpp))' >> $@
	$(Q)echo 'ANTLR_RT_SRC += $$(notdir $$(wildcard antlr4-cpp-runtime/runtime/src/tree/pattern/*.cpp))' >> $@
	$(Q)echo 'ANTLR_RT_SRC += $$(notdir $$(wildcard antlr4-cpp-runtime/runtime/src/tree/xpath/*.cpp))' >> $@
	$(Q)echo 'ANTLR_RT_SRC += $$(notdir $$(wildcard antlr4-cpp-runtime/runtime/src/misc/*.cpp))' >> $@

$(PACKAGES_DIR)/$(ANTLR_JAR) : 
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(WGET) -O $@ $(ANTLR_JAR_URL)

$(PACKAGES_DIR)/$(ANTLR_RUNTIME_SRC_ZIP) : 
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(WGET) -O $@ $(ANTLR_RUNTIME_SRC_URL)
	
$(PACKAGES_DIR)/$(ANTLR_RUNTIME_WIN_ZIP) : 
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(WGET) -O $@ $(ANTLR_RUNTIME_WIN_URL)

endif # End Rules

