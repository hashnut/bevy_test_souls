#pragma once
#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "Core/Public/Internationalization/Text.h"
#include "GsUIRegionTab.generated.h"

class UWidgetSwitcher;
class UGsButton;

UCLASS()
class UGsUIRegionTab :
	public UUserWidget,
	public IGsToggleGroupEntry
{
	GENERATED_BODY()
	// 멤버 변수
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherType;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTab;
	// 탭 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textTab;

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

};