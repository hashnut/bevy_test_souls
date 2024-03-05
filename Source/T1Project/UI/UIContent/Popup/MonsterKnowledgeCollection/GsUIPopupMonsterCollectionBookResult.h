
#pragma once

#include "CoreMinimal.h"
#include "../UI/UILib/Base/GsUIPopup.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"
#include "GsUIPopupMonsterCollectionBookResult.generated.h"

class UTileView;
class UGsButton;
class UTextBlock;
class UGsPoolUObject;

UCLASS()
class T1PROJECT_API UGsUIPopupMonsterCollectionBookResult : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOK;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTileView* _tileListMonsterBook;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textRewardPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimationTime, meta = (ToolTip = "Portrait Appear Time"))
	float _portraitOpenDelay = 0.f;

	UPROPERTY()
	UGsPoolUObject* _poolBookResultDataSet;

protected:
	int32 _currentCount = 0;
	int32 _maxCount = 0;
	float _deltaTime = 0.f;

	struct FGsBookResultData
	{
		const class FGsMonsterKnowledgeBook* _data = nullptr;
		bool _isLevelUp = false;
	};
	TArray<FGsBookResultData> _resultSet;

public:
	// 2022/12/27 PKT - 상태를 관리자에게 알릴 필요가 있을 경우 호출 됨.
	DECLARE_DELEGATE(FOnCloseDelegate);
	FOnCloseDelegate OnCloseDelegate;

protected:
	void NativeOnInitialized() override;
	void BeginDestroy() override;
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION()
	void OnClose();

public:
	void SetData(const TArray<FGsMonsterCollectionBookResult>& InData);
};