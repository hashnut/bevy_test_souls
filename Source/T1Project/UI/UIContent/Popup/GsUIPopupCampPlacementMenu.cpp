// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/GsUIPopupCampPlacementMenu.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "Management/ScopeGame/GsCampManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"



void UGsUIPopupCampPlacementMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCampPlacementMenu::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCampPlacementMenu::OnClickCancel);
}

void UGsUIPopupCampPlacementMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIPopupCampPlacementMenu::NativeDestruct()
{
	FGsCampManager* campMgr = GSCamp();
	if (nullptr != campMgr)
	{
		if (campMgr->IsClientSpawned())
		{
			campMgr->DespawnClientCampActor();
		}
	}

	Super::NativeDestruct();
}

void UGsUIPopupCampPlacementMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FGsCampManager* campMgr = GSCamp();
	if (nullptr != campMgr)
	{
		bool isBuildable = campMgr->IsOkBuildCampArea();
		_btnOk->SetSwitcherIndex(isBuildable ? 0 : 1);
	}
}

void UGsUIPopupCampPlacementMenu::OnClickOk()
{
	if(1 == _btnOk->GetSwitcherIndex())
	{
		return;
	}

	GSCamp()->OnBuildCamp();
}

void UGsUIPopupCampPlacementMenu::OnClickCancel()
{
	GSCamp()->CancelBuildCamp();
}

void UGsUIPopupCampPlacementMenu::OnInputOk()
{
	OnClickOk();
}

void UGsUIPopupCampPlacementMenu::OnInputCancel()
{
	OnClickCancel();
}