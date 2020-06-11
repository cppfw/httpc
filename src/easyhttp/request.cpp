#include "request.hpp"

#include "init_guard.hpp"

using namespace easyhttp;

request::request(std::function<void(const response&)>&& ch) :
		handle(curl_easy_init()),
		completed_handler(std::move(ch))
{
	// curl_easy_setopt(this->handle, CURLOPT_VERBOSE, 1);
}

request::~request()noexcept{
	ASSERT(this->is_idle)
	curl_easy_cleanup(this->handle);
}

void request::start(){
	if(!this->is_idle){
		throw std::logic_error("request is already running");
	}

	this->is_idle = false; // this should go before start_request()
	init_guard::inst().start_request(this->shared_from_this());
}

bool request::cancel()noexcept{
	// TODO:
	return false;
}

void request::set_url(const std::string& url){
	ASSERT(this->is_idle)
	curl_easy_setopt(this->handle, CURLOPT_URL, url.c_str());
}
