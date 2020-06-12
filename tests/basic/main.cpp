#include "../../src/easyhttp/init_guard.hpp"

#include <thread>

#include <utki/debug.hpp>
#include <utki/string.hpp>
#include <nitki/semaphore.hpp>

int main(int argc, char** argv){
	// test basic request
	{
		nitki::semaphore completed;

		easyhttp::init_guard easyhttp_guard;

		std::vector<uint8_t> data;

		auto r = std::make_shared<easyhttp::request>([&completed, &data](easyhttp::request& r){
			ASSERT_INFO_ALWAYS(r.get_response().status == easyhttp::status_code::ok, "status code is not OK: " << unsigned(r.get_response().status))
			data = std::move(r.get_response().body);
			completed.signal();
		});

		r->set_url("http://izdelie.icu");

		r->start();

		completed.wait();

		TRACE(<< "body.size() = " << data.size() << std::endl)
		TRACE(<< utki::make_string(data) << std::endl)
	}

	// test cancelling non-active request
	{
		easyhttp::init_guard easyhttp_guard;

		auto r = std::make_shared<easyhttp::request>([](easyhttp::request& r){

		});

		r->set_url("http://izdelie.icu");

		ASSERT_ALWAYS(!r->cancel())
	}

	// test cancelling active request
	{
		easyhttp::init_guard easyhttp_guard;

		bool completed = false;

		auto r = std::make_shared<easyhttp::request>([&completed](easyhttp::request& req){
			completed = true;
			auto r = req.get_response();
			TRACE(<< "HTTP request completed, status = " << unsigned(r.status) << ", code = " << unsigned(r.response_code) << std::endl)
		});

		r->set_url("https://speed.hetzner.de/1GB.bin");

		nitki::semaphore sema;

		bool sema_signalled = false;

		r->set_data_handler([&sema, &sema_signalled](utki::span<const uint8_t> d) -> size_t {
			TRACE_ALWAYS(<< "first data chunk received, " << d.size() << " bytes, cancelling the request..." << std::endl)
			TRACE(<< utki::make_string(d) << std::endl)
			if(!sema_signalled){
				sema_signalled = true;
				sema.signal();
			}
			return d.size();
		});

		TRACE_ALWAYS(<< "running cancel test..." << std::endl)
		r->start();

		sema.wait();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		ASSERT_ALWAYS(!completed)
		ASSERT_ALWAYS(r->cancel())
		ASSERT_ALWAYS(!completed)

		sema_signalled = false; // allow signalling on data received

		TRACE_ALWAYS(<< "waiting 1 second to check the request was cancelled and no more data is received" << std::endl)
		ASSERT_ALWAYS(!sema.wait(1000))
		TRACE_ALWAYS(<< "cancel test completed" << std::endl)
	}

	return 0;
}
