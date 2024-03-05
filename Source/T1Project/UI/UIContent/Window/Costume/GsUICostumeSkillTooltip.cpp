#include "GsUICostumeSkillTooltip.h"
#include "TextBlock.h"

void UGsUICostumeSkillTooltip::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}
void UGsUICostumeSkillTooltip::ClearData()
{
	_dataList.Reset();
	_textBlockTitle->SetText(FText::GetEmpty());
	_textBlockDesc->SetText(FText::GetEmpty());
}
void UGsUICostumeSkillTooltip::SetData(int index, FText title, FText desc)
{	
	_dataList.Emplace(index, TPair<FText, FText>(title, desc));
}

void UGsUICostumeSkillTooltip::OpenIndex(int index)
{
	if (auto iter = _dataList.Find(index))
	{
		_textBlockTitle->SetText((*iter).Key);
		_textBlockDesc->SetText((*iter).Value);

		Open();
	}
}