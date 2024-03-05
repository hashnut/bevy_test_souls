#pragma once

#include "CoreMinimal.h"

#include "Runtime/Core/Public/Delegates/DelegateCombinations.h"
#include "Runtime/Json/Public/Serialization/JsonSerializerMacros.h"
#include "Runtime/Slate/Public/Framework/Text/ITextDecorator.h"
#include "Runtime/Slate/Public/Framework/Text/SlateHyperlinkRun.h"
#include "Runtime/SlateCore/Public/Styling/SlateTypes.h"
#include "Runtime/UMG/Public/Components/RichTextBlockDecorator.h"

#include "GsRichTextBlockLinkDecorator.generated.h"

/**
 * FHyperlinkDecorator, FTutorialHyperlinkDecorator 참고하여 수정
 */
class FGsRichTextLinkDecorator : public ITextDecorator
{
protected:
	FHyperlinkStyle _hyperLinkStyle;
	FSlateHyperlinkRun::FOnClick _clickDelegate;

public:
	explicit FGsRichTextLinkDecorator(URichTextBlock* InOwner, const FSlateHyperlinkRun::FOnClick& InClickDelegate, 
		UGsRichTextBlockLinkDecorator* InDecorator);

public:
	virtual bool Supports(const FTextRunParseResults& InRunParseResult, const FString& InText) const override;

	virtual TSharedRef< ISlateRun > Create(const TSharedRef<class FTextLayout>& InTextLayout, 
		const FTextRunParseResults& InRunParseResult, const FString& InOriginalText, 
		const TSharedRef< FString >& OutModelText, const ISlateStyle* InStyle) override;
};

//----------------------------------------------------------------------------------------------------------------------


/**
 * 하이퍼 링크를 생성하는 데코레이터. type, data 메타정보를 정의하여 사용.
 * <a textstyle="Sample" id="url" href="http://www.google.com">구글열기</>
 * <a textstyle="Sample" id="openui" name="openUI", param="필요시 사용">팝업띄우기</>
 * textstyle 없을 경우 Default 행 사용.
 * 참고링크: https://answers.unrealengine.com/questions/920385/umg-richtextblock-hyperlink-href-markup.html 
 */
UCLASS(Abstract, Blueprintable)
class T1PROJECT_API UGsRichTextBlockLinkDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	using FGsOnClickParamMap = TMap<FString, FString>;
	DECLARE_DELEGATE_OneParam(FGsOnClickRichTextBlockLinkDecorator, const FGsOnClickParamMap&);

	// UObject 와 인자를 같이 전달하기 위한 타입
	using FGsOnClickParamUObject = TPair<UObject*, FGsOnClickParamMap>;
	DECLARE_DELEGATE_OneParam(FGsOnClickRichTextBlockLinkDecoratorUObject, const FGsOnClickParamUObject&);

	// UObject 대신 UniqueId 를 전달하기 위한 타입
	using FGsOnClickParamId = TPair<uint32, FGsOnClickParamMap>;
	DECLARE_DELEGATE_OneParam(FGsOnClickRichTextBlockLinkDecoratorId, const FGsOnClickParamId&);

private:
	struct FGsHyperLinkParamMapJsonSerializable final : public FJsonSerializable
	{

	public:
		FJsonSerializableKeyValueMap _map;

	public:
		FGsHyperLinkParamMapJsonSerializable() = default;
		FGsHyperLinkParamMapJsonSerializable(FJsonSerializableKeyValueMap InMap) :
			_map(MoveTemp(InMap))
		{
		}

	public:
		BEGIN_JSON_SERIALIZER
			JSON_SERIALIZE_MAP("_", _map);
		END_JSON_SERIALIZER

	};

private:
	static const FString JSON_PARAM_KEY;

public:
	UPROPERTY(EditAnywhere, Category = Appearance)
	FHyperlinkStyle _style;

private:
	TMap<FString, FGsOnClickRichTextBlockLinkDecorator> _linkDelegateMap;
	TArray<TPair<UObject*, FGsOnClickRichTextBlockLinkDecoratorUObject>> _linkDelegateUObjectList;
	TMap<uint32, FGsOnClickRichTextBlockLinkDecoratorId> _linkDelegateUniqueIdMap;

public:
	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
	void AddUniqueOnClickDelegate(const FString& InId, FGsOnClickRichTextBlockLinkDecorator InDelegate);
	void AddUniqueOnClickDelegateUObject(UObject* InOwner, FGsOnClickRichTextBlockLinkDecoratorUObject InDelegate);
	void AddUniqueOnClickDelegateId(uint32 InUniqueId, FGsOnClickRichTextBlockLinkDecoratorId InDelegate);

protected:
	void OnClickLink(const FSlateHyperlinkRun::FMetadata& InMetadata);

	void ProcessURL(const FSlateHyperlinkRun::FMetadata& InMetadata);
	void ProcessOpenUI(const FSlateHyperlinkRun::FMetadata& InMetadata);

protected:
	static FJsonSerializableKeyValueMap MakeParamMap(const FString& InJson);

public:
	static FText MakeLinkText(const FString& InId, const FText& InTitle, const TMap<FString, FString>& InParamMap);
	static FText MakeCustomLinkText(const FText& InCustomText, const FString& InId, const FText& InTitle, const TMap<FString, FString>& InParamMap);

public:
	void RemoveOnClickDelegateUObject(UObject* InOwner);
	void RemoveOnClickDelegate(uint32 InUniqueId);
};