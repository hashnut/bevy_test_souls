#pragma once

/**
* file		GsUIMonsterKnowledgeCollectionView.h
* @brief	몬스터 지식 리스트에서 몬스터 컬렉션 뷰 리스트
* @author	PKT
* @date		2021/11/23
**/

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIMonsterCollectionPortraitSet.generated.h"

class UWrapBox;
class UGsDynamicPanelSlotHelper;

UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionPortraitSet : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperPortraitSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWrapBox* _boxPortraitSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfPortraitSet;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;	

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnRefreshEntryPortraitSet(int32 InIndex, UWidget* InEntry);

public:
	void SetData(int32 InMaxCount);
	void Invalidate(uint32 InId);

//	//----------------------------------------------------------------------------------------------------------------------
//		// Tutorial
//private:
//	bool _bIsTutorial = false;
//
//public:
//	DECLARE_DELEGATE_OneParam(FOnTutorialSettingComplete, UWidget*);
//	FOnTutorialSettingComplete OnTutorialSettingComplete;
//
//public:
//	void SetTutorial(bool bIsTutorial) { _bIsTutorial = bIsTutorial; }
//	bool IsTutorial() const { return _bIsTutorial; }
};