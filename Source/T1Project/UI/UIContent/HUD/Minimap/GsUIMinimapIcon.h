#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIMinimapIcon.generated.h"

class UImage;
UCLASS()
class T1PROJECT_API UGsUIMinimapIcon : public UUserWidget
{
	GENERATED_BODY()

		// 멤버 변수
public:
	// 아이콘 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUIMinimapIcon", meta = (BindWidget))
	UImage* _iconImage;
};
