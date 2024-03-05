#pragma once
#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconDefault.h"
#include "GsUIIconRegionSpot.generated.h"

/*
	 아이콘:지역맵 스팟
*/

class UWidgetSwitcher;

UCLASS()
class T1PROJECT_API UGsUIIconRegionSpot : public UGsUIIconDefault
{
	GENERATED_BODY()

protected:

	// 지역 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textSpotName;

public:
	// 이름 세팅
	void SetSpotName(FText In_name);
};