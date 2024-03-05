#pragma once

//https://docs.microsoft.com/ko-kr/cpp/cpp/ellipses-and-variadic-templates?view=vs-2019
//bak1210 : 가변인자처리 진행중

#define VARIADIC_FUNC(ArgTypes, FuncName)\
template<typename T = ArgTypes, typename... T2>\
void FuncName(const T& Type, const T2&... Types)\
{\
	InternalVariadicFunc(Type);\
	FuncName(Types...);\
}\
proteted:\
void InternalVariadicFunc(T& inPrams);\
void FuncName();
