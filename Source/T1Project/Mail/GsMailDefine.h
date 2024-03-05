#pragma once
/*
	우편 define
*/
// 우편 탭슬롯 타입
enum class EGMailTabType
{	
	User, // 개인
	Guild,	// 기사단
	Account, // 계정	
};

enum class EGMailReqType
{
	None,
	ReceiveOne,	// 1개 받기
	ReceiveAll,	// 모두 받기
	DeleteOne,	// 1개 삭제
	DeleteAll,	// 모두 삭제
};
