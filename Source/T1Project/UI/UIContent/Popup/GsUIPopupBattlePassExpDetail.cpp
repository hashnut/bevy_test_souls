// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/GsUIPopupBattlePassExpDetail.h"
#include "Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Engine/AssetManager.h"

void UGsUIPopupBattlePassExpDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupBattlePassExpDetail::OnClickClose);
}

void UGsUIPopupBattlePassExpDetail::SetDetailInfo(FText name, FText exp, FSoftObjectPath iconPath)
{
	_nameText = name;
	_expText = exp;

	if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(iconPath))
	{
		_iconImg.SetResourceObject(loadObject);
		_iconImg.SetImageSize(FVector2D(110, 110));
	}
}

void UGsUIPopupBattlePassExpDetail::OnClickClose()
{
	Close();
}

void UGsUIPopupBattlePassExpDetail::OnInputCancel()
{
	OnClickClose();
}