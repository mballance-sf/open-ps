
TESTS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))
GOOGLETEST_VERSION=1.8.0
GOOGLETEST_DIR=googletest-release-$(GOOGLETEST_VERSION)
GOOGLETEST_ZIP=$(GOOGLETEST_DIR).tar.gz
GOOGLETEST_URL=https://github.com/google/googletest/archive/release-$(GOOGLETEST_VERSION).tar.gz

SRC_DIRS += $(BUILD_DIR)/$(GOOGLETEST_DIR)/googletest
SRC_DIRS += $(BUILD_DIR)/$(GOOGLETEST_DIR)/googletest/src
SRC_DIRS += $(BUILD_DIR)/$(GOOGLETEST_DIR)/googletest/include

else # Rules

$(PACKAGES_DIR)/$(GOOGLETEST_ZIP) :
	if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	cd $(PACKAGES_DIR) ; wget -O $(GOOGLETEST_ZIP) $(GOOGLETEST_URL)

$(BUILD_DIR)/googletest.unpack : $(PACKAGES_DIR)/$(GOOGLETEST_ZIP)
	if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	cd $(BUILD_DIR) ; tar xvzf $^
	touch $@

endif


