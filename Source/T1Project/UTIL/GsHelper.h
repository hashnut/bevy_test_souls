#pragma once

#include <tuple>

#define MACRO(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value 


template<typename T>
inline T*
RawPtr(TWeakPtr<T> t)
{
	return t.IsValid() ? t.Pin().Get() : nullptr;
}


