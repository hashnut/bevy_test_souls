//// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIBillboard.h"

#include "GameObject/Define/EGsTalkType.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Message/GsMessageGameObject.h"

#include "GsUIBillboardTalkBalloon.generated.h"


class AActor;
class UTextBlock;
class UCanvasPanel;
class UGsGameObjectBase;
struct FGsSchemaTalkBalloonData;
struct FGsSchemaTalkBalloonDataSet;
struct IGsMessageParam;
/**
 * NPC 말풍선 UI
 */
UCLASS()
class T1PROJECT_API UGsUIBillboardTalkBalloon : public UGsUIBillboard
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnUpdateTalkBalloonDelegate, float);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTalkBalloonStateChanged);

private:
	enum class TalkBalloonState
	{
		Opened,
		Closed,
		Closing,
	};

	enum class TalkBalloonUpdateType
	{
		NORMAL,			// 디폴트
		STATE_CHANGED,	// 캐릭터 상태 변경
		ANIM_NOTIFIED,	// 애님 노티파이
		COMMUNITY,		// 서버에 요청한 데이터 수신(커뮤니티)
		HIT,			// 피격 당했을 때
	};

	const float METER_TO_CM = 100.0f;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTalk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panelTalkBalloon;

protected:
	const FGsSchemaTalkBalloonData* _talkBalloonData = nullptr;	// 말풍선 데이터
	UGsGameObjectBase* _localPlayer = nullptr;					// 로컬 플레이어 캐싱
	AActor* _attachedActor = nullptr;							// 부착된 액터 캐싱
	const FGsSchemaTalkBalloonDataSet* _animNotifiedDataSet = nullptr; // 노티파이온 데이터 캐싱

protected:
	// 말풍선 로직 변수
	bool _isDead = false;
	bool _isOptionEnd = false;				// 옵션 OFF 일 때 말풍선 종료를 했는지 확인
	bool _wasLocalPlayerInRange = false;	// 플레이어가 범위 안에 있었는지(이전 프레임)
	float _remainKeepTime = 0.0f;			// 유지 시간
	float _remainIntervalTime = 0.0f;		// 대기 시간
	float _additionalDist = 300.0f;			// 종료하는 추가 범위

	int32 _currentOrder = INDEX_NONE;	// 말풍선 순서
	int32 _currentIndex = 0;	// 말풍선 순서에 대한 말풍선 Index

	TMap<EGsTalkBalloonState, TArray<int32>> _displayIndexListMap;	// 출력할 대사 인덱스 배열
	TArray<int32> _randomIndexList;		// _displayIndexList를 섞은 배열

	EGsTalkBalloonState _currentState = EGsTalkBalloonState::None;
	EGsTalkBalloonState _nextState = EGsTalkBalloonState::None;

	TalkBalloonUpdateType _updateReason;

	// 말풍선 상태 및 상태에 따른 업데이트 함수 Map
	TalkBalloonState _talkBalloonState = TalkBalloonState::Closed;
	TMap<TalkBalloonState, FOnUpdateTalkBalloonDelegate> _updateDelegateMap;
	bool _tick1 = false;
	bool _tick2 = false;

public:
	FOnTalkBalloonStateChanged OnStateChanged;

	// 커뮤니티 소녀 말풍선 정보
protected:
	MsgGameObjHandleArray _gameObjectDelegate;
	FString _localGuildName;
	FString _localUserName;
	FString _worldGuildName;
	FString _worldUserName;
	ItemId _highestPriceItemId;
	ItemId _highestGradeItemId;
	bool _isRequestingData;

protected:
	bool _isDebuging = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void OnChangeRenderState(bool bIsRendered) override;

public:
	// 말풍선 시작 및 종료
	void StartTalkBalloon();
	void EndTalkBalloon();

private:
	// 상태 갱신
	void UpdateTalkBalloon(float InDeltaTime);
	void UpdateOpened(float InDeltaTime);
	void UpdateClosing(float InDeltaTime);
	void UpdateClosed(float InDeltaTime);

	// 상태 전환
	void SetOpen();
	void SetCloseWithEffect();
	void SetClose();

	void UpdateData(TalkBalloonUpdateType InUpdateReason);
	void UpdateDataNormal(int32 InCurrentOrder, int32 InCount = 0);
	void UpdateDataStateChanged();
	void UpdateDataAnimNotified();
	void UpdateDataOnReceiveCommunity();
	void UpdateDataOnHit();

public:
	void SetTalkBalloonData(const FGsSchemaTalkBalloonData* InSchemaData);
	void SetActor(AActor* InActor);
	void SetText(const FText& InText);
	void SetActorState(uint8 InState);
	void ShowNotifiedTalkBallon(const FGsSchemaTalkBalloonDataSet* InSchemaDataSet);
	void SetIsDead(bool InIsDead);

protected:
	void SetNextDataByIndex(int32 InIndex);
	void ResetRandomIndexList();
	void InitDisplayIndexList();
	void RequestServerData(int32 InIndex);

public:
	TalkBalloonState GetTalkBalloonState() { return _talkBalloonState; };

protected:
	int32 GetNextOrder(int32 InOrder);
	int32 GetOriginalIndex(int32 InIndex);
	FText GetNextText(int32 InIndex);
	EGsTalkBalloonState GetStateEnumFromString(uint8 InState);

public:
	bool IsClosed();
	bool IsOptionEnabled();

protected:
	bool IsLocalPlayerInRange(int32 InRadius);
	bool IsRequiredDataRequest(int32 InIndex);
	bool CanShowTalkBalloon(int32 InIndex);

protected:
	float ComputeDesiredSize(const FText& InText) const;
	void DrawDebugTalkBalloonRange();

public:
	void SetDrawDebugTalkBalloonRange(bool InVisible);

protected:
	// BP에 종료 애니메이션 요청
	UFUNCTION(BlueprintImplementableEvent)
	void PlayCloseAnimation();

protected:
	void OnReceiveCommunityRank(const IGsMessageParam* InParam);
	void OnReceiveCommunityMarket(const IGsMessageParam* InParam);

public:
	void OnHit();
};