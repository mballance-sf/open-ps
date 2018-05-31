
XML_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

LIBXML2_WIN32_VERSION=2.7.8
LIBXML2_WIN64_VERSION=2.9.1
LIBXML2_SRC_VERSION=2.7.8
LIBXML2_SRC_DIR=libxml2-$(LIBXML2_SRC_VERSION)
LIBXML2_WIN32_URL=ftp://ftp.zlatkovic.com/libxml/libxml2-$(LIBXML2_WIN32_VERSION).win32.zip
LIBXML2_WIN64_URL=ftp://ftp.zlatkovic.com/libxml/64bit/libxml2-$(LIBXML2_WIN64_VERSION).win32-x86_64.7z
LIBXML2_SRC_URL=ftp://xmlsoft.org/libxml2/libxml2-sources-$(LIBXML2_SRC_VERSION).tar.gz
ZLIB_WIN32_VERSION=1.2.5
ZLIB_WIN64_VERSION=1.2.8
ZLIB_SRC_VERSION=1.2.11
ZLIB_WIN32_DIR=zlib-$(ZLIB_WIN32_VERSION).win32.zip
ZLIB_WIN64_DIR=zlib-$(ZLIB_WIN64_VERSION).win32-x86_64
ZLIB_SRC_DIR=zlib-$(ZLIB_SRC_VERSION)
ZLIB_WIN32_URL=ftp://xmlsoft.org/libxml2/win32/$(ZLIB_WIN32_DIR).zip
ZLIB_WIN64_URL=ftp://xmlsoft.org/libxml2/win32/64bit/$(ZLIB_WIN64_DIR).7z
ZLIB_SRC_URL=http://www.zlib.net/zlib-$(ZLIB_SRC_VERSION).tar.gz

ifneq (1,$(RULES))

MODEL_EXPORT_HEADERS=$(notdir $(wildcard $(MODEL_DIR)/src/I*.h))

XML_SRC_FULL=$(wildcard $(XML_DIR)/src/*.cpp)
XML_SRC=$(notdir $(XML_SRC_FULL))

SRC_DIRS += $(XML_DIR)/src

LIBXML2_DEPS = $(PLATFORM_LIB_DIR)/$(DLIBPREF)xml2$(DLIBEXT)
LIBZLIB_DEPS = $(PLATFORM_LIB_DIR)/$(DLIBPREF)z$(DLIBEXT)

XML_DEPS += $(PLATFORM_LIB_DIR)/$(DLIBPREF)pss_xml$(DLIBEXT) $(LIBXML2_DEPS) $(LIBZLIB_DEPS)

ifeq (true,$(IS_WIN))
ifeq (gcc,$(COMPILER))
XML_LINK := -L. -lpss_xml $(LIBXML2_LINK)
else
XML_LINK := -L. -lpss_xml $(LIBXML2_LINK)
endif

else # Linux

#XML_DEPS = $(XML_SRC:.cpp=.o) $(LIBXML2_SRC_DIR)/output/lib/libxml2.so $(ZLIB_SRC_DIR)/output/lib/libzlib.so
CXXFLAGS += -I$(LIBXML2_SRC_DIR)/output/include -I$(ZLIB_SRC_DIR)/output/include


endif

else # Rules

$(PLATFORM_LIB_DIR)/$(DLIBPREF)pss_xml$(DLIBEXT) : \
		$(XML_SRC:.cpp=.o) \
		$(MODEL_DEPS) \
		$(LIBXML2_DEPS) \
		$(LIBZLIB_DEPS)
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(LINK_DLIB)

ifeq (true,$(IS_WIN))
else # Linux

#$(LIBXML2_SRC_DIR)/output/lib/libxml2.so : libxml2.d

#$(ZLIB_SRC_DIR)/output/lib/libzlib.so : zlib.d

$(XML_SRC_FULL) : $(LIBXML2_DEPS) $(LIBZLIB_DEPS)

$(PLATFORM_LIB_DIR)/$(DLIBPREF)xml2$(DLIBEXT) : libxml2.d
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)cp $(LIBXML2_SRC_DIR)/output/lib/libxml2.so $@

libxml2.d : $(PACKAGES_DIR)/libxml2-sources-$(LIBXML2_SRC_VERSION).tar.gz
	$(Q)rm -rf $(LIBXML2_SRC_DIR)
	$(Q)$(UNTARGZ) $^
	$(Q)cd $(LIBXML2_SRC_DIR) ; ./configure --prefix=`pwd`/output
	$(Q)cd $(LIBXML2_SRC_DIR) ; $(MAKE)
	$(Q)cd $(LIBXML2_SRC_DIR) ; $(MAKE) install
	$(Q)touch $@
	
$(PLATFORM_LIB_DIR)/$(DLIBPREF)z$(DLIBEXT) : zlib.d
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)cd $(ZLIB_SRC_DIR)/output/lib ; tar cf - $(DLIBPREF)z$(DLIBEXT)* | \
		(cd `dirname $@` ; tar xvf -)

$(PACKAGES_DIR)/libxml2-sources-$(LIBXML2_SRC_VERSION).tar.gz :
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(WGET) -O $@ $(LIBXML2_SRC_URL)
	
zlib.d : $(PACKAGES_DIR)/$(ZLIB_SRC_DIR).tar.gz
	$(Q)rm -rf $(ZLIB_SRC_DIR)
	$(Q)$(UNTARGZ) $^
	$(Q)cd $(ZLIB_SRC_DIR) ; ./configure --prefix=`pwd`/output
	$(Q)cd $(ZLIB_SRC_DIR) ; $(MAKE)
	$(Q)cd $(ZLIB_SRC_DIR) ; $(MAKE) install
	$(Q)touch $@

$(PACKAGES_DIR)/$(ZLIB_SRC_DIR).tar.gz :
	$(Q)if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(Q)$(WGET) -O $@ $(ZLIB_SRC_URL)

endif	
	

endif
