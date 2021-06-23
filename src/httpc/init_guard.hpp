#pragma once

#include <utki/singleton.hpp>

#include "request.hpp"

namespace httpc{

class init_guard : public utki::intrusive_singleton<init_guard>{
	friend class utki::intrusive_singleton<init_guard>;
	static init_guard::T_Instance instance;
	
	friend class request;
	
	void start_request(std::shared_ptr<request> r);
	bool cancel_request(request& r);

	static void thread_func();
	static void handle_completed_request(const void* CURLMsg_message);
public:
	init_guard(bool init_winsock = true);
	~init_guard()noexcept;
};

}
