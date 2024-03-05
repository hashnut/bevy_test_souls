
#pragma once


enum class ETitleViewListType
{
	eAll,		// 2022/01/17 PKT - 전체
	eAcquire,	// 2022/01/17 PKT - 획득
	eDotHave,	// 2022/01/17 PKT - 미획득
};


// 2022/01/14 PKT - Title 상태 변경 Message
enum class ETitleMessageReason
{
	eTitleStatus,			// 2022/01/14 PKT - 획득 상태가 변경
	eEquipState,			// 2022/01/14 PKT - 장착 상태가 변경
};


// 2022/01/14 PKT - Title State
enum class ETitleState
{
	Disable,		// 2022/01/14 PKT - 획득하지 못한 비활성화 상태
	Activate,		// 2022/01/14 PKT - 획득한 상태, 이미 활성화 되어 있는 상태
	NewlyActivated,	// 2022/01/14 PKT - 새롭게 활성화 된 상태,확인 이후에는 Activate 상태로 돌린다.
};


// 2022/01/19 PKT - Title Reward State
enum class ETitleRewardState
{
	Disable,			// 2022/01/14 PKT - 획득하지 못한 비활성화 상태
	Activate,			// 2022/01/14 PKT - 획득한 상태, 이미 활성화 되어 있는 상태
};


// 2022/08/24 PKT - Title Main Category
enum class ETitleMainCategory
{
	FORNT,			// 2022/08/24 PKT - 앞
	BACK,			// 2022/08/24 PKT - 뒤
	MAX,
};