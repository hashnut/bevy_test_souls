// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "GsUIAbnormalityIconList.generated.h"

/**
 * 상태이상 아이콘 리스트
 */
UCLASS()
class T1PROJECT_API UGsUIAbnormalityIconList : 
	public UUserWidget, 
	public IGsInvalidateUIInterface
{
	GENERATED_BODY()
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnClickAbnormalityIconList, class UGsUIIconAbnormality*);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;
	
	UPROPERTY()
	class UGsDynamicIconSlotHelper* _slotHelper;

	TWeakObjectPtr<class UGsGameObjectBase> _owner;
	TArray<struct FGsAbnormalityData*> _dataList;
	
	MsgGameObjHandle _messageHandler;

public:
	// 리스트 내 아이콘 클릭 시
	FOnClickAbnormalityIconList OnClickList;

public:
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// IGsInvalidateUIInterface override
	virtual void InvalidateAllInternal() override;
	
public:
	// InvalidateAll()시 다음 틱에 수행되므로, 바로 갱신시키고 싶을 경우에 사용
	void InvalidateImmediately();

	// 타겟 설정하면서 이벤트 연결/ 아니면 이벤트 연결후에 넘기기만하면 어떨까.
	void SetTarget(class UGsGameObjectBase* InTarget);

protected:
	// 상태이상 갱신 메시지 받을 때
	void OnUpdateAbnormality(const IGsMessageParam* InParam);

	// 아이콘 생성 시
	UFUNCTION()
	void OnCreateIcon(class UGsUIIconBase* InIcon);
	
	// 아이콘 갱신 시
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, class UGsUIIconBase* InIcon);

	// 아이콘 클릭 시
	UFUNCTION()
	void OnClickSlot(class UGsUIIconAbnormality* InIcon);
};
