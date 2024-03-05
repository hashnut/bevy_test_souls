// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BattlePass/GsUIBattlePassMissionItem.h"
#include "UI/UIContent/Window/BattlePass/GsUIIconBattlePassExp.h"
#include "UI/UIContent/popup/GsUIPopupBattlePassExpDetail.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "BattlePass/GsBattlePassMissionData.h"
#include "PanelWidget.h"

void UGsUIBattlePassMissionItem::NativeConstruct()
{
	Super::NativeConstruct();

	_iconData = MakeShared<IconBattlePassExpData>(IconData::DataType::BattlePassExp, nullptr);

	_iconBattlePassExp->OnLongPressEvent.AddLambda([](TWeakPtr<IconData> iconData) {

			if (auto expData = StaticCastSharedPtr<IconBattlePassExpData>(iconData.Pin()))
			{
				if (auto missionData = expData->_data.Pin())
				{
						if (auto popup = Cast<UGsUIPopupBattlePassExpDetail>((GUI()->OpenAndGetWidget("PopupBattlePassExpDetail")).Get()))
						{
							popup->SetDetailInfo(missionData->GetMissionRewardName(), 
												 missionData->GetMissionRewardDescription(), 
												 missionData->GetMissionRewardIcon());
						}
				}
			}

		});
}

void UGsUIBattlePassMissionItem::NativeDestruct()
{
	_iconData = nullptr;
	Super::NativeDestruct();
}

void UGsUIBattlePassMissionItem::SetData(TWeakPtr<FGsBattlePassMissionData> data)
{
	_data = data;
	if (_iconData) _iconData->_data = _data;

	_iconBattlePassExp->SetData(_iconData);
}

void UGsUIBattlePassMissionItem::RefreshUI()
{
	if (auto data = _data.Pin())
	{
		_titleText = data->GetMissionName();

		//auto exp = data->GetPassMissionRewardExp();
		//_missionExp = exp > 0 ? FText::AsNumber(exp) : FText::GetEmpty();

		auto completePoint = data->GetCompletePoint();
		_missionProgress->SetPercent(completePoint > 0 ? float(data->GetAchievePoint()) / float(completePoint) : 0.f);
		_missionProgress->SetProgressText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { data->GetAchievePoint(), completePoint })));

		_complete->SetVisibility(data->IsCompleteMission() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		_iconBattlePassExp->RefreshUI();
	}

}
