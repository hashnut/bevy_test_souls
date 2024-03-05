// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowBattlePass.h"
#include "Message/MessageParam/GsBattlePassMessageParam.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsBattlePassManager.h"

#include "BattlePass/GsBattlePassData.h"

#include "UI/UIContent/Window/BattlePass/GsUIBattlePassTab.h"
#include "UI/UIContent/Window/BattlePass/GsUIBattlePassPage.h"
#include "UI/UIContent/Window/Community/GsUIAttendanceTab.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "UI/UIContent/Popup/GsUIPopupBattlePassBuyLevel.h"

#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "Engine/AssetManager.h"

#include "Net/GsNetSendServiceWorld.h"

void UGsUIWindowBattlePass::BeginDestroy()
{
	if (nullptr != _tabScrollBoxHelper)
	{
		_tabScrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIWindowBattlePass::OnCreateTabButtonEntry);
		_tabScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowBattlePass::OnRefreshTabButtonEntry);
	}

	Super::BeginDestroy();
}

void UGsUIWindowBattlePass::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowBattlePass::OnClickCloseAllStack);
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowBattlePass::OnClickBack);

	TArray<UWidget*> childList = _tabScrollPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIAttendanceTab>())
		{
			child->RemoveFromParent();
		}
	}

	_tabScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_tabScrollBoxHelper->Initialize(_tabBtnEntryClass, _tabScrollPanel);
	_tabScrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIWindowBattlePass::OnCreateTabButtonEntry);
	_tabScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowBattlePass::OnRefreshTabButtonEntry);

	_tabToggleGroup.OnSelectChanged.BindUObject(this, &UGsUIWindowBattlePass::OnSelectTab);

	_btnLevelUp->OnClicked.AddDynamic(this, &UGsUIWindowBattlePass::OnClickLevelUp);
}

void UGsUIWindowBattlePass::NativeConstruct()
{
	Super::NativeConstruct();

	_tabToggleGroup.Clear();

	_exposeBattlePasses.Reset();
	if (GSBattlePass()->GetTabOpendBattlePassList(_exposeBattlePasses) > 0)
	{
		_exposeBattlePasses.StableSort([](auto le, auto re) { return le.Pin()->GetPassTabIndex() < re.Pin()->GetPassTabIndex(); });

		_tabScrollPanel->ClearChildren();
		_tabScrollBoxHelper->AllClearChildren();
		_tabScrollBoxHelper->RefreshAll(_exposeBattlePasses.Num());
		_tabToggleGroup.SetSelectedIndex(0, true);
	}
}

void UGsUIWindowBattlePass::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIWindowBattlePass::OnCreateTabButtonEntry(UWidget* InEntry)
{
	if (auto tab = Cast<UGsUIAttendanceTab>(InEntry))
	{
		_tabToggleGroup.AddToggleWidget(tab);
	}
}

void UGsUIWindowBattlePass::OnRefreshTabButtonEntry(int32 inIndex, class UWidget* InEntry)
{
	if (auto tab = Cast<UGsUIAttendanceTab>(InEntry))
	{
		if (_exposeBattlePasses.IsValidIndex(inIndex))
		{
			if (auto data = _exposeBattlePasses[inIndex].Pin())
			{
				tab->SetData(data->GetPassTabName(), data->IsSetRedDot());
			}
		}
	}
}

void UGsUIWindowBattlePass::OnSelectTab(int32 index)
{
	_selectedPass = nullptr;
	_passNameText = FText::GetEmpty();
	_passPeriodText = FText::GetEmpty();
	_passDescText = FText::GetEmpty();
	_passExpText = FText::GetEmpty();
	_levelText = FText::GetEmpty();
	_passProgress->SetPercent(0.f);
	_passProgress->SetProgressText(FText::FromString(TEXT("")));
	_battlePassPage->SetSelectedBattlePass(nullptr);

	_passBanner->SetVisibility(ESlateVisibility::Collapsed);

	if (_exposeBattlePasses.IsValidIndex(index))
	{
		_selectedPass = _exposeBattlePasses[index];
		SetSelectedBattlePass(_selectedPass);
	}
}

void UGsUIWindowBattlePass::SetSelectedBattlePass(TWeakPtr<FGsBattlePassData> data)
{
	if (auto battlePassData = data.Pin())
	{
		_passNameText = battlePassData->GetPassName();
		_passPeriodText = FText::FromString(battlePassData->GetPassAvailablePeriod());
		_passDescText = battlePassData->GetPassDesc();
		_btnLevelUp->SetIsEnabled(battlePassData->IsLastLevel() == false);

		if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(battlePassData->GetExpIconPath()))
		{
			_passExpIcon.SetResourceObject(loadObject);
		}

		auto bannerPath = battlePassData->GetPassBannerPath();
		if (bannerPath.ToString().IsEmpty() == false)
		{
			_passBanner->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (UObject* banner = UAssetManager::GetStreamableManager().LoadSynchronous(bannerPath))
			{
				_passBanner->Brush.SetResourceObject(banner);
			}
		}

		if (auto curlevel = battlePassData->GetProgressLevelData().Pin())
		{
			_levelText = FText::AsNumber(curlevel->GetLevel());

			auto completePoint = curlevel->GetNeedOpenExp();
			if (completePoint > 0)
			{
				_passProgress->SetPercent(FMath::Clamp(float(curlevel->GetExp()) / float(completePoint), 0.f, 1.f));
				_passExpText = FText::FromString(FString::Format(TEXT("{0}/{1}"), { curlevel->GetExp(), completePoint }));
			}

			if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(curlevel->IconPath()))
			{
				_passLevelIcon.SetResourceObject(loadObject);
				_passLevelIcon.SetImageSize(FVector2D(80, 80));
			}
		}
		_battlePassPage->SetSelectedBattlePass(battlePassData);
	}
}

void UGsUIWindowBattlePass::OnClickLevelUp()
{
	if (auto battlePassData = _selectedPass.Pin()) 
	{
		auto passId = battlePassData->GetBattlePassId();

		if (auto popup = Cast<UGsUIPopupBattlePassBuyLevel>((GUI()->OpenAndGetWidget("PopupBattlePassBuyLevel")).Get()))
		{
			UGsUIPopupBattlePassBuyLevel::InitParam param = { battlePassData };
			popup->SetInit(&param, [passId](bool isOK, bool isEnough, Level level) {
				if (isOK)
				{
					FGsNetSendServiceWorld::SendReqBuyBattlePassLevel(passId, level);
				}
			});
		}
	}
}

void UGsUIWindowBattlePass::UpdateBattlePass(const FGsBattlePassMsgParamPass* param)
{
	auto passId = param->_passId; 

	if (_selectedPass.IsValid() && _selectedPass.Pin()->GetBattlePassId() == passId)
		_battlePassPage->SetActiveAddReward();
}

void UGsUIWindowBattlePass::AckBattlePassLevelReward(const FGsBattlePassMsgParamLevelReward* param)
{
	auto passId = param->_passId;
	if (_selectedPass.IsValid() && _selectedPass.Pin()->GetBattlePassId() == passId)
		_battlePassPage->AckReceiveReward(param->_level, param->_isAddReward);
}

void UGsUIWindowBattlePass::UpdateBattlePassLevel(const FGsBattlePassMsgParamLevel* param)
{
	auto passId = param->_passId;
	if (_selectedPass.IsValid() && _selectedPass.Pin()->GetBattlePassId() == passId)
	{
		if (auto battlePassData = _selectedPass.Pin())
		{
			_btnLevelUp->SetIsEnabled(battlePassData->IsLastLevel() == false);
			//_passProgress->SetVisibility(battlePassData->IsLastLevel() ?
			//	ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);

			if (auto curlevel = battlePassData->GetProgressLevelData().Pin())
			{
				_levelText = FText::AsNumber(curlevel->GetLevel());

				auto completePoint = curlevel->GetNeedOpenExp();
				if (completePoint > 0)
				{
					_passProgress->SetPercent(FMath::Clamp(float(curlevel->GetExp()) / float(completePoint), 0.f, 1.f));
					_passExpText = FText::FromString(FString::Format(TEXT("{0}/{1}"), { curlevel->GetExp(), completePoint }));
				}

				if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(curlevel->IconPath()))
				{
					_passLevelIcon.SetResourceObject(loadObject);
					_passLevelIcon.SetImageSize(FVector2D(80, 80));
				}
			}
		}
		_battlePassPage->UpdateBattlePassLevel();
		_tabScrollBoxHelper->RefreshAll(_exposeBattlePasses.Num());
	}
}

void UGsUIWindowBattlePass::UpdateBattlePassMission(const FGsBattlePassMsgParamMission* param)
{
	auto passId = param->_passId;
	if (_selectedPass.IsValid() && _selectedPass.Pin()->GetBattlePassId() == passId)
	{
		_battlePassPage->UpdateBattlePassMission(param->_tabId, param->_missionId);
	}
}

void UGsUIWindowBattlePass::UpdateBattlePassMissionTab(const FGsBattlePassMsgParamMissionTab* param)
{ 
	BattlePassId passId = _selectedPass.IsValid() ?_selectedPass.Pin()->GetBattlePassId() : INVALID_BATTLE_PASS_ID;
	if (auto iter = param->_tabId.FindByPredicate([passId](auto& tabPair) { return tabPair.Key == passId; }))
	{
		_battlePassPage->UpdateBattlePassMission((*iter).Key, (*iter).Value);
	}
}

void UGsUIWindowBattlePass::AckBattlePassLevelRewardAll(const FGsBattlePassMsgParamLevelRewardAll* param)
{
	auto passId = param->_passId;
	if (_selectedPass.IsValid() && _selectedPass.Pin()->GetBattlePassId() == passId)
		_battlePassPage->AckReceiveRewardAll(param->_rewardedLevel);
}

void UGsUIWindowBattlePass::OnUpdateRedDot()
{
	_tabScrollBoxHelper->RefreshAll(_exposeBattlePasses.Num());
	_battlePassPage->OnUpdateRedDot();
}

void UGsUIWindowBattlePass::OnForcedClose()
{
	auto PopupBattlePassBuyLevel = GUI()->GetWidgetByKey(TEXT("PopupBattlePassBuyLevel"));
	if (PopupBattlePassBuyLevel.IsValid()) PopupBattlePassBuyLevel->Close();

	auto PopupCommonPay = GUI()->GetWidgetByKey(TEXT("PopupCommonPay"));
	if (PopupCommonPay.IsValid()) PopupCommonPay->Close();

	auto PopupReceiveAchievementRewardAll = GUI()->GetWidgetByKey(TEXT("PopupReceiveAchievementRewardAll"));
	if (PopupReceiveAchievementRewardAll.IsValid()) PopupReceiveAchievementRewardAll->Close();

	Close(true);
}

bool UGsUIWindowBattlePass::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsBattlePass))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_BATTLEPASS);
		return true;
	}

	return false;
}