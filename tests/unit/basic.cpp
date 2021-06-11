#include <tst/set.hpp>
#include <tst/check.hpp>

#include <utki/string.hpp>

#include <nitki/semaphore.hpp>

#include "../../src/easyhttp/request.hpp"
#include "../../src/easyhttp/init_guard.hpp"

namespace{
tst::set set("basic", [](tst::suite& suite){
    suite.add(
        "basic_request",
        [](){
            nitki::semaphore completed;

            std::vector<uint8_t> data;

            easyhttp::status_code req_status = easyhttp::status_code::network_error;

            auto r = std::make_shared<easyhttp::request>([&](easyhttp::request& r){
                // this callback is run from separate thread, so do not use of tst::check() inside
                req_status = r.get_response().status;
                data = std::move(r.get_response().body);
                completed.signal();
            });

            r->set_url("http://izdelie.icu");

            r->start();

            completed.wait();

            tst::check(req_status == easyhttp::status_code::ok, SL) << "status = " << unsigned(req_status);

            LOG([&](auto&o){o << "body.size() = " << data.size() << std::endl;});
            LOG([&](auto&o){o << utki::make_string(data) << std::endl;});
        }
    );
});
}
