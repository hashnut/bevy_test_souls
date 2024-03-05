// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIAbnormalityInfoEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconAbnormality.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Popup/GsUIPopupAbnormalityDetailInfo.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "Data/GsDataContainManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "GameObject/Abnormality/GsPassivityHelper.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UnrealMathUtility.h"


void UGsUIAbnormalityInfoEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 버튼 이벤트 세팅
	_btnSlot->OnClicked.AddDynamic(this, &UGsUIAbnormalityInfoEntry::OnClickSlot);
}

void UGsUIAbnormalityInfoEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (0 < _remainCheckTime)
	{
		_remainCheckTime -= InDeltaTime;
		if (0 >= _remainCheckTime)
		{
			UpdateRemainTimeText();
		}
	}
}

void UGsUIAbnormalityInfoEntry::NativeDestruct()
{
	OnHide();

	_passivityData = nullptr;

	Super::NativeDestruct();
}

void UGsUIAbnormalityInfoEntry::OnHide()
{
	// 아이콘 삭제
	_iconSelector->RemoveAllChildren();
}

void UGsUIAbnormalityInfoEntry::SetData(const FGsAbnormalityData* InData)
{
	if (nullptr == InData)
	{
		return;
	}

	_passivityData = nullptr;
	_tableId = InData->GetTable()->id;
	_level = InData->GetLevel();

	// 이름
	_textBlockName->SetText(InData->GetTable()->ingameName);
	_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorNameTextAbnormality(InData->GetTable()->isBuff));

	// 설명글
	_textDescription = InData->GetTable()->ingameDesc;

	// 시간 설정
	_remainCheckTime = 0.f;
	_maxRemainTime = InData->GetRemainTime();
	_endTime = InData->GetEndAppTime();
	UpdateRemainTimeText();

	// 아이콘
	if (UGsUIIconAbnormality* icon = GetIcon())
	{
		icon->SetData(InData);
	}
}

void UGsUIAbnormalityInfoEntry::SetData(const FGsPassivityData* InData)
{
	if (nullptr == InData)
	{
		return;
	}

	_passivityData = InData;
	_tableId = InData->_data->id;
	_level = 1;
	
	// 이름
	_textBlockName->SetText(InData->GetNameText());
	_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorNameTextPassivity(
		InData->_data->isBuff, InData->_ownerType));

	// 설명글
	_textDescription = InData->GetDescText();
	
	// 패시브 시간 없음
	_remainCheckTime = 0.f;
	_maxRemainTime = 0;
	_endTime = 0.f;	

	// 아르카스의 축복의 경우 포인트 표시
	if (PassivityOwnerType::ARCAS == InData->_ownerType)
	{
		_textRemainTime = FText::GetEmpty();

		const int32 intArcasPoint = FGsPassivityHelper::GetArcasPoint();

		// TEXT: 잔여 {0}
		FText textFormat;
		if (FText::FindText(TEXT("ArcasText"), TEXT("Remain_Arcas"), textFormat))
		{
			_textRemainTime = FText::Format(textFormat, intArcasPoint);
		}
	}
	else
	{
		UpdateRemainTimeText();
	}

	// 아이콘
	if (UGsUIIconAbnormality* icon = GetIcon())
	{
		icon->SetData(InData);
	}
}

void UGsUIAbnormalityInfoEntry::OnClickSlot()
{
	// 상세 정보창 오픈
	TWeakObjectPtr<UGsUIWidgetBase>	widget = GUI()->OpenAndGetWidget(TEXT("PopupAbnormalityDetailInfo"));
	if (widget.IsValid())
	{
		if (UGsUIPopupAbnormalityDetailInfo* popup = Cast<UGsUIPopupAbnormalityDetailInfo>(widget.Get()))
		{
			if (_passivityData)
			{
				popup->SetPassivityData(_passivityData);
			}
			else
			{
				popup->SetAbnormalityData(_tableId, _level);
			}
		}
	}
}

void UGsUIAbnormalityInfoEntry::UpdateRemainTimeText()
{
	if (0.f >= _endTime)
	{
		_remainCheckTime = 0.f;
		_endTime = 0.f;
		_textRemainTime = FText::GetEmpty();
		return;
	}

	double currTime = FApp::GetCurrentTime();
	float remainTime = static_cast<float>(_endTime - currTime);

	if (0.f >= remainTime)
	{
		_remainCheckTime = 0.f;
		_endTime = 0.f;
		_textRemainTime = FText::GetEmpty();
		return;
	}

	// 0.2초에 한 번 씩 갱신
	_remainCheckTime = 0.2f;

	// 최대 남은 시간을 넘지 못함
	if (_maxRemainTime < remainTime)
	{
		remainTime = _maxRemainTime;
	}

	int32 remaintTimeRound = FMath::RoundToInt(remainTime);

	FString strTime;
	FGsTimeStringHelper::GetTimeString(FTimespan::FromSeconds(static_cast<double>(remaintTimeRound)), strTime);
	_textRemainTime = FText::FromString(strTime);
}

UGsUIIconAbnormality* UGsUIAbnormalityInfoEntry::GetIcon()
{
	if (0 == _iconSelector->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = _iconSelector->CreateIcon();
		if (newIcon)
		{
			_iconSelector->AddChild(newIcon);
		}
	}

	// 아이콘을 하나만 생성하므로 0번째 아이콘 리턴
	return Cast<UGsUIIconAbnormality>(_iconSelector->GetChildAt(0));
}
