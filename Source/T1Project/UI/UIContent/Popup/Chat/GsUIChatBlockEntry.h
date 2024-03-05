#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIChatBlockEntry.generated.h"

class UTextBlock;
class UGsButton;

struct FGsChatBlockUser;

UCLASS()
class T1PROJECT_API UGsUIChatBlockEntry :
	public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
public:
	DECLARE_DELEGATE_OneParam(FOnSelected, UserDBId);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _blockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUnblock;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickUnblock();

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetData(FGsChatBlockUser* data);
	void RegreshUI();

	FOnSelected OnReqUnblock;

protected:
	UserDBId _uid;
	FString  _name;

};