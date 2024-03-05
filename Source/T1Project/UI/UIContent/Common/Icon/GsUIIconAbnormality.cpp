// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconAbnormality.h"
#include "Components/Image.h"
#include "Classes/PaperSprite.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "GameObject/Abnormality/GsAbnormalityDataDefine.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "GameObject/Abnormality/GsPassivityHelper.h"


void UGsUIIconAbnormality::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 버튼 이벤트 연결
	_btnSlot->OnClicked.AddDynamic(this, &UGsUIIconAbnormality::OnClickButton);
	_btnSlot->OnPressed.AddDynamic(this, &UGsUIIconAbnormality::OnPressButton);
}

void UGsUIIconAbnormality::NativeConstruct()
{
	Super::NativeConstruct();

	// 깜빡임 기준시간
	if (const UGsGlobalConstant* globalConstData = GData()->GetGlobalData())
	{
		_flickeringSecond = globalConstData->_abnormalityIconFlickeringSecond;
	}
}

void UGsUIIconAbnormality::NativeDestruct()
{
	// 애니메이션 종료
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	OnClickIcon.Clear();
	OnPressIcon.Clear();

	_tId = 0;
	_remainTime = 0;
	SetUIStackCount(0);	

	Super::NativeDestruct();
}

void UGsUIIconAbnormality::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (0.f < _remainTime)
	{
		_remainTime -= InDeltaTime;

		if (_remainTime <= _flickeringSecond)
		{
			if (false == _bIsFlickering)
			{
				_bIsFlickering = true;
				UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
				PlayAnimationFlicker();
			}
		}

		if (0.f > _remainTime)
		{
			_remainTime = 0.f;
			_bIsFlickering = false;
		}
	}
}

void UGsUIIconAbnormality::SetEmptyIcon()
{
	Super::SetEmptyIcon();

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	_tId = 0;
	_remainTime = 0;
	SetUIStackCount(0);	

	SetIconImage(_pathEmptyIcon);
}

void UGsUIIconAbnormality::SetIconImage(const FSoftObjectPath& InPath)
{
	if (InPath.IsValid())
	{
		Super::SetIconImage(InPath);
	}
	else
	{
		Super::SetIconImage(_pathEmptyIcon);
	}
}

void UGsUIIconAbnormality::SetData(const FGsAbnormalityData* InData)
{
	if (nullptr == InData || nullptr == InData->GetTable())
	{
		SetEmptyIcon();
		return;
	}

	// 아이콘이 꺼진채로 Pool에 반납되어도 문제 없도록 처리
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	bool bIsSameId = (InData->GetTableId() == _tId) ? true : false;
	_tId = InData->GetTableId();
	_remainTime = InData->GetCurrRemainTime();
	SetUIStackCount(InData->GetStackCount());

	float endAppTime = InData->GetEndAppTime();	
	if (0 == endAppTime)
	{
		// 제한시간이 없는 타입(페어리 탈것 효과)
		_bIsFlickering = false;
		UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	}
	else
	{
		// 기준시간보다 작을 경우, 깜빡이 애니메이션 재생
		if (_remainTime <= _flickeringSecond)
		{
			_bIsFlickering = true;

			UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
			PlayAnimationFlicker();
		}
		else
		{
			_bIsFlickering = false;
			// 갱신이 되어 다시 기준 시간보다 길어졌을 가능성이 있으므로 애니메이션 멈춤
			UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
		}
	}

	// TODO: 이전화 동일하면 무시하도록 세팅
	
	// 이미지 세팅
	if (false == bIsSameId)
	{
		SetIconImage((_bIsSmallIcon) ? InData->GetTable()->smallIconPath : InData->GetTable()->iconPath);
	}
}

void UGsUIIconAbnormality::SetData(const FGsSchemaAbnormalitySet* InData)
{
	if (nullptr == InData)
	{
		SetEmptyIcon();
		return;
	}

	// 아이콘이 꺼진채로 Pool에 반납되어도 문제 없도록 처리
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	bool bIsSameId = (InData->id == _tId) ? true : false;
	_tId = InData->id;
	_remainTime = 0;
	SetUIStackCount(0);	

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	if (false == bIsSameId)
	{
		SetIconImage((_bIsSmallIcon) ? InData->smallIconPath : InData->iconPath);
	}
}

void UGsUIIconAbnormality::SetData(const FGsPassivityData* InData)
{
	if (nullptr == InData)
	{
		SetEmptyIcon();
		return;
	}

	// 아이콘이 꺼진채로 Pool에 반납되어도 문제 없도록 처리
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 컬렉션 정보이면 _passivityId 값이 0으로 올 수 있음
	bool bIsSameId = false;
	if (0 != InData->_passivityId)
	{
		if (InData->_passivityId == _tId)
		{
			bIsSameId = true;
		}
	}
	
	_tId = InData->_passivityId;
	_remainTime = 0;
	SetUIStackCount(0);

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	if (PassivityOwnerType::ARCAS == InData->_ownerType)
	{
		SetIconImage(FGsPassivityHelper::GetArcasIconPath());
	}
	else
	{
		if (false == bIsSameId)
		{
			auto icon = _bIsSmallIcon ? InData->GetSmallIconPath() : InData->GetIconPath();
			SetIconImage(icon); //ref
		}
	}
}

void UGsUIIconAbnormality::SetData(const FGsSchemaPassivitySet* InData)
{
	if (nullptr == InData)
	{
		SetEmptyIcon();
		return;
	}

	// 아이콘이 꺼진채로 Pool에 반납되어도 문제 없도록 처리
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	bool bIsSameId = (InData->id == _tId) ? true : false;
	_tId = InData->id;
	_remainTime = 0;
	SetUIStackCount(0);

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	if (false == bIsSameId)
	{
		SetIconImage(_bIsSmallIcon ? InData->smallIconPath : InData->iconPath);
	}
}

void UGsUIIconAbnormality::OnClickButton()
{
	OnClickIcon.Broadcast(this);
}

void UGsUIIconAbnormality::OnPressButton()
{
	OnPressIcon.Broadcast(this);
}

void UGsUIIconAbnormality::SetUIStackCount(int32 InCount)
{
	if (1 >= InCount)
	{
		_textStackCount = FText::GetEmpty();
		return;
	}
	
	_textStackCount = FText::FromString(FString::Format(TEXT("x{0}"), { InCount }));
}
