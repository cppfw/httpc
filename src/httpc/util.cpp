#include "util.hpp"

#include <curl/curl.h>

#include <utki/util.hpp>

#include <cstring>

using namespace httpc;

std::string httpc::escape(const std::string& str){
	auto curl = curl_easy_init();
	utki::scope_exit curl_scope_exit([curl](){
		curl_easy_cleanup(curl);
	});

	auto encoded = curl_easy_escape(curl, str.data(), str.size()); 
	utki::scope_exit encoded_scope_exit([encoded](){
		curl_free(encoded);
	});

	std::string ret(encoded, strlen(encoded));

	return ret;
}

