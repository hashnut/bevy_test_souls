#pragma once
#include "CoreMinimal.h"

// 서버에서 내려주는 플레이어블 이벤트 체크 포인트 타입
enum class EGsPlayableEventServerCheckPointType
{
	First = 0,						// 최초
	PrePlayableMovieComplete = 1,	// 천마대전1 무비 플레이 완료
	PlayableEventComplete = 2,		// 플레이어블 이벤트 완료
	PostPlayableMovieComplete = 3,	// 천마대전2 무비 플레이 완료
	CharacterEmptyMovieComplete = 4,// 페어리 나는 시퀀스 플레이 완료
	GameFirstEnterMovieComplete = 5,// 레오성 입장 시네마틱 플레이 완료
};