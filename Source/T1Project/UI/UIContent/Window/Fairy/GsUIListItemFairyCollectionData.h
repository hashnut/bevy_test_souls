#pragma once
#include "CoreMinimal.h"
#include "Fairy/GsFairyCollection.h"
#include "GsUIListItemFairyCollectionData.generated.h"

/**
 * 페어리 콜렉션 리스트 아이템 데이터
 */

UCLASS()
class T1PROJECT_API UGsUIListItemFairyCollectionData :
    public UObject
{
    GENERATED_BODY()

private:
    TWeakPtr<FGsFairyCollection> _data;

public:
    void SetItemData(TWeakPtr<FGsFairyCollection> data) { _data = data; }
    TWeakPtr<FGsFairyCollection> GetData() { return _data; }
};

