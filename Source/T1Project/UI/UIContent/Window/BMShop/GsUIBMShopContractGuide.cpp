// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopContractGuide.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "T1Project.h"


void UGsUIBMShopContractGuide::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnDetail->OnClicked.AddDynamic(this, &UGsUIBMShopContractGuide::OnClickDetail);
	_btnJapanWarn->OnClicked.AddDynamic(this, &UGsUIBMShopContractGuide::OnClickJapanWarn);
}

void UGsUIBMShopContractGuide::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGsHiveManager* hiveMgr = GHive())
	{
		_btnJapanWarn->SetVisibility(hiveMgr->IsDisplayFreeDiaCountry() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UGsUIBMShopContractGuide::OnClickDetail()
{
	if (!GHive()->IsSignIn())
		return;

	FGsUIHelper::OpenHelpPage(EGsHelpContents::BMShopPurchaseWarn);

	/*
	FString contentsKey = TEXT("300145"); // 샌박 콘솔 - 게임 인덱스 1047 커스텀뷰 Id
	if (Configuration::getZone() == ZoneType::REAL)
	{
		contentsKey = TEXT("300156");
	}

	Promotion::showCustomContents(PromotionCustomType::VIEW, TCHAR_TO_UTF8(*contentsKey), [](ResultAPI result, PromotionEventType promotionEventType) {

		if (result.isSuccess())
		{
			switch (promotionEventType)
			{
			case PromotionEventType::OPEN:   break; //block 필요하다면 처리
			case PromotionEventType::CLOSE:  break; //block 해지 필요하다면 처리
			}
		}
		else
		{
		}

		});

	//GSLOG(Warning, TEXT("UGsUIBMShopContractGuide::OnClickDetail()"));
	//FPlatformProcess::LaunchURL(TEXT("http://terms.withhive.com/terms/policy/view/M31"), nullptr, nullptr);
	*/
}

void UGsUIBMShopContractGuide::OnClickJapanWarn()
{
	//GSLOG(Warning, TEXT("UGsUIBMShopContractGuide::OnClickJapanWarn()"));
}