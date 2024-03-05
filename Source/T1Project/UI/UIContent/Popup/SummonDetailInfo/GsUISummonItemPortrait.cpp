
#include "GsUISummonItemPortrait.h"
#include "UI/UIContent/Window/Fairy/GsUISummonPortraitBase.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Image.h"

void UGsUISummonItemPortrait::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCollection->OnClicked.AddDynamic(this, &UGsUISummonItemPortrait::OnClickPortrait);
}

void UGsUISummonItemPortrait::SetData(TWeakPtr<IGsBasePortrait> baseData, float percent, CreatureGenderType gender)
{
	_data = baseData;
	_summonPercent = FText::FromString(FString::Printf(TEXT("%.6f%%"), percent));
	_portrait->SetData(baseData, gender);
	_bgNotCollectedFairy->SetVisibility(_data.Pin()->IsCollected()? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UGsUISummonItemPortrait::RefreshUI()
{
	if (auto data = _data.Pin())
	{
		_portraitName = data->GetItemName();
		_gradeColor = FGsUIColorHelper::GetColorItemGrade(data->GetGradeByItemBase());

		_portrait->RefreshUI();
	}
}

void UGsUISummonItemPortrait::OnClickPortrait()
{
	if (_data.IsValid())
	{
		OnItemSelected.ExecuteIfBound(_data);
	}
}
