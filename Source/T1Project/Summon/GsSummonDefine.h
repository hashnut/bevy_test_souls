#pragma once

/*
	뽑기 define
*/
// 뽑기 버튼 타입
enum class EGsSummonBtnType
{
	HIDE = 0,				// HIDE
	OPEN_ALL = 1,			// 모두 열기
	SHOW_RESULT = 2,		// 결과 바로 보기(고등급)
	QUIT = 3,				// 나가기
	RETRY = 4,				// 다시 뽑기
	RETRY_ITEM = 5,			// 다시 뽑기(아이템)
};
// 뽑기 진행 스텝 타입
enum class EGsSummonProgressStepType
{
	None,					
	OpenCutscene,			// 오프닝 컷신
	ShowAllStone,			// 모든 소환석 보이는 상태
	ShowAllFairy,			// 모든 페어리 보이는 상태
	ZoomStart,
	HighGradeZoom,			// 고등급 줌 상태
	HighGradeAppearCutscene,// 고등급 등장 컷신
	SuccessDragVeilObject,	// 드래그 성공
};
// ui 소환석 입력 타입
enum class EGsUISummonStoneInputType
{
	Touch = 0,			// 터치
	Pull = 1,			// 당기기
};

// 어디서 부터 뽑기 시작인지 정보
enum class EGsSummonOriginContentType
{
	SUMMON_ITEM,			// 소환 아이템 뽑기
	SUMMON_COMPOSE,			// 소환 합성
	SUMMON_WAIT_CHANGE,		// 소환 대기중 교체
	SUMMON_RE_COMPOSE,		// 소환 재합성
	SUMMON_CEILING,			// 소환 천장합성
	SUMMON_EXCHANGE,		// 소환 교체권
};

// 소환 타입
enum class EGsSummonType
{
	SUMMON_FAIRY,		// 페어리 소환
	SUMMON_COSTUME,		// 코스튬 소환
};

// 소환 구매 타입
enum class EGsSummonBuyType
{
	BM_SHOP = 0,		// BM 상점
	ITEM	=1,			// 아이템
};

enum class EGsSummonSceneType
{
	SummonFairy		= 0,		// summon fairy(cinematic)
	SummonCostume	= 1,		// summon costume(ingame->legacy spawn)
	Compose			= 2,		// compose(fairy , costume -> ingame(drag controll), cinematic )
	HighGet			= 3,		// high costume, fairy get(epic, unique, legend)
	HighPre			= 4,		// 고등급 획득전 추가 연출(테미르)
};