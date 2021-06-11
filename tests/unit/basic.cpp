#include <tst/set.hpp>
#include <tst/check.hpp>

#include <utki/string.hpp>

#include <nitki/semaphore.hpp>

#include <thread>

#include "../../src/easyhttp/request.hpp"
#include "../../src/easyhttp/init_guard.hpp"

namespace{
tst::set set("basic", [](tst::suite& suite){
    suite.add(
        "basic_request",
        [](){
            nitki::semaphore completed;

            std::vector<uint8_t> data;

            easyhttp::status_code req_status = easyhttp::status_code::undefined;

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

    suite.add(
        "cancel_non_active_request",
        [](){
            auto r = std::make_shared<easyhttp::request>([](easyhttp::request& r){});

            r->set_url("http://izdelie.icu");

            tst::check(!r->cancel(), SL);
        }
    );

    suite.add(
        "cancel_active_request",
        [](){
            bool completed = false;

            nitki::semaphore sema;

            easyhttp::status_code req_status = easyhttp::status_code::undefined;

            auto r = std::make_shared<easyhttp::request>([&](easyhttp::request& req){
                completed = true;
                auto r = req.get_response();
                LOG([&](auto&o){
                    o << "HTTP request completed, status = " << unsigned(r.status);
                    o << ", code = " << unsigned(r.response_code) << std::endl;
                });
                req_status = r.status;
                sema.signal();
            });

            r->set_url("https://speed.hetzner.de/1GB.bin");

            bool sema_signalled = false;

            r->set_data_handler([&sema, &sema_signalled](utki::span<const uint8_t> d) -> size_t {
                LOG([&](auto&o){
                    o << "first data chunk received, "; << d.size() << " bytes, cancelling the request..." << std::endl;
                    o << utki::make_string(d) << std::endl;
                });
                if(!sema_signalled){
                    sema_signalled = true;
                    sema.signal();
                }
                return d.size();
            });

            LOG([](auot&o){o << "running cancel test..." << std::endl;});
            r->start();

            sema.wait();

            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            tst::check(!completed, SL);
            tst::check(r->cancel(), SL);
            tst::check(!completed, SL);

            sema_signalled = false; // allow signalling on data received

            LOG([](auto&o){o << "waiting 1 second to check the request was cancelled and no more data is received" << std::endl;});
            auto wait_res = sema.wait(1000);
            tst::check(!wait_res, SL);
            LOG([](auto&o){o << "cancel test completed" << std::endl;});

            // since we are cancelling the requet, we expect that request completed handler was not called,
            // so the request status should remain undefined
            tst::check(req_status == easyhttp::status_code::undefined, SL) << "req_status = " << unsigned(req_status);
        }
    );
});
}
