#pragma once
/*
	파티 define
*/
// 파티 멤버 타입
enum class EGsPartyMemberItemType
{
	Me,						// 나 자신
	Member_ImNotLeader,		// 멤버(난 리더 아님)
	Member_ImLeader,		// 멤버(난 리더)
	InviteProgress,			// 초대 대기
	None,					// 없음
};
// ui hud 드로우 메뉴 스위치 타입
enum class EGsUIPartyHUDDrawMenuSwitchType
{
	Open_MemberImLeader,	// 열림(멤버/ 난 리더)
	Open_LeaderImNotLeader,	// 열림(리더/ 난 리더아님)
	Open_MemberImNotLeader,	// 열림(멤버/ 난 멤버)
	NotUse,						// not use
};

// ui hud 멤버 스위치 타입
enum class EGsUIPartyHUDMemberSwitchType
{
	Member,				// 멤버
	Empty_LocalIsLeader,		// 빈칸(local is Leader)
	InviteProgress,		// 초대 대기
	Empty_LocalIsMember,		// empty(local is member)
};
// 정보 타입(개별 갱신용)
enum class EGsPartyMemberInfoType : uint32
{
	Level		=	0x1,				// 레벨			(0)
	HP			=	0x2,				// hp			(1)
	MP			=	0x4,				// mp			(2)
	WeaponType	=	0x8,				// 무기 타입		(3)
	Spawn		=	0x10,				// 스폰 여부		(4)
	HP_Shield	=	0x20,				// 보호막		(5)
	Ready_State =	0x40,				// 준비 상태		(6)
	Confuse		=	0x80,				// 컨퓨즈(HP/MP가림)상태이상 (7)
};
// 아이콘 변경 상태
enum class EGsPartyIconChangeState : uint32 
{
	NotChange,
	Add,
	Del,
};
// 파티 워프 리더 검사 결과
enum class EGsPartyWarpToLeaderResult : uint32
{
	WarpPossible = 0,					// 워프 가능
	NoData = 1,							// 데이터 없음(못감)
	DifferentWorldId = 2,				// 월드 id 가 다름(못감)
	SameChannelIdAndSameMapeId = 3,		// 같은 채널 id와 같은 mapid라 갈필요 없음(못감)
};
