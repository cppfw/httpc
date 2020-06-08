#include "init_guard.hpp"

#include <thread>

#include <curl/curl.h>

using namespace easyhttp;

namespace{
std::thread thread;
volatile bool quit_flag;
CURLM *multi_handle = nullptr;
}

namespace{
void thread_func(){

	while(!quit_flag){
		int num_active_sockets;
		curl_multi_perform(multi_handle, &num_active_sockets);
 
		long timeout;
 
		curl_multi_timeout(multi_handle, &timeout);
		if(timeout < 0){ // no set timeout, use default
			timeout = 1000;
		}

      	CURLMcode rc = curl_multi_poll(multi_handle, NULL, 0, timeout, nullptr);
 
		if(rc != CURLM_OK){
			TRACE_ALWAYS(<< "curl_multi_poll() failed, terminating easyhttp thread" << std::endl)
			break;
    	}
	}

	curl_multi_cleanup(multi_handle);
	multi_handle = nullptr;

	curl_global_cleanup();
}
}

init_guard::init_guard(bool init_winsock){
	long flags = CURL_GLOBAL_SSL;
	if(init_winsock){
		flags |= CURL_GLOBAL_WIN32;
	}
	curl_global_init(flags);

	multi_handle = curl_multi_init();

	quit_flag = false;
	thread = std::thread(&thread_func);
}

init_guard::~init_guard()noexcept{
	quit_flag = true;
	ASSERT(multi_handle)
	curl_multi_wakeup(multi_handle);
	thread.join();
}
