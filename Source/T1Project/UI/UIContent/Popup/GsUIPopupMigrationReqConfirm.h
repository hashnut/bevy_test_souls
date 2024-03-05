#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateHandler.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "GsUIPopupMigrationReqConfirm.generated.h"

class UGsUICurrencySlot;

UCLASS()
class T1PROJECT_API UGsUIPopupMigrationReqConfirm : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	FText _textTitle;

	UPROPERTY(BlueprintReadOnly)
	FText _textRemainTime;

	UPROPERTY(BlueprintReadOnly)
	FText _textDesc;

	UPROPERTY(BlueprintReadOnly)
	FText _textMigrateCount;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _needCurrency;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _currentCurrency;

protected:
	TFunction<void()> _okCallback;

protected:
	UFUNCTION(BlueprintCallable)
	void OnClickConfirm();

	UFUNCTION(BlueprintCallable)
	void OnClickCancel();

	void OnInputCancel();
	void OnInputOk();

	virtual void NativeOnInitialized() override;

public:
	virtual void BeginDestroy() override;

public:
	struct InitParam
	{ 
		ServerMigrateType type;
	};

	void SetInit(InitParam* param, TFunction<void()> okCallback);
};

