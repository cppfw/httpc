include prorab.mk
include prorab-license.mk
include prorab-install-dbgsrc.mk
include prorab-clang-format.mk

$(eval $(call prorab-config, ../config))

this_src_dir := httpclient

this_name := $(this_src_dir)$(this_dbg)

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir, $(this_src_dir))

this_ldlibs += -l nitki$(this_dbg)
this_ldlibs += -l utki$(this_dbg)

this_ldlibs += -l curl

ifeq ($(os),linux)
    this_ldlibs += -l ssl
endif

$(eval $(prorab-build-lib))

this_license_file := ../LICENSE
$(eval $(prorab-license))

$(eval $(prorab-install-dbgsrc))

$(eval $(prorab-clang-format))
