
XML_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

LIBXML2_WIN32_VERSION=2.7.8
LIBXML2_WIN64_VERSION=2.9.1
LIBXML2_SRC_VERSION=2.7.8
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
XML_SRC=$(notdir $(wildcard $(XML_SRC_DIR)/src/*.cpp))

SRC_DIRS += $(XML_DIR)/src

XML_DEPS := $(DLIBPREF)pss_xml$(DLIBEXT)

ifeq (gcc,$(COMPILER))
XML_LINK := -L. -lpss_xml $(LIBXML2_LINK)
else
XML_LINK := -L. -lpss_xml $(LIBXML2_LINK)
endif

else # Rules

$(DLIBPREF)pss_xml$(DLIBEXT) : $(XML_SRC:.cpp=.o)
	$(Q)$(LINK_DLIB)

endif
