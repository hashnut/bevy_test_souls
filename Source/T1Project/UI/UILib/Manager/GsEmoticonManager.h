// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "UObject/NoExportTypes.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsEmoticonManager.generated.h"

class UGsWidgetPoolManager;
struct FGsSchemaEmoticonResData;
class UGsUIHUDEmoticon;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsEmoticonManager :
	public UObject,
	public IGsManager
{
	GENERATED_BODY()
	
private:
	TWeakObjectPtr<class UGsWidgetPoolManager> _poolManager;

	// 풀에서 해제되었을 때 불릴 함수
	TFunction<void(class UUserWidget*)> _onReleaseCompleteFunc = nullptr;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

	UGsUIHUDEmoticon* AddWidget(EGsEmoticonType InType, class UGsGameObjectBase* InTarget, FVector InDir = FVector::ZeroVector);
	void ReleaseWidget(UGsUIHUDEmoticon* InWidget);

private:
	const FGsSchemaEmoticonResData* GetEmoticonResData(EGsEmoticonType InRowKey) const;

	UGsUIHUDEmoticon* Claim(UClass* InClass);
};
