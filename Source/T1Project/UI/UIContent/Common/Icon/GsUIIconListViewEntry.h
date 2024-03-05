#pragma once
#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUIIconListViewEntry.generated.h"

class UGsDynamicIconSlotHelper;
struct FGsSchemaRewardItemBagData;

UCLASS()
class T1PROJECT_API UGsUIIconListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
};