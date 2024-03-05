// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPageFairy.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsFairyMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Sound/GsSoundPlayer.h"
#include "Data/GsResourceManager.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Fairy/GsSchemaFairyEnchantLevel.h"
#include "DataSchema/Fairy/GsSchemaFairyEnchant.h"
#include "Fairy/GsFairyData.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemIngredient.h"
#include "Summon/GsSummonFunc.h"

#include "Summon/GsSummonDefine.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyItem.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyStat.h"
#include "UI/UIContent/Window/Fairy/GsUIFairySkillItem.h"
#include "UI/UIContent/Window/Fairy/GsDivFairyEnchant.h"
#include "UI/UIContent/Window/Fairy/GsUIIconFairyEnchant.h"
#include "UI/UIContent/Popup/GsUIPopupFlavorText.h"
#include "UI/UIContent/Popup/GsUIPopupFairyCollection.h"
#include "UI/UIContent/Popup/GsUIPopupFairySkillTooltip.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyBase.h"
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyWait.h"
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyResult.h"
#include "UI/UIContent/Popup/GsUIPopupSummonMileage.h"

#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "PaperSprite.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Transform.h"
#include "UMG/Public/Animation/UMGSequencePlayer.h"

using ActTaskState = UGsUIPageFairy::ActTaskState;

void UGsUIPageFairy::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPageFairy::OnCreateEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPageFairy::OnRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	if (_skillPanelHelper)
	{
		_skillPanelHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPageFairy::OnCreateSkillEntry);
		_skillPanelHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPageFairy::OnRefreshSkillEntry);
		_skillPanelHelper = nullptr;
	}

	if (_statScrollHelper)
	{
		_statScrollHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPageFairy::OnRefreshStatEntry);
		_statScrollHelper = nullptr;
	}

	_tabGradeIndex.Reset();
	_toggleGroupGrade.Clear();

	_stateManager.clearTask(true);
	_timerOnAnimationEnd.Unbind();

	if (_taskManager._taskFinished)
	{
		_taskManager.clearTask();
		_taskManager._taskFinished();
		_taskManager._taskFinished = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPageFairy::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIFairyItem>())
		{
			child->RemoveFromParent();
		}
	}

	TWeakObjectPtr<UGsUIPageFairy> thiz = this;
	_stateManager.pushTask(ActTaskState::Info, MakeShared<FGsTaskStateAct>(TEXT("BaseInfo"), 
			[thiz](){ if(thiz.IsValid()) thiz.Get()->OnStateFairyInfo(); } ));

	_stateManager.pushTask(ActTaskState::Enchant, MakeShared<FGsTaskStateAct>(TEXT("Enchant"), 
			[thiz](){ if (thiz.IsValid()) thiz.Get()->OnStateFairyEnchant(); }));

	_timerOnAnimationEnd.BindDynamic(this, &UGsUIPageFairy::OnStateChaneAnimationFinished);
	BindToAnimationFinished(_animationEnchant, _timerOnAnimationEnd);

	_listFairy->OnUserScrolled.AddDynamic(this, &UGsUIPageFairy::OnScrollFairyList);

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->SetEmptyListPanel(_emptyPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIPageFairy::OnCreateEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPageFairy::OnRefreshEntry);


	_skillPanelHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_skillPanelHelper->Initialize(_entryFairySkillClass, _fairySkillBox);
	_skillPanelHelper->OnCreateEntry.AddDynamic(this, &UGsUIPageFairy::OnCreateSkillEntry);
	_skillPanelHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPageFairy::OnRefreshSkillEntry);

	childList = _statListPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIFairyStat>())
		{
			child->RemoveFromParent();
		}
	}

	_statScrollHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_statScrollHelper->Initialize(_entryItemStat, _statListPanel);
	_statScrollHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPageFairy::OnRefreshStatEntry);

	_btnSummon->OnClicked.AddDynamic(this, &UGsUIPageFairy::OnClickSummon);
	_btnCollection->OnClicked.AddDynamic(this, &UGsUIPageFairy::OnOpenCollection);
	_btnFlavor->OnClicked.AddDynamic(this, &UGsUIPageFairy::OnOpenFlavorText);
	_btnEnchant->OnClicked.AddDynamic(this, &UGsUIPageFairy::OnChangeEnchantMode); 
	_fairyVehicleSkill->OnClickFairySkillIcon.BindUObject(this, &UGsUIPageFairy::OnTooltipVehicleFairySkill);

	_fairyEnchant->OnCancelEnchant.BindUObject(this, &UGsUIPageFairy::OnCancelFairyEnchant);
	_fairyEnchant->OnStartEnchant.BindUObject(this, &UGsUIPageFairy::OnStartFairyEnchant);

	TMap<FairyGrade, UGsSwitcherButton*> gradeTabs;
	{
		gradeTabs.Add(FairyGrade::MAX, _btnGradeAll);
		gradeTabs.Add(FairyGrade::LEGEND, _btnGradeLegend);
		gradeTabs.Add(FairyGrade::UNIQUE, _btnGradeUnique);
		gradeTabs.Add(FairyGrade::EPIC, _btnGradeEpic);
		gradeTabs.Add(FairyGrade::RARE, _btnGradeRare);
		gradeTabs.Add(FairyGrade::MAGIC, _btnGradeMagic);
		gradeTabs.Add(FairyGrade::NORMAL, _btnGradeNormal);
	}

	int32 index = 0;
	for (auto iter : gradeTabs)
	{
		_tabGradeIndex.Add(index, iter.Key);
		_toggleGroupGrade.AddToggleWidget(iter.Value);

		index++;
	}

	_gradeTab = -1;
	_selectedFairyId = INVALID_FAIRY_ID;
	_initData = INVALID_ITEM_ID;
	_toggleGroupGrade.OnSelectChanged.BindUObject(this, &UGsUIPageFairy::OnGradeTabChanged);

	_btnOpenPopupSummonMileage->OnClicked.AddDynamic(this, &UGsUIPageFairy::OnClickOpenPopupSummonMileage);
}

void UGsUIPageFairy::NativeConstruct()
{
	Super::NativeConstruct();

	_changedState = 0;
	auto endTime = _animationEnchant->GetEndTime();
	PlayAnimation(_animationEnchant, endTime, 1, EUMGSequencePlayMode::Reverse);

	_resIcon->OnPressedLong.AddUObject(this, &UGsUIPageFairy::OnPressedLongIcon);
	

	SetMileageButtonState();

	FGsMessageHolder* msg = GMessage();
	_handlerUserInfo = msg->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUIPageFairy::OnUpdateCurrency);

	MUI& msgUI = msg->GetUI();
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::CHANGED_SUMMON_MILEAGE_DATA, this, &UGsUIPageFairy::OnChangedSummonMileageData));
}

void UGsUIPageFairy::NativeDestruct()
{
	SetTutorialTarget(-1); //인덱스임
	OnTutorialSettingComplete.Unbind();

	_selectReserveFairyId = INVALID_FAIRY_ID;
	_toggleGroupGrade.SetSelectedIndex(0);

	if (_taskManager._taskFinished)
	{
		_taskManager.clearTask(true);
		_taskManager._taskFinished();
		_taskManager._taskFinished = nullptr;
	}

	_resIcon->OnPressedLong.RemoveAll(this);
	_enchantItem = nullptr;
	_gradeTab = 0;

	// 메시지 해제
	if (FGsMessageHolder* msgMgr = GMessage())
	{
		msgMgr->GetUserBasicInfo().Remove(_handlerUserInfo);

		if (0 != _uiMsgArray.Num())
		{
			for (auto iter : _uiMsgArray)
			{
				msgMgr->GetUI().Remove(iter);
			}
			_uiMsgArray.Empty();
		}
	}

	Super::NativeDestruct();
}

void UGsUIPageFairy::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_gradeTab < 0)
		_toggleGroupGrade.SetSelectedIndex(0, true);

	_taskManager.doTasks();

	if (_changedState == 1)
	{
		_changedState = 2;
		auto endTime = _animationEnchant->GetEndTime();
		if (_curState == ActTaskState::Enchant)
		{
			PlayAnimation(_animationEnchant, endTime, 1, EUMGSequencePlayMode::Forward);
		}
		else if (_curState == ActTaskState::Info)
		{
			PlayAnimation(_animationEnchant, endTime, 1, EUMGSequencePlayMode::Reverse);
		}
	}
}

#pragma region IGsSelectedPanelInterface
void UGsUIPageFairy::InitializeTab(int32 tabIndex)
{
	IGsSelectedPanelInterface::InitializeTab(tabIndex);

	_gradeTab = -1;
	_selectedFairyId = INVALID_FAIRY_ID;
	_skill->SetVisibility(ESlateVisibility::Collapsed);
	_btnSummon->SetIsEnabled(false);

	_fairyName->SetText(FText::GetEmpty());
	_fairyLevel->SetText(FText::GetEmpty());

	_scrollBoxHelper->RefreshAll(0);
	//_toggleGroupGrade.SetSelectedIndex(0, true);
}

void UGsUIPageFairy::ReleasedTab()
{
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFairySkillTooltip")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFlavorText")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFairyCollection")).Get())
		popup->Close();
}

void UGsUIPageFairy::OnSelected()
{
	if (_selectedFairyId == INVALID_FAIRY_ID)
	{
		InitializeTab(_tabIndex);
	}

	if (_tabGradeIndex.Find(_gradeTab))
		SetRefreshFairyItems(_tabGradeIndex[_gradeTab]);

	if (_selectedFairyId != INVALID_FAIRY_ID)
		OnSelectedFairyItem(_selectedFairyId);

	if (_selectReserveFairyId != INVALID_FAIRY_ID)
	{
		SetReserveFairyData();
	}
	else if (_initData != INVALID_ITEM_ID)
	{
		SetReserveActionByItem();
		_initData = INVALID_ITEM_ID;
	}

	UpdateRedDot();

	_stateManager.ChangeTask(ActTaskState::Info);
}

void UGsUIPageFairy::OnUnselected()
{
	const auto& dictionary = GSFairy()->GetFairyDictionary();
	for(auto e : dictionary)
	{
		e->removeNewMark();
	}

	GSFairy()->DisableFairyRedDot();
}

void UGsUIPageFairy::OnUpdateItemList(TArray<FairyId>& arr)
{
	auto selectTab = _tabGradeIndex[_gradeTab];
	bool isRefresh = selectTab == FairyGrade::MAX;
	if (!isRefresh)
	{
		for (auto e : arr)
		{
			if (auto data = GSFairy()->GetFairyData(e).Pin())
			{
				isRefresh = data->GetFairyGrade() == selectTab;
				if (isRefresh) break;
			}
		}
	}
	if(isRefresh) SetRefreshFairyItems(selectTab, _stateManager._curState);

	UpdateRedDot();
}
#pragma endregion
void UGsUIPageFairy::OnStateFairyInfo()
{
	if (_curState == ActTaskState::Enchant)
	{
		_changedState = 0;
		PlayAnimationReverse(_animationEnchant);

		GMessage()->GetFairy().SendMessage(MessageFairy::ENCHANT_CANCEL, nullptr);

	}
	_curState = ActTaskState::Info;
}

void UGsUIPageFairy::OnStateFairyEnchant()
{
	if (_curState == ActTaskState::Info)
	{
		_changedState = 0;
		PlayAnimationForward(_animationEnchant);

		GMessage()->GetFairy().SendMessage(MessageFairy::ENCHANT_READY, nullptr);
	}

	_curState = ActTaskState::Enchant;
}

/* 페어리 소환 : ToServer */
void UGsUIPageFairy::OnClickSummon()
{
	auto equipFairy = GSFairy()->GetEquipFairyData();
	if (equipFairy.IsValid() && equipFairy.Pin()->GetFaryID() == _selectedFairyId)
		return;

	if (_selectedFairyId == INVALID_FAIRY_ID)
		return;

	TWeakPtr<FGsFairyData> weakData = GSFairy()->GetFairyData(_selectedFairyId);
	if(weakData.IsValid() && weakData.Pin()->isCollectedFairy())
		FGsNetSendServiceWorld::SendReqFairyEquip(_selectedFairyId);
}

void UGsUIPageFairy::OnOpenCollection()
{
	if (auto PopupCollection = Cast<UGsUIPopupFairyCollection>(GUI()->OpenAndGetWidget(TEXT("PopupFairyCollection"))))
	{
		UGsUIPopupFairyCollection::PopupInitData param = { _selectedFairyId };
		PopupCollection->InitializeData(&param);
	}
}

void UGsUIPageFairy::OnOpenFlavorText()
{
	if (auto fairyData = GSFairy()->GetFairyData(_selectedFairyId).Pin())
	{		
		if (auto popup = Cast<UGsUIPopupFlavorText>(GUI()->OpenAndGetWidget(TEXT("PopupFlavorText")).Get()))
		{
			UGsUIPopupFlavorText::PopupInitData param{ fairyData->GetFairyName(), fairyData->GetFairyFlavorText() };
			popup->InitializeData(&param);
		}
	}
}

void UGsUIPageFairy::OnChangeEnchantMode() 
{ 
	_stateManager.ChangeTask(ActTaskState::Enchant); 
}

void UGsUIPageFairy::OnTooltipFairySkill(TWeakPtr<FGsFairyPassivityInfo> info)
{
	if (auto fairyData = GSFairy()->GetFairyData(_selectedFairyId).Pin())
	{
		const FGsSchemaPassivitySet* baseStatPassivity;
		TArray<FGsFairyLevelInfo> passivityList;
		fairyData->GetFairyLevelPassivityList(baseStatPassivity, passivityList);

		auto useLevel = passivityList.FindByPredicate([info](auto& e) { return info.Pin()->level == e._level; });

		int useIndex = 0;
		if (info.IsValid() && info.Pin()->passivity)
		{
			auto passivityId = info.Pin()->passivity->id;
			useIndex = useLevel->_skillPassivities.IndexOfByPredicate([passivityId](const auto e) { return e && e->id == passivityId; });
		}

		if (useIndex == INDEX_NONE) return;;

		TArray<TPair<FairyLevelId, const FGsSchemaPassivitySet*>> skillList;
		Algo::TransformIf(passivityList, skillList, [useIndex](auto& e) { 
			return e._skillPassivities.IsValidIndex(useIndex); }, [useIndex](auto& e) {
			return TPair<FairyLevelId, const FGsSchemaPassivitySet*>(e._level, e._skillPassivities[useIndex]); });

		if (skillList.IsValidIndex(0))
		{
			if (auto popup = Cast<UGsUIPopupFairySkillTooltip>(GUI()->OpenAndGetWidget(TEXT("PopupFairySkillTooltip")).Get()))
			{
				UGsUIPopupFairySkillTooltip::PopupInitData param{ fairyData, skillList, info.Pin()->level };
				popup->InitializeData(&param);
			}
		}
	}
}

void UGsUIPageFairy::OnTooltipVehicleFairySkill(FairyLevelId level)
{
	if (auto fairyData = GSFairy()->GetFairyData(_selectedFairyId).Pin())
	{
		TArray<TPair<FairyLevelId, const FGsSchemaPassivitySet*>> skillList;

		auto curLevel = fairyData->GetCurrentLevel();
		skillList.Add(TPair<FairyLevelId, const FGsSchemaPassivitySet*>(curLevel, fairyData->GetByVehiclePassivity(curLevel)));

		if (skillList.IsValidIndex(0) && skillList[0].Value)
		{
			if (auto popup = Cast<UGsUIPopupFairySkillTooltip>(GUI()->OpenAndGetWidget(TEXT("PopupFairySkillTooltip")).Get()))
			{
				UGsUIPopupFairySkillTooltip::PopupInitData param{ fairyData, skillList, INVALID_FAIRY_LEVEL, true };
				popup->InitializeData(&param);
			}
		}
	}
}

void UGsUIPageFairy::OnCreateEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairyItem>(InEntry))
	{
		entry->WidgetType = UGsUIFairyItem::UIItemType::ITEM;
		entry->OnItemSelected.BindUObject(this, &UGsUIPageFairy::OnSelectedFairyItem);
	}
}

void UGsUIPageFairy::OnRefreshEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairyItem>(InEntry))
	{
		if (_fairyGradeList.IsValidIndex(index))
		{
			entry->SetData(_fairyGradeList[index]);
			entry->SetSelected(false);
			if (auto data = _fairyGradeList[index].Pin())
			{
				entry->SetSelected(data->_id == _selectedFairyId);
			}
			entry->RefreshUI();
		}
	}
}

void UGsUIPageFairy::OnRefreshStatEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairyStat>(InEntry))
	{
		if (_statTxtList.IsValidIndex(index))
		{
			auto statDetail = _statTxtList[index];
			entry->SetText(statDetail._statText.Key, statDetail._statText.Value, statDetail._level, statDetail._isLock);
		}
	}
}

void UGsUIPageFairy::OnCreateSkillEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairySkillItem>(InEntry))
	{
		entry->OnSkillTooltip.BindUObject(this, &UGsUIPageFairy::OnTooltipFairySkill);
	}
}

void UGsUIPageFairy::OnRefreshSkillEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairySkillItem>(InEntry))
	{
		if (_fairyLevelData.IsValidIndex(index))
		{
			entry->SetData(_fairyLevelData[index]);
			entry->RefreshUI();
		}
	}
}

void UGsUIPageFairy::OnStateChaneAnimationFinished()
{
	if (_changedState == 0)
	{
		_changedState = 1;
	}
}

void UGsUIPageFairy::OnPressedLongIcon()
{
	if (_enchantItem)
	{
		FGsItemDetailOpenMessageParam param(_enchantItem.Get(), false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
	}
}

void UGsUIPageFairy::SetReserveActionByItem()
{
	if (const auto itemData = GItem()->GetItemTableDataByTID(_initData))
	{
		if (itemData->categoryMain == ItemCategoryMain::FAIRY && itemData->categorySub == ItemCategorySub::ENCHANT_FAIRY)
		{
			ItemId enchantItemId = _initData; 

			if (const UGsTable* table = FGsSchemaFairyEnchant::GetStaticTable())
			{
				TArray<const FGsSchemaFairyEnchant*> fairyEnchantData;
				table->GetAllRows<FGsSchemaFairyEnchant>(fairyEnchantData);

				if (auto data = fairyEnchantData.FindByPredicate([enchantItemId](const auto e) {return e->materialItemId == enchantItemId; }))
				{
					for (auto& pair : _tabGradeIndex)
					{
						if (pair.Value == (*data)->grade)
						{
							auto tabIndex = pair.Key;
							_selectedFairyId = INVALID_FAIRY_ID;
							_toggleGroupGrade.SetSelectedIndex(tabIndex, true);
							break;
						}
					}
				}
				}
		}
	}
}

void UGsUIPageFairy::OnGradeTabChanged(int32 index)
{
	if (_gradeTab == index) return;

	_gradeTab = index;
	//_selectedFairyId = INVALID_FAIRY_ID;
	auto selectTab = _tabGradeIndex[index];
	SetRefreshFairyItems(selectTab);
}

void UGsUIPageFairy::SetRefreshFairyItems(FairyGrade grade, ActTaskState state)
{
	UpdateItemSelection(INVALID_FAIRY_ID);
	
	_fairyGradeList.Empty(0);
	_scrollBoxHelper->RefreshAll(0);

	GSFairy()->GetGradeFairyDictionary(grade, _fairyGradeList, true);

	auto selectFairy = _selectedFairyId;
	if (_selectedFairyId == INVALID_FAIRY_ID)
	{
		if (auto equipFairy = GSFairy()->GetEquipFairyData().Pin())
		{
			selectFairy = equipFairy->GetFaryID();
		}

		if (_fairyGradeList.Num() > 0)
		{
			selectFairy = _fairyGradeList[0].Pin()->_id;
		}
	}
	else
	{
		if (auto fairyData = GSFairy()->GetFairyData(_selectedFairyId).Pin())
		{
			if (fairyData->isCollectedFairy())
			{
				if (auto equipFairy = GSFairy()->GetEquipFairyData().Pin())
					 _btnSummon->SetIsEnabled(equipFairy->GetFaryID() != _selectedFairyId);
				else _btnSummon->SetIsEnabled(true);
			}
		}
	}

	_scrollBoxHelper->RefreshAll(_fairyGradeList.Num());

	auto children = _listPanel->GetAllChildren();
	if (selectFairy != _selectedFairyId && selectFairy != INVALID_FAIRY_ID)
	{
		Algo::ForEachIf(children, [](auto e) { return Cast<UGsUIFairyItem>(e) != nullptr; }, [selectFairy](auto e) {
			if (auto item = Cast<UGsUIFairyItem>(e))
			{
				item->SetSelected(item->GetFairyId() == selectFairy);
			}
		});
	}

	if (auto child = children.FindByPredicate([](auto e) { if (auto item = Cast<UGsUIFairyItem>(e)) return item->IsSelected(); return false;  }))
	{
		_listFairy->ScrollWidgetIntoView(*child, false, EDescendantScrollDestination::Center);
	}

	if (IsTutorial() &&
		_tutorialTargetIndex < _fairyGradeList.Num())
	{
		OnTutorialSettingComplete.ExecuteIfBound(_listPanel->GetChildAt(_tutorialTargetIndex));
	}

	if(state == ActTaskState::None || _stateManager._curState != state)
		_stateManager.ChangeTask(ActTaskState::Info);
}

void UGsUIPageFairy::OnSelectedFairyItem(FairyId id)
{
	//if (_selectedFairyId == id) return;
	if (id == INVALID_FAIRY_ID) return;

	_statTxtList.Empty();
	_statScrollHelper->RefreshAll(0);
	
	TWeakPtr<FGsFairyData> weakData = GSFairy()->GetFairyData(id);

	UpdateItemSelection(id);
	
	if (auto fairyData = weakData.Pin())
	{
		FairyGrade grade = fairyData->GetFairyGrade();
		FLinearColor color = FGsUIColorHelper::GetColorFairyGrade(grade);

		_fairyName->SetText(fairyData->GetFairyName());
		_fairyName->SetColorAndOpacity(color);

		_textFairyGrade = FGsSummonFunc::GetFairyGradeText(grade);
		_colorTextGrade = color;
		_selectGrade->SetActiveWidgetIndex(static_cast<int>(grade) - 1);

		const FGsSchemaPassivitySet* baseStatPassivity{ nullptr };
		TArray<FGsFairyLevelInfo> levelInfoList;
		fairyData->GetFairyLevelPassivityList(baseStatPassivity, levelInfoList);

		TArray<TPair<FText, FText>> lockStatTxtList;
		if (baseStatPassivity)
		{
			GSEffectText()->GetEffectTextListPassivity(baseStatPassivity->id, lockStatTxtList);
			Algo::ForEach(lockStatTxtList, [this](auto& e) {_statTxtList.Add({ 0, e, false });});
		}

		for (auto& levelInfo : levelInfoList) //UI 바뀌면....
		{
			if (levelInfo._statPassivity)
			{
				lockStatTxtList.Reset();

				GSEffectText()->GetEffectTextListPassivity(levelInfo._statPassivity->id, lockStatTxtList);
				Algo::ForEach(lockStatTxtList, [this, levelInfo](auto& e) {
					_statTxtList.Add({ levelInfo._level, e, levelInfo._isOpenLevel == false });});
			}
		}

		_statScrollHelper->RefreshAll(_statTxtList.Num());

		_fairyLevelData.Reset();
		TArray<const FGsSchemaPassivitySet*> skillList;
		fairyData->GetCurFairyLevelSkillPassivityList(skillList);
		auto level = fairyData->GetCurrentLevel();

		Algo::Transform(skillList, _fairyLevelData, [level](const auto passivity) {
			return MakeShared<FGsFairyPassivityInfo>(level, passivity, false, false); });

		_skillPanelHelper->RefreshAll(_fairyLevelData.Num());

		bool isSkillValid = skillList.IsValidIndex(0) && skillList[0];

		if (auto vehicleSkill = fairyData->GetByVehiclePassivity())
		{
			_fairyVehicleSkill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_fairyVehicleSkill->SetData(vehicleSkill, INVALID_FAIRY_LEVEL);

			isSkillValid = true;
		}
		else
		{
			_fairyVehicleSkill->SetVisibility(ESlateVisibility::Collapsed);
		}

		_skill->SetVisibility(isSkillValid ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		auto flavorText = fairyData->GetFairyFlavorText();
		_btnFlavor->SetIsEnabled(flavorText.IsEmpty() == false);
		
		if (fairyData->isCollectedFairy())
		{
			FText levelFormat;
			FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);

			_fairyLevel->SetText(FText::Format(levelFormat, fairyData->GetCurrentLevel()));

			if (auto equipFairy = GSFairy()->GetEquipFairyData().Pin())
				 _btnSummon->SetIsEnabled(equipFairy->GetFaryID() != id);
			else _btnSummon->SetIsEnabled(true);
		}
		else
		{
			_btnSummon->SetIsEnabled(false);
			_fairyLevel->SetVisibility(ESlateVisibility::Hidden);
		}

		if (_selectedFairyId != id)
		{
			FGsFairyMsgParamInt param(id);
			GMessage()->GetFairy().SendMessage(MessageFairy::CHANGE_SELECTION, &param);
		}
	}

	_selectedFairyId = id;

	bool enableEnchant = GSFairy()->GetFairyEnchantData(id, 0) != nullptr;
	_btnEnchant->SetIsEnabled(enableEnchant);

	if(enableEnchant) SetSelectedFairyUpgradeInfo(id);
	else if (_stateManager._curState == ActTaskState::Enchant)
	{
		_stateManager.ChangeTask(ActTaskState::Info);
	}
}

void UGsUIPageFairy::SetSelectedFairyUpgradeInfo(FairyId fairyId)
{
	if (auto fairyData = GSFairy()->GetFairyData(fairyId).Pin())
	{
		FairyLevelId curLevel = fairyData->GetCurrentLevel();
		FairyLevelId nextLevel = fairyData->GetUpgradeLevel();

		FText levelFormat;
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);

		FLinearColor gradeColor = FGsUIColorHelper::GetColorFairyGrade(fairyData->GetFairyGrade());

		_levelImageSelect->SetActiveWidgetIndex(fairyData->IsEnableUpgade() ? nextLevel - 1 : curLevel - 1);
		_fairyEnchantSelect->SetActiveWidgetIndex(fairyData->IsEnableUpgade() ? 0 : 1);
		_materialPanel->SetVisibility(fairyData->IsEnableUpgade() ? 
			ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed );

		int32 ownItemCount = 0;
		if (const auto enchantData = GSFairy()->GetFairyEnchantData(fairyId))
		{
			const FGsSchemaFairyEnchantLevel* enchantLevel =
				enchantData->enchantLevelList.FindByPredicate([curLevel](FGsSchemaFairyEnchantLevel& e) { return e.level == curLevel; });

			auto itemList = GItem()->FindByTID(enchantData->materialItemId, ItemType::MAX);
			for (auto item : itemList)
			{
				if (false == item.IsValid())
					continue;

				if (auto ingredientItem = item.Pin()->GetCastItem<FGsItemIngredient>())
					ownItemCount += ingredientItem->GetAmount();
			}

			_resIcon->SetEmptyIcon();
			if (const auto tableData = FGsItem::GetTableDataByID(enchantData->materialItemId))
			{
				_textItemName = tableData->name;
				_colorItemName = FGsUIColorHelper::GetColorItemGrade(tableData->grade);

				_resIcon->SetIconImage(tableData->icon);
				
				if (!_enchantItem || _enchantItem->GetTID() != enchantData->materialItemId)
					 _enchantItem = MakeShareable(FGsItem::Create(enchantData->materialItemId, ownItemCount));
				else _enchantItem->SetAmount(ownItemCount);
			}

			///// 페어리 레벨단계 UI 및 성장재료
			//UMaterialInstanceDynamic* material = _iconImage->GetDynamicMaterial();
			_textItemRequireCount = FText::FromString(FString::FormatAsNumber(enchantLevel ? (*enchantLevel).materialItemCount : 0));

			_textItemCount = FText::FromString(FString::FormatAsNumber(ownItemCount));
			_colorItemCount = FGsUIColorHelper::GetColor((*enchantLevel).materialItemCount > ownItemCount ? EGsUIColorType::DEFAULT_LACK : EGsUIColorType::DEFAULT_ENOUGH);

			///// 페어리 성장확률
			int totalProbability = (*enchantLevel).levelIncreasedProb + (*enchantLevel).levelMaintainedProb + (*enchantLevel).levelDecreasedProb;
			float upgradeSuccessRate = ((*enchantLevel).levelIncreasedProb * 100.f) / float(totalProbability); //나중에 표시

			_textProbability = FText::FromString(FString::Printf(TEXT("%.2f%%"), upgradeSuccessRate));

			//if ((int)(upgradeSuccessRate) % 100 > 0)
			//{
			//	_textProbability = FText::FromString(FString::Printf(TEXT("%.2f%%"), upgradeSuccessRate / 100.f));
			//}
			//else
			//{
			//	_textProbability = FText::FromString(FString::Printf(TEXT("%d%%"), static_cast<int>(upgradeSuccessRate / 100.f)));
			//}
		}

		_fairyEnchant->InitEnchantDivision(fairyData);
	}
}

void UGsUIPageFairy::OnCancelFairyEnchant()
{
	_stateManager.ChangeTask(ActTaskState::Info);
}

void UGsUIPageFairy::OnStartFairyEnchant(TWeakPtr<FGsFairyData> enchantFairy)
{
	_taskManager._taskFinished = []() {
		if (auto uiManager = GUI())
		{
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantWait"));
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantProgress"));
		}
		if (auto fairyManager = GSFairy())
		{
			if (fairyManager->IsEffectPlaying())
			{
				fairyManager->OnSummonComplete(nullptr);
			}
		}
	};

	bool noUseEnchantNoticeMsg = true;
	if(auto fairyData = enchantFairy.Pin())
	{ 
		auto level = fairyData->GetCurrentLevel();
			
		if (const auto enchantData = GSFairy()->GetFairyEnchantData(fairyData->_id))
		{
			const FGsSchemaFairyEnchantLevel* enchantLevel =
				enchantData->enchantLevelList.FindByPredicate([level](FGsSchemaFairyEnchantLevel& e) { return e.level == level; });

			noUseEnchantNoticeMsg = enchantLevel && (*enchantLevel).levelDecreasedProb == 0;
		}
	}

	FText msg;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Growth_Popup_Info"), msg); /*"페어리를 성장시키겠습니까?"*/
	FText subMsg;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Growth_Popup_AlertLevelDown"), subMsg); /*(성장에 실패했을 때, 페어리 레벨이 하락할 수 있습니다)*/

	if (noUseEnchantNoticeMsg)
	{
		UGsUIPopupYesNo* weak_popup = FGsUIHelper::PopupYesNo(msg, [enchantFairy, this](bool yes) {
				if (yes)
				{
					auto useID = enchantFairy.IsValid() ? enchantFairy.Pin()->_id : INVALID_FAIRY_ID;
					if(useID != INVALID_FAIRY_ID) EnchantProcess(useID);
				}
				else
				{
					CancelEnchant();
				}
			});
	}
	else
	{
	//	if (subMsg.ToString().Find(TEXT("<pn>"), ESearchCase::IgnoreCase) == INDEX_NONE)
	//	{
	//		subMsg = FText::FromString(FString::Format(TEXT("<pn>{0}</>"), { *subMsg.ToString() }));
	//	}
		UGsUIPopupYesNoTwoMessage* weak_popup = FGsUIHelper::PopupYesNoTwoMessage(msg, subMsg, [enchantFairy, this](bool yes) {
				if (yes)
				{
					auto useID = enchantFairy.IsValid() ? enchantFairy.Pin()->_id : INVALID_FAIRY_ID;
					if(useID != INVALID_FAIRY_ID) EnchantProcess(useID);
				}
				else
				{
					CancelEnchant();
				}
			});
	}
}

void UGsUIPageFairy::UpdateItemSelection(FairyId selectId)
{
	int32 itemCount = _scrollBoxHelper->GetItemCount();
	for (int i = 0; i < itemCount; ++i)
	{
		UWidget* child = _listPanel->GetChildAt(i);

		if (auto item = Cast<UGsUIFairyItem>(child))
		{
			if (selectId == INVALID_FAIRY_ID)
			{
				item->SetSelected(false);
			}
			else
			{
				if (item->IsSelected())
				{
					item->SetSelected(item->GetFairyId() == selectId, false);
					item->RefreshUI();
				}
				else
				{
					item->SetSelected(item->GetFairyId() == selectId, false);
					if (item->IsSelected())
						item->RefreshUI();
				}
			}
		}
	}
}

void UGsUIPageFairy::OnChangeSummonFairy()
{
	auto selectTab = _tabGradeIndex[_gradeTab];
	SetRefreshFairyItems(selectTab); //순서변경

	auto preSelect = _selectedFairyId;
	_selectedFairyId = INVALID_FAIRY_ID;

	UpdateItemSelection(preSelect);
}

void UGsUIPageFairy::OnEnchantFairyResult(FairyId id, FairyEnchantResult result)
{
	_taskManager._taskFinished = [this]() {
		if (auto uiManager = GUI())
		{
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantWait"));
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantProgress"));
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantSuccess"));
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantFailure"));
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantSaveFailure"));
		}

		StopEffectSound(EffectSoundResType::Enchant_NotSafe_Touch);
		StopEffectSound(EffectSoundResType::Enchant_NotSafe_Success);
		StopEffectSound(EffectSoundResType::Enchant_NotSafe_Fail);
		StopEffectSound(EffectSoundResType::Enchant_NotSafe_Guard);

		if(auto fairyManager = GSFairy())
			fairyManager->SetEffectPlaying(false);
	};

	FString trayName;
	switch (result)
	{
	case FairyEnchantResult::INCREASE : trayName = TEXT("TrayFairyEnchantSuccess"); break;
	case FairyEnchantResult::DECREASE : trayName = TEXT("TrayFairyEnchantFailure"); break;
	case FairyEnchantResult::MAINTAIN : trayName = TEXT("TrayFairyEnchantSaveFailure"); break;
	default: break;
	}

	if (!trayName.IsEmpty())
	{
		auto fairyID = id;
		auto task = MakeShared<FGsTaskTrayUI>(TEXT("TrayFairyEnchantResult"), trayName, [fairyID, trayName, result, this]() {
				if (auto uiManager = GUI())
				{
					uiManager->CloseByKeyName(TEXT("TrayFairyEnchantProgress"));
					if (auto widget = Cast<UGsUITrayEnchantFairyBase>(uiManager->OpenAndGetWidget(*trayName)))
					{
						FText title;
						FText desc;
						switch (result)
						{
						case FairyEnchantResult::INCREASE:
							FText::FindText(TEXT("FairyText"), TEXT("EnchantResult_Success_Title"), title);
							FText::FindText(TEXT("FairyText"), TEXT("EnchantResult_Success_Info"), desc);
							PlayEffectSound(EffectSoundResType::Enchant_NotSafe_Success);
							break;
						case FairyEnchantResult::DECREASE:
							FText::FindText(TEXT("FairyText"), TEXT("EnchantResult_Fail_Title"), title);
							FText::FindText(TEXT("FairyText"), TEXT("EnchantResult_Fail_Info"), desc);
							PlayEffectSound(EffectSoundResType::Enchant_NotSafe_Fail);
							break;
						case FairyEnchantResult::MAINTAIN:
							FText::FindText(TEXT("FairyText"), TEXT("EnchantResult_Save_Title"), title);
							FText::FindText(TEXT("FairyText"), TEXT("EnchantResult_Save_Info"), desc);
							PlayEffectSound(EffectSoundResType::Enchant_NotSafe_Guard);
							break;
						default: break;
						}

						auto data = GSFairy()->GetFairyData(fairyID);
						UGsUITrayEnchantFairyResult::EnchantResultEffectInitData initparam(data, title, desc);
						widget->InitializeData(&initparam);
					}
			}
			}, [trayName]() {

				if (auto uiManager = GUI())
				{
					uiManager->CloseByKeyName(*trayName);
				}

				if (GSFairy()->IsEffectPlaying())
				{
					GSFairy()->OnSummonComplete(nullptr);
				}
			});
		_taskManager.pushTask(task);
	}
	else if(result == FairyEnchantResult::NONE) //system failure
	{

	}

	_fairyEnchant->OnEnchantResult(result);

	OnSelectedFairyItem(id);

}

void UGsUIPageFairy::OnSetScrollToSelectedFairyItem(FairyId id)
{
	if (IsSelectedTab() == false)
	{
		_gradeTab = 0;
		_selectedFairyId = INVALID_FAIRY_ID;
		_selectReserveFairyId = id;
	}
}

void UGsUIPageFairy::SetReserveFairyData()
{
	if (_selectReserveFairyId != INVALID_FAIRY_ID)
	{
		OnSelectedFairyItem(_selectReserveFairyId);

		UWidget* selectedItem = nullptr;
		int32 itemCount = _scrollBoxHelper->GetItemCount();
		for (int32 i = 0; i < itemCount; ++i)
		{
			UWidget* child = _listPanel->GetChildAt(i);
			if (auto item = Cast<UGsUIFairyItem>(child))
			{
				if (item->GetFairyId() == _selectReserveFairyId)
				{
					selectedItem = child;
					break;
				}
			}
		}

		if (selectedItem)
		{
			_listFairy->ScrollWidgetIntoView(selectedItem, true, EDescendantScrollDestination::Center);
		}

		_selectReserveFairyId = INVALID_FAIRY_ID;
	}
}

void UGsUIPageFairy::EnchantProcess(FairyId useID)
{
	auto waitTask = MakeShared<FGsTaskTrayUI>(TEXT("TrayFairyEnchantWait"), TEXT("TrayFairyEnchantWait"), [this]() {
		if (auto uiManager = GUI())
		{
			if (auto widget = Cast<UGsUITrayEnchantFairyBase>(uiManager->OpenAndGetWidget(TEXT("TrayFairyEnchantWait"))))
			{
				UGsUITrayEnchantFairyWait::EnchantWaitEffectInitData param(1.2f, true);
				widget->InitializeData(&param);
			}
			PlayEffectSound(EffectSoundResType::Enchant_NotSafe_Touch);
		}
		}, [useID]() {

			GSFairy()->SetEffectPlaying(true);
			FGsNetSendServiceWorld::SendReqFairyEnchant(useID);
		});

	auto progressTask = MakeShared<FGsTaskTrayUI>(TEXT("TrayFairyEnchantProgress"), TEXT("TrayFairyEnchantProgress"), [useID]() {
		if (auto uiManager = GUI())
		{
			uiManager->CloseByKeyName(TEXT("TrayFairyEnchantWait"));
			if (auto widget = Cast<UGsUITrayEnchantFairyBase>(uiManager->OpenAndGetWidget(TEXT("TrayFairyEnchantProgress"))))
			{
				//UGsUITrayEnchantFairyWait::PopupInitData param(3.f, true);
				widget->InitializeData(nullptr);
			}
		}
		});
	_taskManager.pushTask(waitTask);
	_taskManager.pushTask(progressTask);
};

void UGsUIPageFairy::CancelEnchant()
{
	_fairyEnchant->CancelEnchant();
}

void UGsUIPageFairy::UpdateRedDot()
{
	bool reddot = GSFairy()->IsFairyRedDot();

	TArray<TWeakPtr<FGsFairyData>> gradelist;
	for (auto e : { TPair<FairyGrade, UImage*>{ FairyGrade::MAX, _redDotAll }, 
					TPair<FairyGrade, UImage*>{ FairyGrade::LEGEND, _redDotLegend },
					TPair<FairyGrade, UImage*>{ FairyGrade::UNIQUE, _redDotUnique }, 
					TPair<FairyGrade, UImage*>{ FairyGrade::EPIC, _redDotEpic },
					TPair<FairyGrade, UImage*>{ FairyGrade::RARE, _redDotRare }, 
					TPair<FairyGrade, UImage*>{ FairyGrade::MAGIC, _redDotMagic },
					TPair<FairyGrade, UImage*>{ FairyGrade::NORMAL, _redDotNormal } })
	{	
		if (reddot)
		{
			gradelist.Empty(0);
			GSFairy()->GetGradeFairyDictionary(e.Key, gradelist);

			e.Value->SetVisibility(gradelist.FindByPredicate([](TWeakPtr<FGsFairyData> data) {
				return data.IsValid() && data.Pin()->_isNew;
				}) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
		else e.Value->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIPageFairy::PlayEffectSound(EffectSoundResType InResType)
{
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	_effectSoundID[(uint8)InResType] = soundPlayer->PlaySound2D(GetSoundResName(InResType));
}

void UGsUIPageFairy::StopEffectSound(EffectSoundResType InResType)
{
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	uint64 soundID = _effectSoundID[(uint8)InResType];
	if (true == soundPlayer->IsPlaying(soundID))
	{
		soundPlayer->StopSound(soundID);
	}
}


void UGsUIPageFairy::SetTutorialTarget(int32 InIndex)
{
	_tutorialTargetIndex = InIndex;

	if (-1 == _tutorialTargetIndex)
	{
		OnTutorialSettingComplete.Unbind();
		_listFairy->SetAllowOverscroll(true);
	}
	else
	{
		_listFairy->SetAllowOverscroll(false);
	}
}

void UGsUIPageFairy::OnScrollFairyList(float InCurrentOffset)
{
	// 튜토리얼 중 스크롤 못하게 막음
	if (IsTutorial())
	{
		_listFairy->SetScrollOffset(0.f);
	}
}

void UGsUIPageFairy::OnClickOpenPopupSummonMileage()
{
	TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->OpenAndGetWidget(TEXT("PopupSummonMileage"));

	if (wigdet.IsValid() == false)
	{
		return;
	}


	TWeakObjectPtr<UGsUIPopupSummonMileage> popupSummonMileage = Cast<UGsUIPopupSummonMileage>(wigdet);
	popupSummonMileage->InitSummonMileage(EGsSummonType::SUMMON_FAIRY);
}

// 재화 갱신 메시지
void UGsUIPageFairy::OnUpdateCurrency(uint64 InType)
{
	CurrencyType targetCurrency = static_cast<CurrencyType>(InType);

	// 유니크, 일반 체크
	if (targetCurrency != CurrencyType::SUMMON_COSTUME_MILEAGE &&
		targetCurrency != CurrencyType::SUMMON_FAIRY_MILEAGE &&
		targetCurrency != CurrencyType::SUMMON_UNIQUE_COSTUME_MILEAGE &&
		targetCurrency != CurrencyType::SUMMON_UNIQUE_FAIRY_MILEAGE
		)
	{
		return;
	}

	SetMileageButtonState();
}
// 소환 마일리지 데이터 갱신
void UGsUIPageFairy::OnChangedSummonMileageData(const  IGsMessageParam* inParam)
{
	SetMileageButtonState();
}
// 마일리지 버튼 상태 갱신
void UGsUIPageFairy::SetMileageButtonState()
{
	// 소환 마일리지 보상 받을것이 있는가
	bool isExistSummonMileageReward = GSSummon()->IsExistSummonMileageRewardByType(EGsSummonType::SUMMON_FAIRY);

	ESlateVisibility rewardEffectVisibility =
		(isExistSummonMileageReward == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_mileageRewardEffectWidget->SetVisibility(rewardEffectVisibility);
}