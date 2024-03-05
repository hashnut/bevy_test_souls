#pragma once

#include "CoreMinimal.h"
#include "Management/GsScopeHolder.h"
#include "Classes/GsManager.h"

#include "Message/MessageHandler/GsSystemMessageHandlerInterface.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "Map/GsMapIconType.h"

#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "GsMinimapManager.generated.h"

/*
 미니맵: 아이콘, 지도 관리
*/
class UGsUIIconBase;
class UGsMinimapIconBase;
class UGsMinimapIconObject;
class UGsMinimapIconLocal;
class UGsWidgetPoolManager;
class UGsMinimapSceneImage;
class UGsGameObjectBase;
class UGsGameObjectLocalPlayer;
class UGsMinimapIconHandler;

struct FGsSchemaMinimapIconData;
struct FGsMinimapIconTeamMessageParam;


UCLASS()
class UGsMinimapManager :
	public UObject,
	public IGsManager,
	public IGsMessageHandler, 
	public IGsEventProgressInterface
{
	GENERATED_BODY()
	
	// 멤버 변수
private:
	// 중심 플레이어(원래는 로컬인데 관전 모드에선 관전 타겟)
	UGsGameObjectBase* _centerPlayer = nullptr;
	// 씬 이미지 관리
	UPROPERTY()
	UGsMinimapSceneImage* _sceneImage;
	// 로컬 이전 위치
	FVector _oldLocalPos = FVector::ZeroVector;

	// 아이콘 핸들러(미니맵에 아이콘 관련 처리가 너무 많아서 분리)
	UPROPERTY()
	UGsMinimapIconHandler* _iconHandler;

	// 예약 데이터(채널 이동)
	int _reserveChannelId;
	bool _reserveIsCurrentChannel;
	ChannelCongestionStatus _reserveChannelState;

	// 생성자, 소멸자
public:
	virtual ~UGsMinimapManager() {}


	// 가상함수
public:
	// IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float In_delta) override;

	// IGsEventProgressInterface
	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

protected:
	// IGsSystemMessageHandler
	virtual void InitializeMessage() override;
	virtual void FinalizeMessage() override;

	// 로직 함수
public:
	void ReserveChannelMove(int In_channelId, 
		bool In_reserveCurrentChannel,
		ChannelCongestionStatus In_reserveChannelState);
	void SendRequestSelectChannel();
	bool CheckCanChangeChannel();

	// get, set
public:
	// 캐릭터 세팅(가운데 표시될)
	void SetCenterObject(UGsGameObjectBase* In_centerObj);
	// 캐릭터 삭제될때
	void RemoveCenterObject();
	// scene image 가져오기
	UGsMinimapSceneImage* GetSceneImage() { return _sceneImage; }
	UGsMinimapIconHandler* GetIconHandler() { return _iconHandler; }

	bool IsPeaceState()const ;
};

#define GSMinimap() UGsScopeHolder::GetGameManager<UGsMinimapManager>(UGsScopeGame::EManagerType::Minimap)
