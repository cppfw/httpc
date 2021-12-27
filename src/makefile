include prorab.mk
include prorab-license.mk

$(eval $(call prorab-config, ../config))

this_name := httpc

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir,.)

this_ldlibs += -lnitki

this_ldlibs += -lcurl

ifeq ($(os),linux)
    this_ldlibs += -lssl
endif

$(eval $(prorab-build-lib))

this_src_dir := 
this_license_file := ../LICENSE
$(eval $(prorab-license))
