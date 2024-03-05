#pragma once

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GsUIGuildSnsIconListEntry.generated.h"

class UGsButton;
class UGsUIGuildSnsIcon;

/** 기사단 SNS 아이콘 리스트 정보 */
UCLASS()
class UGsUIGuildSnsIconListItem final : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickIcon, const int32 /*InIndex*/)

public:
	int32 _index = 0;
	int32 _iconIndex = 0;
	bool _hasUrl;
	FOnClickIcon _onClickIcon;

public:
	static UGsUIGuildSnsIconListItem* CreateInstance(const FString& InSnsShortcutUrl);
};

/** 기사단 SNS 아이콘 리스트 렌더러 */
UCLASS()
class UGsUIGuildSnsIconListEntry final : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildSnsIcon* _icon;

protected:
	void NativeOnInitialized() final;

	void NativeOnListItemObjectSet(UObject* InListItemObject) final;

	void Invalidate(const UObject* InListItemObject);

protected:
	UFUNCTION()
	void OnClickIcon();
};
