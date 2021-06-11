#include <tst/application.hpp>

#include "../../src/easyhttp/init_guard.hpp"

namespace{
class application : public tst::application{
    easyhttp::init_guard easyhttp_init_guard;
public:
    application() :
            tst::application("unit tests", "unit tests implemented using tst testing framewrok")
    {}
};
}

tst::application_factory app_fac([](){
    return std::make_unique<application>();
});
