#pragma once

#include <memory>
#include <functional>
#include <vector>

#include <curl/curl.h>

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
	std::vector<uint8_t> body;
};

class request : public std::enable_shared_from_this<request>{
	friend class init_guard;

	CURL* handle;

	volatile bool is_idle = true;

	response resp;

	std::function<void(const response&)> completed_handler;

public:
	request(std::function<void(const response&)>&& ch);

	~request()noexcept;

	void run();

	/**
	 * @brief Cancel active request.
	 * @return true if request was cancelled and no completed callback was called.
	 * @return false in case the request has completed befere cancelling.
	 */
	bool cancel()noexcept;

	void set_method(method m);

	void set_url(const std::string& url);
};

}
