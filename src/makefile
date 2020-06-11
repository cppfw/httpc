include prorab.mk

debug := false

this_name := easyhttp

this_out_dir := build

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir,.)

ifeq ($(debug),true)
    this_cxxflags += -DDEBUG
    this_cxxflags += -O0
else
    this_cxxflags += -O3
endif

this_cxxflags += -Wall # enable all warnings
this_cxxflags += -Wno-comment # no warnings on nested comments
this_cxxflags += -funsigned-char # the 'char' type is unsigned
this_cxxflags += -Wnon-virtual-dtor # warn if base class has non-virtual destructor
this_cxxflags += -fno-operator-names # do not treat 'and', 'bitand','bitor','compl','not','or','xor' as keywords
this_cxxflags += -Werror # treat warnings as errors
this_cxxflags += -Wfatal-errors # stop on first error encountered
this_cxxflags += -fstrict-aliasing # in order to comply with the c++ standard more strictly
this_cxxflags += -std=c++14
this_cxxflags += -g

this_ldlibs += -lnitki -lstdc++

this_libcurl_a := $(abspath $(d)../curl-7.70.0/lib/.libs/libcurl.a)

ifeq ($(os),linux)
    this_cxxflags += -fPIC # Since we are building shared library, we need Position-Independend Code
    this_cxxflags += -I$(d)../curl-7.70.0/include
    this_ldlibs += $(this_libcurl_a) -lgnutls -lz
else ifeq ($(os),windows)
    this_ldlibs += -lcurl
else ifeq ($(os),macosx)
    this_cxxflags += -stdlib=libc++ # this is needed to be able to use c++11 std lib
    this_cxxflags += -I$(d)../curl-7.70.0/include
    this_ldlibs += $(this_libcurl_a)
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
