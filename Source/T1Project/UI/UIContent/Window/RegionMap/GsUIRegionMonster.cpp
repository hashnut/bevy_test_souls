#include "GsUIRegionMonster.h"

#include "Core/Public/Delegates/Delegate.h"
#include "Components/Image.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Paper2D/Classes/PaperSprite.h"

#include "UI/UIContent/Window/RegionMap/Monster/GsUIRegionMonsterList.h"
#include "UI/UIContent/Window/RegionMap/Monster/GsUIIconRegionMonsterMaterial.h"
#include "UI/UIContent/Window/RegionMap/Monster/GsUIRegionMonsterDropItemList.h"
#include "UI/UIContent/Window/Boss/GsUIWindowBossInfo.h"
#include "UI/UIContent/Window/NPCPortrait/GsUIMonsterPortrait.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipTitleDesc.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Map/WorldMap/GsRegionInfo.h"
#include "Map/WorldMap/GsRegionMonsterInfo.h"
#include "Map/WorldMap/GsWorldMapDefine.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/Item/GsSchemaItemMaterialData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"

#include "Message/GSMessageContents.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsUIUtil.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedBossEnum.h"
#include "DataCenter/Public/Unlock/GsUnlockDefine.h"

#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"

void UGsUIRegionMonster::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	_monsterList->OnSelectMonsterList.BindUObject(this, &UGsUIRegionMonster::OnSelectMonsterList);
	_btnMoveBossInfo->OnClicked.AddDynamic(this, &UGsUIRegionMonster::OnClickMoveBossInfo);

}
// 해당 씬 데이터로 세팅
void UGsUIRegionMonster::SetSceneData()
{
	_monsterInfoPanel->SetVisibility(ESlateVisibility::Hidden);
	_monsterList->InitRegionMonsterListData();
}

// info 정보 초기화
void UGsUIRegionMonster::InitInfo(FGsRegionMonsterInfo* In_monsterInfo)
{
	if (nullptr == In_monsterInfo)
	{
		_monsterInfoPanel->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	_monsterInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	const FGsSchemaNpcData* npcData = In_monsterInfo->_npcData;
	const FGsSchemaNpcBalance* balanceData = In_monsterInfo->_npcBalance;

	if (npcData == nullptr || 
		balanceData == nullptr)
	{
		return;
	}

	_textMonsterName = npcData->nameText;	
	_textMonsterGrade = UGsGameObjectUtil::GetRankText(npcData->grade);

	if (nullptr != In_monsterInfo)
	{
		_textMonsterOffensive = UGsGameObjectUtil::GetOffensiveText(In_monsterInfo->_isOffensive);
	}

	_monsterPortraitBP->SetData(npcData);


	// 종족 텍스트
	_textMonsterCreatureRace = UGsGameObjectUtil::GetRaceText(npcData->race);

	// 약점 재질 텍스트
	_textMonsterWeaknessMaterial = UGsGameObjectUtil::GetRaceWeaknessItemMaterialText(npcData->race);


	// 속성 텍스트
	_textMonsterElement = UGsGameObjectUtil::GetMonsterElementalText(npcData->element);

	SetFieldBossTimeInfo(In_monsterInfo->_fieldBossData);
}

// 리스트 선택
void UGsUIRegionMonster::OnSelectMonsterList(int32 In_index)
{
	if (In_index == -1)
	{
		return;
	}

	FGsRegionMonsterInfo* monsterInfo = 
		GSWorldMap()->GetCurrentRegionMonsterInfoByIndex(In_index);
	InitInfo(monsterInfo);

	_dropItemEquip->InitList(EGsRegionMonsterDropItemType::Equip, In_index);
	_dropItemEtc->InitList(EGsRegionMonsterDropItemType::Etc, In_index);

	GSWorldMap()->SetBackupSelMonsterListIndex(In_index);
}

void UGsUIRegionMonster::SetFieldBossTimeInfo(const FGsSchemaFieldBossData* In_fieldBossData)
{
	// no field boss
	if (In_fieldBossData == nullptr)
	{
		_currentFieldBossId = 0;

		_spawnTimeWidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
		_btnMoveBossInfo->SetVisibility(ESlateVisibility::Collapsed);		
	}
	// field boss
	else
	{
		_currentFieldBossId = In_fieldBossData->bossId;

		_spawnTimeWidgetSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_btnMoveBossInfo->SetVisibility(ESlateVisibility::Visible);

		EGsRegionMapMonsterSpawnTimeSwitchType switchType =
		(In_fieldBossData->bossType == BossType::COOPERATION || In_fieldBossData->bossType == BossType::COOPERATION_MUTANT)?
			EGsRegionMapMonsterSpawnTimeSwitchType::Cooperation : 
			EGsRegionMapMonsterSpawnTimeSwitchType::Competition;

		_spawnTimeWidgetSwitcher->SetActiveWidgetIndex((int)switchType);

		// 1. competition
		if (switchType == EGsRegionMapMonsterSpawnTimeSwitchType::Competition)
		{
			UGsUIUtil::SetFieldBossCompetitionSpawnInfo(In_fieldBossData, _competitionSpawnIntervalTextBlock);
		}
		//// 2. cooperation
		//else
		//{
		//	UGsUIUtil::SetFieldBossCooperationSpawnInfo(In_fieldBossData, _cooperationSpawnTimeListView);
		//}
	}
}

void UGsUIRegionMonster::OnClickMoveBossInfo()
{
	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::BOSS))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_BOSS);
	}

	FGsGameStateBoss::GetInstance()->SelectedFieldBoss(_currentFieldBossId);
}