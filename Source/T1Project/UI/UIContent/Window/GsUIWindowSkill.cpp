// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowSkill.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Common/Skill/GsUITargetDetailInfoBase.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Window/Skill/GsUISkillWeaponTab.h"
#include "UI/UIContent/Window/Skill/GsUISkillListEntry.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfo.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfoPassivity.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfoCostume.h"
#include "UI/UIContent/Window/Skill/GsUISkillSlotSetEquip.h"
#include "UI/UIContent/Window/Skill/GsUISkillSlotEquip.h"
#include "UI/UIContent/Popup/GsUIPopupSkillEnchant.h"
#include "UI/UIContent/Popup/GsUIPopupSkillCycleSetting.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsSkillPreviewManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemHelper.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "DataSchema/Costume/GsSchemaCostumeSkill.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "Costume/GsCostumeData.h"
#include "Engine/World.h"
#include "Significance/GsGameViewPortClient.h"


void UGsUIWindowSkill::BeginDestroy()
{
	if (_skillListHelper)
	{
		_skillListHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowSkill::OnRefreshEntry);
	}
	_skillListHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIWindowSkill::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::Skill;

	_weaponTabIndexMap.Empty();
	_weaponTabIndexMap.Emplace(CreatureWeaponType::ALL, 0);
	_weaponTabIndexMap.Emplace(CreatureWeaponType::SWORD, 1);
	_weaponTabIndexMap.Emplace(CreatureWeaponType::DAGGER, 2);
	_weaponTabIndexMap.Emplace(CreatureWeaponType::BOW, 3);
	_weaponTabIndexMap.Emplace(CreatureWeaponType::SHOTGUN, 4);
	_weaponTabIndexMap.Emplace(CreatureWeaponType::WAND, 5);
	_weaponTabIndexMap.Emplace(CreatureWeaponType::ORB, 6);

	_weaponTabMap.Empty();
	_weaponTabMap.Emplace(CreatureWeaponType::ALL, _tabCommon);
	_weaponTabMap.Emplace(CreatureWeaponType::SWORD, _tabSword);
	_weaponTabMap.Emplace(CreatureWeaponType::DAGGER, _tabDagger);
	_weaponTabMap.Emplace(CreatureWeaponType::BOW, _tabBow);
	_weaponTabMap.Emplace(CreatureWeaponType::SHOTGUN, _tabShotgun);
	_weaponTabMap.Emplace(CreatureWeaponType::WAND, _tabWand);
	_weaponTabMap.Emplace(CreatureWeaponType::ORB, _tabOrb);

	// 무기 탭 설정(주의: 위의 인덱스와 순서 동일해야 함)
	_toggleGroupWeapon.AddToggleWidget(_tabCommon->GetSwitcherButton());
	_toggleGroupWeapon.AddToggleWidget(_tabSword->GetSwitcherButton());
	_toggleGroupWeapon.AddToggleWidget(_tabDagger->GetSwitcherButton());
	_toggleGroupWeapon.AddToggleWidget(_tabBow->GetSwitcherButton());
	_toggleGroupWeapon.AddToggleWidget(_tabShotgun->GetSwitcherButton());
	_toggleGroupWeapon.AddToggleWidget(_tabWand->GetSwitcherButton());
	_toggleGroupWeapon.AddToggleWidget(_tabOrb->GetSwitcherButton());
	_toggleGroupWeapon.OnSelectChanged.BindUObject(this, &UGsUIWindowSkill::OnClickTabWeapon);	

	// 스킬 카테고리 탭 설정
	_toggleGroupSkillCategory.AddToggleWidgetByParentPanel(_panelSkillCategory);
	_toggleGroupSkillCategory.OnSelectChanged.BindUObject(this, &UGsUIWindowSkill::OnClickTabSkillCategory);

	// 스킬 카테고리 레드닷 마크 세팅
	_redDotCategoryMap.Emplace(SkillCategorySet::ACTIVE, _uiRedDotActive);
	_redDotCategoryMap.Emplace(SkillCategorySet::BUFF, _uiRedDotBuff);
	_redDotCategoryMap.Emplace(SkillCategorySet::PASSIVE, _uiRedDotPassive);
	_redDotCategoryMap.Emplace(COSTUME_CATEGORY_TYPE, _uiRedDotCostume);

	// SkillList 이벤트 세팅 
	_toggleGroupSkillList.OnSelectChanged.BindUObject(this, &UGsUIWindowSkill::OnSelectSkillList);
	_toggleGroupSkillList.OnSlotClicked.BindUObject(this, &UGsUIWindowSkill::OnClickSkillList);

	_skillListHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_skillListHelper->Initialize(_entryWidgetClass, _panelSkillList);
	_skillListHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowSkill::OnRefreshEntry);
	_skillListHelper->SetEmptyListPanel(_panelEmptyList);

	// 슬롯셋 세팅 
	_slotSet->OnClickSkillSlot.BindUObject(this, &UGsUIWindowSkill::OnClickSkillSlot);

	// 뒤로가기 버튼 세팅
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowSkill::OnClickBack);

	// 닫기버튼 세팅
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowSkill::OnClickCloseAllStack);

	// 습득/강화 버튼 이벤트
	_uiDetailInfo->OnClickEnchantOrLearn.BindUObject(this, &UGsUIWindowSkill::OnClickEnchantOrLearn);
	_uiDetailInfo->OnClickEquipOrCancel.BindUObject(this, &UGsUIWindowSkill::OnClickEquipOrCancel);
	_uiDetailInfo->OnClickUpgradeStep.BindUObject(this, &UGsUIWindowSkill::OnClickUpgradeStep);
	_uiDetailInfo->OnClickRemoveEquip.BindUObject(this, &UGsUIWindowSkill::OnClickRemoveEquip);
	_uiDetailInfo->OnClickSkillCycleSetting.BindUObject(this, &UGsUIWindowSkill::OnClickCycleSetting);

	// 습득 버튼만 존재
	_uiDetailInfoPassivity->OnClickLearn.BindUObject(this, &UGsUIWindowSkill::OnClickEnchantOrLearn);

	// 코스튬 동작은 분리해야 할 것도 같고... 확인 필요
	_uiDetailInfoCostume->OnClickEnchantOrLearn.BindUObject(this, &UGsUIWindowSkill::OnClickCostumeLearn);
	_uiDetailInfoCostume->OnClickEquipOrCancel.BindUObject(this, &UGsUIWindowSkill::OnClickCostumeEquip);
	_uiDetailInfoCostume->OnClickRemoveEquip.BindUObject(this, &UGsUIWindowSkill::OnClickCostumeRemoveEquip);
	_uiDetailInfoCostume->OnClickSkillCycleSetting.BindUObject(this, &UGsUIWindowSkill::OnClickCycleSetting);

	_scrollSkillList->OnUserScrolled.AddDynamic(this, &UGsUIWindowSkill::OnScrollSkillList);
}

void UGsUIWindowSkill::NativeConstruct()
{
	Super::NativeConstruct();

	// 슬롯 때문에 민감도 강제 조절
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->SetLongDragTriggerDist(true);

		if (UWorld* world = GetWorld())
		{
			// 드래그 민감도 갱신
			if (UGsGameViewPortClient* viewportClient = Cast<UGsGameViewPortClient>(world->GetGameViewport()))
			{
				viewportClient->UpdateDragTriggerDistance();
			}
		}
	}

	BindMessages();

	UGsSkillManager* skillMgr = GSkill();
	_openSelectedSkillId = INVALID_SKILL_ID;

	// 스킬 리스트맵을 전부 만들어 둔다(레드닷 체크때문에 어쩔 수 없다)
	if (FGsSkillUIDataHandler* skillDataHandler = skillMgr->GetSkillUIDataHandler())
	{
		skillDataHandler->InitSkillDataMap();
		_openSelectedSkillId = skillDataHandler->GetWindowOpenSelectedSkillId();
	}

	CreatureWeaponType selectedWeapon = skillMgr->GetCurrentWeaponType();

	// 장착 마크 설정(맨손 등일 경우 없을 수도 있음). 아래에서 selectedWeapon 정보 수정을 하므로 순서에 유의
	SetEquipMark(selectedWeapon);

	int32 selectedSkillCategory = 0;
	
	// 특정 스킬을 선택된 상태로 열 경우
	if (INVALID_SKILL_ID != _openSelectedSkillId)
	{
		const FGsSkill* skill = skillMgr->FindSkill(_openSelectedSkillId, CreatureWeaponType::MAX, true);
		if (skill && skill->_tableData)
		{
			selectedWeapon = skill->_tableData->requireWeapon;
			if (INVALID_COSTUME_ID == FGsSkillHelper::GetCostumeIdBySkillId(skill->GetSkillId()))
			{
				// 축복 스킬일 경우, 강제로 패시브 탭 선택
				if (CreatureWeaponType::ALL == selectedWeapon &&
					skill->_tableData->isBlessSkill)
				{
					selectedSkillCategory = static_cast<int32>(EGsCategoryTabType::TAB_PASSIVE_SKILL);
				}
				else
				{
					SkillCategorySet categorySet = skill->GetSkillCateogrySet();
					selectedSkillCategory = static_cast<int32>(categorySet);

					// 업그레이드 스킬이면, 리스트에 있는 스킬로 찾아줘야한다. 후...
					if (0 < skill->GetUpgradeStep())
					{
						if (const FGsSkillUIData* skillUIData = FindUpgradeSkill(skill))
						{
							_openSelectedSkillId = skillUIData->_skillId;
						}
					}
				}
			}
			else
			{
				selectedSkillCategory = static_cast<int32>(EGsCategoryTabType::TAB_COSTUME_SKILL);
			}
		}
	}

	// 현재 장착 장비가 없을 경우, 강제로 한손검 선택 
	if (CreatureWeaponType::NONE == selectedWeapon || CreatureWeaponType::HAND == selectedWeapon)
	{
		selectedWeapon = CreatureWeaponType::SWORD;
	}

	// 탭설정
	_toggleGroupWeapon.SetSelectedIndexNoEvent(ConvertWeaponTypeToIndex(selectedWeapon));
	_toggleGroupSkillCategory.SetSelectedIndexNoEvent(selectedSkillCategory);	
	_switcherDetailInfo->SetActiveWidgetIndex(GetDetailInfoPageIndex(selectedSkillCategory));

	// 장착모드 설정
	SetEquipMode(false, true);

	InvalidateAll();
}

void UGsUIWindowSkill::NativeDestruct()
{
	UnbindMessages();

	// 슬롯 때문에 민감도 강제 조절
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->SetLongDragTriggerDist(false);

		if (UWorld* world = GetWorld())
		{
			// 드래그 민감도 갱신
			if (UGsGameViewPortClient* viewportClient = Cast<UGsGameViewPortClient>(world->GetGameViewport()))
			{
				viewportClient->UpdateDragTriggerDistance();
			}
		}
	}

	_skillDataList.Empty();

	if (UGsSkillManager* skillMgr = GSkill())
	{
		if (FGsSkillUIDataHandler* skillDataHandler = skillMgr->GetSkillUIDataHandler())
		{
			skillDataHandler->SetWindowOpenSelectedSkillId(INVALID_SKILL_ID);
		}
	}
	_openSelectedSkillId = INVALID_SKILL_ID;
	_checkRedDotSkillId = INVALID_SKILL_ID;

	// 장착모드 관련 Tray가 열려있을 경우 닫기
	GUI()->CloseByKeyName(TEXT("TraySkillSlotEquipMode"));

	// 튜토리얼 관련
	SetTutorial(nullptr);	

	Super::NativeDestruct();
}

void UGsUIWindowSkill::BindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();

	_msgHandleGameObjectList.Emplace(msgMgr->GetGameObject().AddUObject(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT_EQUIP,
		this, &UGsUIWindowSkill::OnChangeSkillSlotEquip));
	_msgHandleGameObjectList.Emplace(msgMgr->GetGameObject().AddUObject(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT_AUTO,
		this, &UGsUIWindowSkill::OnChangeSkillSlotAuto));
	_msgHandleGameObjectList.Emplace(msgMgr->GetGameObject().AddUObject(MessageGameObject::LOCAL_UPDATE_SKILL_BONUS_LEVEL,
		this, &UGsUIWindowSkill::OnChangeSkillBonusLevel));
	_msgHandleGameObjectList.Emplace(msgMgr->GetGameObject().AddUObject(MessageGameObject::LOCAL_CHANGE_SKILL_CYCLE,
		this, &UGsUIWindowSkill::OnChangeSkillCycle));

	_msgHandleUIList.Emplace(msgMgr->GetUI().AddUObject(MessageUI::SKILL_UPDATE_REDDOT,
		this, &UGsUIWindowSkill::OnSkillUpdateRedDot));
	_msgHandleUIList.Emplace(msgMgr->GetUI().AddUObject(MessageUI::SKILL_ENCHANT_EFFECT_RESULT,
		this, &UGsUIWindowSkill::OnSkillEnchantResult));
}

void UGsUIWindowSkill::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MGameObject& msgGameObject = msgMgr->GetGameObject();
	for (MsgGameObjHandle& handle : _msgHandleGameObjectList)
	{
		msgGameObject.Remove(handle);
	}
	_msgHandleGameObjectList.Empty();

	MUI& msgUI = msgMgr->GetUI();
	for (MsgUIHandle& handle : _msgHandleUIList)
	{
		msgUI.Remove(handle);
	}
	_msgHandleUIList.Empty();
}

bool UGsUIWindowSkill::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsSkill))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SKILL);
		return true;
	}

	return false;
}

void UGsUIWindowSkill::InvalidateAllInternal()
{
	Super::InvalidateAllInternal();

	// 축복탭(ALL)일 경우 패시브 탭만 보이고, 강제로 패시브탭으로 선택 설정
	if (CreatureWeaponType::ALL == GetSelectedWeaponType())
	{
		int32 passivityTabIndex = static_cast<int32>(EGsCategoryTabType::TAB_PASSIVE_SKILL);

		for (int i = 0, maxCount = static_cast<int32>(EGsCategoryTabType::MAX_TYPE); i < maxCount; ++i)
		{
			_toggleGroupSkillCategory.SetVisibilityEntry(i,
				(i == passivityTabIndex) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}
	else
	{
		for (int i = 0, maxCount = static_cast<int32>(EGsCategoryTabType::MAX_TYPE); i < maxCount; ++i)
		{
			_toggleGroupSkillCategory.SetVisibilityEntry(i, ESlateVisibility::Visible);
		}
	}

	if (INVALID_SKILL_ID == _openSelectedSkillId)
	{
		InvalidateSkillList(); // 내부에서 SetSelectedIndex 하여 OnSelectSkillList 를 통해 상세창 갱신됨
		InvalidateSlotSet();
	}
	else
	{
		InvalidateSkillList(_openSelectedSkillId); // 내부에서 SetSelectedIndex 하여 OnSelectSkillList 를 통해 상세창 갱신됨
		InvalidateSlotSet();

		// 해당 아이템이 보이도록 스크롤 조정
		UWidget* scrollTarget = _panelSkillList->GetChildAt(_toggleGroupSkillList.GetSelectedIndex());
		if (scrollTarget)
		{
			_scrollSkillList->ScrollWidgetIntoView(scrollTarget);
		}

		_openSelectedSkillId = INVALID_SKILL_ID;
	}
}

void UGsUIWindowSkill::Close(bool bImmediate, bool bRecoverPrevStack)
{
	// State나갈 때 HUD레드닷 갱신을 하는데, 그 전에 레드닷을 처리하기 위해 여기에 넣음
	if (UGsSkillManager* skillMgr = GSkill())
	{
		// 마지막에 상세창 확인한 스킬의 레드닷 제거
		if (INVALID_SKILL_ID != _checkRedDotSkillId)
		{
			TWeakPtr<const FGsSkill> lastSelectedSkill = skillMgr->FindSkillWeakPtr(_checkRedDotSkillId, CreatureWeaponType::MAX, true);
			if (lastSelectedSkill.IsValid())
			{
				SetSkillRedDotOff(lastSelectedSkill.Pin().Get(), false);
			}
		}
	}

	_checkRedDotSkillId = INVALID_SKILL_ID;

	Super::Close(bImmediate, bRecoverPrevStack);
}

void UGsUIWindowSkill::InvalidateSkillList(SkillId InSelectedSkillId)
{
	// 레드닷 검사 / 업데이트 먼저 수행(리스트에 반영하기 위함)
	UpdateRedDot(true);

	DefCategorySkillDataMap* categoryMapPtr = nullptr;
	if (FGsSkillUIDataHandler* skillDataHandler = GSkill()->GetSkillUIDataHandler())
	{
		categoryMapPtr = skillDataHandler->GetCategoryMapPtr(GetSelectedWeaponType());
	}

	TArray<FGsSkillUIData*> tempDataList;
	if (categoryMapPtr)
	{
		if (TArray<FGsSkillUIData>* dataList = categoryMapPtr->Find(GetSelectedSkillCategorySet()))
		{
			for (FGsSkillUIData& iter : *dataList)
			{
				tempDataList.Emplace(&iter);
			}
		}
	}

	// 업그레이드 현재 스킬 지정
	for (FGsSkillUIData* skillData : tempDataList)
	{
		skillData->SetSkillCurrUpgradeStep();
	}

	// 소팅
	SortSkillList(tempDataList);

	// 리스트 가져와서 없으면 수행
	_skillDataList.Empty();
	for (int32 i = 0; i < tempDataList.Num(); ++i)
	{
		FGsSkillUIData* skillData = tempDataList[i];
		_skillDataList.Emplace(skillData);

		for (int32 j = 0; j < skillData->_chainList.Num(); ++j)
		{
			_skillDataList.Emplace(&skillData->_chainList[j]);
		}
	}

	// 스킬 리스트 갱신
	_skillListHelper->RefreshAll(_skillDataList.Num());

	// 리스트 토글그룹 설정, 리스트 스킬 선택
	_toggleGroupSkillList.Clear();
	_toggleGroupSkillList.AddToggleWidgetByParentPanel(_panelSkillList);

	if (INVALID_SKILL_ID == InSelectedSkillId)
	{
		_toggleGroupSkillList.SetSelectedIndex(0, true);

		if (false == IsTutorial())
		{
			// 스크롤 위치 최상단으로
			_scrollSkillList->ScrollToStart();
		}
	}
	else
	{
		int32 selectedSkillIndex = 0;
		for (int i = 0, maxCount = _skillDataList.Num(); i < maxCount; ++i)
		{
			if (_skillDataList[i]->GetSkillId() == InSelectedSkillId)
			{
				selectedSkillIndex = i;
				break;
			}
		}

		_toggleGroupSkillList.SetSelectedIndex(selectedSkillIndex, true);
	}

	if (IsTutorial())
	{
		if (UWidget* tutorialTarget = GetTargetSkillListItem(_tutorialSkillId))
		{
			SetTutorialTarget(tutorialTarget);
		}
	}
}

void UGsUIWindowSkill::InvalidateSlotSet(bool bInForced)
{
	int32 index = _toggleGroupSkillCategory.GetSelectedIndex();
	EGsCategoryTabType tabType = static_cast<EGsCategoryTabType>(index);

	switch (tabType)
	{
	case EGsCategoryTabType::TAB_PASSIVE_SKILL:
	{
		_slotSet->SetSkillCategory(index);
	}
	break;
	case EGsCategoryTabType::TAB_COSTUME_SKILL:
	{
		const FGsSkill* selectedSkill = GetSelectedSkill();
		_slotSet->SetData(GetSelectedWeaponType(), bInForced);

		// 액티브 스킬 슬롯 세팅
		_slotSet->SetSkillCategory(0);

		// 슬롯 세팅 가능여부 표시
		InvalidateSlotEnableEquipState();
	}
	break;
	default:
	{
		const FGsSkill* selectedSkill = GetSelectedSkill();
		_slotSet->SetData(GetSelectedWeaponType(), bInForced);
		_slotSet->SetSkillCategory(index);

		// 슬롯 세팅 가능여부 표시
		InvalidateSlotEnableEquipState();
	}
	break;
	}
	
	InvalidateMPInfo();
}

void UGsUIWindowSkill::InvalidateSkillListTarget(SkillId InSkillId)
{
	for (int32 i = 0; i < _skillDataList.Num(); ++i)
	{
		if (const FGsSkill* skill = _skillDataList[i]->GetSkill())
		{
			if (skill->GetSkillId() == InSkillId)
			{
				_skillListHelper->RefreshByIndex(i);
			}
		}
	}
}

void UGsUIWindowSkill::InvalidateSlotEnableEquipState()
{
	if (const FGsSkill* selectedSkill = GetSelectedSkill())
	{
		_slotSet->SetEnableEquip(CanEquipSkill(selectedSkill), _bIsEquipMode, IsCostumeCategory(), selectedSkill);
	}
	else
	{
		_slotSet->SetEnableEquip(false, _bIsEquipMode, IsCostumeCategory(), selectedSkill);
	}
}

void UGsUIWindowSkill::InvalidateMPInfo()
{
	_panelRootMPInfo->SetVisibility(ESlateVisibility::HitTestInvisible);

	float mpUsage = 0.f;
	float mpRecovery = 0.f;
	if (UGsSkillManager* skillMgr = GSkill())
	{
		if (FGsSkillUIDataHandler* skillDataHandler = skillMgr->GetSkillUIDataHandler())
		{
			CreatureWeaponType currWeaponType = GetSelectedWeaponType();
			mpUsage = skillDataHandler->CalcMpUsage(currWeaponType);
			mpRecovery = skillDataHandler->CalcMPRecovery();
		}
	}

	FText textMPUsage;
	FGsUIStringHelper::GetFormatedFractionalDigitsText(mpUsage, 0, 2, textMPUsage);
	_textBlockMPUsage->SetText(textMPUsage);
	
	FText textMPRecovery;
	FGsUIStringHelper::GetFormatedFractionalDigitsText(mpRecovery, 0, 2, textMPRecovery);
	_textBlockMPRecovery->SetText(textMPRecovery);
}

void UGsUIWindowSkill::SetEquipMark(CreatureWeaponType InType)
{
	for (TPair<CreatureWeaponType, UGsUISkillWeaponTab*>& iter : _weaponTabMap)
	{
		iter.Value->SetEquipMark((InType == iter.Key) ? true : false);
	}
}

void UGsUIWindowSkill::SetEquipMode(bool bIsEquipMode, bool bIsForced)
{
	bool bIsChanged = (bIsEquipMode != _bIsEquipMode) ? true : false;
	_bIsEquipMode = bIsEquipMode;

	if (bIsChanged ||
		bIsForced)
	{
		if (_bIsEquipMode)
		{
			if (false == GUI()->IsActiveWidget(TEXT("TraySkillSlotEquipMode")))
			{
				GUI()->Open(TEXT("TraySkillSlotEquipMode"));
			}
		}
		else
		{
			GUI()->CloseByKeyName(TEXT("TraySkillSlotEquipMode"));
		}

		// 상세창 버튼상태(장착,취소,해제) 변경
		if (UGsUITargetDetailInfoBase* detailInfo = GetCurrentDetailInfo())
		{
			if (const FGsSkill* skill = GetSelectedSkill())
			{
				detailInfo->SetEquipMode(CanEquipSkill(skill, false), _bIsEquipMode);
			}
			else
			{
				detailInfo->SetEquipMode(false, _bIsEquipMode);
			}
		}

		// 스킬슬롯 갱신
		InvalidateSlotEnableEquipState();
	}
}

void UGsUIWindowSkill::OnClickTabWeapon(int32 InIndex)
{
	// 축복탭일 경우 패시브 탭만 보이고, 강제로 패시브탭으로 선택 설정
	if (CreatureWeaponType::ALL == GetSelectedWeaponType())
	{
		_toggleGroupSkillCategory.SetSelectedIndexNoEvent(static_cast<int32>(EGsCategoryTabType::TAB_PASSIVE_SKILL));
	}

	// 주의: 순서에 유의. 위에서 _toggleGroupSkillCategory 값을 준 것이 GetDetailInfoPageIndex 에서 영향 받음
	int32 detailInfoIndex = GetDetailInfoPageIndex();
	_switcherDetailInfo->SetActiveWidgetIndex(detailInfoIndex);	

	InvalidateAll();
}

void UGsUIWindowSkill::OnClickTabSkillCategory(int32 InIndex)
{
	int32 detailInfoIndex = GetDetailInfoPageIndex(InIndex);
	_switcherDetailInfo->SetActiveWidgetIndex(detailInfoIndex);

	InvalidateAll();
}

void UGsUIWindowSkill::OnSelectSkillList(int32 InIndex)
{
	// 이전에 선택되었던 스킬의 레드닷 해제
	// 마지막에 상세창 확인한 스킬의 레드닷 제거
	if (INVALID_SKILL_ID != _checkRedDotSkillId)
	{
		if (UGsSkillManager* skillMgr = GSkill())
		{
			TWeakPtr<const FGsSkill> checkRedDotSkill = skillMgr->FindSkillWeakPtr(_checkRedDotSkillId, CreatureWeaponType::MAX, true);
			if (checkRedDotSkill.IsValid())
			{
				SetSkillRedDotOff(checkRedDotSkill.Pin().Get(), true);
			}
		}

		_checkRedDotSkillId = INVALID_SKILL_ID;
	}

	// 스킬이 바뀌면 장착 모드 해제
	SetEquipMode(false);
		
	const FGsSkill* selectedSkill = nullptr;

	FGsSkillUIData* selectedSkillData = GetSelectedSkillData();
	if (selectedSkillData)
	{
		selectedSkill = selectedSkillData->GetSkill();
		_selectedUpgradeStep = selectedSkill->GetUpgradeStep();

		PlaySkillPreview(selectedSkillData);

		InvalidateSlotSet();
	}

	// 상세창 갱신
	if (UGsUITargetDetailInfoBase* detailInfo = GetCurrentDetailInfo())
	{
		detailInfo->SetDataAll(GetSelectedSkillData());
		detailInfo->SetEquipMode(CanEquipSkill(selectedSkill), _bIsEquipMode);

		// 마지막 확인한 스킬
		if (selectedSkill)
		{
			if (selectedSkill->_isNew)
			{
				_checkRedDotSkillId = selectedSkill->GetSkillId();
			}
		}
	}
}

void UGsUIWindowSkill::OnClickSkillList(int32 InIndex, bool bIsSelected)
{
	// 레드닷 해제 체크
	if (_skillDataList.IsValidIndex(InIndex))
	{
		if (const FGsSkill* clickedSkill = _skillDataList[InIndex]->GetSkill())
		{
			if (clickedSkill->_isNew)
			{
				if (_checkRedDotSkillId == clickedSkill->GetSkillId())
				{
					_checkRedDotSkillId = INVALID_SKILL_ID;
				}

				SetSkillRedDotOff(clickedSkill, true);
			}
		}
	}

	if (false == bIsSelected)
	{
		// 이미 선택한 스킬을 다시 클릭할 경우 IsSelected가 false
		if (_toggleGroupSkillList.GetSelectedIndex() == InIndex)
		{
			if (FGsSkillUIData* skillData = GetSelectedSkillData())
			{
				PlaySkillPreview(skillData);
			}
		}		
	}
}

void UGsUIWindowSkill::OnClickSkillSlot(UGsUISkillSlotEquip* InSlot)
{
	if (_bIsEquipMode)
	{
		if (InSlot &&
			MAX_COSTUME_SKILL_SLOT_ID == InSlot->GetSlotId())
		{
			// TEXT: 코스튬 스킬 슬롯에는 장착할 수 없습니다
			FText textMsg;
			if (FText::FindText(TEXT("SkillUIText"), TEXT("Notice_CostumeSkill_WrongSlot"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}

			return;
		}

		RequestSetSkillSlot(InSlot->GetSlotId(), GetSelectedSkillData());
	}
	else
	{
		if (nullptr == InSlot ||
			nullptr == InSlot->GetSkill())
		{
			return;
		}

		SkillId skillId = InSlot->GetSkill()->GetSkillId();

		int32 index = -1;
		for (int i = 0, maxCount = _skillDataList.Num(); i < maxCount; ++i)
		{
			if (_skillDataList[i]->GetSkillId() == skillId)
			{
				index = i;
			}
		}

		// 못찾으면 변경하지 않음
		if (-1 == index)
		{
			return;
		}

		_toggleGroupSkillList.SetSelectedIndex(index);

		ScrollSkillListToTarget(index);
	}
}

void UGsUIWindowSkill::OnClickEnchantOrLearn(bool bIsEnchant)
{
	const FGsSkill* selectedSkill = GetSelectedSkill();
	if (nullptr == selectedSkill)
	{
		return;
	}

	// 강화/업그레이드일 경우 창 열기
	if (bIsEnchant)
	{
		TWeakObjectPtr<UGsUIWidgetBase> popupWidget = GUI()->OpenAndGetWidget(TEXT("PopupSkillEnchant"));
		if (popupWidget.IsValid())
		{
			UGsUIPopupSkillEnchant* popup = Cast<UGsUIPopupSkillEnchant>(popupWidget.Get());
			if (popup)
			{
				popup->SetData(selectedSkill);
			}
		}

		return;
	}

	// 습득일 경우 처리
	if (false == FGsSkillHelper::CanLearn(selectedSkill))
	{
		return;
	}

	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
		return;
	}

	FGsSkillUIDataHandler* skillDataHandler = GSkill()->GetSkillUIDataHandler();
	if (nullptr == skillDataHandler)
	{
		return;
	}

	const TArray<ItemId>* skillBookListPtr = skillDataHandler->GetExistSkillBookList(selectedSkill->GetSkillId());
	if (nullptr == skillBookListPtr ||
		0 == skillBookListPtr->Num())
	{
		return;
	}

	for (const ItemId& skillBookItemId : *skillBookListPtr)
	{
		TArray<TWeakPtr<FGsItem>> skillBookList = itemMgr->FindByTID(skillBookItemId, ItemType::CONSUME);
		for (TWeakPtr<const FGsItem> item : skillBookList)
		{
			if (item.IsValid())
			{
				// 잠기지 않은 스킬북 찾기
				if (false == item.Pin()->GetLock())
				{
					// 스킬북 사용
					itemMgr->TryUseItem(item.Pin()->GetDBID());
					return;
				}
			}
		}
	}
}

void UGsUIWindowSkill::OnClickEquipOrCancel(bool bIsEquipMode)
{
	SetEquipMode(bIsEquipMode);	
}

void UGsUIWindowSkill::OnClickUpgradeStep(uint8 InUpgradeStep)
{
	if (FGsSkillUIData* data = GetSelectedSkillData())
	{
		_selectedUpgradeStep = InUpgradeStep;

		if (const FGsSkill* skill = data->GetUpgradeSkillByStep(_selectedUpgradeStep))
		{
			PlaySkillPreview(data);

			if (UGsUITargetDetailInfoBase* detailInfo = GetCurrentDetailInfo())
			{
				// 업그레이드 스킬은 레벨이 공유된다
				int32 overrideLevel = 0;
				if (data->IsUpgradeSkill())
				{
					if (const FGsSkill* currSkill = data->GetSkill())
					{
						overrideLevel = currSkill->GetTotalLevel();
					}
				}

				detailInfo->SetData(skill, overrideLevel);
				detailInfo->SetEquipMode(CanEquipSkill(skill, false), _bIsEquipMode);
			}
		}
	}
}

void UGsUIWindowSkill::OnClickRemoveEquip()
{
	if (const FGsSkill* skill = GetSelectedSkill())
	{
		SkillId rootSkillId = FGsSkillHelper::GetUpgradeRootSkillId(skill);
		if (INVALID_SKILL_ID == rootSkillId)
		{
			rootSkillId = skill->GetSkillId();
		}

		FGsNetSendServiceWorld::SendRequestSkillSlotSet(skill->GetSkillRequireWeaponType(),
			rootSkillId, skill->GetSkillId(), INVALID_SKILL_SLOT_ID);
	}
}

void UGsUIWindowSkill::OnClickCostumeLearn(bool)
{	
	if (FGsSkillUIData* skillUIData = GetSelectedSkillData())
	{
		if (skillUIData->_costume)
		{
			FGsNetSendServiceWorld::SendRequestCostumeSkillLearn(skillUIData->_costume->id);
		}
	}
}

void UGsUIWindowSkill::OnClickCostumeEquip(bool)
{
	RequestSetSkillSlot(MAX_COSTUME_SKILL_SLOT_ID, GetSelectedSkillData());
}

void UGsUIWindowSkill::OnClickCostumeRemoveEquip()
{
	if (UGsUISkillSlotEquip* slot = _slotSet->FindSlotById(MAX_COSTUME_SKILL_SLOT_ID))
	{
		slot->RequestRemoveSlot();
	}
}

void UGsUIWindowSkill::OnClickCycleSetting()
{
	FGsSkillUIData* selectedSkillData = GetSelectedSkillData();
	if (nullptr == selectedSkillData)
	{
		return;
	}

	const FGsSkill* selectedSkill = selectedSkillData->GetSkill();
	if (nullptr == selectedSkill ||
		false == selectedSkill->_isLearn)
	{
		// 버튼 비활성화 등으로 선택 자체가 안되어야 하지만 안전을 위해 
		return;
	}	

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupSkillCycleSetting"));
	if (false == widget.IsValid())
	{
		return;
	}

	if (UGsUIPopupSkillCycleSetting* popup = Cast<UGsUIPopupSkillCycleSetting>(widget.Get()))
	{
		if (selectedSkillData->IsUpgradeSkill())
		{
			const FGsSkill* rootSkill = selectedSkillData->GetUpgradeRootSkill();
			if (nullptr == rootSkill)
			{
				return;
			}

			popup->SetData(rootSkill->GetSkillId(), selectedSkill);
		}
		else
		{
			popup->SetData(selectedSkill->GetSkillId(), selectedSkill);
		}
	}
}

void UGsUIWindowSkill::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _skillDataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (FGsSkillUIData* skillData = _skillDataList[InIndex])
	{
		if (UGsUISkillListEntry* skillEntry = Cast<UGsUISkillListEntry>(InEntry))
		{
			skillEntry->SetData(skillData);
		}
	}
}

void UGsUIWindowSkill::OnChangeSkillSlotEquip(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		const FGsGameObjectMessageParamSkillSlot* paramSlot = static_cast<const FGsGameObjectMessageParamSkillSlot*>(InParam);
		if (paramSlot)
		{
			
			if (paramSlot->_skill)
			{
				// 체인 스킬 사용에 의해 슬롯이 바뀌지 않도록 설정
				if (1 < paramSlot->_skill->GetChainStep())
				{
					return;
				}

				// 스킬 슬롯 Equip 상태 동일하면 갱신하지 않음(체인스킬 원상복구 시)
				if (const FGsSkill* skill = _slotSet->GetSkill(paramSlot->_skill->GetSlotId()))
				{
					if (skill->GetSkillId() == paramSlot->_skill->GetSkillId())
					{
						return;
					}
				}
			}
			
			SkillId selectedId = INVALID_SKILL_ID;
			const FGsSkill* selectedSkill = GetSelectedSkill();
			if (selectedSkill)
			{
				selectedId = selectedSkill->GetSkillId();
			}

			// 착용시 체인스킬을 모두 Equip마크 처리 등 일부 슬롯만 갱신하기 어려워 전체 갱신
			InvalidateSkillList(selectedId);

			// 스킬 슬롯 갱신
			_slotSet->InvalidateSlotBySlotId(paramSlot->_slotId);

			// 슬롯 세팅 가능여부 표시			
			InvalidateSlotEnableEquipState();

			InvalidateMPInfo();
		}
	}
}

void UGsUIWindowSkill::OnChangeSkillSlotAuto(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsGameObjectMessageParamSkillSlot* paramSlot = static_cast<const FGsGameObjectMessageParamSkillSlot*>(InParam);
	if (paramSlot)
	{
		if (paramSlot->_skill)
		{
			InvalidateSkillListTarget(paramSlot->_skill->GetSkillId());

			_slotSet->InvalidateSlotBySlotId(paramSlot->_slotId);

			InvalidateMPInfo();
		}
	}
}

void UGsUIWindowSkill::OnChangeSkillBonusLevel(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsSkill* updatedSkill = nullptr;
	if (const FGsPrimitiveUInt32* param = InParam->Cast<const FGsPrimitiveUInt32>())
	{
		updatedSkill = GSkill()->FindSkill(param->_data, CreatureWeaponType::MAX, true);
	}

	if (nullptr == updatedSkill ||
		nullptr == updatedSkill->_tableData)
	{
		return;
	}

	// 현재 보고있는 탭과 동일한 경우에만 리스트를 갱신
	if (GetSelectedWeaponType() != updatedSkill->_tableData->requireWeapon)
	{
		return;
	}

	if (GetSelectedSkillCategorySet() != updatedSkill->GetSkillCateogrySet())
	{
		return;
	}
	
	SkillId selectedId = INVALID_SKILL_ID;
	if (const FGsSkill* skill = GetSelectedSkill())
	{
		selectedId = skill->GetSkillId();
	}

	InvalidateSkillList(selectedId);
}

void UGsUIWindowSkill::OnChangeSkillCycle(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsGameObjectMessageParamSkill* param = InParam->Cast<const FGsGameObjectMessageParamSkill>();
	if (nullptr == param)
	{
		return;
	}

	InvalidateSkillListTarget(param->_skillId);
	InvalidateMPInfo();
}

void UGsUIWindowSkill::OnSkillUpdateRedDot(const IGsMessageParam*)
{
	SkillId selectedId = INVALID_SKILL_ID;
	if (const FGsSkill* skill = GetSelectedSkill())
	{
		selectedId = skill->GetSkillId();
	}

	InvalidateSkillList(selectedId);
}

void UGsUIWindowSkill::OnSkillEnchantResult(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsUIMsgParamSkillEnchantResult* param = InParam->Cast<const FGsUIMsgParamSkillEnchantResult>();
	if (nullptr == param)
	{
		return;
	}

	if (SkillEnchantResult::NONE == param->_result)
	{
		return;
	}

	InvalidateSkillList(param->_resultSkillId);
}

int32 UGsUIWindowSkill::ConvertWeaponTypeToIndex(CreatureWeaponType InType) const
{
	if (_weaponTabIndexMap.Contains(InType))
	{
		return _weaponTabIndexMap[InType];
	}

	// 없으면 그냥 0으로 리턴
	return 0;
}

CreatureWeaponType UGsUIWindowSkill::ConvertIndexToWeaponType(int32 InIndex) const
{	
	for (const TPair<CreatureWeaponType, int32>& itPair : _weaponTabIndexMap)
	{
		if (itPair.Value == InIndex)
		{
			return itPair.Key;
		}
	}

	// 없으면 그냥 첫번째 탭
	return CreatureWeaponType::ALL;
}

bool UGsUIWindowSkill::CanEquipSkill(const FGsSkill* InSkill, bool bShowMessage) const
{
	if (nullptr == InSkill)
	{
		return false;
	}

	// 습득한 스킬 중 체인 2단계부터는 세팅 불가
	if (InSkill->_isLearn)
	{
		if (InSkill->IsChainSkill())
		{
			if (1 < InSkill->GetChainStep())
			{
				if (bShowMessage)
				{
					// TEXT: 1단계 체인 스킬만 스킬 슬롯에 등록할 수 있습니다.
					FText textMsg;
					if (FText::FindText(TEXT("SkillUIText"), TEXT("Notice_ChainSkill_NotRegistration"), textMsg))
					{
						FGsUIHelper::TrayMessageTicker(textMsg);
					}
				}

				return false;
			}
		}
	}

	return InSkill->_isLearn;
}

void UGsUIWindowSkill::SortSkillList(OUT TArray<FGsSkillUIData*>& OutList)
{
	// 튜토리얼일 경우에 사용하는 소팅
	if (IsTutorial())
	{
		// 리스트 소팅
		// 1순위: 튜토리얼 스킬
		// 2순위부턴 일반 소팅과 같음
		OutList.Sort([this](FGsSkillUIData& A, FGsSkillUIData& B)
			{
				if (_tutorialSkillId == A.GetSkillId())
				{
					return true;
				}
				else if (_tutorialSkillId != A.GetSkillId() && _tutorialSkillId != B.GetSkillId())
				{
					const FGsSkill* skillA = A.GetSkill();
					const FGsSkill* skillB = B.GetSkill();
					if (nullptr == skillA || nullptr == skillB)
					{
						return true;
					}

					if (skillA->_isLearn && !skillB->_isLearn)
					{
						return true;
					}
					else if (skillA->_isLearn == skillB->_isLearn)
					{
						return SortSkillData(skillA->_tableData, skillB->_tableData);
					}
				}

				return false;
			});
	}
	else
	{
		// 코스튬 스킬의 경우
		if (IsCostumeCategory())
		{
			// 리스트 소팅
			// 1순위: 습득
			// 2순위: 코스튬 등급 낮은 순
			// 3순위: CostumeId 오름차순
			OutList.Sort([this](FGsSkillUIData& A, FGsSkillUIData& B)
				{
					if (nullptr == A._costume || nullptr == B._costume)
					{
						return true;
					}

					// 습득상태(코스튬도 체크하기 위해 IsExist로 체크)
					if (A.IsCollectedCostume() && !B.IsCollectedCostume())
					{
						// 1순위: 배운스킬 우선
						return true;
					}
					else if (A.IsCollectedCostume() == B.IsCollectedCostume())
					{
						// 스킬등급
						if (A._costume->grade < B._costume->grade)
						{
							return true;
						}
						else if (A._costume->grade == B._costume->grade)
						{
							// id 오름차순
							return (A._costume->id <= B._costume->id) ? true : false;
						}

						return false;
					}

					return false;
				});
		}
		else
		{
			// 리스트 소팅
			// 1순위: 습득
			// 2순위: 등급 낮은 순
			// 3순위: 무기스킬
			// 4순위: SkillId 오름차순
			OutList.Sort([this](FGsSkillUIData& A, FGsSkillUIData& B)
				{
					const FGsSkill* skillA = A.GetSkill();
					const FGsSkill* skillB = B.GetSkill();
					if (nullptr == skillA || nullptr == skillB)
					{
						return true;
					}

					// 습득상태(코스튬도 체크하기 위해 IsExist로 체크)
					if (skillA->_isLearn && !skillB->_isLearn)
					{
						// 1순위: 배운스킬 우선
						return true;
					}
					else if (skillA->_isLearn == skillB->_isLearn)
					{
						return SortSkillData(skillA->_tableData, skillB->_tableData);
					}

					return false;
				});
		}
	}
}

bool UGsUIWindowSkill::SortSkillData(const FGsSchemaSkillSet* InA, const FGsSchemaSkillSet* InB)
{
	// 스킬등급
	if (InA->grade < InB->grade)
	{
		return true;
	}
	else if (InA->grade == InB->grade)
	{
		// 무기스킬 우선(공통스킬 후순)
		if (CreatureWeaponType::ALL > InA->requireWeapon)
		{
			return true;
		}
		else if (InA->requireWeapon == InB->requireWeapon)
		{
			// SkillId 오름차순
			return (InA->id <= InB->id) ? true : false;
		}
	}

	return false;
}

void UGsUIWindowSkill::ScrollSkillListToTarget(int32 InIndex)
{
	float offset = _listItemHeight * InIndex;
	_scrollSkillList->SetScrollOffset(offset);
}

void UGsUIWindowSkill::RequestSetSkillSlot(SkillSlotId InSlotId, FGsSkillUIData* InSkillData)
{
	if (nullptr == InSkillData)
	{
		return;
	}

	const FGsSkill* selectedSkill = InSkillData->GetSkill();
	if (nullptr == selectedSkill)
	{
		return;
	}

	// 같은 슬롯 클릭했는지 검사
	if (selectedSkill->GetSlotId() == InSlotId)
	{
		return;
	}

	if (false == CanEquipSkill(selectedSkill, true))
	{
		return;
	}

	SkillId rootSkillId = InSkillData->GetSkillId();
	if (InSkillData->IsUpgradeSkill())
	{
		const FGsSkill* rootSkill = InSkillData->GetUpgradeRootSkill();
		if (nullptr == rootSkill)
		{
			return;
		}

		rootSkillId = rootSkill->GetSkillId();
	}

	if (InSkillData->IsCostumeSkill())
	{
		FGsNetSendServiceWorld::SendRequestCostumeSkillSlotSet(InSkillData->_costume->id,
			rootSkillId, InSkillData->GetSkillId(), InSlotId);
	}
	else
	{
		// 동일 그룹이 존재할 경우, 해당슬롯을 해제하는 것은 서버에서 해주기 대문에 클라 검사를 넣을 필요 없음
		FGsNetSendServiceWorld::SendRequestSkillSlotSet(GetSelectedWeaponType(),
			rootSkillId, InSkillData->GetSkillId(), InSlotId);
	}
}

void UGsUIWindowSkill::SetSkillRedDotOff(const FGsSkill* InSkill, bool bUpdateUI)
{
	if (nullptr == InSkill)
	{
		return;
	}

	if (false == InSkill->_isNew)
	{
		return;
	}

	if (UGsSkillManager* skillMgr = GSkill())
	{
		// 레드닷 Off
		skillMgr->SetIsNew(InSkill, false);
	}

	if (bUpdateUI)
	{
		CreatureWeaponType currWeapon = GetSelectedWeaponType();
		if (currWeapon == InSkill->GetSkillRequireWeaponType())
		{
			UpdateRedDot(false);

			// 리스트의 레드닷 제거 위함
			for (int32 i = 0; i < _skillDataList.Num(); ++i)
			{
				if (InSkill->GetSkillId() == _skillDataList[i]->GetSkillId())
				{
					_skillListHelper->RefreshByIndex(i);
					break;
				}
			}
		}
		else
		{
			// 모든 탭 갱신
			UpdateRedDot(true);
		}
	}
}

void UGsUIWindowSkill::UpdateRedDot(bool bAllTab)
{
	CreatureWeaponType weaponType = GetSelectedWeaponType();

	// 해당 탭만 업데이트이면
	if (false == bAllTab)
	{
		bool bIsRedDot = false;
		bool bIsReinforce = false;
		UpdateRedDotInter(weaponType, bIsRedDot, bIsReinforce);

		if (UGsUISkillWeaponTab** weaponTab = _weaponTabMap.Find(weaponType))
		{
			if (UGsUIRedDotBase* uiRedDotWeapon = (*weaponTab)->GetRedDot())
			{
				uiRedDotWeapon->SetRedDot(bIsRedDot, EGsIconRedDotType::NORMAL);
				uiRedDotWeapon->SetRedDot(bIsReinforce, EGsIconRedDotType::REINFORCE);
			}
		}

		return;
	}

	FGsSkillUIDataHandler* skillDataHandler = GSkill()->GetSkillUIDataHandler();
	if (nullptr == skillDataHandler)
	{
		return;
	}

	TMap<CreatureWeaponType, DefCategorySkillDataMap> skillDataMap = skillDataHandler->GetSkillDataMap();

	// 모든 탭 업데이트일 경우
	for (TPair<CreatureWeaponType, DefCategorySkillDataMap>& itPairWeapon : skillDataMap)
	{
		bool bIsRedDot = false;
		bool bIsReinforce = false;

		// 선택된 무기 카테고리일 경우
		if (weaponType == itPairWeapon.Key)
		{
			UpdateRedDotInter(weaponType, bIsRedDot, bIsReinforce);
		}
		else
		{
			// 선택된 무기의 카테고리가 아닐경우, 다 검사할 필요가 없고, 레드닷 감지되면 바로 빠져나감
			for (TPair<SkillCategorySet, TArray<FGsSkillUIData>>& itPairCategory : itPairWeapon.Value)
			{
				for (FGsSkillUIData& skillData : itPairCategory.Value)
				{
					skillData.UpdateRedDot();

					// 레드닷이 있으면 바로 종료
					if (skillData.IsRedDot())
					{
						bIsRedDot = true;
						break;
					}
					else if (skillData.IsReinforce())
					{
						bIsReinforce = true;
					}

					// 체인 스킬 검사
					for (FGsSkillUIData& chainSkillData : skillData._chainList)
					{
						chainSkillData.UpdateRedDot();

						if (chainSkillData.IsRedDot())
						{
							bIsRedDot = true;
							break;
						}
						else if (skillData.IsReinforce())
						{
							bIsReinforce = true;
						}
					}

					// 체인스킬 중 레드닷 있으면 바로 종료
					if (bIsRedDot)
					{
						break;
					}
				}

				// 해당 카테고리에서 레드닷 있으면 바로 종료
				if (bIsRedDot)
				{
					break;
				}
			}
		}

		// 무기 탭 레드닷 업데이트
		if (UGsUISkillWeaponTab** weaponTab = _weaponTabMap.Find(itPairWeapon.Key))
		{
			if (UGsUIRedDotBase* uiRedDotWeapon = (*weaponTab)->GetRedDot())
			{
				uiRedDotWeapon->SetRedDot(bIsRedDot, EGsIconRedDotType::NORMAL);
				uiRedDotWeapon->SetRedDot(bIsReinforce, EGsIconRedDotType::REINFORCE);
			}
		}
	}
}

void UGsUIWindowSkill::UpdateRedDotInter(CreatureWeaponType InType, OUT bool& bOutRedDot, OUT bool& bOutReinforce)
{
	DefCategorySkillDataMap* categoryMapPtr = nullptr;
	if (FGsSkillUIDataHandler* skillDataHandler = GSkill()->GetSkillUIDataHandler())
	{
		categoryMapPtr = skillDataHandler->GetCategoryMapPtr(InType);
	}

	if (nullptr == categoryMapPtr)
	{
		bOutRedDot = false;
		bOutReinforce = false;

		return;
	}

	for (TPair<SkillCategorySet, TArray<FGsSkillUIData>>& itPair : *categoryMapPtr)
	{
		bool bIsRedDotCategory = false;
		bool bIsReinforceCategory = false;

		for (FGsSkillUIData& skillData : itPair.Value)
		{
			skillData.UpdateRedDot();

			if (skillData.IsRedDot())
			{
				bOutRedDot = true;
				bIsRedDotCategory = true;
			}
			else if (skillData.IsReinforce())
			{
				bOutReinforce = true;
				bIsReinforceCategory = true;
			}

			// 체인스킬 검사
			for (FGsSkillUIData& chainSkillData : skillData._chainList)
			{
				chainSkillData.UpdateRedDot();

				if (chainSkillData.IsRedDot())
				{
					bOutRedDot = true;
					bIsRedDotCategory = true;
				}
				else if (skillData.IsReinforce())
				{
					bOutReinforce = true;
					bIsReinforceCategory = true;
				}
			}
		}

		// 카테고리 탭 업데이트
		if (UGsUIRedDotBase** uiRedDotCategory = _redDotCategoryMap.Find(itPair.Key))
		{
			(*uiRedDotCategory)->SetRedDot(bIsRedDotCategory, EGsIconRedDotType::NORMAL);
			(*uiRedDotCategory)->SetRedDot(bIsReinforceCategory, EGsIconRedDotType::REINFORCE);
		}
	}
}

void UGsUIWindowSkill::PlaySkillPreview(FGsSkillUIData* InData)
{
	UGsSkillPreviewManager* skillPreviewMgr = GSSkillPreview();
	if (nullptr == skillPreviewMgr)
	{
		return;
	}

	if (InData)
	{
		const FGsSkill* skill = nullptr;
		if (InData->IsUpgradeSkill())
		{
			skill = InData->GetUpgradeSkillByStep(_selectedUpgradeStep);
		}
		else
		{
			skill = InData->GetSkill();
		}

		if (skill)
		{
			// 패시브 스킬은 가린다
			if (SkillCategorySet::PASSIVE != skill->GetSkillCateogrySet())
			{
				// 프리뷰 재생
				skillPreviewMgr->PlaySkillPreview(skill->GetSkillId(), skill->GetTotalLevel(), InData->_costume);
				return;
			}
		}
	}

	// 값이 없는 상황이면 강제 중지
	skillPreviewMgr->ClearTimer();
	skillPreviewMgr->DespawnAllGameObject();
}

int32 UGsUIWindowSkill::GetDetailInfoPageIndex(int32 InCategoryIndex) const
{
	int32 categoryIndex = InCategoryIndex;
	if (0 > InCategoryIndex)
	{
		categoryIndex = _toggleGroupSkillCategory.GetSelectedIndex();
	}

	EGsCategoryTabType tabType = static_cast<EGsCategoryTabType>(categoryIndex);
	switch (tabType)
	{
	case EGsCategoryTabType::TAB_PASSIVE_SKILL:
		return 1;
	case EGsCategoryTabType::TAB_COSTUME_SKILL:
		return 2;
	}

	return 0;
}

FGsSkillUIData* UGsUIWindowSkill::GetSelectedSkillData() const
{
	int32 index = _toggleGroupSkillList.GetSelectedIndex();
	if (_skillDataList.IsValidIndex(index))
	{
		return _skillDataList[index];
	}

	return nullptr;
}

SkillId UGsUIWindowSkill::GetSelectedSkillId() const
{
	if (const FGsSkillUIData* data = GetSelectedSkillData())
	{
		return data->_skillId;
	}

	return INVALID_SKILL_ID;
}

const FGsSkill* UGsUIWindowSkill::GetSelectedSkill() const
{
	if (FGsSkillUIData* data = GetSelectedSkillData())
	{
		return data->GetSkill();
	}

	return nullptr;
}

CreatureWeaponType UGsUIWindowSkill::GetSelectedWeaponType() const
{
	return ConvertIndexToWeaponType(_toggleGroupWeapon.GetSelectedIndex());
}

SkillCategorySet UGsUIWindowSkill::GetSelectedSkillCategorySet() const
{
	int32 selectedIndex = _toggleGroupSkillCategory.GetSelectedIndex();
	if (static_cast<int32>(EGsCategoryTabType::TAB_COSTUME_SKILL) == selectedIndex)
	{
		return COSTUME_CATEGORY_TYPE;
	}

	return static_cast<SkillCategorySet>(selectedIndex);
}

const FGsSkillUIData* UGsUIWindowSkill::FindUpgradeSkill(const FGsSkill* InSkill)
{
	if (nullptr == InSkill ||
		nullptr == InSkill->_tableData)
	{
		return nullptr;
	}

	CreatureWeaponType weaponType = InSkill->_tableData->requireWeapon;
	if (CreatureWeaponType::NONE == weaponType ||
		CreatureWeaponType::HAND == weaponType ||
		CreatureWeaponType::ALL == weaponType ||
		CreatureWeaponType::MAX == weaponType)
	{
		return nullptr;
	}

	DefCategorySkillDataMap* categoryMapPtr = nullptr;
	if (FGsSkillUIDataHandler* skillDataHandler = GSkill()->GetSkillUIDataHandler())
	{
		categoryMapPtr = skillDataHandler->GetCategoryMapPtr(weaponType);
	}

	if (categoryMapPtr)
	{
		if (TArray<FGsSkillUIData>* dataListPtr = categoryMapPtr->Find(InSkill->GetSkillCateogrySet()))
		{
			for (FGsSkillUIData& data : *dataListPtr)
			{
				data.SetSkillCurrUpgradeStep();

				if (InSkill->GetSkillId() == data._skillId)
				{
					return &data;
				}

				for (TWeakPtr<const FGsSkill> upgradeSkill : data._upgradeList)
				{
					if (InSkill->GetSkillId() == upgradeSkill.Pin()->GetSkillId())
					{
						return &data;
					}
				}
			}
		}
	}

	return nullptr;
}

bool UGsUIWindowSkill::IsCostumeCategory() const
{
	return (static_cast<int32>(EGsCategoryTabType::TAB_COSTUME_SKILL) 
		== _toggleGroupSkillCategory.GetSelectedIndex()) ? true : false;
}

UGsUITargetDetailInfoBase* UGsUIWindowSkill::GetCurrentDetailInfo()
{
	int32 index = _toggleGroupSkillCategory.GetSelectedIndex();
	if (0 > index)
	{
		return nullptr;
	}

	EGsCategoryTabType tabType = static_cast<EGsCategoryTabType>(index);
	switch (tabType)
	{
	case EGsCategoryTabType::TAB_PASSIVE_SKILL:
		return _uiDetailInfoPassivity;
	case EGsCategoryTabType::TAB_COSTUME_SKILL:
		return _uiDetailInfoCostume;
	}

	return _uiDetailInfo;
}

void UGsUIWindowSkill::SetTutorial(const FGsSkill* InSkill)
{
	if (nullptr == InSkill)
	{
		_tutorialSkillId = INVALID_SKILL_ID;

		OnTutorialSkillListEvent.Unbind();
		SetTutorialTarget(nullptr);

		_scrollSkillList->SetAllowOverscroll(true);
	}
	else
	{
		_tutorialSkillId = InSkill->GetSkillId();

		if (GetSelectedWeaponType() != InSkill->_tableData->requireWeapon)
		{
			_toggleGroupWeapon.SetSelectedIndex(ConvertWeaponTypeToIndex(InSkill->_tableData->requireWeapon), false);
		}

		if (UWidget* target = GetTargetSkillListItem(_tutorialSkillId))
		{
			SetTutorialTarget(target);
		}

		_scrollSkillList->SetAllowOverscroll(false);
	}
}

UWidget* UGsUIWindowSkill::GetTargetSkillListItem(SkillId InSkillId) const
{
	for (int i = 0; i < _skillDataList.Num(); ++i)
	{
		if (_skillDataList[i]->_skillId == InSkillId)
		{
			return _panelSkillList->GetChildAt(i);
		}
	}

	return nullptr;
}

void UGsUIWindowSkill::SetTutorialTarget(UWidget* InWidget)
{
	_tutorialTarget = InWidget;

	if (InWidget)
	{
		_scrollSkillList->ScrollToStart();
		OnTutorialSkillListEvent.ExecuteIfBound(_tutorialTarget);
	}
}

UWidget* UGsUIWindowSkill::GetFirstSkillSlot() const
{
	// 0번은 없음
	return _slotSet->FindSlotById(1);
}

void UGsUIWindowSkill::OnScrollSkillList(float InCurrentOffset)
{
	// 튜토리얼 스크롤 락
	if (IsTutorial())
	{
		_scrollSkillList->SetScrollOffset(0.f);
	}
}
