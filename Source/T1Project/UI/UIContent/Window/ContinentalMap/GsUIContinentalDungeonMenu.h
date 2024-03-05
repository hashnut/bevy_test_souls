#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIContinentalDungeonMenu.generated.h"

// condinental dungeon menu

class UGsButton;

class UWidgetAnimation;

UCLASS()
class UGsUIContinentalDungeonMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	// button: auto move
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoMove;
	// button: teleport
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTeleport;
	// animation
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _aniMenu;

private:
	FString _spotName;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// logic function
public:
	UFUNCTION()
	void OnClickAutoMove();
	UFUNCTION()
	void OnClickTeleport();

	// set
public:
	void PlayAniMenu();
	void SetSpotName(FString In_name)
	{
		_spotName = In_name;
	}
};