#pragma once

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GsUIGuildSnsUrlListEntry.generated.h"

class UCommonTextBlock;
class UGsButton;
class UGsUIGuildSnsIcon;
class UTextBlock;
class UWidgetSwitcher;

/** 기사단 SNS URL 리스트 정보 */
UCLASS()
class UGsUIGuildSnsUrlListItem final : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClick, const int32 /*InIndex*/)

public:
	int32 _index = -1;
	int32 _iconIndex = -1;
	FString _url;
	bool _urlCheckCompleted = false;
	bool _enableButton = true;
	FOnClick _onClick;

public:
	static UGsUIGuildSnsUrlListItem* CreateInstance(const FString& InSnsUrl);
};

/** 기사단 SNS URL 리스트 렌더러 */
UCLASS()
class UGsUIGuildSnsUrlListEntry final : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildSnsIcon* _icon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCommonTextBlock* _linkTxt;

protected:
	void NativeOnInitialized() final;

	void NativeOnListItemObjectSet(UObject* InListItemObject) final;

	void Invalidate(const UObject* InListItemObject);

protected:
	UFUNCTION()
	void OnClick();
};
