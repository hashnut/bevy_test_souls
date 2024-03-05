// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildBuildingLevelUp.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Guild/GsGuildHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Net/GsNetSendServiceGuild.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "DataSchema/Guild/GsSchemaGuildBuildingLevelData.h"
#include "PaperSprite.h"

//----------------------------------------------------------------------------------------------------------------------
void UGsUIPopupGuildBuildingLevelUp::FGsBuildingEffectRow::SetData(const TPair<FText, FText>& InData)
{
	_textBlockName->SetText(InData.Key);
	_textBlockValue->SetText(InData.Value);
}

void UGsUIPopupGuildBuildingLevelUp::FGsBuildingEffectRow::SetEmpty()
{
	_textBlockName->SetText(FText::GetEmpty());
	_textBlockValue->SetText(FText::GetEmpty());
}

void UGsUIPopupGuildBuildingLevelUp::FGsBuildingEffectRow::SetColorValue(const FLinearColor& InColor)
{
	_textBlockValue->SetColorAndOpacity(InColor);
}
//----------------------------------------------------------------------------------------------------------------------
void UGsUIPopupGuildBuildingLevelUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildBuildingLevelUp::OnClickCancel);
	_btnLevelUp->OnClickCurrencyButton.BindUObject(this, &UGsUIPopupGuildBuildingLevelUp::OnClickLevelUp);
	_btnTooltip->OnClicked.AddDynamic(this, &UGsUIPopupGuildBuildingLevelUp::OnClickTooltip);

	_currEffectList.Empty();
	_currEffectList.Emplace(FGsBuildingEffectRow(_textBlockCurrEffectName0, _textBlockCurrEffectValue0));
	_currEffectList.Emplace(FGsBuildingEffectRow(_textBlockCurrEffectName1, _textBlockCurrEffectValue1));

	_nextEffectList.Empty();
	_nextEffectList.Emplace(FGsBuildingEffectRow(_textBlockNextEffectName0, _textBlockNextEffectValue0));
	_nextEffectList.Emplace(FGsBuildingEffectRow(_textBlockNextEffectName1, _textBlockNextEffectValue1));
}

void UGsUIPopupGuildBuildingLevelUp::NativeConstruct()
{
	Super::NativeConstruct();

	FGsMessageHolder* messageMgr = GMessage();

	MGuild& msgGuild = messageMgr->GetGuild();
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_BUILDING_UPDATE,
		this, &UGsUIPopupGuildBuildingLevelUp::OnReceiveBuildingUpgrade));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_CHANGE_MY_AUTHORITY,
		this, &UGsUIPopupGuildBuildingLevelUp::OnReceiveChangeMyAuthority));	
	_msgUserInfoHandle = messageMgr->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUIPopupGuildBuildingLevelUp::OnChangeCurrency);
}

void UGsUIPopupGuildBuildingLevelUp::NativeDestruct()
{
	_guildBuildingId = 0;
	_currBuildingLevel = 0;
	_needCost = 0;

	FGsMessageHolder* messageMgr = GMessage();

	MGuild& msgGuild = messageMgr->GetGuild();
	for (MsgGuildHandle& msgHandle : _msgGuildHandleList)
	{
		msgGuild.Remove(msgHandle);
	}
	_msgGuildHandleList.Empty();

	messageMgr->GetUserBasicInfo().Remove(_msgUserInfoHandle);

	Super::NativeDestruct();
}

void UGsUIPopupGuildBuildingLevelUp::OnInputOk()
{
	Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GUILD_ASSET);
	if (myAmount < _needCost)
	{
		Close();
		FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GUILD_ASSET, _needCost - myAmount);

		return;
	}

	if (0 == _currBuildingLevel)
	{
		FGsNetSendServiceGuild::SendReqGuildBuildingOpen(_guildBuildingId);
	}
	else
	{
		FGsNetSendServiceGuild::SendReqGuildBuildingUpgrade(_guildBuildingId);
	}

	// 건설 시간이 있으므로 연달아 진행할 수 없음. 바로 닫는다.
	Close();
}

 void UGsUIPopupGuildBuildingLevelUp::OnInputCancel()
{
	 Close();
}

void UGsUIPopupGuildBuildingLevelUp::SetData(GuildBuildingId InId)
{
	_guildBuildingId = InId;

	const FGsSchemaGuildBuilding* tableData = FGsGuildHelper::GetGuildBuilding(InId);
	if (nullptr == tableData)
	{
		Close();
		return;
	}

	// 아이콘 표시
	_imgIcon->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(tableData->iconPath));

	FGsGuildManager* guildMgr = GGuild();
	const FGsGuildBuildingData* buildingData = guildMgr->GetBuildingData(tableData->id);
	if (buildingData)
	{
		_currBuildingLevel = buildingData->GetCurrBuildingLevel();
	}
	else
	{
		_currBuildingLevel = tableData->initialLevel;
	}

	// 타이틀 표시
	FString strTitle = FString::Format(TEXT("Lv.{0} {1}"), { _currBuildingLevel, *tableData->nameText.ToString() });
	_textBlockTitle->SetText(FText::FromString(strTitle));

	// 현재레벨 정보 찍기
	TArray<TPair<FText, FText>> currTextList;
	FGsGuildHelper::GetBuildingEffectStringList(tableData, _currBuildingLevel, currTextList);
	for (int i = 0; i < _currEffectList.Num(); ++i)
	{
		if (currTextList.IsValidIndex(i))
		{
			_currEffectList[i].SetData(currTextList[i]);
		}
		else
		{
			_currEffectList[i].SetEmpty();
		}
	}

	// 다음 레벨 정보 찍기
	TArray<TPair<FText, FText>> nextTextList;
	FGsGuildHelper::GetBuildingEffectStringList(tableData, _currBuildingLevel + 1, nextTextList);
	for (int i = 0; i < _nextEffectList.Num(); ++i)
	{
		if (nextTextList.IsValidIndex(i))
		{
			_nextEffectList[i].SetData(nextTextList[i]);

			if (currTextList.IsValidIndex(i))
			{
				if (currTextList[i].Value.ToString() == nextTextList[i].Value.ToString())
				{
					_nextEffectList[i].SetColorValue(FLinearColor::White);
				}
				else
				{
					_nextEffectList[i].SetColorValue(_colorChangedEffectValue);
				}
			}
		}
		else
		{
			_nextEffectList[i].SetEmpty();
		}
	}

	// 길드 운영비 표시
	_textBlockGuildAsset->SetText(FText::AsNumber(FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GUILD_ASSET)));
	
	_bIsNextLevel = false;
	int32 needGuildLevel = 0;
	bool bCanLevelUp = false;
	const FGsSchemaGuildBuildingLevelData* nextLevelData = FGsGuildHelper::GetBuildingLevelData(tableData, 
		_currBuildingLevel + 1);
	if (nextLevelData)
	{
		_bIsNextLevel = true;
		needGuildLevel = nextLevelData->needLevel;
		_needCost = nextLevelData->levelUpCost;

		if (guildMgr->GetGuildLevel() >= needGuildLevel)
		{
			bCanLevelUp = true;
		}
	}

	// 다음 레벨을 위한 필요 정보 표시
	if (bCanLevelUp)
	{
		_textBlockNeedGuildLevel->SetText(FText::GetEmpty());
	}
	else
	{
		// TEXT: 다음 레벨을 위해 기사단 레벨 {0} 필요
		FText textNeedLevel;
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("UI_Building_NeedLevel"), textFormat))
		{
			textNeedLevel = FText::Format(textFormat, needGuildLevel);
		}
		_textBlockNeedGuildLevel->SetText(textNeedLevel);
	}

	// 버튼 정보 표시
	FText textBtnName;
	if (0 == _currBuildingLevel)
	{
		// TEXT: 건설
		FText::FindText(TEXT("GuildText"), TEXT("UI_Button_Build"), textBtnName);
	}
	else
	{
		// TEXT: 레벨업
		FText::FindText(TEXT("GuildText"), TEXT("UI_Button_LevelUp"), textBtnName);
	}
	_btnLevelUp->SetButtonName(textBtnName);
	_btnLevelUp->SetData(CurrencyType::GUILD_ASSET, _needCost);
	_btnLevelUp->UpdateAmountTextColorLocalPlayer();
	_btnLevelUp->SetIsEnabled(bCanLevelUp);

	// B2: 캠프에서 사용하던 툴팁 버튼 비활성화. 추후 필요시 활성하여 활용
	_btnTooltip->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPopupGuildBuildingLevelUp::OnReceiveBuildingUpgrade(const IGsMessageParam*)
{
	// TEXT: 기사단 건물 정보가 변경되었습니다.
	FText textMsg;
	FText::FindText(TEXT("GuildText"), TEXT("Alert_BuildingChange"), textMsg);	
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

	Close();
}

void UGsUIPopupGuildBuildingLevelUp::OnReceiveChangeMyAuthority(const IGsMessageParam*)
{
	// 창 열고있는데, 권한이 없어졌으면 강제로 닫는다.
	if (false == FGsGuildHelper::CanManageBuilding(GGuild()->GetAuthority()))
	{
		Close();
	}
}

void UGsUIPopupGuildBuildingLevelUp::OnChangeCurrency(uint64 InCurrencyType)
{
	CurrencyType currencyType = static_cast<CurrencyType>(InCurrencyType);
	if (CurrencyType::GUILD_ASSET == currencyType)
	{
		_btnLevelUp->UpdateAmountTextColorLocalPlayer();
	}
}

void UGsUIPopupGuildBuildingLevelUp::OnClickLevelUp()
{
	OnInputOk();
}

void UGsUIPopupGuildBuildingLevelUp::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupGuildBuildingLevelUp::OnClickTooltip()
{
	// B2: 캠프가 건물에서 제거되어 안내용 툴팁 사용 안함. 추후 필요 시 활용
}
