
#include "GsTitleData.h"
#include "DataSchema/Title/GsSchemaTitle.h"
#include "DataSchema/Title/GsSchemaTitleSubCategory.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "GsTitleHelper.h"
#include "T1Project/T1Project.h"



FGsTitleData::FGsTitleData(const FGsSchemaTitle* InData)
	: _data(InData) 
	, _statDetailInfos(GetStatData(InData))
	, _categoryId(GetCategoryIdByData(InData))
	, _colorTitle(GetTitleColor(InData))
	, _colorTitleHUD(GetTitleHUDColor(InData))
	, _status(TitleStatus::DISABLED)
	, _progressCount(NOT_IN_PROGRESS_COUNT)
	, _isEquip(false)
	, _textDesc(_data->desc)
{
	_textDesc = FText::Format(_data->desc, _data->conditionalCount);
	_textCombin = _textDesc;

	const static int32 INIT_PROGRESS_COUNT = 0;
	DescTextCombine(INIT_PROGRESS_COUNT);
}

TArray<FGsTitleData::StatDetailInfo> FGsTitleData::GetStatData(const FGsSchemaTitle* InData) const
{
	TArray<StatDetailInfo> outList;
	outList.Empty();

	do 
	{
		if (true == InData->passivitySetId.IsNull())
		{	// 2022/03/21 PKT - 패시비티가 없는 것도 존재.
			break;
		}

		const FGsSchemaPassivitySet* passivitySetData = InData->passivitySetId.GetRow();
		if (nullptr == passivitySetData)
		{	
			break;
		}		

		/**
		* Stat Info
		*/
		FGsEffectStringHelper::FGsEffectAddData effectAddData;
		if (false == FGsEffectStringHelper::AddPassivityEffect(passivitySetData, effectAddData))
		{
			GSLOG(Error, TEXT("not find Stat [%d]"), passivitySetData->id);
			break;
		}

		for (const auto& pair : effectAddData._statMap)
		{
			StatDetailInfo detailInfo;
			detailInfo._type = pair.Key;
			detailInfo.value = pair.Value;
			FGsStatHelper::GetDisPlayStatName(detailInfo._type, detailInfo._name);

			outList.Emplace(detailInfo);
		}

	} while (0);

	outList.Sort([](const StatDetailInfo& lhr, const StatDetailInfo& rhr)
		{
			return lhr._type < rhr._type;
		}
	);

	return outList;
}

TitleCategoryId FGsTitleData::GetCategoryIdByData(const FGsSchemaTitle* InData) const
{
	const FGsSchemaTitleSubCategory* categoryData = InData->subCategoryRowId.GetRow();
	if (nullptr == categoryData)
	{
		GSLOG(Error, TEXT("nullptr == categoryData : Title ID [%d]"), InData->id);
		return INVALID_TITLE_CATEGORY_ID;
	}
	return categoryData->id;
}

FLinearColor FGsTitleData::GetTitleColor(const struct FGsSchemaTitle* InData) const
{
	ItemGrade transformItemGrade = ItemGrade::MAX;
	switch (InData->grade)
	{
	case TitleGrade::NORMAL:	// 노멀
		transformItemGrade = ItemGrade::NORMAL;
		break;
	case TitleGrade::MAGIC:	// 매직
		transformItemGrade = ItemGrade::MAGIC;
		break;
	case TitleGrade::RARE:	// 레어
		transformItemGrade = ItemGrade::RARE;
		break;
	case TitleGrade::EPIC:	// 에픽
		transformItemGrade = ItemGrade::EPIC;
		break;
	case TitleGrade::UNIQUE:	// 유니크
		transformItemGrade = ItemGrade::UNIQUE;
		break;
	case TitleGrade::LEGEND:	// 레전드
		transformItemGrade = ItemGrade::LEGEND;
		break;
	}

	if (transformItemGrade == ItemGrade::MAX)
	{
		GSLOG(Error, TEXT("not find TitleGrade Color"));
		// 2022/01/20 PKT - 등급을 못찾았다고 세팅 자체를 안하면 안될것 같아서.. 색깔을 바꿔준다.
		transformItemGrade = ItemGrade::NORMAL;
	}

	return FGsUIColorHelper::GetColorItemGrade(transformItemGrade);
}

FLinearColor FGsTitleData::GetTitleHUDColor(const struct FGsSchemaTitle* InData) const
{
	EGsUIColorType colorType = EGsUIColorType::TITLE_HUD_GRADE_NORMAL;
	switch (InData->grade)
	{
	case TitleGrade::NORMAL:	// 노멀
		colorType = EGsUIColorType::TITLE_HUD_GRADE_NORMAL;
		break;
	case TitleGrade::MAGIC:	// 매직
		colorType = EGsUIColorType::TITLE_HUD_GRADE_MAGIC;
		break;
	case TitleGrade::RARE:	// 레어
		colorType = EGsUIColorType::TITLE_HUD_GRADE_RARE;
		break;
	case TitleGrade::EPIC:	// 에픽
		colorType = EGsUIColorType::TITLE_HUD_GRADE_EPIC;
		break;
	case TitleGrade::UNIQUE:	// 유니크
		colorType = EGsUIColorType::TITLE_HUD_GRADE_UNIQUE;
		break;
	case TitleGrade::LEGEND:	// 레전드
		colorType = EGsUIColorType::TITLE_HUD_GRADE_LEGEND;
		break;
	}

	return FGsUIColorHelper::GetColor(colorType);
}

void FGsTitleData::DescTextCombine(int32 InCount)
{
	if (_data->isProgressDisplayFlag)
	{
		int32 percent = StaticCast<int32>((StaticCast<float>(InCount) / _data->conditionalCount) * 100);
		percent = FMath::Clamp(percent, 0, 100);

		// 2023/9/25 PKT - https://jira.com2us.com/jira/browse/CHR-20796
		//					위의 요청으로 100%달성 시 진행 상태 Text 뺌
		const static int32 COMPLETED_PERCENT = 100;
		if (COMPLETED_PERCENT <= percent)
		{
			_textDesc = _textCombin;
		}
		else
		{
			FText progressPercent;
			FGsTitleHelper::FindText(TEXT("UI_Msg_ProgressPercent"), progressPercent);
			_textDesc = FText::FromString(
				FString::Format(TEXT("{0}{1}"), { _textCombin.ToString(), FText::Format(progressPercent, percent).ToString() })
			);
		}		
	}
}


void FGsTitleData::UpdateStatus(TitleStatus InStatus, int32 InConditionCount)
{
	_status = InStatus;
	_progressCount = InConditionCount;

	if (TitleStatus::DISABLED != _status)
	{
		_progressCount = _data->conditionalCount;
	}

	DescTextCombine(_progressCount);

	_OnStateChangedTitleData.ExecuteIfBound(this, ETitleMessageReason::eTitleStatus);
}


void FGsTitleData::SetChangedEquipState(bool InIsEquip)
{
	if (true == InIsEquip && false == IsAcquire())
	{	// 2022/01/12 PKT - 장착을 하려고 했으나 아직 획득하지 못한 것.
		GSLOG(Error, TEXT("Haven't gotten it yet"));
		return;
	}

	if (_isEquip != InIsEquip)
	{
		_isEquip = InIsEquip;
		// 2022/01/12 PKT - 상태가 변경 되었음을 알린다.
		_OnStateChangedTitleData.ExecuteIfBound(this, ETitleMessageReason::eEquipState);
	}
}

void FGsTitleData::Confirm()
{
	if (TitleStatus::ACTIVED != _status )
	{	
		return;
	}

	_status = TitleStatus::COMPLETED;
	
	_OnStateChangedTitleData.ExecuteIfBound(this, ETitleMessageReason::eTitleStatus);
}

TitleId FGsTitleData::GetId() const
{
	return _data->id;
}

TitleCategoryId FGsTitleData::GetCategoryId() const
{
	return _categoryId;
}

TitleEquipSlot FGsTitleData::GetEquipSlotType() const
{
	return _data->equipSlotType;
}

const FText& FGsTitleData::GetTitle() const
{
	return _data->name;
}

const FLinearColor& FGsTitleData::GetTitleColor() const
{
	return _colorTitle;
}

const FLinearColor& FGsTitleData::GetTitleHUDColor() const
{
	return _colorTitleHUD;
}


const FText& FGsTitleData::GetDesc() const
{
	return _textDesc;
}

const FText& FGsTitleData::GetPassivitySubDesc() const
{
	return _data->passivityDesc;
}

TitleGrade FGsTitleData::GetGrade() const
{
	return _data->grade;
}

bool FGsTitleData::IsAcquire() const
{
	return (TitleStatus::DISABLED != _status);
}

bool FGsTitleData::IsEquip() const
{
	return _isEquip;
}

bool FGsTitleData::IsNewAcquire() const
{
	return (TitleStatus::ACTIVED == _status);
}

PassivityId FGsTitleData::GetPassivityId() const
{
	const FGsSchemaPassivitySet* passivitySetData = _data->passivitySetId.GetRow();
	if (nullptr == passivitySetData)
	{
		return INVALID_PASSIVITY_ID;
	}

	return passivitySetData->id;
}

int32 FGsTitleData::GetStatCount() const
{
	return _statDetailInfos.Num();
}

StatType FGsTitleData::GetStatType(int32 InIndex) const
{
	if (false == _statDetailInfos.IsValidIndex(InIndex))
	{
		return StatType::NONE;
	}

	return _statDetailInfos[InIndex]._type;
}

const FText& FGsTitleData::GetStatName(int32 InIndex) const
{
	if (false == _statDetailInfos.IsValidIndex(InIndex))
	{
		return FText::GetEmpty();
	}

	return _statDetailInfos[InIndex]._name;
}

int32 FGsTitleData::GetStatValue(int32 InIndex) const
{
	if (false == _statDetailInfos.IsValidIndex(InIndex))
	{
		return 0;
	}

	return _statDetailInfos[InIndex].value;
}

bool FGsTitleData::IsContainsStat(StatType InStatType) const
{
	return (nullptr != _statDetailInfos.FindByPredicate([InStatType](const StatDetailInfo& InData)
		{
			return (InData._type == InStatType);
		}
	));
}

FGsTitleData::FOnStateChangedTitleData& FGsTitleData::OnStateChangedDelegate()
{
	return _OnStateChangedTitleData;
}