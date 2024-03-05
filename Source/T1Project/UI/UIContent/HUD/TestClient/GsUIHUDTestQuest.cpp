// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDTestQuest.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"


#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "GsUIHUDTestQuestSlot.h"
#include "Runtime/Engine/classes/Kismet/GamePlayStatics.h"
#include "GameMode/GsGameModeTestClient.h"

#include "WrapBox.h"
#include "../../../UIControlLib/ContentWidget/GsButton.h"



void UGsUIHUDTestQuest::NativeOnInitialized()
{
	// 스크롤 셋팅
	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_scrollBoxHelper->Initialize(_scrollBoxEntryClass, _wrapBox);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIHUDTestQuest::OnRefreshEntryScrollBox);
	}

	GMessage()->GetGameObject().AddUObject(MessageGameObject::AI_QUEST_AUTO_MOVE_STOP, this,
		&UGsUIHUDTestQuest::OnQuestAutoMoveStop);


	_btnRefresh->OnClicked.AddDynamic(this, &UGsUIHUDTestQuest::OnClickRefrash);

	Super::NativeOnInitialized();
}

void UGsUIHUDTestQuest::OnClickRefrash()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::TEST_CLIENT_AUTOMOVE, nullptr);

	AGsGameModeTestClient* testGameMode = Cast<AGsGameModeTestClient>(UGameplayStatics::GetGameMode(GetWorld()));
	if (nullptr == testGameMode)
	{
		return;
	}
	testGameMode->RefreshNavDest();

	ClearSelect();

	UpdateList();

}

void UGsUIHUDTestQuest::InvalidateAllInternal()
{
	UpdateList();
}

void UGsUIHUDTestQuest::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIHUDTestQuest::OnRefreshEntryScrollBox);
		_scrollBoxHelper = nullptr;
	}


	Super::BeginDestroy();
}

void UGsUIHUDTestQuest::OnRefreshEntryScrollBox(int32 InIndex, UWidget * InEntry)
{
	AGsGameModeTestClient* testGameMode = Cast<AGsGameModeTestClient>(UGameplayStatics::GetGameMode(GetWorld()));
	if (nullptr == testGameMode)
	{
		return;
	}
	
	int8 listNum = testGameMode->GetNavDestActor().Num();
	if (listNum > InIndex)
	{
		AActor* obj = testGameMode->GetNavDestActor()[InIndex];
		if (nullptr != obj)
		{
			UGsUIHUDTestQuestSlot* slot = Cast<UGsUIHUDTestQuestSlot>(InEntry);
			if (nullptr != slot)
			{
				slot->SetTitleText(FText::FromString(obj->GetName()));
				slot->Update(InIndex);
				slot->_delegateClickSlot.BindUObject(this, &UGsUIHUDTestQuest::OnClickSlot);
				slot->_delegateClickTeleport.BindUObject(this, &UGsUIHUDTestQuest::OnClickTeleport);
			}
		}
	}
}

void UGsUIHUDTestQuest::OnClickSlot(UGsUIHUDTestQuestSlot* inSelect)
{
	AGsGameModeTestClient* testGameMode = Cast<AGsGameModeTestClient>(UGameplayStatics::GetGameMode(GetWorld()));
	if (nullptr == testGameMode)
	{
		return;
	}

	int32 listNum = testGameMode->GetNavDestActor().Num();

	if (_selectedSlot != inSelect)
	{
		// 다른 슬롯을 눌렀을 경우
		if (_selectedSlot.IsValid())
		{
			_selectedSlot->SetIsSelected(false);
		}
		_selectedSlot = inSelect;
		inSelect->SetIsSelected(true);

		FGsPrimitiveText param(inSelect->GetTitleText());
		GMessage()->GetGameObject().SendMessage(MessageGameObject::TEST_CLIENT_AUTOMOVE, &param);
	}
	else
	{
		// 같은 슬롯을 눌렀을 경우 (해지)
		if (_selectedSlot.IsValid())
		{
			_selectedSlot->SetIsSelected(false);
			_selectedSlot = nullptr;

			GMessage()->GetGameObject().SendMessage(MessageGameObject::TEST_CLIENT_AUTOMOVE, nullptr);
		}
		
	}
}

void UGsUIHUDTestQuest::OnClickTeleport(UGsUIHUDTestQuestSlot* inSelect)
{
	// 워프를  눌렀을 경우 (해지)
	ClearSelect();

	FGsPrimitiveText param(inSelect->GetTitleText());
	GMessage()->GetGameObject().SendMessage(MessageGameObject::TEST_CLIENT_WARP, &param);
}

void UGsUIHUDTestQuest::UpdateList()
{
	AGsGameModeTestClient* testGameMode = Cast<AGsGameModeTestClient>(UGameplayStatics::GetGameMode(GetWorld()));
	if (nullptr == testGameMode)
	{
		return;
	}

	int8 listNum = testGameMode->GetNavDestActor().Num();
	_scrollBoxHelper->RefreshAll(listNum);
	
}

void UGsUIHUDTestQuest::OnQuestAutoMoveStop(const IGsMessageParam*)
{
	ClearSelect();
}

void UGsUIHUDTestQuest::ClearSelect()
{
	if (_selectedSlot.IsValid())
	{
		_selectedSlot->SetIsSelected(false);
		_selectedSlot = nullptr;
	}
}





