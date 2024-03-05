#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "EGsContentsEventState.h"
#include "Net/MessageHandler/GsNetMessageHandlerContentsEvent.h"

class FGsContentsEventBase;
struct FGsSchemaContentsEventGroup;

// 컨텐츠이벤트 대체 설명의 출력 타입
enum class EGsContentsEventGroupAltDescDisplayType
{
	None,
	EndDate,
};

/**
 * 컨텐츠이벤트그룹
 */
class FGsContentsEventGroup final
{

	friend class UGsContentsEventManager;

public:
	const FGsSchemaContentsEventGroup* _contentsEventGroupData;

private:
	/** 소유한 컨텐츠이벤트목록. 인스턴스는 매니저가 가지고, 여기서는 포인터만 가짐. */
	const TArray<TWeakPtr<const FGsContentsEventBase>> _contentsEvents;

	/** 현재 활성화 상태 */
	EGsContentsEventState _state = EGsContentsEventState::Deactivated;

	EGsContentsEventGroupAltDescDisplayType _altDescDisplayType = EGsContentsEventGroupAltDescDisplayType::EndDate;

	/** 시작 날짜시간. 서버에서 수신한 데이터로 오버라이드됨. */
	FDateTime _overridedStartUtc;

	/** 종료 날짜시간. 서버에서 수신한 데이터로 오버라이드됨. */
	FDateTime _overridedEndUtc;

public:
	FGsContentsEventGroup(const FGsSchemaContentsEventGroup* InContentsEventGroup, TArray<TWeakPtr<const FGsContentsEventBase>>& InContentsEvents);

private:
	void Initialize();

private:
	/** 서버로부터 수신 받은 데이터를 덮어씀. */
	void OverrideData(const FGsNetMessageHandlerContentsEvent::FGsContentsEventGroupData& InContentsEventGroupData);

	/** 활성화 상태 변경하기 */
	void SetState(const EGsContentsEventState InContentsEventState);

public:
	/** 현재 활성화 상태 */
	EGsContentsEventState GetState() const;

	/** 컨텐츠이벤트그룹 ID */
	ContentsEventGroupId GetContentsEventGroupId() const;

	/** 활성화 여부 무관하게 소유한 모든 컨텐츠이벤트 목록을 반환 */
	const TArray<TWeakPtr<const FGsContentsEventBase>>& GetContentsEventAll() const;

	/** 시작 날짜시간 */
	FDateTime GetStartUtc() const;

	/** 종료 날짜시간 */
	FDateTime GetEndUtc() const;

	/** 활성화 여부 */
	bool IsActivated() const;

	bool CanActivate() const;

	/** 디스플레이 여부 */
	bool CanDisplay() const;

	/** 디스플레이 순서 */
	int32 GetDisplayPriority() const;

	FSoftObjectPath GetIcon() const;

	FText GetTitle() const;

	FText GetDescription() const;

	FText GetAltDescription() const;

};
