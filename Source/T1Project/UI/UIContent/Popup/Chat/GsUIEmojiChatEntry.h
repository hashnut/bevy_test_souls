#pragma once
#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconDefault.h"
#include "GsUIEmojiChatEntry.generated.h"

class UGsButton;

DECLARE_DELEGATE_OneParam(FOnClickEmojiItem, int32);

UCLASS()
class T1PROJECT_API UGsUIEmojiChatEntry :
    public UGsUIIconDefault
{
	GENERATED_BODY()

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIEmojiButton", meta = (BindWidget, ShowOnlyInnerProperties))
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEmoji;

	int32 _emojiID = 0;

public:
	FOnClickEmojiItem OnClickSlot;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelected();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetEmojiID(int32 emojiID, FSoftObjectPath pathEmoticon);
};

