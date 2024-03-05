

#include "GsUITrayMonsterKnowledgeSlateFilter.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "MonsterKnowledgeCollection/MonsterKnowledgeCollectionTypeDefine.h"
#include "GameObject/Stat/GsStatHelper.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUICheckBoxCommonText.h"

#include "../Runtime/UMG/Public/Components/WrapBox.h"



void UGsUITrayMonsterKnowledgeSlateFilter::NativeOnInitialized()
{
	UGsUITray::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnClose);

	_btnApply->OnClicked.AddDynamic(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnApply);

	_checkBoxGradeAll->_onClickSlot.BindUObject(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnClickedGradeAll);

	_checkBoxStatAll->_onClickSlot.BindUObject(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnClickedStatAll);

	_slotHelperGrade = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperGrade->Initialize(_subClassOfGrade, _panelGradeListRoot);
	_slotHelperGrade->OnCreateEntry.AddDynamic(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnCreateEntryGrade);
	_slotHelperGrade->OnRefreshEntry.AddDynamic(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnRefreshEntryGrade);

	_slotHelperStat = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperStat->Initialize(_subClassOfStat, _panelStatListRoot);
	_slotHelperStat->OnCreateEntry.AddDynamic(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnCreateEntryStat);
	_slotHelperStat->OnRefreshEntry.AddDynamic(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnRefreshEntryStat);


	FText title;
	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slateFilterGradeAll"), title);
	_checkBoxGradeAll->SetText(title);

	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slateFilterPassivityAll"), title);
	_checkBoxStatAll->SetText(title);
}

void UGsUITrayMonsterKnowledgeSlateFilter::BeginDestroy()
{
	if (_slotHelperGrade)
	{
		_slotHelperGrade->OnRefreshEntry.RemoveDynamic(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnRefreshEntryGrade);
		_slotHelperGrade = nullptr;
	}
	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnClose);
	}
	
	if (_btnApply)
	{
		_btnApply->OnClicked.RemoveDynamic(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnApply);
	}	

	UGsUITray::BeginDestroy();
}

void UGsUITrayMonsterKnowledgeSlateFilter::NativeConstruct()
{
	UGsUITray::NativeConstruct();

	//FGsMonsterKnowledgeCollectionManager* manager = GsMonsterKnowledgeCollection();
	//if (nullptr == manager)
	//{
	//	GSLOG(Error, TEXT("nullptr == manager"));
	//	return;
	//}

	//int32 maxCount = StaticCast<int32>(MonsterKnowledgeSlateGrade::MAX);
	//_slotHelperGrade->RefreshAll(maxCount);

	//// default set
	//_checkBoxGradeAll->SetIsSelected(true);

	//const TArray<TPair<StatType, FText>>& statNames = manager->GetStatNames();
	//maxCount = statNames.Num();
	//_slotHelperStat->RefreshAll(maxCount);

	//// default set
	//_checkBoxStatAll->SetIsSelected(true);
}

void UGsUITrayMonsterKnowledgeSlateFilter::NativeDestruct()
{
	UGsUITray::NativeDestruct();
}

void UGsUITrayMonsterKnowledgeSlateFilter::OnClose()
{
	Close();
}

void UGsUITrayMonsterKnowledgeSlateFilter::OnApply()
{
	//FGsMonsterKnowledgeCollectionManager* manager = GsMonsterKnowledgeCollection();
	//if (nullptr == manager)
	//{
	//	GSLOG(Error, TEXT("nullptr == manager"));
	//	return;
	//}

	//FGsMonsterKnowledgeSlatFilterParam filter;
	//
	//// 2021/12/12 PKT - garde
	//if (false == _checkBoxGradeAll->GetIsSelected())
	//{
	//	int32 maxCount = StaticCast<int32>(MonsterKnowledgeSlateGrade::MAX);
	//	for (int32 idx = 0; idx < maxCount; ++idx)
	//	{
	//		TWeakObjectPtr<UGsUICheckBoxCommonText> checkBox = Cast<UGsUICheckBoxCommonText>(_slotHelperGrade->GetEntry(idx));
	//		if (false == checkBox.IsValid())
	//		{
	//			GSLOG(Error, TEXT("false == checkBox.IsValid()"));
	//			return;
	//		}

	//		if (true == checkBox->GetIsSelected())
	//		{
	//			filter.gradeList.Emplace(StaticCast<MonsterKnowledgeSlateGrade>(idx));
	//		}
	//	}
	//}
	//
	//// 2021/12/12 PKT - passivity
	//if (false == _checkBoxStatAll->GetIsSelected())
	//{
	//	const TArray<TPair<StatType, FText>>& statList = manager->GetStatNames();
	//	int32 maxCount = statList.Num();
	//	for (int32 idx = 0; idx < maxCount; ++idx)
	//	{
	//		TWeakObjectPtr<UGsUICheckBoxCommonText> checkBox = Cast<UGsUICheckBoxCommonText>(_slotHelperStat->GetEntry(idx));
	//		if (false == checkBox.IsValid())
	//		{
	//			GSLOG(Error, TEXT("false == checkBox.IsValid()"));
	//			return;
	//		}
	//		if (true == checkBox->GetIsSelected())
	//		{				
	//			filter.statTypesList.Emplace(statList[idx].Key);
	//		}
	//	}
	//}

	//OnApplyFilter.ExecuteIfBound(filter);
	
	Close();
}

void UGsUITrayMonsterKnowledgeSlateFilter::OnCreateEntryGrade(UWidget* InEntry)
{
	TWeakObjectPtr<UGsUICheckBoxCommonText> checkBox = Cast<UGsUICheckBoxCommonText>(InEntry);
	if (false == checkBox.IsValid())
	{
		GSLOG(Error, TEXT("false == checkBox.IsValid()"));
		return;
	}

	if (false == checkBox->OnClickCheckBox().IsBoundToObject(this))
	{
		checkBox->OnClickCheckBox().BindUObject(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnClickedGrade);
	}
}

void UGsUITrayMonsterKnowledgeSlateFilter::OnRefreshEntryGrade(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUICheckBoxCommonText> checkBox = Cast<UGsUICheckBoxCommonText>(InEntry);
	if (false == checkBox.IsValid())
	{
		GSLOG(Error, TEXT("false == checkBox.IsValid()"));
		return;
	}

	MonsterKnowledgeSlateGrade type = StaticCast<MonsterKnowledgeSlateGrade>(InIndex);
	FText title;
	switch (type)
	{
	case MonsterKnowledgeSlateGrade::NORMAL:	// "일반"
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slateFilterGradeNormal"), title);
		break;
	case MonsterKnowledgeSlateGrade::MAGIC:		// "매직"
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slateFilterGradeMagic"), title);
		break;
	case MonsterKnowledgeSlateGrade::RARE:		// "레어"
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slateFilterGradeRare"), title);
		break;
	case MonsterKnowledgeSlateGrade::EPIC:		// "에픽"
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slateFilterGradeEpic"), title);
		break;
	case MonsterKnowledgeSlateGrade::UNIQUE:	// "유니크"
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slateFilterGradeUnique"), title);
		break;
	case MonsterKnowledgeSlateGrade::LEGEND:	// "레전드"
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slateFilterGradeLegend"), title);
		break;
	}

	checkBox->SetText(title);
	checkBox->SetIsSelected(false);
}

void UGsUITrayMonsterKnowledgeSlateFilter::OnCreateEntryStat(UWidget* InEntry)
{
	TWeakObjectPtr<UGsUICheckBoxCommonText> checkBox = Cast<UGsUICheckBoxCommonText>(InEntry);
	if (false == checkBox.IsValid())
	{
		GSLOG(Error, TEXT("false == checkBox.IsValid()"));
		return;
	}

	if (false == checkBox->OnClickCheckBox().IsBoundToObject(this))
	{
		checkBox->OnClickCheckBox().BindUObject(this, &UGsUITrayMonsterKnowledgeSlateFilter::OnClickedStat);
	}		
}

void UGsUITrayMonsterKnowledgeSlateFilter::OnRefreshEntryStat(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUICheckBoxCommonText> checkBox = Cast<UGsUICheckBoxCommonText>(InEntry);
	if (false == checkBox.IsValid())
	{
		GSLOG(Error, TEXT("false == checkBox.IsValid()"));
		return;
	}

	//FGsMonsterKnowledgeCollectionManager* manager = GsMonsterKnowledgeCollection();
	//if (nullptr == manager)
	//{
	//	GSLOG(Error, TEXT("nullptr == manager"));
	//	return;
	//}
	//
	//const TArray<TPair<StatType, FText>>& statNames = manager->GetStatNames();
	//if (false == statNames.IsValidIndex(InIndex))
	//{
	//	GSLOG(Error, TEXT("false == statNames.IsValidIndex(InIndex)"));
	//	return;
	//}

	//checkBox->SetText(statNames[InIndex].Value);
	//checkBox->SetIsSelected(false);
}


void UGsUITrayMonsterKnowledgeSlateFilter::OnClickedGradeAll(bool bIsChecked, int32 InIndexInGroup)
{
	if (true == bIsChecked)
	{
		int32 maxCount = StaticCast<int32>(MonsterKnowledgeSlateGrade::MAX);
		for (int32 idx = 0; idx < maxCount; ++idx)
		{
			TWeakObjectPtr<UGsUICheckBoxCommonText> checkBox = Cast<UGsUICheckBoxCommonText>(_slotHelperGrade->GetEntry(idx));
			if (false == checkBox.IsValid())
			{
				GSLOG(Error, TEXT("false == checkBox.IsValid()"));
				return;
			}
			checkBox->SetIsSelected(false);
		}
	}
}

void UGsUITrayMonsterKnowledgeSlateFilter::OnClickedGrade(bool bIsChecked, int32 InIndexInGroup)
{
	if (true == bIsChecked)
	{
		_checkBoxGradeAll->SetIsSelected(false);
	}
}

void UGsUITrayMonsterKnowledgeSlateFilter::OnClickedStatAll(bool bIsChecked, int32 InIndexInGroup)
{
	if (true == bIsChecked)
	{
		//FGsMonsterKnowledgeCollectionManager* manager = GsMonsterKnowledgeCollection();
		//if (nullptr == manager)
		//{
		//	GSLOG(Error, TEXT("nullptr == manager"));
		//	return;
		//}

		//int32 maxCount = manager->GetStatNames().Num();
		//for (int32 idx = 0; idx < maxCount; ++idx)
		//{
		//	TWeakObjectPtr<UGsUICheckBoxCommonText> checkBox = Cast<UGsUICheckBoxCommonText>(_slotHelperStat->GetEntry(idx));
		//	if (false == checkBox.IsValid())
		//	{
		//		GSLOG(Error, TEXT("false == checkBox.IsValid()"));
		//		return;
		//	}
		//	checkBox->SetIsSelected(false);
		//}
	}
}

void UGsUITrayMonsterKnowledgeSlateFilter::OnClickedStat(bool bIsChecked, int32 InIndexInGroup)
{
	if (true == bIsChecked)
	{
		_checkBoxStatAll->SetIsSelected(false);
	}
}