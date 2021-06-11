#include "../../src/easyhttp/init_guard.hpp"

#include <thread>

#include <utki/debug.hpp>
#include <utki/string.hpp>
#include <nitki/semaphore.hpp>

int main(int argc, char** argv){

	// test cancelling active request
	{
		easyhttp::init_guard easyhttp_guard;

		bool completed = false;

		nitki::semaphore sema;

		auto r = std::make_shared<easyhttp::request>([&completed, &sema](easyhttp::request& req){
			completed = true;
			auto r = req.get_response();
			TRACE_ALWAYS(<< "HTTP request completed, status = " << unsigned(r.status) << ", code = " << unsigned(r.response_code) << std::endl)
			ASSERT_INFO_ALWAYS(r.status == easyhttp::status_code::ok, "r.status = " << unsigned(r.status))
			sema.signal();
		});

		r->set_url("https://speed.hetzner.de/1GB.bin");

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

	// test set_headers()
	{
		nitki::semaphore completed;

		easyhttp::init_guard easyhttp_guard;

		std::vector<uint8_t> data;

		auto r = std::make_shared<easyhttp::request>([&completed, &data](easyhttp::request& r){
			ASSERT_INFO_ALWAYS(r.get_response().status == easyhttp::status_code::ok, "status code is not OK: " << unsigned(r.get_response().status))
			ASSERT_INFO_ALWAYS(r.get_response().response_code == easyhttp::http_code::ok, "http code is not OK: " << unsigned(r.get_response().response_code))
			data = std::move(r.get_response().body);
			completed.signal();
		});

		r->set_url("https://postman-echo.com/headers");

		std::string custom_header_value = "Lorem ipsum dolor sit amet";

		r->set_headers({
				{"my-sample-header", custom_header_value},
				{"Accept", "application/json"},
				{"Content-Type", "application/json"}
			});

		r->start();

		completed.wait();

		auto resp_str = utki::make_string(data);

		ASSERT_INFO_ALWAYS(resp_str.find(custom_header_value) != std::string::npos, "expected string not found in response body: " << resp_str)

		TRACE(<< "body.size() = " << data.size() << std::endl)
		TRACE(<< resp_str << std::endl)
	}

	return 0;
}
