// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconDefault.h"
#include "GsUIIconAbnormality.generated.h"

struct FGsAbnormalityData;
struct FGsSchemaAbnormalitySet;
struct FGsPassivityData;
struct FGsSchemaPassivitySet;

class UGsUITooltip;

/**
 * 상태이상(버프) 아이콘 
 */
UCLASS()
class T1PROJECT_API UGsUIIconAbnormality : public UGsUIIconDefault
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnClickAbnormalityIcon, UGsUIIconAbnormality*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPressAbnormalityIcon, UGsUIIconAbnormality*);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIIconAbnormality")
	bool _bIsSmallIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIIconAbnormality")
	FSoftObjectPath _pathEmptyIcon;

	// 필요한 아이콘WBP에서만 바인딩하여 사용
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIIconAbnormality")
	FText _textStackCount;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UGsButton* _btnSlot;

	float _flickeringSecond = 0.f;
	float _remainTime = 0.f;
	int32 _tId = 0;
	bool _bIsFlickering = false;
public:
	UGsUITooltip* _targetTooltip;
public:
	FOnClickAbnormalityIcon OnClickIcon;
	FOnPressAbnormalityIcon OnPressIcon;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// UGsUIIconBase override
	virtual void SetEmptyIcon() override;
	virtual void SetIconImage(const FSoftObjectPath& InPath) override;

public:
	void SetData(const FGsAbnormalityData* InData);
	void SetData(const FGsSchemaAbnormalitySet* InData);
	void SetData(const FGsPassivityData* InData);
	void SetData(const FGsSchemaPassivitySet* InData);

	int32 GetTableId() const { return _tId; }

protected:
	UFUNCTION()
	void OnClickButton();

	UFUNCTION()
	void OnPressButton();

	// 반짝이 애니메이션 재생(BP에서 구현)
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationFlicker();

protected:
	void SetUIStackCount(int32 InCount);
};
