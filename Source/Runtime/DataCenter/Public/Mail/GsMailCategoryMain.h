// Engine\Plugins\VirtualProduction\DMX\DMXProtocol\Source\DMXProtocol\Public\DMXProtocolTypes.h 에서 복사해 옴
#pragma once

#include "../Plugins/RSQL/Source/Runtime/Public/GsNameSelectorBase.h"
#include "Shared/Client/SharedEnums/SharedMailEnum.h"
#include "StructuredArchive.h"

class FArchive;
class FOutputDevice;
class UObject;

#include "GsMailCategoryMain.generated.h"

USTRUCT(BlueprintType)
struct DATACENTER_API FGsMailCategoryMain : public FGsNameSelectorBase
{

	GENERATED_BODY()

	DECLARE_GS_NAME_SELECTOR_STATICS(true)

	FGsMailCategoryMain();

	FGsMailCategoryMain(const MailCategoryMain InCategoryMainType);

	explicit FGsMailCategoryMain(const FName& InName);

	/** Returns the Protocol this name represents */
	MailCategoryMain GetMailCategoryMain() const;

	//~ CategoryMainType operators
	operator MailCategoryMain();

	operator const MailCategoryMain() const;

	//~ Comparison operators
	bool operator==(const MailCategoryMain& Other) const;

	bool operator!=(const MailCategoryMain& Other) const;

	bool operator==(const FGsMailCategoryMain& Other) const;

	bool operator!=(const FGsMailCategoryMain& Other) const;

	bool ExportTextItem(FString& ValueStr, FGsMailCategoryMain const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const;

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText);

	/** IsValid member accessor */
	bool IsValid() const;

	/**
	 * Map 의 Key 로 사용하기 위해 해쉬 생성기 추가
	 */
	friend FORCEINLINE uint32 GetTypeHash(const FGsMailCategoryMain& CategoryMainTypeNameSelector)
	{
		return GetTypeHash(CategoryMainTypeNameSelector.GetMailCategoryMain());
	}

};

template<>
struct TStructOpsTypeTraits<FGsMailCategoryMain> : public TStructOpsTypeTraitsBase2<FGsMailCategoryMain>
{
	enum
	{
		WithImportTextItem = true,
		WithExportTextItem = true
	};
};

// 아래 LexXXXString 함수가 필수는 아닙니다. 다만, Unreal 에서의 일반적인 사용법에 맞춰 추가했습니다.
const FString LexToString(const MailCategoryMain InCategoryMainType);
void LexFromString(OUT MailCategoryMain& OutCategoryMainType, const TCHAR* InBuffer);