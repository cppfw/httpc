#pragma once

#include <utki/singleton.hpp>

#include "request.hpp"

namespace easyhttp{

class init_guard : public utki::singleton<init_guard>{
	friend class request;
	
	void start_request(std::shared_ptr<request> r);
	bool cancel_request(request& r);

	static void thread_func();
	static void handle_completed_request(const CURLMsg& m);
public:
	init_guard(bool init_winsock = true);
	~init_guard()noexcept;
};

}
