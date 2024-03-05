// 201904-15 BSAM - It is a data asset used only in the Skill Editor.

/**
 *  Asset container for SKillEditor.
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimSequenceSkill.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimMontageSkill.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "SkillEditorDataAsset.generated.h"

// BSAM - Forward declarations
struct FGsSkillSetData;
class USkillMetaData;

// conver용 임시
USTRUCT()
struct FSkillSetJson
{
	GENERATED_BODY()

public:
	int32 id;
	//ActionCategory actionCategory;
	TArray<int32> notifyInfoList;
};

DECLARE_DELEGATE_OneParam(FOnObjectsSelected, const TArray<UObject*>& /*InObjects*/);

UCLASS()
// 기본적으로 Skill Editor 전용 데이터들이다. 다만 클라, 서버 클래스, 구조체 정보들을 참조 한다(따로 안만드는 이유는 수정, 변경될때 마다 작업이 너무 많다, 대부분 종복코드일텐데...)
class SKILLEDITOR_API USkillEditorDataAsset : public UDataAsset
{
	GENERATED_BODY()		
	
public:
	UPROPERTY(EditAnywhere, Category = SkillEditorData)
	bool AutoTotalRuntimeSetting;

	UPROPERTY(EditAnywhere, Category = SkillEditorData)
	ActionCategory ActionCategory;

	UPROPERTY(EditAnywhere, Category = SkillEditorData)
	SkillCategory SkillCategory;

	UPROPERTY(EditAnywhere, Category = SkillEditorData)
	CommonActionType CommonCategory;

	UPROPERTY(EditAnywhere, Category = SkillEditorData)
	EReactionType ReactionCategory;
	
	UPROPERTY(EditAnywhere, Category = SkillEditorData)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, Category = SkillEditorData)
	UAnimSequence* AnimSequence;

	UPROPERTY(EditAnywhere, Category = RAON_Utils)
	TArray<FSpawnBP> SpawnBPData;

	UPROPERTY(EditAnywhere, Category = RAON_Utils)
	bool SpawnBPDataRendering;

	UPROPERTY(EditAnywhere, Category = RAON_Utils)
	float ActorScale = 1.0f;

	//UPROPERTY(EditAnywhere, Category = RAON_Utils)
	//FTransform ActorTM;

	UPROPERTY()	
	TArray<FAnimNotifyEvent> SkillNotifies;	

	UPROPERTY()	
	TArray<USkillMetaData*> ListSkillSetData;

	UPROPERTY()
	UAnimSequenceSkill* AnimSequenceSkill;

	UPROPERTY()
	UAnimMontageSkill* AnimMontageSkill;

	enum::ActionCategory BeforeActionCategory;
	enum::SkillCategory BeforeSkillCategory;
	enum::CommonActionType BeforeCommonCategory;
	enum::EReactionType BeforeReactionCategory;

	UAnimSequence* BeforeAnimSequence;
	UAnimMontage* BeforeAnimMontage;
	
	FSimpleDelegate OnChangeEdited;
	FSimpleDelegate OnChangeSectionCategoryEdited;
	FOnObjectsSelected OnNotifyIdChangedEx;

	TArray<FSkillSetJson> SkillSetJson;
	float RateScale;
	
	FSimpleDelegate OnSpawnBPDataGsCharacterBaseSettingUpdate;
	FSimpleDelegate OnSpawnBPDataGsCharacterBaseRemoveUpdate;
	FSimpleDelegate OnSpawnBPDataGsCharacterBaseRemoveAllUpdate;
	FSimpleDelegate OnSpawnBPDataRendering;

	FSimpleDelegate OnSpawnTransformUpdate;

	FSimpleDelegate OnActorScaleUpdate;

public:
	USkillEditorDataAsset();

public:
	void Selected();
	void Pate(USkillEditorDataAsset* InSource);
	//void NotifiesChanged();
	void NotifiesChangedEx(const TArray<UObject*>& InObjects);
	void Convert();
	bool Export();
	// [B.Y] 원본 테이블 클리어 처리
	// 어디에 있어야될지 잘 몰라서 일단 여기에 Static함수로 정의
	static void ClearTable();

	// Section Category가 변경 됬다는 말이며 해당 변경사항에 따라 Section이 1개로 줄어들수도 있다
	void ChangeSectionCategory(int32 InChangedSkillCategory);
	bool IsDivide();
	void SkillSectionDataNextIdSet();
	void SkillSectionDataSet();
	void SetSkillUesNotifyCollisionDraw(int32 InSectionIndex, bool InUseSkillNotifyCollision);
	//void SectionsChanged();

private:
	void CreateAnimSequenceSkill();
	void CreateAnimSequenceSkillSection();
	void CreateAnimMontageSkill();
	void CreateAnimMontageSkillSection();	
	bool IsSectionDataValid();

#if WITH_EDITORONLY_DATA
	void SkillCategoryHide();
	void SkillCategoryShow();

	void CommonCategoryHide();
	void CommonCategoryShow();

	void ReactionCategoryHide();
	void ReactionCategoryShow();

	void AnimMontageHide();
	void AnimMontageShow();

	void AnimSequenceHide();
	void AnimSequenceShow();

	// [B.Y] SkillnotifySet Composite 작업
	void AddSkillsetDataTable(class UGsTable* InSkillSetTable, class USkillSectionData* InSkillSectionData);
	// [B.Y] 클라이언트에서 사용할 SkillLevleSet 통합 처리 작업
	void AddSkillLevelSetDataTable(struct OUT FGsSchemaSkillSet* OutSkillSetData);

	// [B.Y] Data Valid 체크
	// Collision
	bool CheckSkillCollision(const struct FGsSchemaSkillCollision& InSkillCollision);
#endif

protected:
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;	

public:
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
	virtual void PostInterpChange(FProperty* PropertyThatChanged) override;
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	virtual void PreEditChange(class FEditPropertyChain& PropertyAboutToChange) override;

#if WITH_EDITORONLY_DATA
	virtual void Serialize(FStructuredArchive::FRecord Record) override;
#endif
};
