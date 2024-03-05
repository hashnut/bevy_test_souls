#include "GsUIIconRegionSelectButtonSanctum.h"

#include "Sanctum/GsSanctumData.h"

#include "Guild/GsGuildHelper.h"

#include "DataSchema/Guild/GsSchemaGuildEmblem.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Paper2D/Classes/PaperSprite.h"

// 성소 정보 세팅
void UGsUIIconRegionSelectButtonSanctum::SetSanctumInfo(const FGsSanctumData* In_data, FText In_spotName)
{
	if (In_data == nullptr)
	{
		return;
	}
	// 1. 점령되었는가 아닌가
	bool  isSanctumOccupied = In_data->IsOccupied();

	_isSanctumOccupied = isSanctumOccupied;

	// 점령되었으면 select true (switch 인덱스: 1), 아니면 false(switch index: 0)
	/*bool isSelect = (isSanctumOccupied == true)? true: false;

	SetSelect(isSelect);*/

	// 점령되었으면 별도 세팅할 데이터(길드명, 길드 아이콘 표시)
	if (isSanctumOccupied == true)
	{
		// 아이콘 세팅
		const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(In_data->_guildEmblemId);
		if (emblemData == nullptr)
		{
			_imgGuildEmblemIcon->SetVisibility(ESlateVisibility::Collapsed);
			_imgGuildEmblemIcon1->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{ 
			_imgGuildEmblemIcon->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(emblemData->iconPathSmall));
			_imgGuildEmblemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			_imgGuildEmblemIcon1->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(emblemData->iconPathSmall));
			_imgGuildEmblemIcon1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		// 기사단 이름 세팅
		_textGuildName = FText::FromString(In_data->_guildName);
	}

	// 공통 세팅
	// 스팟 이름 세팅
	SetSpotName(In_spotName);
}

void UGsUIIconRegionSelectButtonSanctum::SetSelect(bool In_select)
{
	// 0: 비선택 비활성
	// 1: 비선택 활성
	// 2: 선택 비활성
	// 3: 선택 활성

	int switchIndex = 0;
	if (In_select == true)
	{
		if (_isSanctumOccupied == true)
		{
			switchIndex = 3;
		}
		else
		{
			switchIndex = 2;
		}
	}
	else
	{
		if (_isSanctumOccupied == true)
		{
			switchIndex = 1;
		}
		else
		{
			switchIndex = 0;
		}
	}
	_switcherSelect->SetActiveWidgetIndex(switchIndex);
	PlaySelectAnimation(In_select);

	ESlateVisibility selectImageVisibility = (In_select == true)? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_imgSelect->SetVisibility(selectImageVisibility);
}