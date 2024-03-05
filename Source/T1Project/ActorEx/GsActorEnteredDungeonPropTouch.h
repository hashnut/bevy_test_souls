#pragma once

#include "CoreMinimal.h"
#include "ActorEx/GsActorPropTouch.h"
#include "../UI/UILib/Base/GsUIBillboard.h"
#include "GsActorEnteredDungeonPropTouch.generated.h"


UCLASS()
class T1PROJECT_API AGsActorEnteredDungeonPropTouch : public AGsActorPropTouch
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _nameplateWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsActorPropTouch", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _interactionWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _rootWidget;
		
public:
	AGsActorEnteredDungeonPropTouch();

	virtual void BeginPlay() override;
	virtual UGsUIBillboard* GetWidget() const override;

protected:
	class UGsUIBillboardEliteDungeonWarp* GetPlate() const;
};