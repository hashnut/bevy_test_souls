#include "GsUIHUDParty.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "../Dungeon/GsDungeonGroupParty.h"
#include "../Dungeon/GsDungeonCommonEnum.h"
#include "../Dungeon/GsDungeonHelper.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentDungeon.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/Stage/StageGame/GsStageGameDungeon.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateDungeon.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Dungeon/GsPartyDungeonState.h"

#include "Party/GsPartyFunc.h"
#include "Party/GsPartyInviteeInfo.h"
#include "Party/GsPartyMemberInfo.h"
#include "Party/GsPartySlotInfo.h"

#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"
#include "UI/UIContent/HUD/Party/GsUIPartyMemberItem.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "WidgetSwitcher.h"
#include "VerticalBox.h"
#include "Widget.h"
#include "TextBlock.h"
#include "RichTextBlock.h"

#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/Dungeon/GsSchemaCoopPartyDungeonConfig.h"

#include "UTIL/GsTableUtil.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Net/GsNetSendService.h"





void UGsUIHUDParty::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	// 멤버 리스트 만드는넘 초기화
	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIHUDParty::OnMemberListRefreshEntry);

	_btnOpenSetting->OnClicked.AddDynamic(this, &UGsUIHUDParty::OnClickOpenSetting);
	_btnLeave->OnClicked.AddDynamic(this, &UGsUIHUDParty::OnClickLeave);
	_autoMatchingOn->OnClicked.AddDynamic(this, &UGsUIHUDParty::OnClickMatchingOn);

	_switcherButtonPartyDungonWork->OnClicked.AddDynamic(this, &UGsUIHUDParty::OnClickAddIn);
	_switcherButtonPartyDungonStart->OnClicked.AddDynamic(this, &UGsUIHUDParty::OnClickReady);
}

void UGsUIHUDParty::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshUI();

	if (FGsMessageHolder* msgM = GMessage())
	{
		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::PARTY_HUD_ALL_LIST_CHANGE, this, &UGsUIHUDParty::OnAllListChange)
		);
		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::PARTY_HUD_ONE_LIST_CHANGE, this, &UGsUIHUDParty::OnOneListChange)
		);
		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::PARTY_HUD_INFO_UPDATE, this, &UGsUIHUDParty::OnPartyInfoChange)
		);
		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::LOCAL_ABNORMALITY_ADD, this, &UGsUIHUDParty::OnAddAbnormality)
		);
		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::LOCAL_ABNORMALITY_REMOVE, this, &UGsUIHUDParty::OnRemoveAbnormality)
		);
	}
}

void UGsUIHUDParty::NativeDestruct()
{
	if (FGsMessageHolder* msgM = GMessage())
	{
		if (_arrDelegateGameObjectMsg.Num() != 0)
		{
			for (auto iter : _arrDelegateGameObjectMsg)
			{
				msgM->GetGameObject().Remove(iter);
			}
			_arrDelegateGameObjectMsg.Empty();
		}
	}
	Super::NativeDestruct();
}

void UGsUIHUDParty::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIHUDParty::OnMemberListRefreshEntry);
		_scrollBoxHelper = nullptr;
	}
	Super::BeginDestroy();
}

// 멤버 리스트 데이터 세팅
void UGsUIHUDParty::SetMemberListData()
{
	int partyMemberMax = GData()->GetGlobalData()->_partyMemberMax;
	// 무조건 4개는 잡아놓음
	_scrollBoxHelper->RefreshAll(partyMemberMax);
}

void UGsUIHUDParty::OnMemberListRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUIPartyMemberItem* item = Cast<UGsUIPartyMemberItem>(InEntry);

	if (item == nullptr)
	{
		return;
	}

	IGsPartySlotInfo* slotInfo = GSParty()->GetPartySlotInfo(InIndex);

	// 정보가 없으면 빈슬롯
	if (slotInfo == nullptr)
	{
		item->SetData(InIndex, true);
	}
	else if (true == slotInfo->GetIsMember())
	{
		FGsPartyMemberInfo* memberInfo = static_cast<FGsPartyMemberInfo*>(slotInfo);
		if (memberInfo == nullptr)
		{
			return;
		}

		bool isMe = GSParty()->IsMySlotId(InIndex);
		bool amILeader = GSParty()->AmILeader();
		EGsPartyMemberItemType itemType = EGsPartyMemberItemType::None;
		if (isMe == true)
		{
			itemType = EGsPartyMemberItemType::Me;
		}
		else if (amILeader == true)
		{
			itemType = EGsPartyMemberItemType::Member_ImLeader;
		}
		else
		{
			itemType = EGsPartyMemberItemType::Member_ImNotLeader;
		}

		bool isCloseDrawMenu = GSParty()->GetIsCloseDrawMenu();
		
		item->SetData(InIndex, false, memberInfo->GetName(), itemType,
			memberInfo->GetWeaponType(),
			memberInfo->GetIsLeader(),
			memberInfo->GetLevel(),
			memberInfo->GetNowHp(),
			memberInfo->GetMaxHp(),
			memberInfo->GetNowShield(),
			memberInfo->GetMpRate(),
			isCloseDrawMenu,
			memberInfo->GetIsSpawned(),
			memberInfo->GetComplatedReady(),
			memberInfo->GetHomeWorldId()
		);
	}
	else
	{
		FGsPartyInviteeInfo* inviteeInfo = static_cast<FGsPartyInviteeInfo*>(slotInfo);
		if (inviteeInfo == nullptr)
		{
			return;
		}

		item->SetData(InIndex, false, slotInfo->GetName(), EGsPartyMemberItemType::InviteProgress, 
		CreatureWeaponType::NONE,	// weapon type
		false,						// is leader
		-1,							// level
		0,							// curr hp
		0,							// max hp
		0,							// curr shield id
		-1.f,						// mp rate
		false,						// is close draw menu
		true,						// is spawned
		false,						// is ready
		inviteeInfo->GetHomeWorldId());	// home world id

		item->StartProgressBar(inviteeInfo->GetStartTimeTick());
	}
}

// 모든 리스트 변경
void UGsUIHUDParty::OnAllListChange(const IGsMessageParam*)
{
	RefreshUI();
}

// 한개 리스트 변경
void UGsUIHUDParty::OnOneListChange(const IGsMessageParam* In_param)
{
	const FGsPrimitiveInt32* param = In_param->Cast<const FGsPrimitiveInt32>();
	int slotIndex = param->_data;

	_scrollBoxHelper->RefreshByIndex(slotIndex);
}

// ui 갱신(스위치와 멤버 리스트)
void UGsUIHUDParty::RefreshUI()
{
	InvalidateInfo();

	InvalidateAddIn();

	InvalidateReady();

	if (GSParty()->GetIsInParty())
	{
		_verticalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		SetMemberListData();
	}
	else
	{
		_verticalBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

// 2022/06/16 PKT - 파티 정보
void UGsUIHUDParty::InvalidateInfo()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == gameStateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
		return;
	}

	const FGsDungeonStateHandler* dungeonStateHandler = gameStateDungeon->StateHandler();
	const FGsDungeonStateBase* partyDungeonState
		= (nullptr != dungeonStateHandler) ? dungeonStateHandler->GetState(DungeonCategory::PARTY) : nullptr;

	if (nullptr == partyDungeonState)
	{
		GSLOG(Error, TEXT("nullptr == partyDungeonState"));
		return;
	}

	const static int32 SWITCHER_INDEX_CREATE_PARTY = 0;
	const static int32 SWITCHER_INDEX_IN_PARTY = 1;

	// 2022/06/16 PKT - 파티 정보가 필요한 상황인가?

	if (true == GSParty()->GetIsInParty() || EDungeonProgressState::EMatching == partyDungeonState->GetProgressState())
	{
		_switcherPartyJoinState->SetActiveWidgetIndex(SWITCHER_INDEX_IN_PARTY);

		// 2022/06/16 PKT - 파티 정보 설명
		EInfoDescType type = EInfoDescType::Normal;
		if (EDungeonProgressState::ENone != partyDungeonState->GetProgressState())
		{	// 2022/06/20 PKT - 던전파티 상태가 무언가 진행 중이면 무조건 던전 타입으로!!
			type = EInfoDescType::Dungeon;
		}

		InvalidateInfoDesc(type);
	}
	else
	{
		_switcherPartyJoinState->SetActiveWidgetIndex(SWITCHER_INDEX_CREATE_PARTY);
	}
}

void UGsUIHUDParty::InvalidateInfoDesc(EInfoDescType InType)
{
	const int32 SWITCHER_INDEX_PARTY_DESC = 0;		// 2022/06/16 PKT - 파티 설명
	const int32 SWITCHER_INDEX_MATCHING_DESC = 1;	// 2022/06/16 PKT - 매칭 중 설명

	if (GSParty()->GetIsInParty())
	{	// 2022/06/20 PKT - 파티에 대한 설명
		_switcherWidgetInfoDesc->SetActiveWidgetIndex(SWITCHER_INDEX_PARTY_DESC);

		const int32 SWITCHER_INDEX_NORMAL_PARTY = 0;	// 2022/06/16 PKT - 일반 파티
		const int32 SWITCHER_INDEX_DUNGEON_PARTY = 1;	// 2022/06/16 PKT - 던전 파티

		if (EInfoDescType::Normal == InType)
		{
			_switcherWidgetPartyDesc->SetActiveWidgetIndex(SWITCHER_INDEX_NORMAL_PARTY);
			_btnOpenSetting->SetIsEnabled(true);
		}
		else if (EInfoDescType::Dungeon == InType)
		{
			_switcherWidgetPartyDesc->SetActiveWidgetIndex(SWITCHER_INDEX_DUNGEON_PARTY);
			_btnOpenSetting->SetIsEnabled(false);
		}

		SetTextPartyInfo(InType);
	}
	else
	{	// 2022/06/16 PKT - 현재로선 파티 중이 아닐땐 매칭 중 상황 만 존재 함.
		_switcherWidgetInfoDesc->SetActiveWidgetIndex(SWITCHER_INDEX_MATCHING_DESC);
	}
}

void UGsUIHUDParty::InvalidateAddIn()
{
	// 2022/06/16 PKT - Widget Default
	_switcherButtonPartyDungonWork->SetVisibility(ESlateVisibility::Collapsed);

	if (false == GSParty()->GetIsInParty())
	{	// 2022/06/20 PKT - 파티 상태에서만 활성화
		return;
	}

	// 2022/06/24 PKT - 추후 추가 컨텐츠에서 같은 UI 혹은 같은 자리에 다른 UI 를 배치 할 수도 있기 때문에 리턴값으로 검사.
	if (true == DisplayPartyDungeonAddIn())
	{	// 2022/06/24 PKT - 파티 던전용 UI를 처리 했으므로 빠져 나간다.
		return;
	}
}

void UGsUIHUDParty::InvalidateReady()
{
	// 2022/06/16 PKT - Widget Default
	_switcherButtonPartyDungonStart->SetVisibility(ESlateVisibility::Collapsed);

	// 2022/06/24 PKT - 추후 추가 컨텐츠에서 같은 UI 혹은 같은 자리에 다른 UI 를 배치 할 수도 있기 때문에 리턴값으로 검사.
	if (true == DisplayPartyDungeonReady())
	{	// 2022/06/24 PKT - 파티 던전용 UI를 처리 했으므로 빠져 나간다.
		return;
	}
}


bool UGsUIHUDParty::DisplayPartyDungeonAddIn()
{
	bool bResult = false;

	do 
	{
		FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
		FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
		if (nullptr == contents)
		{
			GSLOG(Error, TEXT("nullptr == contents"));
			break;
		}

		FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
		if (nullptr == gameStateDungeon)
		{
			GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
			break;
		}

		const FGsDungeonStateHandler* dungeonStateHandler = gameStateDungeon->StateHandler();
		if (nullptr == dungeonStateHandler)
		{
			GSLOG(Error, TEXT("nullptr == dungeonStateHandler"));
			break;
		}

		const FGsPartyDungeonState* partyDungeonState
			= StaticCast<const FGsPartyDungeonState*>(dungeonStateHandler->GetState(DungeonCategory::PARTY));

		EDungeonProgressState progressState = partyDungeonState->GetProgressState();
		if (EDungeonProgressState::ENone == progressState || EDungeonProgressState::EMatching == progressState)
		{	// 2022/06/24 PKT - 파티 던전과 관련 없거나 매칭 중이라면..
			break;
		}
		else
		{
			_switcherButtonPartyDungonWork->SetVisibility(ESlateVisibility::Visible);

			if (EDungeonProgressState::EJoinParty == progressState || EDungeonProgressState::EReady == progressState)
			{
				// 2023/2/15 PKT - 던전 들어가기 전 파티 상태 일때..( 목록 보기..)
				_switcherWidgetAddIn->SetActiveWidgetIndex(StaticCast<int32>(ESWITCER_ADDIN_INDEX::Shotcut));
			}
			else if (EDungeonProgressState::EInDungeon == progressState)
			{
				int32 maxAskHelpCount = GSDungeon()->GetPartyDungeonConfig()->callHelpCount;

				FText textAskHelpCount;
				// 2022/06/24 PKT - 도움요청{0}/{1}
				FText::FindText(TEXT("PartyText"), TEXT("AskHelp_Count"), textAskHelpCount);
				textAskHelpCount = FText::Format(textAskHelpCount, { partyDungeonState->GetHelpseekingCount(), maxAskHelpCount });

				if (GSParty()->AmILeader() && partyDungeonState->GetHelpseekingEnable())
				{
					// 2022/06/20 PKT - 도움 요청 가능한 상태
					_textAskHelpEnable->SetText(textAskHelpCount);
					_switcherButtonPartyDungonWork->SetVisibility(ESlateVisibility::Visible);
					_switcherWidgetAddIn->SetActiveWidgetIndex(StaticCast<int32>(ESWITCER_ADDIN_INDEX::AskHelpEnable));
				}
				else
				{
					// 2022/06/20 PKT - 도움 요청 불가능 상태				
					_textAskHelpDisable->SetText(textAskHelpCount);
					_switcherButtonPartyDungonWork->SetVisibility(ESlateVisibility::HitTestInvisible);
					_switcherWidgetAddIn->SetActiveWidgetIndex(StaticCast<int32>(ESWITCER_ADDIN_INDEX::AskHelpUnEnable));
				}
			}
		}

		bResult = true;

	} while (0);
	
	return bResult;
}


bool UGsUIHUDParty::DisplayPartyDungeonReady()
{
	bool bResult = false;
	do 
	{
		if (GLevel()->IsPartyDungeonMap())
		{	// 2022/06/20 PKT - 이미 파티 던전 내 진입해 있다면 버튼 숨김.
			break;
		}

		FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
		FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
		if (nullptr == contents)
		{
			GSLOG(Error, TEXT("nullptr == contents"));
			break;
		}

		FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
		if (nullptr == gameStateDungeon)
		{
			GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
			break;
		}

		const FGsDungeonStateHandler* dungeonStateHandler = gameStateDungeon->StateHandler();
		const FGsDungeonStateBase* dungeonState 
			= (nullptr != dungeonStateHandler) ? dungeonStateHandler->GetState(DungeonCategory::PARTY) : nullptr;

		if (nullptr == dungeonState)
		{
			GSLOG(Error, TEXT("nullptr == dungeonState"));
			break;
		}

		if (EDungeonProgressState::EJoinParty != dungeonState->GetProgressState() && EDungeonProgressState::EReady != dungeonState->GetProgressState())
		{
			// 2022/06/20 PKT - 파티 입장 대기 상태가 아니면 버튼 숨김.
			break;
		}

		_switcherButtonPartyDungonStart->SetVisibility(ESlateVisibility::Visible);
		_switcherButtonPartyDungonStart->SetIsEnabled(true);

		if (true == GSParty()->AmILeader())
		{
			bool isEnable = true;

			const TArray<FGsPartyMemberInfo* > arrPartyMemberInfo = GSParty()->GetAllPartyMemberInfo();
			for (int32 idx = 0; idx < arrPartyMemberInfo.Num(); ++idx)
			{
				if (nullptr == arrPartyMemberInfo[idx])
				{
					continue;
				}

				int32 partySlotId = arrPartyMemberInfo[idx]->GetPartySlotId();

				if (true == GSParty()->IsMySlotId(partySlotId))
				{
					continue;
				}

				FGsPartyMemberInfo* memberInfo = GSParty()->FindPartyMemberInfo(partySlotId);
				if (nullptr != memberInfo && false == memberInfo->GetComplatedReady())
				{
					isEnable = false;
					break;
				}
			}

			_switcherWidgetReadty->SetActiveWidgetIndex(StaticCast<int32>(ESWITCHER_READY_INDEX::Start));
			// 2022/06/20 PKT - 시작 버튼 활성 여부
			_switcherButtonPartyDungonStart->SetIsEnabled(isEnable);
		}
		else
		{
			FGsPartyMemberInfo* myMemberInfo = GSParty()->FindMyPartyMemberInfo();
			if (nullptr == myMemberInfo)
			{	// 2023/3/14 PKT - 없으면 안되는 Data이긴 하나. https://jira.com2us.com/jira/browse/C2URWQ-3218 JIRA이슈로 올라와 대응 함.
				return false;
			}

			if (false == myMemberInfo->GetComplatedReady())
			{	// 2022/06/20 PKT - Ready On 활성화
				_switcherWidgetReadty->SetActiveWidgetIndex(StaticCast<int32>(ESWITCHER_READY_INDEX::ReadyOn));
			}
			else
			{	// 2022/06/20 PKT - Ready Cancel 활성화
				_switcherWidgetReadty->SetActiveWidgetIndex(StaticCast<int32>(ESWITCHER_READY_INDEX::ReadyCancel));
			}
		}

		bResult = true;

	} while (0);

	return bResult;
}

void UGsUIHUDParty::SetTextPartyInfo(EInfoDescType InType)
{
	const TArray<FGsPartyMemberInfo* > arrPartyMemberInfo = GSParty()->GetAllPartyMemberInfo();
	int nowCount = arrPartyMemberInfo.Num();
	int partyMemberMax = GData()->GetGlobalData()->_partyMemberMax;

	if (EInfoDescType::Normal == InType)
	{	// 2022/06/16 PKT - 일반 파티
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("NormalPartyTitle"), findText);

		FText makeText = FText::Format(findText,
			FText::FromString(FString::FromInt(nowCount)), FText::FromString(FString::FromInt(partyMemberMax)));

		_textNormalPartyDesc->SetText(makeText);
	}
	else if (EInfoDescType::Dungeon == InType)
	{
		FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
		FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
		if (nullptr == contents)
		{
			GSLOG(Error, TEXT("nullptr == contents"));
			return;
		}

		FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
		if (nullptr == gameStateDungeon)
		{
			GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
			return;
		}

		const FGsDungeonStateHandler* dungeonStateHandler = gameStateDungeon->StateHandler();
		const FGsPartyDungeonState* partyDungeonState
			= (nullptr != dungeonStateHandler) 
			? StaticCast<const FGsPartyDungeonState*>(dungeonStateHandler->GetState(DungeonCategory::PARTY)) : nullptr;

		if (nullptr == partyDungeonState)
		{
			GSLOG(Error, TEXT("nullptr == partyDungeonState"));
			return;
		}

		const FGsDungeonGroupBase* dungeonGroupData = GSDungeon()->FindGroupDataById(partyDungeonState->GetApplyGroupId());
		if (nullptr == dungeonGroupData)
		{
			GSLOG(Error, TEXT("nullptr == dungeonGroupData"));
			return;
		}

		const FGsDungeonGroupParty* partyDungeonGroupData = StaticCast<const FGsDungeonGroupParty*>(dungeonGroupData);
		const FGsDungeonData* dungeonData = partyDungeonGroupData->FindDungeonByDifficulty(partyDungeonState->GetApplyDifficulty());

		FText textTitle;
		FGsDungeonHelper::DungeonNameByDifficulty(partyDungeonGroupData, dungeonData, true, textTitle);		
		textTitle = FText::FromString(FString::Format(TEXT("{0} {1}/{2}"), { *textTitle.ToString(), nowCount, partyMemberMax }));

		_richTextDungeonPartyDesc->SetText(textTitle);
	}
}

// 모든 드로우 메뉴 닫기
void UGsUIHUDParty::CloseAllDrawMenu()
{
	TArray<UWidget*> childList = _verticalBox->GetAllChildren();
	for (UWidget* iter : childList )
	{
		UGsUIPartyMemberItem* item = Cast<UGsUIPartyMemberItem>(iter);
		if (item == nullptr)
		{
			continue;
		}
		item->CloseDrawMenu();
	}
}

void UGsUIHUDParty::OnClickOpenSetting()
{
	GSParty()->OpenPartySettingUI();

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDParty::OnClickLeave()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == gameStateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
		return;
	}

	const FGsDungeonStateHandler* dungeonStateHandler = gameStateDungeon->StateHandler();
	if (nullptr == dungeonStateHandler)
	{
		GSLOG(Error, TEXT("nullptr == dungeonStateHandler"));
		return;
	}

	const auto& partyDungeonState = dungeonStateHandler->GetState(DungeonCategory::PARTY);
	if (partyDungeonState && EDungeonProgressState::EInDungeon == partyDungeonState->GetProgressState())
	{
		// 2023/2/27 PKT - 파티 상태가 파티 던전내 있다면 던전 나가기로 로직 변경
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::TRY_EXIT_DUNGEON, nullptr);
	}
	else
	{
		FGsPartyFunc::OpenLeavePopup();
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDParty::OnClickMatchingOn()
{
	// 2022/06/16 PKT - Window 바로 가기
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == gameStateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
		return;
	}

	const FGsDungeonStateHandler* dungeonStateHandler = gameStateDungeon->StateHandler();
	const FGsPartyDungeonState* partyDungeonState
		= (nullptr != dungeonStateHandler) ? StaticCast<const FGsPartyDungeonState*>(dungeonStateHandler->GetState(DungeonCategory::PARTY)) : nullptr;
	if (nullptr == partyDungeonState)
	{
		GSLOG(Error, TEXT("nullptr == partyDungeonState"));
		return;
	}

	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_DUNGEON);

	FGsPrimitivePairInt32 param(partyDungeonState->GetApplyGroupId(), StaticCast<int32>(partyDungeonState->GetApplyDifficulty()));
	GMessage()->GetDungeon().SendMessage(MessageContentDungeon::SHOTCUT_PARTY_DUNGEON, &param);	

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDParty::OnClickAddIn()
{
	ESWITCER_ADDIN_INDEX index = StaticCast<ESWITCER_ADDIN_INDEX>(_switcherWidgetAddIn->GetActiveWidgetIndex());
	if (ESWITCER_ADDIN_INDEX::Shotcut == index)
	{	// 2022/06/16 PKT - Window 바로 가기
		FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
		FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
		if (nullptr == contents)
		{
			GSLOG(Error, TEXT("nullptr == contents"));
			return;
		}

		FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
		if (nullptr == gameStateDungeon)
		{
			GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
			return;
		}

		const FGsDungeonStateHandler* dungeonStateHandler = gameStateDungeon->StateHandler();
		const FGsPartyDungeonState* partyDungeonState
			= (nullptr != dungeonStateHandler) ? StaticCast<const FGsPartyDungeonState*>(dungeonStateHandler->GetState(DungeonCategory::PARTY)) : nullptr;
		if (nullptr == partyDungeonState)
		{
			GSLOG(Error, TEXT("nullptr == partyDungeonState"));
			return;
		}

		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_DUNGEON);

		FGsPrimitivePairInt32 param(partyDungeonState->GetApplyGroupId(), StaticCast<int32>(partyDungeonState->GetApplyDifficulty()));
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::SHOTCUT_PARTY_DUNGEON, &param);
	}
	else if (ESWITCER_ADDIN_INDEX::AskHelpEnable == index)
	{	// 2022/06/16 PKT - 도움 요청
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::PARTY_DUNGEON_REQUEST_HELPSEEKING, nullptr);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDParty::OnClickReady()
{
	ESWITCHER_READY_INDEX index = StaticCast<ESWITCHER_READY_INDEX>(_switcherWidgetReadty->GetActiveWidgetIndex());
	if (ESWITCHER_READY_INDEX::Start == index)
	{	// 2022/06/16 PKT - 시작 요청
		FGsNetSendService::SendPartyDungeonEntrance();
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);
	}
	else if (ESWITCHER_READY_INDEX::ReadyOn == index)
	{	// 2022/06/16 PKT - 준비 상태 요청
		FGsNetSendService::SendPartyDungeonReady(true);
	}
	else if (ESWITCHER_READY_INDEX::ReadyCancel == index)
	{	// 2022/06/16 PKT - 준비 취소 상태 요청
		FGsNetSendService::SendPartyDungeonReady(false);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}


void UGsUIHUDParty::OnPartyInfoChange(const IGsMessageParam*)
{
	InvalidateInfo();

	InvalidateAddIn();

	InvalidateReady();
}

void UGsUIHUDParty::OnAddAbnormality(const IGsMessageParam* InParam)
{
	if (const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>())
	{
		if (static_cast<int32>(AbnormalityEffectType::UTIL_CONFUSE) == param->_second)
		{
			UpdateAbnormalityConfuse();
		}
	}
}

void UGsUIHUDParty::OnRemoveAbnormality(const IGsMessageParam* InParam)
{
	if (const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>())
	{
		if (static_cast<int32>(AbnormalityEffectType::UTIL_CONFUSE) == param->_second)		
		{
			UpdateAbnormalityConfuse();
		}
	}
}

void UGsUIHUDParty::UpdateAbnormalityConfuse()
{
	FGsPartyManager* partyMgr = GSParty();
	if (nullptr == partyMgr)
	{
		return;
	}

	int partyMemberMax = GData()->GetGlobalData()->_partyMemberMax;
	for (int i = 0; i < partyMemberMax; ++i)
	{
		// 내 슬롯만 처리
		if (false == partyMgr->IsMySlotId(i))
		{
			continue;
		}

		IGsPartySlotInfo* slotInfo = partyMgr->GetPartySlotInfo(i);
		if (nullptr == slotInfo)
		{
			continue;
		}

		FGsPartyMemberInfo* memberInfo = static_cast<FGsPartyMemberInfo*>(slotInfo);
		if (nullptr == memberInfo)
		{
			continue;
		}

		memberInfo->UpdateAbnormalityConfuse();
	}
}
