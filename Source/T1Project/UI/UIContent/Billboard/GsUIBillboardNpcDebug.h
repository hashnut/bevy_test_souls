#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIBillboard.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsUIBillboardNpcDebug.generated.h"

class UImage;
class UWidgetSwitcher;
class UGsButton;
class UGsGameObjectBase;
class UGsUIIconBase;
class UVerticalBox;

//----------------------------------------
// Npc Info
//----------------------------------------
UCLASS()
class UGsUIBillboardNpcDebug : public UGsUIBillboard
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _npcInfoTextBlock;			

protected:
	//virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	void SetNpcInfoText(const FString& inNpcInfo);

};