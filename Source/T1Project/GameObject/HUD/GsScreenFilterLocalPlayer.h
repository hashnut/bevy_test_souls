// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../Message/GsMessageGameObject.h"
#include "DataSchema/GameObject/Common/GsSchemaScreenFilterInfo.h"

/**
 * 스크린 필터 제어 클래스
 * LocalPlayer 전용 클래스
 */

// Enum타입값에 따라 우선순위를 판단
// 더 뒤에 등록되는 타입이 높은 우선순위를 가진다.
// 타입의 정의는 순차적인 값을 기준으로 정한다. -> 1,2,3
enum class EGsScreenFilterType
{
	Abnormality = 0,
	SkillCollision = 1,

	Max,
};

class T1PROJECT_API FGsScreenFilterLocalPlayer
{
private:
	class FGsFilterRef
	{
	public:
		EGsScreenFilterType					Type		= EGsScreenFilterType::Max;
		int									RefCount	= 0;
		const FGsSchemaScreenFilterInfo*	Data		= nullptr;			
		bool operator== (const FGsFilterRef& rhs) const
		{
			return (Type == rhs.Type) && (Data == rhs.Data);
		}
	};

private:
	class UGsGameObjectBase* _owner = nullptr;
	TArray<FGsFilterRef>					_activeFilters;		// 활성화된 전체 목록
	FGsFilterRef*							_currentActive;		// 현재 활성화된 필터

public:
	void Initialize(class UGsGameObjectBase* Owner);
	void Finalize();

public:
	// 추후 우선순위 판단등의 규칙이 필요할수 있어 활성화된 경로를 알기위해 함수를 구분해둠
	void ActiveScreenFilter(EGsScreenFilterType ScreenFilterType, const struct FGsSchemaScreenFilterInfo& ScreenFilterInfo);
	void DeactiveScreenFilter(EGsScreenFilterType ScreenFilterType);
};
