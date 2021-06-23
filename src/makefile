include prorab.mk

$(eval $(call prorab-config, ../config))

this_name := httpc

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir,.)

this_ldlibs += -lnitki

this_libcurl_a := $(abspath $(d)../curl-7.70.0/lib/.libs/libcurl.a)

ifeq ($(os),$(filter $(os),linux macosx))
    this_cxxflags += -I$(d)../curl-7.70.0/include
    this_ldlibs += $(this_libcurl_a) -lssl -lgnutls -lz
else ifeq ($(os),windows)
    this_ldlibs += -lcurl
endif

$(eval $(prorab-build-lib))

ifneq ($(filter $(os),linux)$(filter $(os),macosx),)

# add dependency on libcurl.a
$(prorab_this_name): $(this_libcurl_a)

define this_rules
$(this_libcurl_a):
$(.RECIPEPREFIX)$(a)(cd $(d)../curl-7.70.0 && ./configure --with-gnutls --without-libidn2)
$(.RECIPEPREFIX)$(a)$(MAKE) -C $(d)../curl-7.70.0
endef
$(eval $(this_rules))

endif
