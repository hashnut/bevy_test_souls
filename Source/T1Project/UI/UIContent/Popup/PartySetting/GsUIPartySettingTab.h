#pragma once
#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "Core/Public/Internationalization/Text.h"
#include "GsUIPartySettingTab.generated.h"

class UWidgetSwitcher;
class UGsButton;

UCLASS()
class UGsUIPartySettingTab :
	public UUserWidget,
	public IGsToggleGroupEntry
{
	GENERATED_BODY()

	// ToggleGroup을 안쓰고 단독으로 사용할 경우를 위하여 추가
public:
	DECLARE_DELEGATE(FGsToggleButtonClicked);

	// 멤버 변수
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
	// 탭이름(비활성화)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textDisable;
	bool _bIsSelected = false;
	// 사용 가능한가
	bool _isUseable = true;
	// 가상 함수

public:
	FGsToggleButtonClicked OnToggleButtonClicked;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

	// 로직 함수
public:
	// 텍스트 세팅
	void SetText(const FText& In_text );
	// 세팅 텍스트가 다를 때
	void SetText(const FText& In_selectText, const FText& In_unselectText);
	// 클릭 탭
	UFUNCTION()
	void OnClickTab();

	// get, set
public:
	void SetIsUseable(bool In_enable)
	{
		_isUseable = In_enable;
	}
};