#pragma once


enum class EPKBookFilter
{
	// 2022/09/20 PKT - 전체
	All,
	// 2022/09/20 PKT - 처치
	Kill,
	// 2022/09/20 PKT - 도움
	Assist,
	// 2022/09/20 PKT - 사망
	Die,
};

enum class EPKDetectionTargetFilter
{
	// 2022/09/20 PKT - 전체
	All,
	// 2022/09/20 PKT - 접속 중
	Connect,
};