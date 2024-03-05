// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOptionSubArea.h"
#include "GsUIOptionItemArea.h"
#include "GsUIOptionButton.h"
#include "GsUIOptionToggle.h"
#include "GsUIOptionButtonGroup.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/GsUIWindowOption.h"

#include "UserWidget.h"
#include "PanelWidget.h"
#include "VerticalBox.h"
#include "Blueprint/UserWidget.h"
#include "TextBlock.h"
#include "Components/HorizontalBox.h"

#include "Option/GsSchemaOptionDivision.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Option/GsOptionEnumType.h"
#include "DataSchema/Option/GsSchemaOptionItemGroup.h"
#include "DataSchema/Option/GsSchemaOptionItem.h"


void UGsUIOptionSubArea::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIOptionSubArea::OnRefreshEntryScrollBox);
		_scrollBoxHelper = nullptr;
	}

	_toggleGroup.Clear();

	if (nullptr != _itemGroupToggle)
	{
		_itemGroupToggle = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIOptionSubArea::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 스크롤 셋팅
	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_scrollBoxHelper->Initialize(_scrollBoxEntryClass, _verticalBoxSub);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIOptionSubArea::OnRefreshEntryScrollBox);


		_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIOptionSubArea::OnClickSlot);		
	}	

	// 토글 이벤트
	_itemGroupToggle->OnClickSlot.BindUObject(this, &UGsUIOptionSubArea::OnClickItemGroupToggle);
}

void UGsUIOptionSubArea::NativeConstruct()
{	
	// 초기 셋팅
	_divisionIndex = 0;

	// 이벤트 등록
	_msgHandle = GMessage()->GetUI().AddUObject(MessageUI::OPTION_UI_UPDATE, this, &UGsUIOptionSubArea::OnUpdate);

	Super::NativeConstruct();
}

void UGsUIOptionSubArea::NativeDestruct()
{
	if (_msgHandle.Value.IsValid())
	{
		GMessage()->GetUI().Remove(_msgHandle);
	}

	_divisionList.Empty();

	Super::NativeDestruct();
}

void UGsUIOptionSubArea::OnRefreshEntryScrollBox(int32 InIndex, UWidget * InEntry)
{	
	if (false == _isDivision)
		return;	

	int8 listNum = _divisionList.Num();
	if (listNum > InIndex)
	{
		//FGsSchemaOptionDivisionRow row = _divisionList.GetData()[InIndex];
		if (const FGsSchemaOptionDivision* division = _divisionList[InIndex])
		{
			if (UGsUIOptionButtonGroup* slot = Cast<UGsUIOptionButtonGroup>(InEntry))
			{
				slot->SetData(division->NameText);
			}						
		}		
	}
}

void UGsUIOptionSubArea::OnClickSlot(int32 InIndex)
{
	_divisionIndex = InIndex;

	//GOption()->SetOptionWindowDivisionIndex(_divisionIndex);

	// item 업데이트
	UpdataItem(_divisionIndex);
}

void UGsUIOptionSubArea::OnClickItemGroupToggle(EGsOptionItem InType, bool InIsSelect)
{
	GOption()->SetValue(InType, (true == InIsSelect) ? 1 : 0);

	// item 업데이트
	UpdataItem(_divisionIndex);
}

void UGsUIOptionSubArea::Update(bool InIsDivision, const TArray<FGsSchemaOptionDivisionRow>* InDivisionList)
{
	_divisionIndex = GOption()->GetOptionWindowDivisionIndex();
	_isDivision = InIsDivision;

	_divisionList.Empty();
	for (int i = 0; i < InDivisionList->Num(); i++)
	{
		FGsSchemaOptionDivisionRow row = InDivisionList->GetData()[i];
		if (const FGsSchemaOptionDivision* division = row.GetRow())
		{
			bool isAdd = true;
			for (auto iter : division->OptionExceptionPlatformTypeList)
			{
				if (iter == EGsOptionPlatformType::OPTION_WINDOW)
				{
#if WITH_EDITOR || PLATFORM_WINDOWS
					isAdd = false;
					continue;
#endif
				}
				else if (iter == EGsOptionPlatformType::OPTION_MOBILE_AOS)
				{
#if PLATFORM_ANDROID
					isAdd = false;
					continue;
#endif
				}
				else if (iter == EGsOptionPlatformType::OPTION_MOBILE_IOS)
				{
#if PLATFORM_IOS
					isAdd = false;
					continue;
#endif
				}
			}

			if (false == isAdd)
				continue;;

			_divisionList.Emplace(division);
		}
	}

	//_divisionList = InDivisionList;

	for (const auto& iter : _verticalBox->GetAllChildren())
	{
		iter->SetVisibility(ESlateVisibility::Visible);
	}

	if (true == InIsDivision)
	{			
		_scrollBoxHelper->RefreshAll(_divisionList.Num());
		_toggleGroup.Clear();
		_toggleGroup.AddToggleWidgetByParentPanel((UPanelWidget*)_verticalBoxSub);
		_toggleGroup.SetSelectedIndex(_divisionIndex);
	}

	((UWidget*)_verticalBoxSub)->SetVisibility((false == InIsDivision) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	// item 업데이트
	OnClickSlot(_divisionIndex);
}

void UGsUIOptionSubArea::UpdataItem(int32 InIndex)
{
	if (0 >= _divisionList.Num() || InIndex >= _divisionList.Num())
	{
		return;
	}	

	bool isItemGroupShow = true;
	bool isItemShow = true;

	//FGsSchemaOptionDivisionRow row = _divisionList.GetData()[InIndex];
	if (const FGsSchemaOptionDivision* division = _divisionList[InIndex])
	{
		if (const FGsSchemaOptionItemGroup* itemGroup = division->OptionGroup.GetRow())
		{		
			isItemGroupShow = true;
			float outValue;
			bool isVisible = true;
			if (GOption()->GetValue(itemGroup->optionItemType, outValue, isVisible))
			{
				isItemShow = (0 == outValue) ? true : false;
			}

			_itemGroupToggle->SetData(itemGroup->optionItemType, !isItemShow);
			_itemGroupToggle->SetButtonText(itemGroup->textOn, itemGroup->textOff);
			_itemGroupToggle->SetVisibility((itemGroup->isToggleOn) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			_textMain->SetText(itemGroup->textMain);

			if (itemGroup->textSub.IsEmpty())
			{
				_horizontalBoxSub->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				_horizontalBoxSub->SetVisibility(ESlateVisibility::Visible);
				_textSub->SetText(itemGroup->textSub);
			}
		}
		else
		{
			isItemGroupShow = false;
			isItemShow = true;
		}
		
		if (nullptr != _itemArea)
		{
			TArray<FGsSchemaOptionItemRow> optionItemList;
			const TArray<FGsSchemaOptionItemRow>* InOptionList = &division->OptionList;
			for (int i = 0; i < InOptionList->Num(); i++)
			{
				FGsSchemaOptionItemRow itemRow = InOptionList->GetData()[i];
				if (const FGsSchemaOptionItem* item = itemRow.GetRow())
				{
					bool isVisible = GOption()->IsVisible(item->OptionItemType);
					if (true == isVisible)
					{
						bool isAdd = true;
						for (auto iter : item->OptionExceptionPlatformTypeList)
						{
							if (iter == EGsOptionPlatformType::OPTION_WINDOW)
							{
#if WITH_EDITOR || PLATFORM_WINDOWS
								isAdd = false;
								continue;
#endif
							}
							else if (iter == EGsOptionPlatformType::OPTION_MOBILE_AOS)
							{
#if PLATFORM_ANDROID
								isAdd = false;
								continue;
#endif
							}
							else if (iter == EGsOptionPlatformType::OPTION_MOBILE_IOS)
							{
#if PLATFORM_IOS
								isAdd = false;
								continue;
#endif
							}
						}

						if (isAdd)
							optionItemList.Emplace(itemRow);
					}
				}
			}

			_itemArea->Update(&optionItemList);
		}		
		
		((UWidget*)_itemArea)->SetVisibility((false == isItemShow)? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		((UWidget*)_itemGroup)->SetVisibility((false == isItemGroupShow) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}


void UGsUIOptionSubArea::OnUpdate(const IGsMessageParam*)
{
	if (0 >= _divisionList.Num())
	{
		return;
	}

	//FGsSchemaOptionDivisionRow row = _divisionList.GetData()[_divisionIndex];
	if (const FGsSchemaOptionDivision* division = _divisionList[_divisionIndex])
	{
		TArray<FGsSchemaOptionItemRow> optionItemList;
		const TArray<FGsSchemaOptionItemRow>* InOptionList = &division->OptionList;
		for (int i = 0; i < InOptionList->Num(); i++)
		{
			FGsSchemaOptionItemRow itemRow = InOptionList->GetData()[i];
			if (const FGsSchemaOptionItem* item = itemRow.GetRow())
			{
				bool isVisible = GOption()->IsVisible(item->OptionItemType);
				if (true == isVisible)
				{
					bool isAdd = true;
					for (auto iter : item->OptionExceptionPlatformTypeList)
					{
						if (iter == EGsOptionPlatformType::OPTION_WINDOW)
						{
#if WITH_EDITOR || PLATFORM_WINDOWS
							isAdd = false;
							continue;
#endif
						}
						else if (iter == EGsOptionPlatformType::OPTION_MOBILE_AOS)
						{
#if PLATFORM_ANDROID
							isAdd = false;
							continue;
#endif
						}
						else if (iter == EGsOptionPlatformType::OPTION_MOBILE_IOS)
						{
#if PLATFORM_IOS
							isAdd = false;
							continue;
#endif
						}
					}

					if (isAdd)
						optionItemList.Emplace(itemRow);

				}
			}
		}

		_itemArea->UpdateList(&optionItemList);		
	}
}