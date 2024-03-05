#include "GsRichTextBlockLinkDecorator.h"

#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/Json/Public/Serialization/JsonSerializerMacros.h"
#include "Runtime/Slate/Public/Widgets/Input/SRichTextHyperlink.h"
#include "Runtime/Slate/Public/Widgets/Input/SRichTextHyperlink.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "T1Project.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

//----------------------------------------------------------------------------------------------------------------------
FGsRichTextLinkDecorator::FGsRichTextLinkDecorator(URichTextBlock* InOwner, 
	const FSlateHyperlinkRun::FOnClick& InClickDelegate, UGsRichTextBlockLinkDecorator* InDecorator)
{
	_hyperLinkStyle = InDecorator->_style;
	_clickDelegate = InClickDelegate;
}

bool FGsRichTextLinkDecorator::Supports(const FTextRunParseResults& InRunParseResult, const FString& InText) const
{
	if (InRunParseResult.Name != TEXT("a"))
	{
		return false;
	}

	const FTextRange* const idRange = InRunParseResult.MetaData.Find(TEXT("id"));
	if (nullptr == idRange)
	{
		return false;
	}

	return true;
}

TSharedRef<ISlateRun> FGsRichTextLinkDecorator::Create(const TSharedRef<class FTextLayout>& InTextLayout,
	const FTextRunParseResults& InRunParseResult, const FString& InOriginalText,
	const TSharedRef< FString >& OutModelText, const ISlateStyle* InStyle)
{
	FString textStyleName = TEXT("Default");
	const FTextRange* const textStyleRange = InRunParseResult.MetaData.Find(TEXT("textstyle"));
	if (nullptr != textStyleRange)
	{
		const FString textStyleNameConst = InOriginalText.Mid(textStyleRange->BeginIndex, 
			textStyleRange->EndIndex - textStyleRange->BeginIndex);
		textStyleName = *textStyleNameConst;
	}

	FTextRange modelRange;
	modelRange.BeginIndex = OutModelText->Len();
	*OutModelText += InOriginalText.Mid(InRunParseResult.ContentRange.BeginIndex, 
		InRunParseResult.ContentRange.EndIndex - InRunParseResult.ContentRange.BeginIndex);
	modelRange.EndIndex = OutModelText->Len();

	FRunInfo runInfo(InRunParseResult.Name);
	for (const TPair<FString, FTextRange>& Pair : InRunParseResult.MetaData)
	{
		runInfo.MetaData.Add(Pair.Key, InOriginalText.Mid(Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
	}

	if (!textStyleName.IsEmpty() && InStyle->HasWidgetStyle<FTextBlockStyle>(FName(*textStyleName)))
	{
		_hyperLinkStyle.SetTextStyle(InStyle->GetWidgetStyle<FTextBlockStyle>(FName(*textStyleName)));
	}

	return FSlateHyperlinkRun::Create(runInfo, OutModelText, _hyperLinkStyle, 
		_clickDelegate, nullptr, nullptr, modelRange);
}

const FString UGsRichTextBlockLinkDecorator::JSON_PARAM_KEY = TEXT("p");

//----------------------------------------------------------------------------------------------------------------------
TSharedPtr<ITextDecorator> UGsRichTextBlockLinkDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FGsRichTextLinkDecorator(InOwner, 
		FSlateHyperlinkRun::FOnClick::CreateUObject(this, &UGsRichTextBlockLinkDecorator::OnClickLink), this));
}

void UGsRichTextBlockLinkDecorator::AddUniqueOnClickDelegate(const FString& InId, FGsOnClickRichTextBlockLinkDecorator InDelegate)
{
	_linkDelegateMap.Remove(InId);
	_linkDelegateMap.Emplace(InId, InDelegate);
}

void UGsRichTextBlockLinkDecorator::AddUniqueOnClickDelegateId(uint32 InUniqueId, FGsOnClickRichTextBlockLinkDecoratorId InDelegate)
{
	_linkDelegateUniqueIdMap.Remove(InUniqueId);
	_linkDelegateUniqueIdMap.Emplace(InUniqueId, InDelegate);
}

void UGsRichTextBlockLinkDecorator::AddUniqueOnClickDelegateUObject(UObject* InOwner, FGsOnClickRichTextBlockLinkDecoratorUObject InDelegate)
{
	//_linkDelegateUObjectList.Remove(InOwner);
	_linkDelegateUObjectList.Add(MakeTuple(InOwner, InDelegate));
}

void UGsRichTextBlockLinkDecorator::OnClickLink(const FSlateHyperlinkRun::FMetadata& InMetadata)
{
	const FString* const parsedId = InMetadata.Find(TEXT("id"));
	if (nullptr == parsedId)
	{
		return;
	}

#if WITH_EDITOR
	GSLOG(Log, TEXT("click link id: %s"), **parsedId);
#endif

	FGsOnClickRichTextBlockLinkDecorator* OnClickHyperLink = _linkDelegateMap.Find(*parsedId);
	if (nullptr != OnClickHyperLink)
	{
		const FString* JsonParam = InMetadata.Find(UGsRichTextBlockLinkDecorator::JSON_PARAM_KEY);
		if (nullptr != JsonParam)
		{
			GSLOG(Log, TEXT("OnClickLink JsonParam: %s"), **JsonParam);
			OnClickHyperLink->ExecuteIfBound(UGsRichTextBlockLinkDecorator::MakeParamMap(*JsonParam));
		}
		else
		{
			OnClickHyperLink->ExecuteIfBound(FGsOnClickParamMap());
		}
		
		return;
	}

	FName id = FName(**parsedId);
	if (TEXT("url") == id)
	{
		ProcessURL(InMetadata);
		return;
	}
	else if (TEXT("openui") == id)
	{
		ProcessOpenUI(InMetadata);
		return;
	}

	// 1번째 방법 - 메시징
	for (const TPair<uint32, FGsOnClickRichTextBlockLinkDecoratorId>& pair : _linkDelegateUniqueIdMap)
	{
		const FString* JsonParam = InMetadata.Find(UGsRichTextBlockLinkDecorator::JSON_PARAM_KEY);
		if (nullptr != JsonParam)
		{
#if WITH_EDITOR
			GSLOG(Log, TEXT("OnClickLink JsonParam: %s"), **JsonParam);
#endif
			pair.Value.ExecuteIfBound(MakeTuple(pair.Key, UGsRichTextBlockLinkDecorator::MakeParamMap(*JsonParam)));
		}
		else
		{
			pair.Value.ExecuteIfBound(MakeTuple(pair.Key, FGsOnClickParamMap()));
		}

	}
	// 2번째 방법
	// UObject 와 함께 결합된 녀석에 대한 처리 -> 최적화 필요
//	for (const TPair<UObject*, FGsOnClickRichTextBlockLinkDecoratorUObject>& pair : _linkDelegateUObjectList)
//	{
//		if (nullptr != pair.Key)
//		{
//			const FString* JsonParam = InMetadata.Find(UGsRichTextBlockLinkDecorator::JSON_PARAM_KEY);
//			if (nullptr != JsonParam)
//			{
//#if WITH_EDITOR
//				GSLOG(Log, TEXT("OnClickLink JsonParam: %s"), **JsonParam);
//#endif
//				pair.Value.ExecuteIfBound(MakeTuple(pair.Key, UGsRichTextBlockLinkDecorator::MakeParamMap(*JsonParam)));
//			}
//			else
//			{
//				pair.Value.ExecuteIfBound(MakeTuple(pair.Key, FGsOnClickParamMap()));
//			}
//		}
//	}
}

void UGsRichTextBlockLinkDecorator::ProcessURL(const FSlateHyperlinkRun::FMetadata& InMetadata)
{
	const FString* href = InMetadata.Find(TEXT("href"));
	if (href)
	{
		FPlatformProcess::LaunchURL(**href, nullptr, nullptr);
	}
}

void UGsRichTextBlockLinkDecorator::ProcessOpenUI(const FSlateHyperlinkRun::FMetadata& InMetadata)
{
	const FString* name = InMetadata.Find(TEXT("name"));
	if (nullptr == name)
	{
		return;
	}

	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(FName(**name));

	// 추가 파라미터 정보가 있으면 넘긴다.
	const FString* param = InMetadata.Find(TEXT("param"));
	if (param)
	{
		if (widget.IsValid())
		{
			widget->OnHyperlink(*param);
		}
	}
}

FJsonSerializableKeyValueMap UGsRichTextBlockLinkDecorator::MakeParamMap(const FString& InJson)
{
	FGsHyperLinkParamMapJsonSerializable ParamMap;

	if (!ParamMap.FromJson(InJson.Replace(TEXT("\'"), TEXT("\""))))
	{
		GSLOG(Warning, TEXT("!ParamMap.FromJson('%s')"), *InJson);
	}

	return MoveTemp(ParamMap._map);
}

FText UGsRichTextBlockLinkDecorator::MakeLinkText(const FString& InId, const FText& InTitle, const TMap<FString, FString>& InParamMap)
{
	// HACK: 이유를 알 수 없으나, RichTextBlock 에 단 하나의 하이퍼링크만 있을 경우, 마우스포인터는 핸드로 바뀌는데, 클릭 이벤트가 발생하지 않음.
	// 일반 텍스트를 하나만 넣으면 정상 동작하므로, 문자열 맨 끝에 스페이스를 하나 추가함.

	static const FText FORMAT_TEXT = FText::FromString(TEXT("<a textstyle=\"link\" id=\"{0}\" {1}=\"{2}\">{3}</> "));
	static const FText JSON_PARAM_KEY_TEXT = FText::FromString(UGsRichTextBlockLinkDecorator::JSON_PARAM_KEY);

	const FText IdText = FText::FromString(InId);
	const FString JsonStr = FGsHyperLinkParamMapJsonSerializable(InParamMap).ToJson(false).Replace(TEXT("\""), TEXT("\'"));

	return FText::Format(FORMAT_TEXT, IdText, JSON_PARAM_KEY_TEXT, FText::FromString(JsonStr), InTitle);
}

FText UGsRichTextBlockLinkDecorator::MakeCustomLinkText(const FText& InCustomText, const FString& InId, const FText& InTitle, const TMap<FString, FString>& InParamMap)
{
	static const FText FORMAT_TEXT_CUSTOM = FText::FromString(TEXT("<a textstyle=\"{0}\" id=\"{1}\" {2}=\"{3}\">{4}</> "));
	static const FText JSON_PARAM_KEY_TEXT_CUSTOM = FText::FromString(UGsRichTextBlockLinkDecorator::JSON_PARAM_KEY);

	const FText IdText = FText::FromString(InId);
	const FString JsonStr = FGsHyperLinkParamMapJsonSerializable(InParamMap).ToJson(false).Replace(TEXT("\""), TEXT("\'"));

	return FText::Format(FORMAT_TEXT_CUSTOM, InCustomText, IdText, JSON_PARAM_KEY_TEXT_CUSTOM, FText::FromString(JsonStr), InTitle);
}

void UGsRichTextBlockLinkDecorator::RemoveOnClickDelegateUObject(UObject* InOwner)
{
	_linkDelegateUObjectList.RemoveAll([&InOwner](const auto& elem)
		{
			return elem.Key == InOwner;
		});
}

void UGsRichTextBlockLinkDecorator::RemoveOnClickDelegate(uint32 InUniqueId)
{
	_linkDelegateUniqueIdMap.Remove(InUniqueId);
}