#pragma once

// bak1210
// 아이템 관련 메시만 추가한다.
// 컨텐츠 레이어와 혼동금지( 컨텐츠 : 인벤토리, 상세창 ) 아이템을 사용하는것들
UENUM()
enum class MessageItem : uint8
{
	ADDITEM = 0,
	ADDITEMLIST,

	UPDATEITEM,
	UPDATEITEMLIST,

	REMOVEITEM,
	REMOVEITEMS,

	LOCKITEM, // 로컬 플레이어 상태이상을 통하여 아이템 사용 Lock 인 경우

	USEITEM,
	SELLITEM,

	MAX
};


