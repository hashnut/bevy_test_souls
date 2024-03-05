// Engine\Plugins\VirtualProduction\DMX\DMXProtocol\Source\DMXProtocol\Public\DMXProtocolTypes.h 에서 복사해 옴
#pragma once

#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "StructuredArchive.h"

class FArchive;
class FOutputDevice;
class UObject;

#include "GsStatTypeNameSelector.generated.h"

USTRUCT(BlueprintType)
struct DATACENTER_API FGsStatTypeNameSelector : public FGsNameSelectorBase
{

	GENERATED_BODY()

	DECLARE_GS_NAME_SELECTOR_STATICS(true)

	FGsStatTypeNameSelector();

	FGsStatTypeNameSelector(const StatType InStatType);

	explicit FGsStatTypeNameSelector(const FName& InName);

	/** Returns the Protocol this name represents */
	StatType GetStatType() const;

	//~ StatType operators
	operator StatType();

	operator const StatType() const;

	//~ Comparison operators
	bool operator==(const StatType& Other) const;

	bool operator!=(const StatType& Other) const;

	bool operator==(const FGsStatTypeNameSelector& Other) const;

	bool operator!=(const FGsStatTypeNameSelector& Other) const;

	bool ExportTextItem(FString& ValueStr, FGsStatTypeNameSelector const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const;

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText);

	/** IsValid member accessor */
	bool IsValid() const;

	/**
	 * Map 의 Key 로 사용하기 위해 해쉬 생성기 추가
	 */
	friend FORCEINLINE uint32 GetTypeHash(const FGsStatTypeNameSelector& StatTypeNameSelector)
	{
		return GetTypeHash(StatTypeNameSelector.GetStatType());
	}

};

template<>
struct TStructOpsTypeTraits<FGsStatTypeNameSelector> : public TStructOpsTypeTraitsBase2<FGsStatTypeNameSelector>
{
	enum
	{
		WithImportTextItem = true,
		WithExportTextItem = true
	};
};

// 아래 LexXXXString 함수가 필수는 아닙니다. 다만, Unreal 에서의 일반적인 사용법에 맞춰 추가했습니다.
const FString LexToString(const StatType InStatType);
void LexFromString(OUT StatType& OutStatType, const TCHAR* InBuffer);