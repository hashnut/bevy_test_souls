#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIRegionTouchMenu.generated.h"

class UGsButton;

class UWidgetAnimation;

UCLASS()
class UGsUIRegionTouchMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoMove;
	// animation
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _aniMenu;
private:

	FVector _movePos;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	// logic function
public:
	UFUNCTION()
	void OnClickAutoMove();

	// set
public:
	void SetData(FVector In_pos);
	

};