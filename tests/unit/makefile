include prorab.mk
include prorab-test.mk

$(eval $(call prorab-config, ../../config))

this_name := tests

this_srcs += $(call prorab-src-dir, .)

this_ldlibs += -lnitki
this_ldlibs += -lutki
this_ldlibs += -ltst

this_ldlibs += ../../src/out/$(c)/libhttpc$(this_dbg)$(dot_so)

this_no_install := true

$(eval $(prorab-build-app))

this_test_cmd := $(prorab_this_name) --jobs=auto --junit-out=out/$(c)/junit.xml 
this_test_deps := $(prorab_this_name)
this_test_ld_path := ../../src/out/$(c)
ifeq ($(os),macosx)
    this_test_ld_path += /usr/local/opt/curl/lib
endif
$(eval $(prorab-test))

$(eval $(call prorab-include, ../../src/makefile))
