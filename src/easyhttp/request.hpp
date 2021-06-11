#pragma once

#include <memory>
#include <functional>
#include <vector>
#include <map>

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
	undefined,
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

/**
 * @brief HTTP request.
 * The lifespan of all request objects must be limited by lifespan of init_guard singleton.
 * Otherwise the behavior is undefined.
 */
class request : public std::enable_shared_from_this<request>{
	friend class init_guard;

	void* CURL_handle;
	void* curl_slist_handle = nullptr;

	volatile bool is_idle = true;

	response resp;

	std::function<void(request&)> completed_handler;

	std::function<size_t(utki::span<const uint8_t>)> data_handler;

	static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

	void free_headers()noexcept;
public:
	request(decltype(completed_handler)&& ch);

	~request()noexcept;

	void start();

	const response& get_response()const{
		return this->resp;
	}

	/**
	 * @brief Cancel active request.
	 * @return true if request was cancelled and no completed callback was called.
	 * @return false in case the request has completed befere cancelling or was not active at all.
	 */
	bool cancel()noexcept;

	void set_method(method m);

	void set_url(const std::string& url);

	void set_data_handler(decltype(data_handler)&& handler);

	void set_headers(const std::map<std::string, std::string>& name_value, utki::span<const std::string> name = utki::span<const std::string>());
};

}
