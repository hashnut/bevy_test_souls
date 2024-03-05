#include "GsUIPopupScanOption.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Popup/PartySetting/GsUIPartySettingTab.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"

#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Scan/GsScanDefine.h"

#include "UMG/Public/Components/PanelWidget.h"

void UGsUIPopupScanOption::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupScanOption::OnClickClose);
	// 툴팁 click으로하면 다시 켜져서
	// press로
	_btnTooltip->OnPressed.AddDynamic(this, &UGsUIPopupScanOption::OnClickTooltip);

	// boss type on/off
	_toggleBossTargetOnOff->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnBossTypeOnOff(In_selected);
		});

	// 보스 토글
	FText findTextBossToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_Boss"), findTextBossToggle);

	_toggleSelBossType->SetText(findTextBossToggle);
	_toggleSelBossType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectBossType(In_selected);
		});

	// named toggle
	FText findTextNamedToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_Named"), findTextNamedToggle);

	_toggleSelNamedType->SetText(findTextNamedToggle);
	_toggleSelNamedType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectNamedType(In_selected);
		});


	// user type on/off
	_toggleUserTargetOnOff->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnUserTypeOnOff(In_selected);
		});

	// attacker remote toggle
	FText findTextAttackerRemoteToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_inBattleUser"), findTextAttackerRemoteToggle);

	_toggleSelAttackerRemoteType->SetText(findTextAttackerRemoteToggle);
	_toggleSelAttackerRemoteType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectAttackerRemoteType(In_selected);
		});

	// enemy guild remote toggle
	FText findTextEnemyGuildToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_EnemyGuild"), findTextEnemyGuildToggle);

	_toggleSelEnemyGuildType->SetText(findTextEnemyGuildToggle);
	_toggleSelEnemyGuildType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectEnemyGuildType(In_selected);
		});

	// alert user remote toggle
	FText findTextAlertTargetToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_AlertUser"), findTextAlertTargetToggle);

	_toggleSelAlertTargetType->SetText(findTextAlertTargetToggle);
	_toggleSelAlertTargetType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectAlertTargetType(In_selected);
		});


	// other server remote toggle
	FText findTextOtherServerToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_OtherServer"), findTextOtherServerToggle);

	_toggleSelOtherServerType->SetText(findTextOtherServerToggle);
	_toggleSelOtherServerType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectOtherServerType(In_selected);
		});

	// choas remote toggle
	FText findTextChaosToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_Chaotic"), findTextChaosToggle);

	_toggleSelChaosType->SetText(findTextChaosToggle);
	_toggleSelChaosType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectChaosType(In_selected);
		});


	// 유저 토글
	FText findTextRemoteToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_NormalUser"), findTextRemoteToggle);

	_toggleSelNormalRemoteType->SetText(findTextRemoteToggle);
	_toggleSelNormalRemoteType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectRemoteType(In_selected);
		});


	// monster type on/off
	_toggleMonsterTargetOnOff->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnMonsterTypeOnOff(In_selected);
		});

	// attacker monster toggle
	FText findTextAttackerMonsterToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_inBattleMonster"), findTextAttackerMonsterToggle);

	_toggleSelAttackerMonsterType->SetText(findTextAttackerMonsterToggle);
	_toggleSelAttackerMonsterType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectAttackerMonsterType(In_selected);
		});

	// quest monster toggle
	FText findTextQuestMonsterToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_QuestMonster"), findTextQuestMonsterToggle);

	_toggleSelQuestMonsterType->SetText(findTextQuestMonsterToggle);
	_toggleSelQuestMonsterType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectQuestMonsterType(In_selected);
		});

	// 몬스터 토글
	FText findTextNormalMonsterToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_NormalMonster"), findTextNormalMonsterToggle);

	_toggleSelNormalMonsterType->SetText(findTextNormalMonsterToggle);
	_toggleSelNormalMonsterType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectNormalMonsterType(In_selected);
		});


	// etc type on/off
	_toggleETCTargetOnOff->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnETCTypeOnOff(In_selected);
		});

	// treasure monster toggle
	FText findTextTreasureMonsterToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_TreasureMonster"), findTextTreasureMonsterToggle);

	_toggleSelTreasureMonsterType->SetText(findTextTreasureMonsterToggle);
	_toggleSelTreasureMonsterType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectTreasureMonsterType(In_selected);
		});

	// camp toggle
	FText findTextCampToggle;
	FText::FindText(TEXT("ScanText"), TEXT("Target_CampTower"), findTextCampToggle);

	_toggleSelCampType->SetText(findTextCampToggle);
	_toggleSelCampType->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectCampType(In_selected);
		});


	_toggleAutoRefreashScan->SetToggleCallback([this](int32 In_index, bool In_selected)
		{
			OnSelectAutoScanType(In_selected);
		});


	// 시간 선택 그룹 초기화
	_toggleGroup.AddToggleWidgetByParentPanel(_panelSelectTime);
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupScanOption::OnClickSelectTime);

	// 시간 선택1
	FText findTextTime1;
	FText::FindText(TEXT("ScanText"), TEXT("Second_1"), findTextTime1);

	_radioButtonSelTime_1->SetText(findTextTime1);

	// 시간 선택2
	FText findTextTime2;
	FText::FindText(TEXT("ScanText"), TEXT("Second_2"), findTextTime2);

	_radioButtonSelTime_2->SetText(findTextTime2);

	// 시간 선택3
	FText findTextTime3;
	FText::FindText(TEXT("ScanText"), TEXT("Second_3"), findTextTime3);

	_radioButtonSelTime_3->SetText(findTextTime3);

	FString onText = TEXT("On");
	FString offText = TEXT("Off");

	_toggleAutoRefreashScan->SetText(FText::FromString(onText), FText::FromString(offText));
	

	// 툴팁 텍스트 세팅
	FText findTextTooltipDesc;
	FText::FindText(TEXT("ScanText"), TEXT("ScanOptionGuide"), findTextTooltipDesc);

	_tooltipInfo->SetDescText(findTextTooltipDesc);
	_tooltipInfo->Close();

}

void UGsUIPopupScanOption::NativeConstruct()
{
	Super::NativeConstruct();
	// 옵션에서 로드해오기
	// 0. group 
	bool groupBossCheck = GSGameObject()->GetScanGroupTargetType(EGsScanOptionGroupTargetType::Boss);
	_toggleBossTargetOnOff->SetIsSelected(groupBossCheck);
	bool groupRemoteCheck = GSGameObject()->GetScanGroupTargetType(EGsScanOptionGroupTargetType::Remote);
	_toggleUserTargetOnOff->SetIsSelected(groupRemoteCheck);
	bool groupMonsterCheck = GSGameObject()->GetScanGroupTargetType(EGsScanOptionGroupTargetType::Monster);
	_toggleMonsterTargetOnOff->SetIsSelected(groupMonsterCheck);
	bool groupETCCheck = GSGameObject()->GetScanGroupTargetType(EGsScanOptionGroupTargetType::ETC);
	_toggleETCTargetOnOff->SetIsSelected(groupETCCheck);

	// 1. 탐지 대상 값 세팅
	// boss
	bool bossCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::Boss);
	_toggleSelBossType->SetIsSelected(bossCheck);
	bool namedCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::Named);
	_toggleSelNamedType->SetIsSelected(namedCheck);

	// remote
	bool attackerRemoteCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::AttackerRemote);
	_toggleSelAttackerRemoteType->SetIsSelected(attackerRemoteCheck);
	bool enemyGuildCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::EnemyGuild);
	_toggleSelEnemyGuildType->SetIsSelected(enemyGuildCheck);
	bool alertTargetCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::AlertTarget);
	_toggleSelAlertTargetType->SetIsSelected(alertTargetCheck);
	bool otherServerCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::OtherServerRemote);
	_toggleSelOtherServerType->SetIsSelected(otherServerCheck);
	bool chaosCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::Chaos);
	_toggleSelChaosType->SetIsSelected(chaosCheck);
	bool normalRemoteCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::NormalRemote);
	_toggleSelNormalRemoteType->SetIsSelected(normalRemoteCheck);

	// monster
	bool attackerMonsterCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::AttackerMonster);
	_toggleSelAttackerMonsterType->SetIsSelected(attackerMonsterCheck);
	bool questMonsterCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::QuestMonster);
	_toggleSelQuestMonsterType->SetIsSelected(questMonsterCheck);
	bool normalMonsterCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::NormalMonster);
	_toggleSelNormalMonsterType->SetIsSelected(normalMonsterCheck);

	// ETC
	bool treasureMonsterCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::TreasureMonster);
	_toggleSelTreasureMonsterType->SetIsSelected(treasureMonsterCheck);
	bool campCheck = GSGameObject()->GetScanDetectType(EGsScanOptionTargetType::Camp);
	_toggleSelCampType->SetIsSelected(campCheck);
	

	// 2. 자동 스캔 값 세팅	
	bool autoScanCheck = GSGameObject()->GetIsAutoScan();
	_toggleAutoRefreashScan->SetIsSelected(autoScanCheck);

	// 3. 타이머 값 세팅
	int autoScanIndex = GSGameObject()->GetScanTimmerIndex();
	_toggleGroup.SetSelectedIndex(autoScanIndex);

	
}
void UGsUIPopupScanOption::NativeDestruct()
{
	_tooltipInfo->Close();
	if(UGsGameObjectManager* gameObjectManager = GSGameObject())
	{ 
		gameObjectManager->SaveScanOption();
	}
	
	Super::NativeDestruct();
}
// 닫기 클릭
void UGsUIPopupScanOption::OnClickClose()
{
	Close();
}

// 시간 클릭
void UGsUIPopupScanOption::OnClickSelectTime(int32 In_index)
{
	GSGameObject()->SetScanTimmerIndex(In_index);
}

// 보스 타입 선택
void UGsUIPopupScanOption::OnSelectBossType(bool In_isSelect)
{
	_toggleSelBossType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::Boss);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::Boss);
	}
}

// named type select
void UGsUIPopupScanOption::OnSelectNamedType(bool In_isSelect)
{
	_toggleSelNamedType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::Named);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::Named);
	}
}
// 타유저 타입 선택
void UGsUIPopupScanOption::OnSelectRemoteType(bool In_isSelect)
{
	_toggleSelNormalRemoteType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::NormalRemote);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::NormalRemote);
	}
}
// 몬스터 타입 선택
void UGsUIPopupScanOption::OnSelectNormalMonsterType(bool In_isSelect)
{
	_toggleSelNormalMonsterType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::NormalMonster);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::NormalMonster);
	}
}
// 자동 스캔 선택
void UGsUIPopupScanOption::OnSelectAutoScanType(bool In_isSelect)
{
	_toggleAutoRefreashScan->SetIsSelected(In_isSelect);

	GSGameObject()->SetIsAutoScan(In_isSelect);

	FGsUIMsgParamBool param(In_isSelect);
	GMessage()->GetUI().SendMessage(MessageUI::AUTO_SCAN_ACTIVE, &param);
}

// boss type on/off(boss, named)
void UGsUIPopupScanOption::OnBossTypeOnOff(bool In_isSelect)
{
	_toggleBossTargetOnOff->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanGroupTargetType(EGsScanOptionGroupTargetType::Boss);
	}
	else
	{
		GSGameObject()->RemoveScanGroupTargetType(EGsScanOptionGroupTargetType::Boss);
	}
}

// 툴팁 클릭
void UGsUIPopupScanOption::OnClickTooltip()
{
	_tooltipInfo->ToggleOpenClose();
}

// user type on/off(attacker Remote, enemy guild, alert target, chaos, normal remote)
void UGsUIPopupScanOption::OnUserTypeOnOff(bool In_isSelect)
{
	_toggleUserTargetOnOff->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanGroupTargetType(EGsScanOptionGroupTargetType::Remote);
	}
	else
	{
		GSGameObject()->RemoveScanGroupTargetType(EGsScanOptionGroupTargetType::Remote);
	}
}

// attacker remote type select
void UGsUIPopupScanOption::OnSelectAttackerRemoteType(bool In_isSelect)
{
	_toggleSelAttackerRemoteType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::AttackerRemote);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::AttackerRemote);
	}
}

// enemy guild type select
void UGsUIPopupScanOption::OnSelectEnemyGuildType(bool In_isSelect)
{
	_toggleSelEnemyGuildType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::EnemyGuild);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::EnemyGuild);
	}
}
// alert target type select
void UGsUIPopupScanOption::OnSelectAlertTargetType(bool In_isSelect)
{
	_toggleSelAlertTargetType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::AlertTarget);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::AlertTarget);
	}
}
// other server target type select
void UGsUIPopupScanOption::OnSelectOtherServerType(bool In_isSelect)
{
	_toggleSelOtherServerType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::OtherServerRemote);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::OtherServerRemote);
	}
}
// chaos type select
void UGsUIPopupScanOption::OnSelectChaosType(bool In_isSelect)
{
	_toggleSelChaosType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::Chaos);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::Chaos);
	}
}

// monster type on/off(attacker Monster, questMonster, normalMonster)
void UGsUIPopupScanOption::OnMonsterTypeOnOff(bool In_isSelect)
{
	_toggleMonsterTargetOnOff->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanGroupTargetType(EGsScanOptionGroupTargetType::Monster);
	}
	else
	{
		GSGameObject()->RemoveScanGroupTargetType(EGsScanOptionGroupTargetType::Monster);
	}
}

// attacker monster type select
void UGsUIPopupScanOption::OnSelectAttackerMonsterType(bool In_isSelect)
{
	_toggleSelAttackerMonsterType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::AttackerMonster);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::AttackerMonster);
	}
}
// quest monster type select
void UGsUIPopupScanOption::OnSelectQuestMonsterType(bool In_isSelect)
{
	_toggleSelQuestMonsterType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::QuestMonster);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::QuestMonster);
	}
}

// etc type on/off(tresure monster, camp)
void UGsUIPopupScanOption::OnETCTypeOnOff(bool In_isSelect)
{
	_toggleETCTargetOnOff->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanGroupTargetType(EGsScanOptionGroupTargetType::ETC);
	}
	else
	{
		GSGameObject()->RemoveScanGroupTargetType(EGsScanOptionGroupTargetType::ETC);
	}
}

// treasure monster type select
void UGsUIPopupScanOption::OnSelectTreasureMonsterType(bool In_isSelect)
{
	_toggleSelTreasureMonsterType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::TreasureMonster);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::TreasureMonster);
	}
}

// camp type select
void UGsUIPopupScanOption::OnSelectCampType(bool In_isSelect)
{
	_toggleSelCampType->SetIsSelected(In_isSelect);

	if (In_isSelect == true)
	{
		GSGameObject()->AddScanDetectType(EGsScanOptionTargetType::Camp);
	}
	else
	{
		GSGameObject()->RemoveScanDetectType(EGsScanOptionTargetType::Camp);
	}
}

// 2022/10/20 PKT - KeyBoard Mapping
void UGsUIPopupScanOption::OnInputCancel()
{
	OnClickClose();
}