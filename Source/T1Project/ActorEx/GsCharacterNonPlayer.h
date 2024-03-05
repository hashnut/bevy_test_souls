// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsCharacterNonPlayerBase.h"
#include "GameObject/Interaction/GsActorInteractionInterface.h"
#include "Quest/GsActorQuestTargetInterface.h"
#include "GsCharacterNonPlayer.generated.h"



class UCapsuleComponent;
class UMaterialInterface;
class UGsUIBillboardCreature;
class UGsUIBillboardInteractionButton;


UCLASS()
class T1PROJECT_API AGsCharacterNonPlayer : public AGsCharacterNonPlayerBase
{
	GENERATED_BODY()	
	
	// 인터랙션 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _interactionWidget = nullptr;
	
	//// 대사 연출 접근 금지용
#if 0
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Block", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* _interactionBlockCollision = nullptr;
#endif

private:
	// 디버그용 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	class UGsUIBillboardDebugText* _billboardDebugText = nullptr;

public:
	// Sets default values for this character's properties
	AGsCharacterNonPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	virtual void InitWidgetComponent() override;

public:
	virtual void OnHit(HitResultType ResultType, EGsStateBase CurrentState,
		const struct FGsSchemaCreatureCommonInfo* CommonData) override;
	
// 디버깅용 텍스트 출력 함수
#if	!UE_BUILD_SHIPPING
	void ShowDebugTextInfo(bool isOn);
	//void OnDebugTextInfo(const struct FGsDebugTextInfo& prevDebugTextInfo, const FGsDebugTextInfo& curDebugTextInfo);
	void OnDebugTextInfo(TArray<FString>&& InWholeDebugTextArray);
#endif

	// logic func
protected:
	void SetQuestMarkLocation();

protected:
	virtual void OnChangeRenderState(bool bIsRendered) override;

	// virtual get, set
public:
	// 인터랙션 위젯 컴퍼넌트 접근
	virtual UWidgetComponent* GetInteractionWidgetComponent() override { return _interactionWidget; }

public:
	// 인터랙션 클릭 콜백 세팅
	virtual void SetInteractionClickCallback(TFunction<void(bool)> In_callback) override;
	// 인터랙션 버튼 쓸지
	virtual void SetUseInteractionButton(bool In_isUse) override;
	void ShowInteractionButton(bool inShow);
	// 막는 처리 컬리더 접근
	UCapsuleComponent* GetInteractionBlockCollider() { return nullptr; }
	virtual  UGsUIBillboardInteractionButton* GetInteractionButton() const override;
};
