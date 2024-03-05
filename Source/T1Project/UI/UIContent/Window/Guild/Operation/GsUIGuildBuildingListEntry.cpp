// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildBuildingListEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsGrayscaleImage.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupGuildBuildingLevelUp.h" 
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "EffectText/GsEffectTextDefine.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "DataSchema/Guild/GsSchemaGuildBuildingLevelData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildBuildingData.h"
#include "Guild/GsGuildHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "PaperSprite.h"


void UGsUIGuildBuildingListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnLevelUp->OnClickCurrencyButton.BindUObject(this, &UGsUIGuildBuildingListEntry::OnClickLevelUp);
}

void UGsUIGuildBuildingListEntry::OnHide()
{
	_tableData = nullptr;
	_bShowLevelUpTime = false;
	_checkRemainSec = 0.f;	
	_upgradeEndSec = 0;
	_needLevelUpCost = 0;
	_bIsAuthority = false;
}

void UGsUIGuildBuildingListEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_bShowLevelUpTime)
	{
		_checkRemainSec -= InDeltaTime;
		if (0 >= _checkRemainSec)
		{
			SetUIUpgradeRemainTime();
			_checkRemainSec = TIME_UPDATE_SEC;
		}
	}
}

void UGsUIGuildBuildingListEntry::SetData(const FGsSchemaGuildBuilding* InData, bool bIsAuthority, bool IsMyGuild)
{
	_tableData = InData;
	_bIsAuthority = _bIsMyGuild ? bIsAuthority : false;
	_bIsMyGuild = IsMyGuild;

	InvalidateAll();
}

void UGsUIGuildBuildingListEntry::OnClickLevelUp()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildBuildingLevelUp"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildBuildingLevelUp* popup = Cast<UGsUIPopupGuildBuildingLevelUp>(widget.Get()))
		{
			popup->SetData(_tableData->id);
		}
	}
}

void UGsUIGuildBuildingListEntry::InvalidateAll()
{
	// 아이콘과 이름
	_imgIcon->SetPaperSprite(FGsUIHelper::LoadSpriteFromPath(_tableData->iconPath));
	_textBlockName->SetText(_tableData->nameText);
	
	GuildBuildingStateType buildingState = GuildBuildingStateType::GUILD_BUILDING_STATE_NONE;
	int32 currBuildingLevel = 0;
	_upgradeEndSec = 0;

	FGsGuildManager* guildMgr = GGuild();
	const FGsGuildBuildingData* buildingData = guildMgr->GetBuildingData(_tableData->id, _bIsMyGuild);
	if (buildingData)
	{
		buildingState = buildingData->_buildingState;
		if (GuildBuildingStateType::GUILD_BUILDING_STATE_OPEN == buildingState)
		{
			currBuildingLevel = _tableData->initialLevel;
		}
		else
		{
			currBuildingLevel = buildingData->GetCurrBuildingLevel();
		}

		_upgradeEndSec = buildingData->_upgradeFinishTime;
	}
	else
	{
		currBuildingLevel = _tableData->initialLevel;
	}

	if (const FGsSchemaGuildBuildingLevelData* currLevelData = 
		FGsGuildHelper::GetBuildingLevelData(_tableData, currBuildingLevel))
	{
		_maxLevelUpSec = currLevelData->levelUpSecond;
	}
	else
	{
		_maxLevelUpSec = 0;
	}

	// 건물 레벨 표시
	FString strLevel = FString::Format(TEXT("Lv.{0}"), { currBuildingLevel });
	_textBlockLevel->SetText(FText::FromString(strLevel));

	// 효과 텍스트 표시
	FText textEffect;
	GetBuildingEffectString(_tableData, currBuildingLevel, textEffect);
	_textBlockEffect->SetText(textEffect);

	bool bCanLevelUp = false;
	int32 currLevel = 0;
	int32 needLevel = 0;
	_needLevelUpCost = 0;

	bool bIsNextLevel = GetLevelData(currBuildingLevel, currLevel, needLevel, _needLevelUpCost);

	if (currLevel >= needLevel)
	{
		bCanLevelUp = true;
	}

	SetUINeedLevel(needLevel, _tableData->guildStatType, bCanLevelUp);
	
	// 건설/레벨업, 건설중/레벨업중을 구분하기 위해 isFirst체크	
	bool bIsFirst = false;
	if (0 == currBuildingLevel)
	{
		bIsFirst = true;
	}
	else
	{
		if(GuildBuildingStateType::GUILD_BUILDING_STATE_OPEN == buildingState)
		{
			bIsFirst = true;
		}
	}

	_imgIcon->SetGrayscale(bIsFirst);

	if (_bIsMyGuild)
	{
		_switcherLevelUp->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// 버튼표시
		if (GuildBuildingStateType::GUILD_BUILDING_STATE_NONE == buildingState)
		{
			_switcherLevelUp->SetActiveWidgetIndex(0);

			_bShowLevelUpTime = false;
			_checkRemainSec = 0.f;

			// 다음 레벨을 못찾으면 최대 레벨로 간주
			SetUILevelUpButton(bIsFirst, !bIsNextLevel);

			// 버튼 활성화 여부
			_btnLevelUp->SetIsEnabled(bCanLevelUp);
		}
		else
		{
			_switcherLevelUp->SetActiveWidgetIndex(1);

			_bShowLevelUpTime = true;
			_checkRemainSec = TIME_UPDATE_SEC; // 1초에 한번씩 했더니 오차가 많이나서 좀더 당김

			SetUIUpgradeState(bIsFirst);
		}
	}
	else
	{
		_switcherLevelUp->SetVisibility(ESlateVisibility::Hidden);

		_bShowLevelUpTime = false;
		_checkRemainSec = 0.f;
	}

	_btnLevelUp->SetVisibility(_bIsAuthority ? ESlateVisibility::Visible: ESlateVisibility::Hidden);

	// 권한 없으면서, 건설 전이면 dimmed
	bool bIsDimmed = false;
	if (false == _bIsAuthority)
	{
		if (bIsFirst)
		{
			bIsDimmed = true;
		}
	}
	_imgDimmed->SetVisibility(bIsDimmed ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	// 레드닷 체크
	_uiRedDot->SetRedDot(guildMgr->IsRedDotBuilding(_tableData->id), EGsIconRedDotType::NORMAL);
}

void UGsUIGuildBuildingListEntry::SetUINeedLevel(int32 InNeedLevel, GuildStatType InStatType, bool InCanLevelUp)
{
	// 필요 레벨 텍스트 표시
	FText textNeedLevel;
	if (false == InCanLevelUp)
	{
		if (0 < InNeedLevel)
		{
			if (GuildStatType::GUILD_DUNGEON_BUFF == InStatType)
			{
				// TEXT: 다음 레벨을 위해 기사단 던전 {0}단계 클리어 필요
				FText textFormat;
				if (FText::FindText(TEXT("GuildText"), TEXT("UI_Building_NeedDungeonLevel"), textFormat))
				{
					textNeedLevel = FText::Format(textFormat, InNeedLevel);
				}
			}
			else
			{
				// TEXT: 다음 레벨을 위해 기사단 레벨 {0} 필요
				FText textFormat;
				if (FText::FindText(TEXT("GuildText"), TEXT("UI_Building_NeedLevel"), textFormat))
				{
					textNeedLevel = FText::Format(textFormat, InNeedLevel);
				}
			}
		}
	}
	_textBlockNeedGuildLevel->SetText(textNeedLevel);
}

void UGsUIGuildBuildingListEntry::SetUILevelUpButton(bool bInIsFirst, bool bInIsMaxLevel)
{
	FText textButtonName;
	if (bInIsMaxLevel)
	{
		_btnLevelUp->SetButtonEnable(false);

		// TEXT: 최대 레벨
		FText::FindText(TEXT("GuildText"), TEXT("MaxLevel"), textButtonName);
	}
	else
	{
		_btnLevelUp->SetButtonEnable(true);

		if (bInIsFirst)
		{
			// TEXT: 건설
			FText::FindText(TEXT("GuildText"), TEXT("UI_Button_Build"), textButtonName);
		}
		else
		{
			// TEXT: 레벨업
			FText::FindText(TEXT("GuildText"), TEXT("UI_Button_LevelUp"), textButtonName);
		}
	}
	
	_btnLevelUp->SetButtonName(textButtonName);

	_btnLevelUp->SetData(CurrencyType::GUILD_ASSET, _needLevelUpCost);
	//_btnLevelUp->UpdateAmountTextColorLocalPlayer(); // R2: 구매시 확인 팝업 띄우는 항목은 흰글씨 표기
}

void UGsUIGuildBuildingListEntry::SetUIUpgradeState(bool bInIsFirst)
{
	FText textUpgrade;
	if (bInIsFirst)
	{
		// TEXT: 건설중
		FText::FindText(TEXT("GuildText"), TEXT("Build_ing"), textUpgrade);
	}
	else
	{
		// TEXT: 레벨업중
		FText::FindText(TEXT("GuildText"), TEXT("LevelUp_ing"), textUpgrade);
	}
	_textBlockUpgrade->SetText(textUpgrade);

	SetUIUpgradeRemainTime();
}

void UGsUIGuildBuildingListEntry::SetUIUpgradeRemainTime()
{
	time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();
	time_t remainSec = 0;
	if (serverNowSec < _upgradeEndSec)
	{
		remainSec = _upgradeEndSec - serverNowSec;
	}
	
	// 시간이 끝나면 타이머 끝내고 갱신
	if (0 == remainSec)
	{
		_bShowLevelUpTime = false;
	}

	if (0 < _maxLevelUpSec)
	{
		if (_maxLevelUpSec < remainSec)
		{
			remainSec = _maxLevelUpSec;
		}
	}

	FString strRemainTime;
	FGsTimeStringHelper::GetTimeStringCeil(FTimespan::FromSeconds(remainSec), strRemainTime);

	_textBlockUpgradeRemainTime->SetText(FText::FromString(strRemainTime));
}

bool UGsUIGuildBuildingListEntry::GetLevelData(int32 InBuildingLevel, 
	OUT int32& OutCurrLevel, OUT int32& OutNeedLevel, OUT int32& OutNeedLevelUpCost)
{
	GuildLevel currGuildLevel = 0;
	if (const FGsGuildDataDetail* guildData = GGuild()->GetGuildDetailData(_bIsMyGuild))
	{
		if (GuildStatType::GUILD_DUNGEON_BUFF == _tableData->guildStatType)
		{
			OutCurrLevel = guildData->_dungeonBestLevel;
		}
		else
		{
			OutCurrLevel = guildData->_guildLevel;
		}
	}
	
	const FGsSchemaGuildBuildingLevelData* nextLevelData = FGsGuildHelper::GetBuildingLevelData(
		_tableData, InBuildingLevel + 1);
	if (nextLevelData)
	{
		OutNeedLevel = nextLevelData->needLevel;
		OutNeedLevelUpCost = nextLevelData->levelUpCost;

		return true;
	}

	return false;
}

bool UGsUIGuildBuildingListEntry::GetBuildingEffectString(const FGsSchemaGuildBuilding* InData, int32 InBuildingLevel,
	OUT FText& OutText)
{
	// [B1] FGsGuildHelper::GetBuildingEffectString 과 표시 내용이 달라져서 같이 사용하지 않음
	if (nullptr == InData)
	{
		OutText = FText::GetEmpty();
		return false;
	}

	if (0 == InBuildingLevel)
	{
		OutText = InData->openText;
		return true;
	}

	const FGsSchemaGuildBuildingLevelData* levelData = FGsGuildHelper::GetBuildingLevelData(InData, InBuildingLevel);
	if (nullptr == levelData)
	{
		OutText = FText::GetEmpty();
		return false;
	}

	switch (InData->guildStatType)
	{
	case GuildStatType::GUILD_USER_NUMBER:
	{
		//예: 기사단 최대인원 25
		FString strEffect = FString::Format(TEXT("{0} {1}"), {
				InData->guildStatValueText.ToString(), levelData->guildStatValue });
		OutText = FText::FromString(strEffect);
		return true;
	}
	break;
	case GuildStatType::GUILD_BUFF:
	case GuildStatType::GUILD_DUNGEON_BUFF:
	{
		// TEXT: 버프 활성 가능, 레벨업 시 버프 효과 향상
		return FText::FindText(TEXT("GuildBuildingText"), TEXT("BuildingDesc_301"), OutText);
	}
	break;
	case GuildStatType::GUILD_PASSIVE_STAT:
	{
		//예: 패시비티 스탯이나 효과 명시
		const FGsSchemaPassivitySet* passivitySet = levelData->passivityId.GetRow();
		if (passivitySet)
		{
			int32 effectTextOption = 0;
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_INVOKE_CAUSE, effectTextOption);
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_INVOKE_RATE, effectTextOption);
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_INVOKE_TARGET, effectTextOption);

			FString strEffect;
			if (FGsEffectStringHelper::GetPassivityEffectString(passivitySet->id, strEffect, effectTextOption))
			{
				// TEXT: 건물 보유 시, {0}
				FText textFormat;
				if (FText::FindText(TEXT("GuildBuildingText"), TEXT("BuildingDesc_401"), textFormat))
				{
					OutText = FText::Format(textFormat, FText::FromString(strEffect));
					return true;
				}
			}
		}
	}
	break;
	}

	return false;
}
