// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GsSchemaEnums.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "GsBlueprintFunctionLibraryUI.generated.h"

class UGsUIWidgetBase;

/**
 * UI관련 static 함수 모음. World 인자로 받는 항목은 코드에서만 사용.
 */
UCLASS()
class T1PROJECT_API UGsBlueprintFunctionLibraryUI : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GsUIHelper")
	static class UGsWidgetPoolManager* GetWidgetPoolManager();	

	UFUNCTION(BlueprintCallable, Category = "GsUIHelper")
	static class UGsUIDealScrollBase* AddDealScroll(FName InKey, class USceneComponent* InTarget,
		const FString& InValue, const FVector& InDir, int64 InGameId);

	static const FName GetDealScrollKey(HitResultType InType, bool bInIsDamaged);

	UFUNCTION(BlueprintCallable, Category = "GsUIHelper")
	static class UGsUIHUDEmoticon* AddEmoticon(EGsEmoticonType InType, class UGsGameObjectBase* InTarget, FVector InDir = FVector::ZeroVector);

	// 입력된 좌표에 출력(VFX위젯의 좌상단 기준)
	UFUNCTION(BlueprintCallable, Category = "GsUIHelper")
	static UGsUIVFX* AddVFX(TSubclassOf<UGsUIVFX> InClass, const FVector2D& InPosition, int32 InZOrder = 0);	
	UFUNCTION(BlueprintCallable, Category = "GsUIHelper")
	static UGsUIVFX* AddVFXToPointerPos(TSubclassOf<UGsUIVFX> InClass, int32 InZOrder = 0);

	static class UGsUITrayLongPressBar* AddLongPressBar(class UWidget* InTarget, int32 InZOrder = 0);
	static class UGsUITrayItemNameToolTip* AddNameTooltip(class UWidget* InTarget, int32 InZOrder = 0);

private:
	static UGsUIWidgetBase* AddWidgetToTargetTop(class UWidget* InTarget, UClass* InWidgetClass, 
		OUT FVector2D& OutTargetPos);

public:
	static void RemoveVFX(UGsUIVFX* InVfx);
	static void RemoveWidget(UGsUIWidgetBase* InWidget);
	static bool GetPointerPositionAbsolute(OUT FVector2D& OutAbsolutePosition);
	static bool GetPointerPositionLocal(UObject* WorldContextObject, OUT FVector2D& OutLocalPosition);

	// GsUIManager를 통해 UI 열기
	UFUNCTION(BlueprintCallable, Category = "GsUIHelper")
	static UGsUIWidgetBase* OpenUI(const FName& InKey, bool bInCheckDuplicate);

	// GsUIManager에서 현재 뷰포트에 활성중인 UI 얻어오기.
	UFUNCTION(BlueprintCallable, Category = "GsUIHelper")
	static UGsUIWidgetBase* GetUI(const FName& InKey);
	
	// GsUIManager를 통해 UI 닫기
	UFUNCTION(BlueprintCallable, Category = "GsUIHelper")
	static void CloseUI(const FName& InKey);

	// 주의!! StopAllAnimation 쓰지마세요
	// UMGSequencePlayer 쌓이는 버그 수정을 위한 함수. UGsUIWidgetBase::StopAllAnimationsEx 와 동일기능
	UFUNCTION(BlueprintCallable, Category = "GsUIHelper")
	static void StopAllAndClearAnimation(class UUserWidget* InWidget);
};
