#pragma once
#include "CoreMinimal.h"
#include "Costume/GsCostumeCollection.h"
#include "GsUIListCostumeCollectionData.generated.h"

/**
 * 코스춤 콜렉션 리스트 아이템 데이터
 */

UCLASS()
class T1PROJECT_API UGsUIListCostumeCollectionData :
    public UObject
{
    GENERATED_BODY()

private:
    TWeakPtr<FGsCostumeCollection> _data;
    CreatureGenderType _genderType { CreatureGenderType::ALL };

public:
    void SetItemData(TWeakPtr<FGsCostumeCollection> data, 
        bool useGender = true, CreatureGenderType genderType = CreatureGenderType::ALL) 
    { 
        _data = data; 
        _genderType = useGender ? genderType : CreatureGenderType::ALL;
    }
    TWeakPtr<FGsCostumeCollection> GetData() { return _data; }
    CreatureGenderType GetGenderType() { return _genderType;  }
};
