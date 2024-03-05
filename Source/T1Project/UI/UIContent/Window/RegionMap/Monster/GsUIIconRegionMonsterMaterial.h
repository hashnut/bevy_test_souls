#pragma once
#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconDefault.h"
#include "GsUIIconRegionMonsterMaterial.generated.h"

/*
	 아이콘:지역맵 몬스터 재질
*/
class UGsButton;

UCLASS()
class T1PROJECT_API UGsUIIconRegionMonsterMaterial : public UGsUIIconDefault
{
	GENERATED_BODY()
protected:
	// 툴팁 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;

	// get
public:
	UGsButton* GetTooltipButton()
	{
		return _btnTooltip;
	}
};