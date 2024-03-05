// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIDropDownMenuCommon.generated.h"

struct IMenuData;

// DropDown Menu Entry 설정 정보
USTRUCT()
struct FMenuData
{
	GENERATED_BODY()

	// Text Combobox 기능
	FText _menuName = FText::GetEmpty();
	FLinearColor _menuColor = FLinearColor::Transparent;
	FLinearColor _menuSelectColor = FLinearColor::Transparent;
	FLinearColor _menuDisableColor = FLinearColor::Transparent;

	//Icon Type Menu 사용 정보
	FSoftObjectPath _menuIcon = FSoftObjectPath();
	FVector2D	 _imgSize = FVector2D::ZeroVector;
	FLinearColor _imgTintColor = FLinearColor::Transparent;

	//Menu에 보관 데이터 menu 생성시 MakeShared 다른 곳에서 shared로 가지고 있지 않도록 주의(reference count 증가 방지)
	TSharedPtr<IMenuData> _data{ nullptr }; 

protected:
	TWeakObjectPtr<UUserWidget> _content{ nullptr };

public:
	template<typename T, typename TEnableIf<TIsDerivedFrom<T, UUserWidget>::IsDerived, T>::Type>
	TWeakObjectPtr<T> GetContent() { return Cast<T>(_content.Get()); }

	friend class UGsUIDropDownMenuCommon;
};

class UGsUIDropDownMenuEntry;
class UGsSwitcherButton;
class UPanelWidget;
class UPanelSlot;

/**
 * Dynamic하게 생성 또는 변경되는 DropDown Menu
 * Icon 을 사용하는 경우는 UGsTableWeaponTypeMenu 에 넣고 등록해서 사용할 수도 있다
 * Color 변경, Text 변경 Icon 변경 가능
 */
UCLASS()
class T1PROJECT_API UGsUIDropDownMenuCommon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE_RetVal_TwoParams(bool, FOnSelectedMenu, int32, bool);
	DECLARE_DELEGATE_TwoParams(FOnCreateMenuEntry, int32, FMenuData&);
	DECLARE_DELEGATE_OneParam(FOnRemoveMenuEntry, const FMenuData&);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _usePanel; // DropDown Menu가 On/Off 되는 Panel

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _selectedName;	// 선택된 메뉴의 이름 출력

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FSlateColor _selectedColor; // 선택된 메뉴의 Color를 동일하게 바꿀 때

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FSlateBrush _selectediconBrush;	// 선택된 메뉴의 Icon Image로 동일하게 바꿀 때

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnMenuOn;	// DropDown Menu On/Off Button
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (DisplayName = "DropDownEntry"))
	TSubclassOf<UGsUIDropDownMenuEntry> _entryWidget;	// 생성 메뉴 Entry

	UPROPERTY(BlueprintReadOnly)
	UPanelWidget* _menuPanel{ nullptr }; // DropDown Menu Entry가 붙는 Panel

protected:
	UPROPERTY()
	TArray<FMenuData> _reservedData;

	UPROPERTY()
	TArray<UPanelSlot*> _menuSlots;

	int32 _selectedIndex{ -1 };

	float _mouseLeaveTime{ 0.f };
	bool  _useMenuPanel{ true };
	bool  _useOutClose{ true };

public:
	FOnSelectedMenu    OnSelectedEntry;
	FOnCreateMenuEntry OnCreateEntry;
	FOnRemoveMenuEntry OnRemovedEntry;

public:
	UFUNCTION(BlueprintCallable)
	void OnSelectedMenuEntry(const UGsUIDropDownMenuEntry* Entry);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

protected:
	UPanelWidget* FindMenuPanel(UPanelWidget* panel);

	void SetMenuEntryColorProperty(UGsUIDropDownMenuEntry* Entry, FLinearColor enableColor, FLinearColor selectColor = FLinearColor::Transparent, FLinearColor disableColor = FLinearColor::Transparent);

	void RemoveAll();

	UFUNCTION(BlueprintCallable)
	virtual void OnMenuPopup();

public:
	UFUNCTION(BlueprintCallable)
	void OpenMenuPopup();

	UFUNCTION(BlueprintCallable)
	void CloseMenuPopup();

	UFUNCTION(BlueprintCallable)
	bool IsOpenPopup();

public:
	virtual void InitializeMenu(int32 Count, int32 init = 0);
	virtual void RefreshUI();

	int32 GetSelectedMenuIndex() const { return _selectedIndex; }
	void  SetSelectMenuEntry(int32 index, bool init = false);
	void  SetEnableMenuEntry(int32 index, bool set, bool collapsed = false);
	void  SetUseMouseOutClose(bool set) { _useOutClose = set; }

	const TArray<FMenuData>& GetMenuEntryDataList() const { return _reservedData; }
	const FMenuData& GetMenuEntryData(int32 index) const;

public:
	const FText GetMenuEntryName(int32 index) const;
	const FLinearColor GetMenuEntryColor(int32 index) const;
	const FSoftObjectPath GetMenuEntryIconPath(int32 index) const;
	TWeakPtr<IMenuData> GetMenuEntryOptionData(int32 index);
	
	virtual void SetMenuEntryName(int32 index, FText name);
	virtual void SetMenuEntryColor(int32 index, FLinearColor enableColor, FLinearColor selectColor = FLinearColor::Transparent, FLinearColor disableColor = FLinearColor::Transparent);
	virtual void SetMenuEntryIconPath(int32 index, FSoftObjectPath path, FVector2D imgSize = FVector2D::ZeroVector, FLinearColor tintColor = FLinearColor::Transparent);
	virtual void SetMenuEntryOptionData(int32 index, TSharedPtr<IMenuData> data);
};
