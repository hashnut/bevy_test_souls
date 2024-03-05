

#include "GsUIPartyDungeonPartyListViewEntry.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Dungeon/GsDungeonHelper.h"
#include "Dungeon/GsDungeonGroupParty.h"
#include "Dungeon/CoopPartyDungeon/GsCoopPartyDungeonGroupInfo.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "Party/GsPartyFunc.h"
#include "DataSchema/GameObject/GsSchemaWeaponUIInfo.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"

#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/GsUIPopupOK.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"

#include "PaperSprite.h"
#include "Image.h"
#include "TextBlock.h"
#include "WidgetSwitcher.h"

#include "Net/GsNetSendService.h"





void UGsUIPartyDungeonPartyListViewEntry::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIPartyDungeonPartyListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 2022/06/03 PKT - 참가 버튼
	_btnJoin->OnClicked.AddDynamic(this, &UGsUIPartyDungeonPartyListViewEntry::OnClickedJoin);
	// 2022/06/03 PKT - 난입 버튼
	_btnBreakInto->OnClicked.AddDynamic(this, &UGsUIPartyDungeonPartyListViewEntry::OnClickedBreakInto);
}

void UGsUIPartyDungeonPartyListViewEntry::NativeDestruct()
{
	_partyId = INVALID_PARTY_ID;

	Super::NativeDestruct();
}

void UGsUIPartyDungeonPartyListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsPartyDungeonPartyListViewEntryData* entryData
		= Cast<UGsPartyDungeonPartyListViewEntryData>(inListItemObject);

	if (nullptr == entryData || nullptr == entryData->GetData())
	{
		GSLOG(Error, TEXT("nullptr == entryData || false == entryData->GetData().IsValid()"));
		return;
	}

	const FGsPartyDungeonPartyStatusList* partyStatusList = StaticCast<const FGsPartyDungeonPartyStatusList*>(entryData->GetData());
	if (nullptr == partyStatusList)
	{
		GSLOG(Error, TEXT("nullptr == partyStatusList"));
		return;
	}

	// 2022/06/02 PKT - 무기 아이콘
	TFunction<void(CreatureWeaponType InWeaponType, UImage** OutImage)> MakeWeaponIcon = [](CreatureWeaponType InWeaponType, UImage** OutImage)
	{
		const FGsSchemaWeaponUIInfo* tableData
			= (InWeaponType == CreatureWeaponType::NONE) ? nullptr : FGsPartyFunc::GetWeaponUIInfo(InWeaponType);
		if (tableData == nullptr)
		{
			return;
		}

		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(tableData->_weaponTypeImagePath);
		if (spriteImg)
		{
			(*OutImage)->SetBrushFromAtlasInterface(spriteImg);
		}
	};

	_partyId = partyStatusList->_partyId;

	OnJoin = entryData->OnJoin;
	OnIntrude = entryData->OnIntrude;

	DisplayDifficulty(partyStatusList);
	
	DisplayLeader(partyStatusList, MakeWeaponIcon);

	DisplayMember(partyStatusList, MakeWeaponIcon);

	DisplayRoomCondition(partyStatusList);
}

void UGsUIPartyDungeonPartyListViewEntry::DisplayDifficulty(const struct FGsPartyDungeonPartyStatusList* InInfo)
{
	// 2022/06/08 PKT - 난이도
	FText grade;
	FGsDungeonHelper::FindTextDifficulty(InInfo->_difficulty, false, grade);
	_textDifficulty->SetText(grade);

	// 2022/06/08 PKT - 난이도 색상
	FLinearColor colorDifficulty = FGsUIColorHelper::GetDungeonDifficultyType(InInfo->_difficulty);
	_textDifficulty->SetColorAndOpacity(colorDifficulty);
}

void UGsUIPartyDungeonPartyListViewEntry::DisplayLeader(const struct FGsPartyDungeonPartyStatusList* InInfo
	, TFunction<void(CreatureWeaponType InWeaponType, UImage ** OutImage)> InMakeWeaponImg /*= nullptr*/)
{
	// 2022/06/03 PKT - 리더 레벨
	FText message;
	FText::FindText(TEXT("PartyDungeonText"), TEXT("PartyList_LeaderLevel"), message);

	if (INVALID_LEVEL < InInfo->_leaderLevel)
	{
		_textLeaderLevel->SetText(FText::Format(message, InInfo->_leaderLevel));
		_textLeaderName->SetText(FText::FromString(InInfo->_leaderName));

		InMakeWeaponImg(InInfo->_leaderWeaponType, &_imgLeaderWeapon);
	}
	else
	{
		FText unknowLevel = FText::FromString(FString("??"));
		_textLeaderLevel->SetText(FText::Format(message, unknowLevel));

		InMakeWeaponImg(CreatureWeaponType::ALL, &_imgLeaderWeapon);
	}

	_textLeaderName->SetText(FText::FromString(InInfo->_leaderName));
}

void UGsUIPartyDungeonPartyListViewEntry::DisplayMember(const FGsPartyDungeonPartyStatusList* InInfo
	, TFunction<void(CreatureWeaponType InWeaponType, UImage** OutImage)> InMakeWeaponImg /*= nullptr*/)
{
	// [U1] | ejrrb10 | 파티던전 크래시 이슈 지속 발생하여, 안전하게 참조하도록 수정합니다
	// https://com2us-raon.sentry.io/issues/4277070890/events/b5c53a3e05494fdeb53692fa72a3037c/
	// Note : MakeTuple 한 객체를 Emplace 하는 과정에서 Move Semantics 적용으로, 기존 값이 nullptr 가 될 수 있음

	TArray<UImage*> memberDisplayImage;
	TArray<UTextBlock*> memberDisplayTextBlock;

	// 2022/06/08 PKT - 0번 슬롯
	memberDisplayImage.Emplace(_imgfirstMemberWeapon);
	memberDisplayTextBlock.Emplace(_textfirstMemberLevel);

	// 2022/06/08 PKT - 1번 슬롯
	memberDisplayImage.Emplace(_imgSecondMemberWeapon);
	memberDisplayTextBlock.Emplace(_textSecondMemberLevel);

	// 2022/06/08 PKT - 2번 슬롯
	memberDisplayImage.Emplace(_imgThirdemberWeapon);
	memberDisplayTextBlock.Emplace(_textThirdMemberLevel);


	// 2023/3/23 PKT - 무조건 꺼주고
	for (int32 idx = 0; idx < (MAX_PARTY_MEMBER - 1); ++idx)
	{	// 2022/06/08 PKT - Hide
		if (memberDisplayImage.IsValidIndex(idx) && memberDisplayTextBlock.IsValidIndex(idx))
		{
			if (memberDisplayImage[idx] && memberDisplayTextBlock[idx])
			{
				memberDisplayImage[idx]->SetVisibility(ESlateVisibility::Collapsed);
				memberDisplayTextBlock[idx]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}

	for (int32 idx = 0; idx < InInfo->_memberList.Num(); ++idx)
	{
		if (memberDisplayImage.IsValidIndex(idx) && memberDisplayTextBlock.IsValidIndex(idx))
		{
			if (memberDisplayImage[idx] && memberDisplayTextBlock[idx])
			{
				memberDisplayImage[idx]->SetVisibility(ESlateVisibility::HitTestInvisible);
				memberDisplayTextBlock[idx]->SetVisibility(ESlateVisibility::HitTestInvisible);


				const TPair<CreatureWeaponType, Level>& memberInfo = InInfo->_memberList[idx];

				// 2022/06/08 PKT - 레벨 설정
				FText message;
				FText::FindText(TEXT("PartyDungeonText"), TEXT("PartyList_LeaderLevel"), message);

				if (INVALID_LEVEL < memberInfo.Value)
				{
					InMakeWeaponImg(memberInfo.Key, &memberDisplayImage[idx]);
					memberDisplayTextBlock[idx]->SetText(FText::Format(message, memberInfo.Value));
				}
				else
				{
					InMakeWeaponImg(CreatureWeaponType::ALL, &memberDisplayImage[idx]);

					FText unknowLevel = FText::FromString(FString("??"));
					memberDisplayTextBlock[idx]->SetText(FText::Format(message, unknowLevel));
				}

			}
		}
	}
}

void UGsUIPartyDungeonPartyListViewEntry::DisplayRoomCondition(const FGsPartyDungeonPartyStatusList* InInfo)
{
	DungeonGroupId targetDungeonGroupId = InInfo->_dungeonGroupId;
	DungeonDifficulty targetDungeonDifficulty = InInfo->_difficulty;

	FGsDungeonManager* dungeonManager = GSDungeon();
	if (nullptr == dungeonManager)
	{
		GSLOG(Error, TEXT("nullptr == dungeonManager"));
		return;
	}

	const FGsDungeonGroupBase* dungeonGroup = dungeonManager->FindGroupDataById(targetDungeonGroupId);
	if (nullptr == dungeonGroup)
	{
		GSLOG(Error, TEXT("nullptr == dungeonGroup"));
		return;
	}

	const FGsSchemaDungeonGroupData* schemaData = dungeonGroup->GetSchemaData();
	if (DungeonCategory::PARTY != schemaData->dungeonCategory)
	{
		int32 categoryNum = StaticCast<int32>(schemaData->dungeonCategory);
		GSLOG(Error, TEXT("Invalid Dungeon Category : [%d]"), categoryNum);
		return;
	}

	const FGsDungeonGroupParty* dungeonGroupParty = StaticCast<const FGsDungeonGroupParty*>(dungeonGroup);
	if (nullptr == dungeonGroupParty)
	{
		GSLOG(Error, TEXT("nullptr == dungeonGroupParty"));
		return;
	}

	// 2022/06/08 PKT - 현재 멤버가 Full이면
	int32 memberCount = InInfo->_memberList.Num() + 1;		// 2022/06/08 PKT - 파티장은 미리 포함 1부터 시작

	FText textFormat;
	FText::FindText(TEXT("PartyDungeonText"), TEXT("PartyList_MemberCount"), textFormat);
	_textMemberCount->SetText(FText::Format(textFormat, { memberCount , MAX_PARTY_MEMBER }));

	if (memberCount < MAX_PARTY_MEMBER)
	{
		_textMemberCount->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_VALID));
	}
	else
	{
		_textMemberCount->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_INVALID));
	}
	

	// 2022/06/08 PKT - 현재 대기 중인가?
	bool isWaitingStatus = (PartyDungeonStepStatus::WAITING == InInfo->_status);

	if (isWaitingStatus)
	{	// 2022/06/08 PKT - 대기 중
		_textInProgressState->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{	// 2022/06/03 PKT - 진행 중
		FText message;
		FText::FindText(TEXT("PartyDungeonText"), TEXT("PartyList_Ongoing"), message);
		_textInProgressState->SetText(message);
		_textInProgressState->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// 2022/06/08 PKT - 난입 가능 한가?
	bool isCanForcedEnter = InInfo->_canForcedEnter;

	const static int32 SWITCHER_INDEX_ENTER = 0;
	const static int32 SWITCHER_INDEX_BREAK_INTO = 1;

	_switcherBtnState->SetActiveWidgetIndex(isWaitingStatus ? SWITCHER_INDEX_ENTER : SWITCHER_INDEX_BREAK_INTO);

	bool isEnable = true;

	bool isNotAllowed = dungeonGroupParty->DifficultyToEnter() < targetDungeonDifficulty;
	if (isNotAllowed || memberCount >= MAX_PARTY_MEMBER)
	{	// 2022/06/08 PKT - 참여 하지 못하는 난이도 or 풀파티
		isEnable = false;
	}

	_btnJoin->SetIsEnabled(isEnable);
	_btnBreakInto->SetIsEnabled(isEnable);
}

void UGsUIPartyDungeonPartyListViewEntry::OnClickedJoin()
{	
	if (OnJoin)
	{
		OnJoin(_partyId);
	}	
}

void UGsUIPartyDungeonPartyListViewEntry::OnClickedBreakInto()
{
	if (OnIntrude)
	{
		OnIntrude(_partyId);
	}
}