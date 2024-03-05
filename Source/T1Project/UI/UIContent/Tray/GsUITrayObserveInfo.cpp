#include "GsUITrayObserveInfo.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeGame/GsObserverManager.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Observer/GsObserverHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"


void UGsUITrayObserveInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnLeftTargetChange->OnClicked.AddDynamic(this, &UGsUITrayObserveInfo::OnClickLeftTargetChange);
	_btnRightTargetChange->OnClicked.AddDynamic(this, &UGsUITrayObserveInfo::OnClickRightTargetChange);
}
void UGsUITrayObserveInfo::NativeConstruct()
{
	Super::NativeConstruct();

	if (FGsMessageHolder* msg = GMessage())
	{
		_msgContensObserverDelegates.Emplace(
			msg->GetObserver().AddUObject(MessageContentObserver::CHANGE_OBSERVE_TARGET,
				this, &UGsUITrayObserveInfo::OnChangeObserveTaret));
	}
}


void UGsUITrayObserveInfo::NativeDestruct()
{
	ClearAllMessage();


	Super::NativeDestruct();
}
void UGsUITrayObserveInfo::ClearAllMessage()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_msgContensObserverDelegates.Num() != 0)
		{
			for (auto iter : _msgContensObserverDelegates)
			{
				msg->GetObserver().Remove(iter);
			}
			_msgContensObserverDelegates.Empty();
		}
	}
}
void UGsUITrayObserveInfo::OnClickLeftTargetChange()
{
	ReqChangeTarget(false);
}

void UGsUITrayObserveInfo::OnClickRightTargetChange()
{
	ReqChangeTarget(true);
}
void UGsUITrayObserveInfo::ReqChangeTarget(bool In_isRight)
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsObserverHandler* observerHandler = hud->GetObserverHandler();
	if (observerHandler == nullptr)
	{
		return;
	}
	observerHandler->OnClickTargetChangeSide(In_isRight);
}
// 관전 대상 변경
void UGsUITrayObserveInfo::OnChangeObserveTaret(const IGsMessageParam* In_param)
{
	// 중앙 텍스트 세팅(현재 타겟)
	UGsGameObjectBase* currentTarget = GSObserver()->GetTargetObserve();
	if (currentTarget != nullptr)
	{
		if (FGsGameObjectData* currentData = currentTarget->GetData())
		{
			FString currentTargetName = currentData->GetName();

			// 포맷을 사용한다면
#ifdef USE_FORMAT
			FText findTextObserveTarget;
			FText::FindText(TEXT("GuildText"), TEXT("Dungeon_Observating_Target"), findTextObserveTarget);

			FString makeTargetStr = FString::Format(*findTextObserveTarget.ToString(), { currentTargetName });

			_textObserveTarget =  FText::FromString(makeTargetStr);
#else
			_textObserveTarget = FText::FromString(currentTargetName);
#endif
		}
	}

#ifdef USE_FORMAT
	FText findTextObserveSideTarget;
	FText::FindText(TEXT("GuildText"), TEXT("Dungeon_Observating_SideButton"), findTextObserveSideTarget);

	FString findStringObserveSideTarget = findTextObserveSideTarget.ToString();
#endif

	// 좌 텍스트 세팅
	UGsGameObjectBase* leftTarget = GSObserver()->GetSideTarget(false);
	if (leftTarget != nullptr)
	{
		if (FGsGameObjectData* leftData = leftTarget->GetData())
		{
			FString leftTargetName = leftData->GetName();					
#ifdef USE_FORMAT
			FString makeLeftTargetStr = FString::Format(*findStringObserveSideTarget, { leftTargetName });
			_textLeftObserveTarget = FText::FromString(makeLeftTargetStr);
#else
			_textLeftObserveTarget = FText::FromString(leftTargetName);
#endif
		}

		_btnLeftTargetChange->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// 버튼 hide
		_btnLeftTargetChange->SetVisibility(ESlateVisibility::Hidden);
	}
	// 우 텍스트 세팅
	UGsGameObjectBase* rightTarget = GSObserver()->GetSideTarget(true);
	if (rightTarget != nullptr)
	{
		if (FGsGameObjectData* rightData = rightTarget->GetData())
		{
			FString rightTargetName = rightData->GetName();
#ifdef USE_FORMAT
			FString makeRightTargetStr = FString::Format(*findStringObserveSideTarget, { rightTargetName });
			_textRightObserveTarget = FText::FromString(makeRightTargetStr);
#else
			_textRightObserveTarget = FText::FromString(rightTargetName);
#endif
		}

		_btnRightTargetChange->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// 버튼 hide
		_btnRightTargetChange->SetVisibility(ESlateVisibility::Hidden);
	}
}