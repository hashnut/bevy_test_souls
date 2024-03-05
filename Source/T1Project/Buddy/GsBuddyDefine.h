#pragma once
/*
	친구 define
*/
// 친구 탭슬롯 타입
enum class EGsBuddyTabType
{	
	Buddy, // 친구
	Apply,	// 신청
	Accpept, // 수락	
};

// 친구 탭슬롯 서브 타입
enum class EGsBuddySubTabType
{
	All,	// 전체
	My,		// 나와 homeWorldId가 같은 
	Other,	// 나와 homeWorldId가 다른	
};