#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateHandler.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIPopupMigrationUserInit.generated.h"

class URichTextBlock;

UCLASS()
class T1PROJECT_API UGsUIPopupMigrationUserInit : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _richTextNotice;

protected:
	TFunction<void()> _okCallback;

protected:
	UFUNCTION(BlueprintCallable)
	void OnClickConfirm();

	UFUNCTION(BlueprintCallable)
	void OnClickCancel();

	void OnInputCancel();
	void OnInputOk();

public:
	struct InitParam
	{
		int _characterCount{ 0 };
	};

	void SetInit(InitParam* param, TFunction<void()> okCallback);
};
