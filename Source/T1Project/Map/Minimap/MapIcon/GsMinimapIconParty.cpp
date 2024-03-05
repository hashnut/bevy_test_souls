#include "GsMinimapIconParty.h"

#include "Management/ScopeGame/GsMinimapManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Map/Minimap/SceneImage/GsMinimapSceneImage.h"

#include "Map/MiniMap/GsMinimapFunc.h"

#include "Party/GsPartyMemberInfo.h"

#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

void UGsMinimapIconParty::Initialize(
	FGsPartyMemberInfo* In_partyMemberInfo,
	UGsUIIconBase* In_iconWidget,
	EGsMapIconType In_iconType,
	float In_updateTime,
	UGsMinimapSceneImage* In_sceneImage,
	int In_depth, int64 In_gameId,
	UUserWidget* In_arrowWidget)
{
	_memberInfo = In_partyMemberInfo;

	_icon = In_iconWidget;
	_iconType = In_iconType;
	_sceneImage = In_sceneImage;
	_iconDepth = In_depth;
	_gameId = In_gameId;

	_widgetArrow = In_arrowWidget;

	// 일단 끈다
	if (_widgetArrow != nullptr)
	{
		_widgetArrow->SetVisibility(ESlateVisibility::Hidden);
	}

	int localChannel = GGameData()->GetCurrentChannel();
	_isSameChannel = (_memberInfo->GetChannelId() == localChannel) ? true : false;
	
	UWorld* world = GSMinimap()->GetWorld();

	if (world == nullptr)
	{
		return;
	}

	world->GetTimerManager().SetTimer(_timerHandle,
		FTimerDelegate::CreateUObject(this, &UGsMinimapIconParty::UpdateTimmer), In_updateTime, true);

	SetIconHidden(false);
}

// 타이머에의한 업데이트
void UGsMinimapIconParty::UpdateTimmer()
{
	// 로컬이 움직이면
	if (true == _localPosDirty ||
		true == _targetObjPosDirty)
	{
		_localPosDirty = false;
		_targetObjPosDirty = false;

		UpdateObjectTransform();
	}
}

// 회전, 위치 갱신
void UGsMinimapIconParty::UpdateObjectTransform()
{
	// 뷰 영역밖인지 체크
	if (_sceneImage == nullptr)
	{
		return;
	}

	bool isOutSide =
		_sceneImage->IsOutSideFromFrame(GetPos());

	// 갱신
	UpdatePos();

	// 채널이 로컬과 같고
	// 화살표 위젯이 있을때만 처리
	if (_isSameChannel == true &&
		_widgetArrow != nullptr)
	{
		// 뷰 영역 값이 바뀜(안, 밖)
		if (_isViewOutside != isOutSide)
		{
			_isViewOutside = isOutSide;
			_widgetArrow->SetVisibility((_isViewOutside == true) ?
				ESlateVisibility::SelfHitTestInvisible :
				ESlateVisibility::Hidden);
		}

		// 뷰영역 밖이면 회전 처리
		if (_isViewOutside == true)
		{
			UpdateArrowRotation(GetPos());
		}
	}
}

// 위치 구하기
FVector2D UGsMinimapIconParty::GetPos()
{
	if (nullptr == _memberInfo)
	{
		return FVector2D::ZeroVector;
	}

	FVector pos = _memberInfo->GetPos();
	return FVector2D(pos);
}

// 클리어
void UGsMinimapIconParty::Finalize()
{
	if (UWorld* world = GSMinimap()->GetWorld())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
		_timerHandle.Invalidate();
	}
	FGsMinimapFunc::ReleaseWidget(_widgetArrow);
	_widgetArrow = nullptr;

	Super::Finalize();
}
void UGsMinimapIconParty::Tick(float In_delta)
{
	if (nullptr == _memberInfo)
	{
		return;
	}

	FVector nowPos = _memberInfo->GetPos();

	if (nowPos != _oldObjPos)
	{
		_oldObjPos = nowPos;
		_targetObjPosDirty = true;
	}
}

// ui 위치 구하기
FVector2D UGsMinimapIconParty::GetUIPos(const FVector2D& In_objectPos)
{
	if (_sceneImage == nullptr)
	{
		return FVector2D::ZeroVector;
	}
	
	// 뷰밖으로 나가있지 않거나
	// 채널이 다르면
	// 기본 계산
	if (_isViewOutside == false ||
		_isSameChannel == false)
	{		
		return Super::GetUIPos(In_objectPos);
	}

	FVector2D insidePos = _sceneImage->CalcInsideViewPos(In_objectPos);
	// 아니면 뷰 안으로 넣는 계산 처리
	return insidePos;
}
// 방향 표시 회전 갱신
void UGsMinimapIconParty::UpdateArrowRotation(const FVector2D& In_objectPos)
{
	if (_widgetArrow == nullptr)
	{
		return;
	}

	float angle = _sceneImage->GetTargetAngleFromLocal(In_objectPos);
	_widgetArrow->SetRenderTransformAngle(angle);
}
