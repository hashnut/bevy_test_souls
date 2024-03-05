// Engine\Plugins\VirtualProduction\DMX\DMXProtocol\Source\DMXProtocol\Public\DMXProtocolTypes.h 에서 복사해 옴
#pragma once

#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"
#include "Shared/Client/SharedEnums/SharedMailEnum.h"
#include "StructuredArchive.h"

class FArchive;
class FOutputDevice;
class UObject;

#include "GsMailCategorySub.generated.h"

USTRUCT(BlueprintType)
struct DATACENTER_API FGsMailCategorySub : public FGsNameSelectorBase
{

	GENERATED_BODY()

	DECLARE_GS_NAME_SELECTOR_STATICS(true)

	FGsMailCategorySub();

	FGsMailCategorySub(const MailCategorySub InCategorySubType);

	explicit FGsMailCategorySub(const FName& InName);

	/** Returns the Protocol this name represents */
	MailCategorySub GetMailCategorySub() const;

	//~ CategorySubType operators
	operator MailCategorySub();

	operator const MailCategorySub() const;

	//~ Comparison operators
	bool operator==(const MailCategorySub& Other) const;

	bool operator!=(const MailCategorySub& Other) const;

	bool operator==(const FGsMailCategorySub& Other) const;

	bool operator!=(const FGsMailCategorySub& Other) const;

	bool ExportTextItem(FString& ValueStr, FGsMailCategorySub const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const;

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText);

	/** IsValid member accessor */
	bool IsValid() const;

	/**
	 * Map 의 Key 로 사용하기 위해 해쉬 생성기 추가
	 */
	friend FORCEINLINE uint32 GetTypeHash(const FGsMailCategorySub& CategorySubTypeNameSelector)
	{
		return GetTypeHash(CategorySubTypeNameSelector.GetMailCategorySub());
	}

};

template<>
struct TStructOpsTypeTraits<FGsMailCategorySub> : public TStructOpsTypeTraitsBase2<FGsMailCategorySub>
{
	enum
	{
		WithImportTextItem = true,
		WithExportTextItem = true
	};
};

// 아래 LexXXXString 함수가 필수는 아닙니다. 다만, Unreal 에서의 일반적인 사용법에 맞춰 추가했습니다.
const FString LexToString(const MailCategorySub InCategorySubType);
void LexFromString(OUT MailCategorySub& OutCategorySubType, const TCHAR* InBuffer);