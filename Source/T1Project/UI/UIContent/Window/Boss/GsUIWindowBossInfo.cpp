#include "GsUIWindowBossInfo.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Net/GsNetSendServiceBoss.h"
#include "DataCenter/Public/DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataCenter/Public/DataSchema/WorldBoss/GsSchemaWorldBossData.h"

void UGsUIWindowBossInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//window button
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowBossInfo::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowBossInfo::OnClickCloseAllStack);

	_guidButton->OnClicked.AddDynamic(this, &UGsUIWindowBossInfo::OnOpenGuid);
	_guidCloseButton->OnClicked.AddDynamic(this, &UGsUIWindowBossInfo::OnCloseGuid);

	//토글 클릭시 보스 윈도우 갱신
	_toggleGroupMainTab.AddToggleWidgetByParentPanel(_panelMainTab);
	_toggleGroupMainTab.OnSlotClicked.BindUObject(this, &UGsUIWindowBossInfo::OnClickMainTab);
}

void UGsUIWindowBossInfo::NativeConstruct()
{
	Super::NativeConstruct();

	//bind message
	if (FGsMessageHolder* msg = GMessage())
	{
		//보스 윈도우 갱신 콜백
		_uiDelegates.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_BOSS
			, this, &UGsUIWindowBossInfo::OnSelectBoss));
	}

	_lastTab = EGsBossTab::None;

	OnCloseGuid();

	//되돌아가기 복구 로직
	if (IsStackRecoverFlag())
	{
		FGsGameStateBoss::GetInstance()->RecoverySelectBoss();
	}
}

void UGsUIWindowBossInfo::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_bossDelegates.Num() > 0)
		{
			for (auto iter : _bossDelegates)
			{
				msg->GetBoss().Remove(iter);
			}
			_bossDelegates.Empty();
		}

		if (_uiDelegates.Num() > 0)
		{
			for (auto iter : _uiDelegates)
			{
				msg->GetUI().Remove(iter);
			}
			_uiDelegates.Empty();
		}
	}

	Super::NativeDestruct();
}

//되돌아가기 복구 로직
bool UGsUIWindowBossInfo::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsBoss))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_BOSS);
		FGsGameStateBoss* bossState = FGsGameStateBoss::GetInstance();
		if (EGsBossTab::FieldBoss == bossState->GetSavedBossType())
		{
			//되돌아가기 복구시 마지막 처치 시간 갱신
			const FGsSchemaFieldBossData* selectBossData = bossState->GetSelectedBossData();
			if (selectBossData)
			{				
				bossState->SelectedFieldBoss(selectBossData->bossId);				
			}
		}		
		else
		{
			const FGsSchemaWorldBossData* worldBossData = bossState->GetSelectedWorldBossData();
			if (worldBossData)
			{
				bossState->SelectedWorldBoss(worldBossData->id);
			}			
		}
		return true;
	}

	return false;
}

void UGsUIWindowBossInfo::OnSelectBoss(const struct IGsMessageParam* inParam)
{
	const FGsUIMsgParamSelectBoss* param = inParam->Cast<const FGsUIMsgParamSelectBoss>();
	if (nullptr == param)
	{
		return;
	}

	_bossWidgetSwitcher->SetActiveWidgetIndex((int32)param->_bossType);

	SelectTab(param->_bossType);

	switch (param->_bossType)
	{
	case EGsBossTab::FieldBoss:
	{
		_fieldBossPanel->SelectBoss(param->_fieldBoss
			, param->_mutantFieldBoss
			, param->_isShowFieldBossMutant);
		_guidButton->SetVisibility(ESlateVisibility::Visible);
		OnCloseGuid();
	}
	break;
	case EGsBossTab::WorldBoss:
	{
		_worldBossPanel->SelectWorldBoss(param->_worldBossData, param->_leftRewardCount);
		_guidButton->SetVisibility(ESlateVisibility::Hidden);
		OnCloseGuid();
	}
	break;
	}
}

bool UGsUIWindowBossInfo::SelectTab(EGsBossTab inBossInfo)
{
	EGsBossTab prevTab = _lastTab;
	if (prevTab == inBossInfo)
	{
		return false;
	}

	_lastTab = inBossInfo;

	_toggleGroupMainTab.SetSelectedIndex((int32)inBossInfo);

	return true;
}

void UGsUIWindowBossInfo::OnClickMainTab(int32 InIndex, bool inSelect)
{
	if (inSelect)
	{
		EGsBossTab tab = static_cast<EGsBossTab>(InIndex);
		SelectTab(tab);
		switch (tab)
		{
		case EGsBossTab::FieldBoss:
		{
			_worldBossPanel->Hide();
			_fieldBossPanel->InvalidBossList();
			FGsGameStateBoss::GetInstance()->SelectFirstFieldBoss();
		}
		break;
		case EGsBossTab::WorldBoss:
		{
			FGsGameStateBoss::GetInstance()->SelectFirstWorldBoss();
		}
		break;
		}
	}
}

void UGsUIWindowBossInfo::OnOpenGuid()
{
	if (_guidPanel->GetVisibility() == ESlateVisibility::Visible)
	{
		_guidPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_guidPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGsUIWindowBossInfo::OnCloseGuid()
{
	_guidPanel->SetVisibility(ESlateVisibility::Hidden);
}
