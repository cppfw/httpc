include prorab.mk
include prorab-test.mk

$(eval $(call prorab-config, ../../config))

this_name := tests

this_srcs += $(call prorab-src-dir, .)

this_cxxflags += -I$(d)../../curl-7.70.0/include

this_ldlibs += -lhttpc -lnitki -lutki -ltst -lgnutls
this_ldflags += -L$(d)../../src/out/$(c)

this_no_install := true

$(eval $(prorab-build-app))

$(eval $(call prorab-depend, $(prorab_this_name), ../../src/out/$(c)/libhttpc$(dot_so)))

this_test_cmd := $(prorab_this_name) --jobs=auto --junit-out=junit.xml 
this_test_deps := $(prorab_this_name)
this_test_ld_path := ../../src/out/$(c) $(CONAN_LIB_DIRS)
$(eval $(prorab-test))

$(eval $(call prorab-include, ../../src/makefile))
