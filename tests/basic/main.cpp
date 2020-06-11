#include "../../src/easyhttp/init_guard.hpp"

#include <utki/debug.hpp>
#include <utki/string.hpp>
#include <nitki/semaphore.hpp>

int main(int argc, char** argv){
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

	return 0;
}
