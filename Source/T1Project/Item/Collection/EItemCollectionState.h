#pragma once

/** 아이템컬렉션 상태 */
enum class EItemCollectionState : uint8
{
	Progressing, // 하나 이상 컬렉션 중
	Opened,      // 컬렉션 열림. 컬렉션은 0
	Completed,   // 컬렉션 완료

	Default = Opened,
};