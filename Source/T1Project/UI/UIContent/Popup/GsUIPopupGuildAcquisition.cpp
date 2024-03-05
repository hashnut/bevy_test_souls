// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildAcquisition.h"

#include "Components/ScrollBox.h"

#include "DataSchema/Guild/GsSchemaGuildAcquisition.h"

#include "Guild/GsGuildHelper.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Popup/PartySetting/GsUIPartySettingTab.h"
#include "UI/UIContent/Popup/Guild/GsUIGuildAcquisitionEntry.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Net/GsNetSendServiceGuild.h"


void UGsUIPopupGuildAcquisition::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_scrollBoxHelper->Initialize(_entryWidgetClass, _panelList);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupGuildAcquisition::OnRefreshEntryOption);
	}

	if (_btnChange)
	{
		_btnChange->OnClicked.AddDynamic(this, &UGsUIPopupGuildAcquisition::OnClickChange);
	}
	if (_btnCancel)
	{
		_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildAcquisition::OnClickCancel);
	}

	if (_toggleAcquisitionOnOff)
	{
		_toggleAcquisitionOnOff->SetToggleCallback([this](int32 InIndex, bool InIsSelected)
			{
				OnToggleAcquisition(InIsSelected);
			});
	}
}

void UGsUIPopupGuildAcquisition::NativeConstruct()
{
	Super::NativeConstruct();

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	/* 데이터 */
	const UGsTableGuildAcquisition* table = Cast<UGsTableGuildAcquisition>(FGsSchemaGuildAcquisition::GetStaticTable());
	if (nullptr == table)
		return;

	TArray<const FGsSchemaGuildAcquisition*> rows;
	if (false == table->GetAllRows(rows))
		return;

	_acquisitionInfoList.Empty();
	_cachedAcquisitionSettings.mGuildDBId = guildManager->GetGuildDBId();
	_currentAcquisitionSettings.mGuildDBId = guildManager->GetGuildDBId();

	for (const FGsSchemaGuildAcquisition* row : rows)
	{
		if (nullptr == row)
			continue;

		_acquisitionInfoList.Emplace(row);
		int32 acquisitionValue = auctionData->GetAcquisitionSetting(row->id);
		_cachedAcquisitionSettings.mAcquisitionSetting[row->id] = acquisitionValue;
		_currentAcquisitionSettings.mAcquisitionSetting[row->id] = acquisitionValue;
	}

	// 열 때 권한을 체크하고 변경되는 권한은 실시간 반영하지 않는다.
	uint32 myAuthority = guildManager->GetAuthority();
	GuildMemberGradeType myMemberGrade = guildManager->GetMemeberGrade();
	_hasAuthority = FGsGuildHelper::CanManageAcquisition(myAuthority, myMemberGrade);

	_cachedAcquisitionSettings.mAcquisition = auctionData->IsAcquisitionOn();
	_currentAcquisitionSettings.mAcquisition = auctionData->IsAcquisitionOn();

	_toggleAcquisitionOnOff->SetIsSelected(auctionData->IsAcquisitionOn());
	_toggleAcquisitionOnOff->SetIsEnabled(_hasAuthority);

	_scrollBoxHelper->RefreshAll(_acquisitionInfoList.Num());

	InvalidateChangeButton();
}

void UGsUIPopupGuildAcquisition::NativeDestruct()
{
	_acquisitionInfoList.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupGuildAcquisition::OnInputOk()
{
	if (false == _btnChange->GetIsEnabled())
		return;

	OnClickChange();
}

void UGsUIPopupGuildAcquisition::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildAcquisition::OnClickChange()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	// 캐싱해놓은 권한이 아닌 최신 권한 정보 재확인
	uint32 myAuthority = guildManager->GetAuthority();
	GuildMemberGradeType myMemberGrade = guildManager->GetMemeberGrade();
	bool hasAuthority = FGsGuildHelper::CanManageAcquisition(myAuthority, myMemberGrade);
	if (false == hasAuthority)
	{
		// TEXT: 권한이 없습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_NoAuthority"), textMsg);
		FGsUIHelper::TrayMessageTicker(textMsg);
		Close();
		return;
	}

	FGsNetSendServiceGuild::SendReqSaveGuildAcquisitionSettings(_currentAcquisitionSettings);
	Close();
	return;
}

void UGsUIPopupGuildAcquisition::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupGuildAcquisition::OnClickedOption(const FGsSchemaGuildAcquisition* InAcquisitionInfo, int32 InValue)
{
	// 버튼 변경된거에 대한 처리
	_currentAcquisitionSettings.mAcquisitionSetting[InAcquisitionInfo->id] = InValue;
	InvalidateChangeButton();
}

void UGsUIPopupGuildAcquisition::OnToggleAcquisition(bool InIsSelected)
{
	// 권한 체크
	if (false == _hasAuthority)
		return;

	_currentAcquisitionSettings.mAcquisition = InIsSelected;
	_toggleAcquisitionOnOff->SetIsSelected(InIsSelected);

	_scrollBoxHelper->RefreshAll(_acquisitionInfoList.Num());
	
	InvalidateChangeButton();
}

void UGsUIPopupGuildAcquisition::OnRefreshEntryOption(int32 InIndex, UWidget* InEntry)
{
	if (false == _acquisitionInfoList.IsValidIndex(InIndex))
		return;

	UGsUIGuildAcquisitionEntry* entry = Cast<UGsUIGuildAcquisitionEntry>(InEntry);
	if (nullptr == entry)
		return;

	entry->SetData(_acquisitionInfoList[InIndex], _currentAcquisitionSettings.mAcquisitionSetting[_acquisitionInfoList[InIndex]->id]);
	if (entry->OnChangeOption.IsBound())
	{
		entry->OnChangeOption.Unbind();
	}
	entry->OnChangeOption.BindUObject(this, &UGsUIPopupGuildAcquisition::OnClickedOption);
	entry->SetIsEnabled(_hasAuthority && _currentAcquisitionSettings.mAcquisition);
}

void UGsUIPopupGuildAcquisition::InvalidateChangeButton()
{
	// 권한을 가졌을 때
	bool isEnabled = _hasAuthority;

	// 기사단 획득 On/Off 가 변경되었거나
	bool isDirtyAcquisition = (_cachedAcquisitionSettings.mAcquisition != _currentAcquisitionSettings.mAcquisition);
	
	// 세부 옵션이 변경되었다면
	bool isDirtyDetail = false;

	for (int32 i = 0; i < MAX_GUILD_ACQUISITION_SETTING_COUNT; ++i)
	{
		if (_cachedAcquisitionSettings.mAcquisitionSetting[i] != _currentAcquisitionSettings.mAcquisitionSetting[i])
		{
			isDirtyDetail = true;
			break;
		}
	}

	// 버튼을 활성화한다.
	_btnChange->SetIsEnabled(isEnabled && (isDirtyAcquisition || isDirtyDetail));
}
