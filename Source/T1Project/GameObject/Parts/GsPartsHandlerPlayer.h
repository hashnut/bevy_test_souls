// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsPartsHandlerBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/Define/EGsPartsType.h"

/**
 * Local, Remote Player 파츠 처리 담당 클래스
 * Local처리가 따로 필요할 경우 이 클래스를 상속 하여 구현
 * NonPlayer 처리가 필요할경우 Creature 클래스로 승격후 세분화 작업
 * 
 * 현재 파츠 처리 방식을 2가지로 구현해 놓고 있음
 * 1. 메시 병합 방식 -> @see : USkeletalMesh* MergeParts() const;
 * 2. USkeletalMeshComponent MasterPose 기능 방식 
 *   @see : void InitSkeletalMeshComponent(USkeletalMeshComponent* Mesh);
 */
class T1PROJECT_API FGsPartsHandlerPlayer : public FGsPartsHandlerBase
{
	using Super = FGsPartsHandlerBase;

private:
	class AGsCharacterPlayer* _playerActor = nullptr;

public:
	FGsPartsHandlerPlayer()				= default;
	virtual ~FGsPartsHandlerPlayer()	= default;

	virtual void Initialize(UGsGameObjectBase* Owner) override;
	virtual void Finalize() override;
	virtual void InitializeActor(class AActor* Owner) override;
protected:
	virtual void InitPartsLayer(UGsGameObjectBase* Owner) override;

public:
	const struct FGsSchemaPartsShapeData* GetDefaultPartsModel(EGsPartsType InType) const;
	uint32 GetDefaultPartsModelIndex(EGsPartsType InType) const;

	virtual void RestoreAllParts() override;

protected:
	void InvalidatePartsData(const UserLookInfo* LookInfoData) override;
	void SetPartsData(const UserLookInfo* LookInfoData) override;

	void UpdatePartsLayerData();

private:
	void AddPartsProc(const UserLookInfo InLookInfo);
	uint32 GetLookInfoByPartsType(const EGsPartsType InPartsType, const UserLookInfo InLookInfo);
	void AttachParts(class FGsCustomizeHandler* InHandler, const struct FGsSchemaPartsShapeData* InData, const EGsPartsType InPartsType);
};
