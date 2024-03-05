#pragma once
#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "Core/Public/Internationalization/Text.h"
#include "GsUIPartyOwnTab.generated.h"

class UWidgetSwitcher;
class UGsButton;

UCLASS()
class UGsUIPartyOwnTab :
	public UUserWidget,
	public IGsToggleGroupEntry
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherType;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTab;
	// 탭 이름(선택)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textSelected;
	// 탭 이름(선택 안됨)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textUnSelected;
	bool _bIsSelected = false;

	// 가상 함수
public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

	// 로직 함수
public:
	// 텍스트 세팅
	void SetText(const FText& In_text);
	// 클릭 탭
	UFUNCTION()
	void OnClickTab();
	// 버튼 활성화 할지
	void SetButtonEnable(bool In_enable);
};