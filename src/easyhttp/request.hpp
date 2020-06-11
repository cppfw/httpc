#pragma once

#include <memory>
#include <functional>
#include <vector>

#include <curl/curl.h>

#include <utki/span.hpp>

#include "http_code.hpp"

namespace easyhttp{

enum class method{
	get,
	post,
	put,
	head,
	delete_
};

enum class status_code{
	ok,
	network_error,
	timeout,
	authentication_failed
};

struct response{
	status_code status;
	http_code response_code;
	std::vector<uint8_t> body;
};

class request : public std::enable_shared_from_this<request>{
	friend class init_guard;

	CURL* handle;

	volatile bool is_idle = true;

	response resp;

	std::function<void(request&)> completed_handler;

	std::function<size_t(utki::span<const uint8_t>)> data_handler;

	static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
public:
	request(decltype(completed_handler)&& ch);

	~request()noexcept;

	void start();

	response& get_response(){
		return this->resp;
	}

	/**
	 * @brief Cancel active request.
	 * @return true if request was cancelled and no completed callback was called.
	 * @return false in case the request has completed befere cancelling.
	 */
	bool cancel()noexcept;

	void set_method(method m);

	void set_url(const std::string& url);

	void set_data_handler(decltype(data_handler)&& handler);
};

}
