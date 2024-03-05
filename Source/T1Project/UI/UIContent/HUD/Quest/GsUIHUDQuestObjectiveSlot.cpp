// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDQuestObjectiveSlot.h"

#include "T1Project.h"

#include "Quest/GsSchemaQuestObjective.h"
#include "Quest/GsSchemaObjectiveKillSet.h"
#include "Quest/Table/GsQuestObjectiveTableCache.h"
#include "Quest/GsQuestInstance.h"
#include "Quest/GsQuestData.h"

#include "HorizontalBox.h"
#include "SizeBox.h"
#include "WidgetSwitcher.h"

void UGsUIHUDQuestObjectiveSlot::Update(bool inComplete, bool inIsReady, FText& inDescText, FText& inCountText)
{		
	_stateTypeSwicher->SetActiveWidgetIndex(inComplete? 1 : 0);

	if (false == _descText.EqualTo(inDescText))
	{
		_descText = inDescText;
	}

	if (false == _countText.EqualTo(inCountText))
	{
		_countText = inCountText;
	}

	if (inComplete)
	{
		 _completeSizeBox->SetVisibility(_countText.IsEmpty()?
				 ESlateVisibility::Collapsed : ESlateVisibility::Visible);		
	}
	else
	{
		_readySwicher->SetActiveWidgetIndex(inIsReady? 1 : 0);

		_progressingSizeBox->SetVisibility(_countText.IsEmpty() ?
			ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}
