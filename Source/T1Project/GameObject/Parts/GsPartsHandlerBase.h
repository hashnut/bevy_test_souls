// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/GsGameObjectHandler.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Define/EGsPartsType.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

/**
 * Object 파츠처리 담당 Base클래스
 * 파츠 데이터 ASync 로드 처리
 */
class T1PROJECT_API FGsPartsHandlerBase : public IGsGameObjectHandler
{
public:
	// Visible 처리 관련 레이어 설정 타입
	enum EVisibleLayer
	{
		Top,
		First,

		Max
	};

protected:
	class UGsGameObjectBase* _owner = nullptr;

	// 현재 파츠 정보
	const struct FGsSchemaPartsShapeData* _arrPartsData[(uint8)EGsPartsType::MAX] =
	{
		nullptr,
	};

	// 파츠 레이어 설정 (USceneComponent 기반)
	// 현재 미사용
	TMap<EVisibleLayer, TArray<class USceneComponent*>> _mapPartsLayer;

public:
	// CreatureGenderType 타입에 따라 맞는 ShapePath 데이터 정보를 분기
	static const struct FGsSchemaPartsShapeInfo* GetShapeInfo(CreatureGenderType GenderType, int PartsId);

public:
	FGsPartsHandlerBase() : IGsGameObjectHandler(GameObjectHandlerType::PARTS) {};
	virtual ~FGsPartsHandlerBase()	= default;

public:
	virtual void Initialize(class UGsGameObjectBase* Owner) override;
	virtual void Finalize() override;	
	
public:
	virtual void InvalidatePartsData(const struct UserLookInfo* LookInfoData);
	virtual void SetPartsData(const struct UserLookInfo* LookInfoData);

	virtual void RestoreAllParts() {};

protected:
	virtual void InitPartsLayer(class UGsGameObjectBase* Owner);
	virtual void ClearPartsLayer();

	void AddPartsLayer(EVisibleLayer LayerType, class USceneComponent* Component);

public:
	// 파츠 레이어 별로 보임/숨김 처리
	virtual void SetVisibleParts(EVisibleLayer Type);

public:
	// 착용 데이터 테이블 얻기
	const struct FGsSchemaPartsShapeData* GetPartsData(EGsPartsType PartsType);
};