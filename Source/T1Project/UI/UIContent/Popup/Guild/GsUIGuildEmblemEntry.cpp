// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildEmblemEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
//#include "Classes/PaperSprite.h"
#include "Classes/Engine/Texture2D.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "Management/ScopeGlobal/GsGuildManager.h"


void UGsUIGuildEmblemEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIGuildEmblemEntry::OnClickSlot);
}

void UGsUIGuildEmblemEntry::OnHide()
{
	_guildLevel = 0;
	_requireGuildLevel = 0;
}

void UGsUIGuildEmblemEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_imgSelected->SetVisibility(_bIsSelected ? 
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIGuildEmblemEntry::SetData(const FGsSchemaGuildEmblem* InData)
{
	// 큰 이미지 스케일해서 쓸거라 Async 로딩함
	TWeakObjectPtr<UGsUIGuildEmblemEntry> weakThis(this);
	FSoftObjectPath path = InData->iconPathBig;
	FGsUIHelper::AsyncLoadTextureFromPath(path, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
		{
			if (UGsUIGuildEmblemEntry* StrongThis = weakThis.Get())
			{
				// 로드 끝난 후라 새로 로드하지 않고, 로드된 항목을 찾아옴
				UTexture2D* loadedTexture = Cast<UTexture2D>(inRes);
				if (loadedTexture)
				{
					StrongThis->SetIconImage(loadedTexture);
				}
			}
		}));	

	// 길드레벨이 제한 레벨 이상일 경우 사용 가능
	_guildLevel = GGuild()->GetGuildLevel();
	_requireGuildLevel = InData->requireLevel;
	
	if (_requireGuildLevel > _guildLevel)
	{
		_panelDimmed->SetVisibility(ESlateVisibility::Visible);
		_textBlockLevel->SetText(FText::AsNumber(_requireGuildLevel));
	}
	else
	{
		_panelDimmed->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIGuildEmblemEntry::SetIconImage(UTexture2D* InTexture)
{
	_imgEmblem->SetBrushFromSoftTexture(InTexture);
}

void UGsUIGuildEmblemEntry::OnClickSlot()
{
	// 제한 레벨 넘어가는 항목은 클릭 안됨
	if (_requireGuildLevel > _guildLevel)
	{
		return;
	}

	// ToggleGroup에 콜백 전달. 스왑을 위해 현재 상태에서 바꿔서 전달
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}
