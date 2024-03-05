// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDChaotic.h"
#include "Kismet/GameplayStatics.h"
#include "Cheat/GsCheatManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "WidgetSwitcher.h"
#include "TextBlock.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "PvP/GsSchemaPVPAlignmentData.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

void UGsUIHUDChaotic::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_chaoticButton->OnClicked.AddDynamic(this, &UGsUIHUDChaotic::OnClickChaoticButton);	
	InitMaxPvpAlignmentPointMax();
}

void UGsUIHUDChaotic::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(TEXT("PVPAlignmentText"), TEXT("Chaotic_Level"), _formatText);
		
	BindMessage();

	_isShowChaoticButton = false;
	_chaoticInfoPanel->SetVisibility(ESlateVisibility::Hidden);

	InvalidChaoticInfo();
}

void UGsUIHUDChaotic::NativeDestruct()
{
	UnbindMessage();

	Super::NativeDestruct();
}

void UGsUIHUDChaotic::OnClickChaoticButton()
{
	if (_isShowChaoticButton)
	{
		_chaoticInfoPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_chaoticInfoPanel->SetVisibility(ESlateVisibility::Visible);
		InvalidChaoticInfo();
	}

	_isShowChaoticButton = !_isShowChaoticButton;

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDChaotic::BindMessage()
{
	MUserBasicInfo& userInfo = GMessage()->GetUserBasicInfo();
	_handlerList.Empty();
	_handlerList.Add(userInfo.AddUObject(MessageUserInfo::PVP_POINT, this, &UGsUIHUDChaotic::OnInvalidPVPAlignmentPoint));
	_handlerList.Add(userInfo.AddUObject(MessageUserInfo::LOOK_INFO, this, &UGsUIHUDChaotic::OnInvalidPVPAlignmentGrade));
}

void UGsUIHUDChaotic::UnbindMessage()
{
	MUserBasicInfo& userInfo = GMessage()->GetUserBasicInfo();
	for (TPair<MessageUserInfo, FDelegateHandle> handler : _handlerList)
	{
		userInfo.Remove(handler);
	}
	_handlerList.Empty();
}

void UGsUIHUDChaotic::InvalidChaoticInfo()
{
	InvalidPVPAlignmentPoint();
	InvalidPVPAlignmentGrade();
}

void UGsUIHUDChaotic::InvalidPVPAlignmentPoint()
{
	if (false == _chaoticInfoPanel->IsVisible())
	{
		return;
	}
		
	int32 point = GGameData()->GetUserData()->pvpAlignmentPoint;
	FString pointText = FString::FromInt(point);
	_chaoticPointTextBlock->SetText(FText::FromString(pointText));

	// 성향에 따라 색상 변경 (위협 상태일 때는 기존 색상을 그대로 따라간다)
	PVPAlignmentGrade pvpAlignmentGrade = GGameData()->GetUserData()->mLookInfo.mPVPAlignmentGrade;
	if (PVPAlignmentGrade::CHAOTIC == pvpAlignmentGrade)
	{
		_chaoticPointTextBlock->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::PVP_ALIGNMENT_CHAOS));
	}
	else
	{
		_chaoticPointTextBlock->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY));
	}
}

void UGsUIHUDChaotic::InvalidPVPAlignmentGrade()
{
	PVPAlignmentGrade pvpAlignmentGrade = GGameData()->GetUserData()->mLookInfo.mPVPAlignmentGrade;
	if (PVPAlignmentGrade::CHAOTIC == pvpAlignmentGrade)
	{
		int32 chaosGrade = GetChaosGrade();		
		FText resultText = FText::Format(_formatText, chaosGrade); // 단일 분해는 고정 1개
		_chaoticInfoWidgetSwitcher->SetActiveWidgetIndex(1);
		_chaosGradeTextBlock->SetText(resultText);
	}
	else
	{
		_chaoticInfoWidgetSwitcher->SetActiveWidgetIndex(0);
	}

	int32 iconIndex = (PVPAlignmentGrade::CHAOTIC == pvpAlignmentGrade ? 1 : 0);
	_chaoticIconWidgetSwitcher->SetActiveWidgetIndex(iconIndex);
}

void UGsUIHUDChaotic::OnInvalidPVPAlignmentPoint(uint64 inId)
{
	InvalidChaoticInfo();
}

void UGsUIHUDChaotic::OnInvalidPVPAlignmentGrade(uint64 inId)
{
	UGsGameObjectBase* find = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == find)
	{
		return;
	}
	
	if (find->GetGameId() != inId)
	{
		return;
	}

	InvalidPVPAlignmentGrade();
}

int32 UGsUIHUDChaotic::GetChaosGrade()
{
	int32 grade = INDEX_NONE;
	if (nullptr == _maxPvpAlignmentData)
	{
		return grade;
	}

	//최대 포인트 이상일 때 최대 Grade로 설정 
	int32 num = _pvpAlignmentDataArray.Num();
	int32 point = GGameData()->GetUserData()->pvpAlignmentPoint;
	if (point > _maxPvpAlignmentData->pvpAlignmentPointMax)
	{
		return grade - 1;
	}
	
	//다른 Grade로 설정
	for (int32 i = 0; i < num; ++i)
	{
		if (point <= _pvpAlignmentDataArray[i]->pvpAlignmentPointMax)
		{
			grade = i;
			break;
		}		
	}

	return grade;
}

void UGsUIHUDChaotic::InitMaxPvpAlignmentPointMax()
{
	_pvpAlignmentDataArray.Empty();
	FGsSchemaPVPAlignmentData::GetStaticTable()->GetAllRows<FGsSchemaPVPAlignmentData>(_pvpAlignmentDataArray);
	_pvpAlignmentDataArray.Sort([&](const FGsSchemaPVPAlignmentData& source, const FGsSchemaPVPAlignmentData& dest)->bool {
		return source.pvpAlignmentPointMax < dest.pvpAlignmentPointMax;
		});

	int32 num = _pvpAlignmentDataArray.Num();
	int32 max = 0;		
	for (int32 i = 0; i < num; ++i)
	{
		if (max < _pvpAlignmentDataArray[i]->pvpAlignmentPointMax)
		{
			max = _pvpAlignmentDataArray[i]->pvpAlignmentPointMax;
			_maxPvpAlignmentData = _pvpAlignmentDataArray[i];
		}
	}
}