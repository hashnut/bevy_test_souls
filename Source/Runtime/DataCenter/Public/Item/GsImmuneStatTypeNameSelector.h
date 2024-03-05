#pragma once

#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "StructuredArchive.h"

class FArchive;
class FOutputDevice;
class UObject;

#include "GsImmuneStatTypeNameSelector.generated.h"

USTRUCT(BlueprintType)
struct DATACENTER_API FGsImmuneStatTypeNameSelector : public FGsNameSelectorBase
{
	GENERATED_BODY()

	DECLARE_GS_NAME_SELECTOR_STATICS(true)

	FGsImmuneStatTypeNameSelector();

	FGsImmuneStatTypeNameSelector(const StatType InStatType);

	explicit FGsImmuneStatTypeNameSelector(const FName& InName);

	/** Returns the Protocol this name represents */
	StatType GetImmuneStatType() const;

	//~ StatType operators
	operator StatType();

	operator const StatType() const;

	//~ Comparison operators
	bool operator==(const StatType& Other) const;

	bool operator!=(const StatType& Other) const;

	bool operator==(const FGsImmuneStatTypeNameSelector& Other) const;

	bool operator!=(const FGsImmuneStatTypeNameSelector& Other) const;

	bool ExportTextItem(FString& ValueStr, FGsImmuneStatTypeNameSelector const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const;

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText);

	/** IsValid member accessor */
	bool IsValid() const;

	/**
	 * Map 의 Key 로 사용하기 위해 해쉬 생성기 추가
	 */
	friend FORCEINLINE uint32 GetTypeHash(const FGsImmuneStatTypeNameSelector& StatTypeNameSelector)
	{
		return GetTypeHash(StatTypeNameSelector.GetImmuneStatType());
	}

};

template<>
struct TStructOpsTypeTraits<FGsImmuneStatTypeNameSelector> : public TStructOpsTypeTraitsBase2<FGsImmuneStatTypeNameSelector>
{
	enum
	{
		WithImportTextItem = true,
		WithExportTextItem = true
	};
};