
#include "GsMinimapIconDetectionTarget.h"
#include "../Common/Icon/GsUIIconBase.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsMinimapManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Message/GsMessagePKBook.h"
#include "Map/Minimap/SceneImage/GsMinimapSceneImage.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "../Option/GsServerOption.h"
#include "../GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "../Map/Minimap/MapIcon/GsMinimapIconObject.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "T1Project.h"


void UGsMinimapIconDetectionTarget::Finalize()
{
	_effectInfoSet.Empty();
	
	_limitMaxDistance = 0.f;
	_distance = 0.f;
	_currentEffectTick = 0.f;

	Super::Finalize();
}

FVector2D UGsMinimapIconDetectionTarget::GetPos()
{
	return (_distance <= _limitMaxDistance) ? GetActorPos2D() : FVector2D::ZeroVector;
}

void UGsMinimapIconDetectionTarget::Tick(float InDelta)
{
	Super::Tick(InDelta);

	ApplyEffect(InDelta);

	UpdatePos();
}

void UGsMinimapIconDetectionTarget::ApplyEffect(float InDelta)
{
	if (_targetObj)
	{
		FGsAbnormalityHandlerBase* abnormalityHandler = _targetObj->GetAbnormalityHandler();
		if (abnormalityHandler && abnormalityHandler->GetAbnormalityEffectType(AbnormalityEffectType::UTIL_HIDING))
		{
			// 2023/1/19 PKT - 은신상태면 숨김.
			SetIconHidden(true);
			return;
		}
		else
		{
			SetIconHidden(false);
		}
	}

	FVector2D targetPos = GetActorPos2D();
	FVector2D localPos = FVector2D::ZeroVector;
	if (nullptr != _localPlayer && nullptr != _localPlayer->GetActor())
	{
		localPos = FVector2D(_localPlayer->GetActor()->GetActorLocation());
	}

	// 2022/09/30 PKT - Calc Distance
	_distance = FVector2D::Distance(localPos, targetPos);

	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
	{
		GSLOG(Error, TEXT("nullptr == serverOption"));
		return;
	}

	// 2022/10/05 PKT - 단위를 미터로 변환
	float alertDistance = serverOption->GetPKBookOption().mNotificationRange * 100;

	if (_distance < alertDistance)
	{	// 2022/09/30 PKT - 옵션에서 지정한 거리보다 가까워 졌으면 "알람" 기능을 활성화
		GMessage()->GetPKBook().SendMessage(MessageContentPKBook::ON_CLOSE_TARGET_DECTION_ALERT, nullptr);
	}

	if (_distance > _limitMaxDistance)
	{
		SetIconHidden(true);
		_currentEffectTick = 0.f;
		return;
	}

	int32 blankRateTick = -1;
	for (const FGsPKDetectionEffectInfo& item : _effectInfoSet)
	{
		if (item.distance >= _distance)
		{
			blankRateTick = item.blinkRate;
			break;
		}
	}

	if (0 > blankRateTick)
	{	// 2022/09/30 PKT - 위에서 분명 최대 거리 측정 했음에도 여기에 온다면??? 
		_currentEffectTick = 0.f;
		GSLOG(Error, TEXT("(0 > blankRateTick"));	
		return;
	}
	else if (0 == blankRateTick)
	{
		_currentEffectTick = 0.f;
		// 2022/09/30 PKT - 혹시 Blank Rate Data가 0이면 항상 켜짐
		SetIconHidden(false);
		
		return;
	}

	_currentEffectTick += InDelta;

	if (_currentEffectTick >= (StaticCast<float>(blankRateTick) * 0.001f))
	{		
		SetIconHidden(false == IsIconHidden());
		_currentEffectTick = 0.f;
	}
}

void UGsMinimapIconDetectionTarget::SetData(
	class UGsGameObjectBase* InTargetObj
	, class UGsUIIconBase* InIconWidget
	, class UGsMinimapSceneImage* InSceneImage
	, EGsMapIconType InIconType
	, float InUpdateTime
	, int64 InGameId
	, int32 InDepth
	, const TArray<FGsPKDetectionEffectInfo>& InEffectInfoSet)
{
	Super::Initialize(InTargetObj, InIconWidget, InIconType, InUpdateTime, InSceneImage, InDepth, InGameId);

	UGsGameObjectManager* gameObjectManager = GSGameObject();
	if (nullptr == gameObjectManager)
	{
		return;
	}

	UGsGameObjectBase* local = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer);
	_localPlayer = (local) ? local->CastGameObject<UGsGameObjectLocalPlayer>() : nullptr;
	if (nullptr == _localPlayer)
	{
		GSLOG(Error, TEXT("nullptr == _localPlayer"));
		return;
	}

	_effectInfoSet = InEffectInfoSet;	
	// 2022/09/30 PKT - 짧은 거리 우선 순위로 정렬
	_effectInfoSet.Sort([](const FGsPKDetectionEffectInfo& lhr, const FGsPKDetectionEffectInfo& rhr)
		{
			return lhr.distance < rhr.distance;
		}
	);

	_distance = 0.f;
	// 2022/09/30 PKT - 효과 제한 거리
	_limitMaxDistance = (0 >= _effectInfoSet.Num()) ? 0.f : _effectInfoSet[_effectInfoSet.Num() - 1].distance;

	_currentEffectTick = 0.f;
}