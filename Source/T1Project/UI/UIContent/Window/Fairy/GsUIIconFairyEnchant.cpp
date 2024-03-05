// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconFairyEnchant.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "UMG/Public/Blueprint/SlateBlueprintLibrary.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Engine/Classes/Engine/GameViewportClient.h"
#include "Components/Image.h"

#include "T1Project.h"
#include "Materials/MaterialInstanceDynamic.h"


void UGsUIIconFairyEnchant::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnIcon->OnClicked.AddDynamic(this, &UGsUIIconFairyEnchant::OnClickItemIcon);
}

void UGsUIIconFairyEnchant::NativeDestruct()
{
	OnClickIcon.Clear();
	OnPressedLong.Clear();

	_btnIcon->_onFinishedLongPressGauge.Clear();

	Super::NativeDestruct();
}

void UGsUIIconFairyEnchant::NativeConstruct()
{
	Super::NativeConstruct();

	_btnIcon->_onFinishedLongPressGauge.AddUObject(this, &UGsUIIconFairyEnchant::OnFinishedLongPress);
}

void UGsUIIconFairyEnchant::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIIconFairyEnchant::SetEmptyIcon()
{
	_imgIcon->SetVisibility(ESlateVisibility::Collapsed);
	_textItemName = FText::GetEmpty();
}

void UGsUIIconFairyEnchant::SetIconImage(const FSoftObjectPath& InPath)
{
	_imgIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (UPaperSprite* loadObject = GResource()->LoadSync<UPaperSprite>(InPath))
	{
		static FVector2D sz = _imgIcon->Brush.GetImageSize();
		if (auto material = _imgIcon->GetDynamicMaterial())
		{
			FSlateAtlasData atlasData = loadObject->GetSlateAtlasData();
			material->SetTextureParameterValue(TEXT("Texture"), atlasData.AtlasTexture);
			material->SetScalarParameterValue(TEXT("OffsetU"), atlasData.StartUV.X);
			material->SetScalarParameterValue(TEXT("OffsetV"), atlasData.StartUV.Y);
			material->SetScalarParameterValue(TEXT("TilingU"), atlasData.SizeUV.X);
			material->SetScalarParameterValue(TEXT("TilingV"), atlasData.SizeUV.Y);
			_imgIcon->SetBrushSize(sz);
		}
	}
}

void UGsUIIconFairyEnchant::SetItemNameColor(FText nameText, const FLinearColor& InColor)
{
	_colorItemName = InColor;
	_textItemName = nameText;
}

void UGsUIIconFairyEnchant::OnFinishedLongPress()
{
	OnPressedLong.Broadcast();
}

void UGsUIIconFairyEnchant::OnClickItemIcon()
{
	OnClickIcon.Broadcast();
}
