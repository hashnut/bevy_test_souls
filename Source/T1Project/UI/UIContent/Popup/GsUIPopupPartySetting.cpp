#include "GsUIPopupPartySetting.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Popup/PartySetting/GsUIPartySettingMemberItem.h"
#include "UI/UIContent/Popup/PartySetting/GsUIPartySettingTab.h"
#include "UI/UIContent/Popup/PartySetting/GsUIPartyOwnTab.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Party/GsPartyInviteeInfo.h"
#include "Party/GsPartyMemberInfo.h"
#include "Party/GsPartySlotInfo.h"
#include "Party/GsPartyInviterInfo.h"

#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendService.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Core/Public/Delegates/Delegate.h"

void UGsUIPopupPartySetting::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupPartySetting::OnMemberListRefreshEntry);

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupPartySetting::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupPartySetting::OnClickCancel);


	// 획득 룰 탭(랜덤, 순차, 자유)
	_toggleGroupOwn.AddToggleWidgetByParentPanel(_panelOwnTab);
	_toggleGroupOwn.OnSelectChanged.BindUObject(this, &UGsUIPopupPartySetting::OnClickOwnTab);

	// 랜덤
	FText findTextLoot1;
	FText::FindText(TEXT("PartyText"), TEXT("PartyTabLeftLootButton1"), findTextLoot1);

	_tabRandom->SetText(findTextLoot1);

	// 순차
	FText findTextLoot3;
	FText::FindText(TEXT("PartyText"), TEXT("PartyTabLeftLootButton3"), findTextLoot3);

	_tabSequence->SetText(findTextLoot3);

	// 자유
	FText findTextLoot2;
	FText::FindText(TEXT("PartyText"), TEXT("PartyTabLeftLootButton2"), findTextLoot2);

	_tabFree->SetText(findTextLoot2);

	_arrayOwnTab.Add(_tabRandom);
	_arrayOwnTab.Add(_tabSequence);
	_arrayOwnTab.Add(_tabFree);
}

void UGsUIPopupPartySetting::OnMemberListRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUIPartySettingMemberItem* item = Cast<UGsUIPartySettingMemberItem>(InEntry);
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
	else if( true == slotInfo->GetIsMember())
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
		else if(amILeader == true)
		{
			itemType = EGsPartyMemberItemType::Member_ImLeader;
		}
		else
		{
			itemType = EGsPartyMemberItemType::Member_ImNotLeader;
		}

		item->SetData(InIndex, false, memberInfo->GetName(), itemType,
			memberInfo->GetWeaponType(),
			memberInfo->GetIsLeader(),
			memberInfo->GetLevel(),
			memberInfo->GetChannelId(),
			memberInfo->GetHomeWorldId());
	}
	else
	{	
		FGsPartyInviteeInfo* inviteeInfo = static_cast<FGsPartyInviteeInfo*>(slotInfo);
		if (inviteeInfo == nullptr)
		{
			return;
		}

		item->SetData(InIndex, false, slotInfo->GetName(), EGsPartyMemberItemType::InviteProgress, 
		CreatureWeaponType::NONE ,		// weapon type
		false,							// is leader
		-1,								// level
		-1,								// channel id
		inviteeInfo->GetHomeWorldId()	// home world id
		);

		item->StartProgressBar(inviteeInfo->GetStartTimeTick());
	}
}

// 멤버 리스트 데이터 세팅
void UGsUIPopupPartySetting::SetMemberListData()
{
	bool isInParty = GSParty()->GetIsInParty();
	int partyMemberMax = 0;
	if (isInParty == true)
	{
		partyMemberMax = GData()->GetGlobalData()->_partyMemberMax;
	}
	// 무조건 4개는 잡아놓음
	_scrollBoxHelper->RefreshAll(partyMemberMax);

	ESlateVisibility noListVisible = (partyMemberMax == 0) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_noList->SetVisibility(noListVisible);
}

void UGsUIPopupPartySetting::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupPartySetting::OnMemberListRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}
void UGsUIPopupPartySetting::NativeConstruct()
{
	Super::NativeConstruct();

	InitTab();
	InitOkButton();
	SetMemberListData();


	if (FGsMessageHolder* msgM = GMessage())
	{
		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::PARTY_SETTING_ALL_LIST_CHANGE, this, &UGsUIPopupPartySetting::OnAllListChange)
		);
		_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
			MessageGameObject::PARTY_SETTING_ONE_LIST_CHANGE, this, &UGsUIPopupPartySetting::OnOneListChange)
		);
	}
	
}

// 탭 초기화
void UGsUIPopupPartySetting::InitTab()
{
	// check in party
	bool isInParty = GSParty()->GetIsInParty();

	// 획득룰 가져오기
	PartyDropOwnershipType currentOwnershipType = GSParty()->GetDropOwnershipType();
	_toggleGroupOwn.SetSelectedIndex((int)currentOwnershipType);

	// 내가 리더인가
	bool isIamLeader = GSParty()->AmILeader();
	// 내가 리더이거나 파티가 아니면 획득룰 탭 활성화하기
	bool isActiveOwnTab = (isIamLeader == true || isInParty == false) ? true : false;
	for (auto iter: _arrayOwnTab)
	{
		if (iter == nullptr)
		{
			continue;
		}

		iter->SetButtonEnable(isActiveOwnTab);
	}
}
// ok 버튼 초기화
void UGsUIPopupPartySetting::InitOkButton()
{
	bool isInParty = GSParty()->GetIsInParty();
	FText findText;
	
	if (isInParty == true)
	{
		// 파티중이면 확인
		FText::FindText(TEXT("PartyText"), TEXT("PartyTabOkButton"), findText);
	}
	else
	{
		// 파티중이 아니면 생성
		FText::FindText(TEXT("PartyText"), TEXT("PartyTabMakeButton"), findText);
	}

	_textOkBtn = findText;
}

void UGsUIPopupPartySetting::NativeDestruct()
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

// 탭 클릭
void UGsUIPopupPartySetting::OnClickTab(int32 In_index)
{
	_switcherSettingType->SetActiveWidgetIndex(In_index);
}

// 획득 룰 탭 클릭
void UGsUIPopupPartySetting::OnClickOwnTab(int32 In_index)
{
	_partyDropOwnershipType = (PartyDropOwnershipType)(In_index);


}

// ok 클릭
void UGsUIPopupPartySetting::OnClickOk()
{	
	bool isInParty = GSParty()->GetIsInParty();
	if (isInParty == false)
	{
		// 초대 받은게 있으면 일단 안함
		FGsPartyInviterInfo info = GSParty()->GetPartyInviteInfo();
		if (info.GetIsInviterExist() == true)
		{
			FText findText;
			FText::FindText(TEXT("PartyText"), TEXT("PartyTicker8"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}
		else
		{
			// 1. 파티에 속하지 않으면 파티 생성
			FGsNetSendService::SendPartyCreate(_partyDropOwnershipType);
		}
	}
	else
	{
		// 먼가 바뀌었을때만 하자
		PartyDropOwnershipType currentOwnershipType = GSParty()->GetDropOwnershipType();
		if (currentOwnershipType != _partyDropOwnershipType)
		{
			// 내가 리더일때만 룰변경 가능하다
			if (GSParty()->AmILeader() == true)
			{
				// 2. 파티에 속했으면 획득 룰 변경
				FGsNetSendService::SendPartyChangeDropOwnershipType(_partyDropOwnershipType);
			}
			else
			{
				// 리더 아니면 룰변경 못한다는 티커 메시지 출력
				FText findText;
				FText::FindText(TEXT("PartyText"), TEXT("PartyTicker9"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			}
		}
	}
	Close();
}
// cancel 클릭
void UGsUIPopupPartySetting::OnClickCancel()
{
	Close();
}
// 모든 리스트 변경
void UGsUIPopupPartySetting::OnAllListChange(const IGsMessageParam*)
{
	SetMemberListData();
}
// 한개 리스트 변경
void UGsUIPopupPartySetting::OnOneListChange(const IGsMessageParam* In_param)
{
	const FGsPrimitiveInt32* param = In_param->Cast<const FGsPrimitiveInt32>();
	int slotIndex = param->_data;

	_scrollBoxHelper->RefreshByIndex(slotIndex);	
}

void UGsUIPopupPartySetting::OnInputCancel()
{
	OnClickCancel();
}