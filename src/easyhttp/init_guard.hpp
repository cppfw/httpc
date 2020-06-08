#pragma once

#include <utki/singleton.hpp>

namespace easyhttp{

class init_guard : public utki::singleton<init_guard>{
public:
	init_guard(bool init_winsock = true);
	~init_guard()noexcept;
};

}
