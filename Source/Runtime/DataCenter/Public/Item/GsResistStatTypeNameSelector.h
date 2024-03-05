#pragma once

#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "StructuredArchive.h"

class FArchive;
class FOutputDevice;
class UObject;

#include "GsResistStatTypeNameSelector.generated.h"

USTRUCT(BlueprintType)
struct DATACENTER_API FGsResistStatTypeNameSelector : public FGsNameSelectorBase
{
	GENERATED_BODY()

	DECLARE_GS_NAME_SELECTOR_STATICS(true)

	FGsResistStatTypeNameSelector();

	FGsResistStatTypeNameSelector(const StatType InStatType);

	explicit FGsResistStatTypeNameSelector(const FName& InName);

	/** Returns the Protocol this name represents */
	StatType GetResistStatType() const;

	//~ StatType operators
	operator StatType();

	operator const StatType() const;

	//~ Comparison operators
	bool operator==(const StatType& Other) const;

	bool operator!=(const StatType& Other) const;

	bool operator==(const FGsResistStatTypeNameSelector& Other) const;

	bool operator!=(const FGsResistStatTypeNameSelector& Other) const;

	bool ExportTextItem(FString& ValueStr, FGsResistStatTypeNameSelector const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const;

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText);

	/** IsValid member accessor */
	bool IsValid() const;

	/**
	 * Map 의 Key 로 사용하기 위해 해쉬 생성기 추가
	 */
	friend FORCEINLINE uint32 GetTypeHash(const FGsResistStatTypeNameSelector& StatTypeNameSelector)
	{
		return GetTypeHash(StatTypeNameSelector.GetResistStatType());
	}

};

template<>
struct TStructOpsTypeTraits<FGsResistStatTypeNameSelector> : public TStructOpsTypeTraitsBase2<FGsResistStatTypeNameSelector>
{
	enum
	{
		WithImportTextItem = true,
		WithExportTextItem = true
	};
};