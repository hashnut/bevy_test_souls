#pragma once
#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "Core/Public/Internationalization/Text.h"
#include "GsUISummonSkipToggleButton.generated.h"

class UWidgetSwitcher;
class UGsButton;

UCLASS()
class UGsUISummonSkipToggleButton :
	public UUserWidget,
	public IGsToggleGroupEntry
{
	GENERATED_BODY()
		// 멤버 변수
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSelect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTab;
	// 스킵 설명(일반 스킵, 합성 스킵)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textSkip;
	
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
	
	// 세팅 텍스트가 다를 때
	void SetText(const FText& In_Text);
	// 클릭 탭
	UFUNCTION()
	void OnClickTab();

};