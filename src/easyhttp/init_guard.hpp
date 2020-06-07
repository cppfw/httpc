#pragma once

#include <utki/singleton.hpp>

namespace httpcpp{

class init_guard : public utki::singleton<init_guard>{
public:
	init_guard();
	~init_guard()noexcept;
};

}
