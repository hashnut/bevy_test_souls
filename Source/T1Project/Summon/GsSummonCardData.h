#pragma once

#include "CoreMinimal.h"

#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

struct FGsFairyData;
struct FGsCostumeData;

// 소환 카드 데이터
class FGsSummonCardData
{
public:
	// 데이터 인덱스
	int _dataIndex;
	// open order
	int _cardOpenOrder;

	// 생성자, 소멸자
public:
	FGsSummonCardData(int In_dataIndex) : _dataIndex(In_dataIndex)
	{
	}
	virtual ~FGsSummonCardData() = default;

	// 가상 함수
public:
	// 내 등급이 높은지
	virtual bool IsMyGradeBigger(FGsSummonCardData* In_target) { return false; }

	virtual bool IsHighCard(){return false;}
};


// 소환 페어리 카드 데이터
class FGsSummonCardDataFairy : public FGsSummonCardData
{
public:
	TSharedPtr<FGsFairyData> _data;
	FairyGrade _grade;

	// 생성자
public:

	FGsSummonCardDataFairy(TSharedPtr<FGsFairyData> In_data, FairyGrade In_grade, int In_dataIndex) :
		FGsSummonCardData(In_dataIndex), _data(In_data), _grade(In_grade)
	{
	}

	// 가상함수
public:
	// 내 등급이 높은지
	virtual bool IsMyGradeBigger(FGsSummonCardData* In_target) override
	{
		FGsSummonCardDataFairy* targetFairyCardData = static_cast<FGsSummonCardDataFairy*>(In_target);
		if (targetFairyCardData == nullptr)
		{
			return false;
		}

		return (int)_grade > (int)targetFairyCardData->_grade;
	}
	virtual bool IsHighCard() override ;
	
};

// 소환 코스튬 카드 데이터
class FGsSummonCardDataCostume : public FGsSummonCardData
{
public:
	TSharedPtr<FGsCostumeData> _data;
	CostumeGrade _grade;

	// 생성자
public:
	FGsSummonCardDataCostume(TSharedPtr<FGsCostumeData> In_data, CostumeGrade In_grade, int In_dataIndex) :
		FGsSummonCardData(In_dataIndex), _data(In_data), _grade(In_grade)
	{
	}
	// 가상함수
public:
	// 내 등급이 높은지
	virtual bool IsMyGradeBigger(FGsSummonCardData* In_target) override
	{
		FGsSummonCardDataCostume* targetCostumeCardData = static_cast<FGsSummonCardDataCostume*>(In_target);
		if (targetCostumeCardData == nullptr)
		{
			return false;
		}

		return (int)_grade > (int)targetCostumeCardData->_grade;
	}

	virtual bool IsHighCard() override;
};