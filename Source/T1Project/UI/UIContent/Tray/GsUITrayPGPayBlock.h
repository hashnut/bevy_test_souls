#pragma once

#include "UI/UILib/Base/GsUITray.h"
#include "GsUITrayPGPayBlock.generated.h"

UCLASS()
class UGsUITrayPGPayBlock : public UGsUITray
{
	GENERATED_BODY()

public:
	static const TCHAR* ASSET_NAME;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnOk;

protected:
	void NativeOnInitialized() final;

protected:
	UFUNCTION()
	void OnClickOk();
};
