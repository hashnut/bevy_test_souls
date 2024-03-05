#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPKBookRecordDataEntry.generated.h"


class UImage;
class UGsButton;
class UTextBlock;
class UWidgetSwitcher;
class UGsUIRedDotDefault;


UCLASS()
class T1PROJECT_API UGsUIPKBookRecordDataEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	// 2022/09/22 PKT - 날짜
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDate;

	// 2022/10/13 PKT - 기록 사유
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherRecordReason;

	// 2022/09/22 PKT - 메인 타겟 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	// 2022/09/22 PKT - 길드 엠블럼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _iconGuildEmblem;

	// 2022/09/22 PKT - 길드 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildName;

	// 2022/09/22 PKT - 지역 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMapName;
	
	// 2022/09/26 PKT - Open Detail Button
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRecordBox;

	// 2022/09/26 PKT - 레드닷
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _redDotNormal;

	// 2022/09/26 PKT - 기록 고유 ID
	PKRecordId _PKRecordId = INVALID_PK_RECORD_ID;

	// 2022/09/26 PKT - 디테일 정보 활성 요청(Delegate)
public:
	DECLARE_DELEGATE_OneParam(FOnTryOpenPopUpDelegate, PKRecordId);
	FOnTryOpenPopUpDelegate OnTryOpenPopUpDelegate;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void BeginDestroy() override;

	// 2022/09/26 PKT - Open Detail 
protected:
	UFUNCTION()
	void OnClicked();	

public:
	void SetData(const class FGsPKRecordData* InData);
};
