
#include "GsUITitleEquipSlotStatInfo.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Title/GsTitleData.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Title/GsTitleHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "T1Project/T1Project.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UIContent/Window/Title/GsUITitleEquipGradeLoopEffect.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/HorizontalBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"

void UGsUITitleEquipSlotStatInfo::NativeConstruct()
{
	Super::NativeConstruct();

	_animationData = nullptr;
	
	UWidgetBlueprintGeneratedClass* genClass = GetWidgetTreeOwningClass();

	if (nullptr != genClass)
	{
		for (UWidgetAnimation* anim : genClass->Animations)
		{
			_animationData = anim;

			break;
		}
	}

	_titleData = nullptr;
	_isSelected = false;

	_effectUnEquip->HideVFX();
	_effectEquip->HideVFX();		
}


void UGsUITitleEquipSlotStatInfo::NativeDestruct()
{
	Super::NativeDestruct();

	_animationData = nullptr;
}

void UGsUITitleEquipSlotStatInfo::SetDisplay(const FGsTitleData* InData)
{
	if (nullptr == InData)
	{	// 2022/06/21 PKT - 장착 중이 아닐때
		_textTitle->SetText(FText::FromString(TEXT("-")));
		_textTitle->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_DISABLE));
		_switcherList->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_textTitle->SetText(InData->GetTitle());
		_textTitle->SetColorAndOpacity(InData->GetTitleColor());

		_switcherList->SetVisibility(ESlateVisibility::HitTestInvisible);

		const static int32 SWITCHER_EMPTY_INDEX = 1;
		const static int32 SWITCHER_SHOW_INDEX = 0;
		
		// 2022/06/21 PKT - Title에 Stat이 없을 수도 있음.
		bool isEmptyStat = (0 >= InData->GetStatCount());
		_switcherList->SetActiveWidgetIndex(isEmptyStat ? SWITCHER_EMPTY_INDEX : SWITCHER_SHOW_INDEX);

		if (isEmptyStat)
		{
			FText message;
			FGsTitleHelper::FindText(TEXT("Notify_EMPTY_Stat_List"), message);
			_textEmptyStat->SetText(message);
			// 2023/3/16 PKT - 덕규님 요청으로 오픈 스팩에는 패시비티가 없으므로.. 칭호 효과 없음 문구 숨겨달라는 요청
			_textEmptyStat->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			auto DisplayStat = [InData](int32 InIndex, UTextBlock** OutStatName, UTextBlock** OutStatValue)
			{
				if (StatType::NONE == InData->GetStatType(InIndex))
				{
					return false;
				}

				(*OutStatName)->SetText(InData->GetStatName(InIndex));

				StatType statType = InData->GetStatType(InIndex);
				StatValue statValue = InData->GetStatValue(InIndex);

				FText valueName;
				FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statType, statValue, valueName);

				(*OutStatValue)->SetText(valueName);

				return true;					
			};
			
			const static int32 FIRST_STAT_INDEX = 0;
			const static int32 SECOND_STAT_INDEX = 1;

			bool isVisibility = DisplayStat(FIRST_STAT_INDEX, &_textFirstStatName, &_textFirstStatValue);
			_horizontalBoxFirst->SetVisibility(isVisibility ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

			isVisibility = DisplayStat(SECOND_STAT_INDEX, &_textSecondStatName, &_textSecondStatValue);
			_horizontalBoxSecond->SetVisibility(isVisibility ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
	}
}

void UGsUITitleEquipSlotStatInfo::SetChangedEquipTitle(bool InIsEquip, bool InIsShowEffect /*= false*/)
{
	if (false == InIsShowEffect)
	{
		return;
	}

	// 2022/06/21 PKT - Show (Equip / UnEquip) Effect
	if (InIsEquip)
	{
		_effectUnEquip->HideVFX();
		_effectEquip->ShowVFX();		
	}
	else
	{
		_effectEquip->HideVFX();
		_effectUnEquip->ShowVFX();
	}
}

void UGsUITitleEquipSlotStatInfo::SetPlayAnimation(float InAtTime, bool InIsForward)
{
	const static int32 NUM_LOOPS_TO_PLAY = 1;
	EUMGSequencePlayMode::Type type = (InIsForward) ? EUMGSequencePlayMode::Type::Forward : EUMGSequencePlayMode::Type::Reverse;
	PlayAnimation(_animationData, InAtTime, NUM_LOOPS_TO_PLAY, type, 1.f);
}

void UGsUITitleEquipSlotStatInfo::SetData(const FGsTitleData* InData, bool InIsShowEffect)
{
	TitleId preTitleId = (nullptr != _titleData) ? _titleData->GetId() : INVALID_TITLE_ID;

	_titleData = InData;

	TitleId curTitleId = (nullptr != _titleData) ? _titleData->GetId() : INVALID_TITLE_ID;

	// 2022/06/21 PKT - Set Display
	SetDisplay(InData);
	// 2022/07/19 PKT - 장착 상태가 변경 되었으면 루프 이펙트도 변경	
	_animationGradeEffect->SetData(_isSelected, (nullptr == _titleData) ? TitleGrade::MAX : InData->GetGrade());

	if (INVALID_TITLE_ID != curTitleId && curTitleId != preTitleId)
	{	// 2022/04/05 PKT - 무언가 새로운 타이틀을 장착
		SetChangedEquipTitle(true, InIsShowEffect);
	}
	else if (INVALID_TITLE_ID != preTitleId && curTitleId == INVALID_TITLE_ID)
	{	// 2022/06/21 PKT - 타이틀 장착 해제
		SetChangedEquipTitle(false, InIsShowEffect);
	}
}


void UGsUITitleEquipSlotStatInfo::PlayEffect(bool InIsSelected)
{
	if (nullptr == _animationData)
	{
		GSLOG(Error, TEXT("nullptr == _animationData"));
		return;
	}

	const float beginTime = 0.f;
	const float endTitme = _animationData->GetEndTime();

	float elapsedTime = GetAnimationCurrentTime(_animationData);

	SetPlayAnimation(elapsedTime, InIsSelected ? false : true);

	_isSelected = InIsSelected;
	// 2022/07/19 PKT - 탭 선택 상태가 변경 되었으면 루프 이펙트도 변경
	_animationGradeEffect->SetData(_isSelected, (nullptr == _titleData) ? TitleGrade::MAX : _titleData->GetGrade());
}