include prorab.mk

$(eval $(call prorab-config, ../../config))

this_name := tests

this_srcs += $(call prorab-src-dir, .)

this_cxxflags += -I$(d)../../curl-7.70.0/include

this_ldlibs += -leasyhttp -lnitki -lutki -ltst
this_ldflags += -L$(d)../../src/out/$(c)

this_no_install := true

$(eval $(prorab-build-app))

include $(d)../test_target.mk

# add dependency on libeasyhttp
$(prorab_this_name): $(abspath $(d)../../src/out/$(c)/libeasyhttp$(dot_so))

$(eval $(call prorab-include, ../../src/makefile))
