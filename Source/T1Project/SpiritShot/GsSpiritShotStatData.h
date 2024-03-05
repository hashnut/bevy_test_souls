#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "EffectText/GsEffectTextDefine.h"

struct FGsSchemaPassivitySet;
struct FGsSchemaPassivityEffectStatSet;


class  FGsSpiritShotStatData final
{
	friend class FGsSpiritShotLevelData;
	friend class FGsSpiritShotManager;

	/************************************************************************/
	/* 스탯 관련 데이터                                                      */
	/************************************************************************/
private:
	/** 스탯 효과 표시를 위한 Stat 데이터 */
	FGsEffectTextCollection _effectCollection;

	/** 현재 스탯 데이터를 만드는 PassivityId List 저장 */
	TArray<PassivityId> _passivityIdList;

public:
	explicit FGsSpiritShotStatData();
	explicit FGsSpiritShotStatData(const TArray<PassivityId>& InIdList);
	explicit FGsSpiritShotStatData(TArray<PassivityId>&& InIdList);


	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
private:
	void MakeStatData();


	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/
public:
	/** 탭 내부 스탯 효과에 담길 텍스트 파싱하여 전달 */
	FText GetStatDescText() const;

	/** 현재 스탯 데이터에 저장된 PassivityId List 조회 */
	TArray<PassivityId> GetPassivityIdList() const;


	/************************************************************************/
	/* Setter (const 가 아닌 Setter 함수는 private 임에 주의하라)           */
	/************************************************************************/
private:
	void SetStatData(const TArray<PassivityId>& InIdList);
	void SetStatData(TArray<PassivityId>&& InIdList);
};