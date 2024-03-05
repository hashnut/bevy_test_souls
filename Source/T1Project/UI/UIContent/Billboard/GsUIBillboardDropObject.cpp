// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBillboardDropObject.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Option/GsGameUserSettings.h"

#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"

#include "Shared/Client/SharedEnums/SharedIffEnum.h"

void UGsUIBillboardDropObject::SetItemNameText(const FText& InName)
{
	if (_itemNameText)
	{
		_itemNameText->SetText(InName);
	}	
}

void UGsUIBillboardDropObject::SetItemNameColor(const FLinearColor& InColor)
{
	if (_itemNameText)
	{
		_itemNameText->SetColorAndOpacity(InColor);
	}
}

// 이름 출력 옵션 갱신
void UGsUIBillboardDropObject::UpdateShowNameOption()
{
	UGsGameUserSettings* gameUserSettings = GGameUserSettings();
	if (gameUserSettings == nullptr)
	{
		return;
	}
	// 옵션 값 확인
	bool isShowDropItemName =
		(bool)gameUserSettings->GetCombatSetting(EGsOptionCombat::DROP_ITEM_NAME);

	ESlateVisibility visibility = 
		(isShowDropItemName == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	SetVisibility(visibility);
}

void UGsUIBillboardDropObject::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateShowNameOption();
}