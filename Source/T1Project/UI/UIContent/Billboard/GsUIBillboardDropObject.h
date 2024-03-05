// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIBillboard.h"
#include "GsUIBillboardDropObject.generated.h"

class UTextBlock;
class UButton;
/**
 * 크리쳐 기본 빌보드. 위젯 컴포넌트에서 사용되는 객체.
 */
UCLASS()
class T1PROJECT_API UGsUIBillboardDropObject : public UGsUIBillboard
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;

protected:
	virtual EGsGameObjectType GetOwnerObjetType() const override { return EGsGameObjectType::DropObject; }
	virtual void NativeConstruct() override;
public:
	void SetItemNameText(const FText& InName);
	void SetItemNameColor(const FLinearColor& InColor);	

	// 이름 출력 옵션 갱신
	void UpdateShowNameOption();
};
