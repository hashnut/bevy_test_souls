#pragma once

#include "Classes/GsManager.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/TextDecorator/GsRichTextBlockLinkDecorator.h"

struct FGsSchemaPassivitySet;

/**
 * Passivity 관련 매니저
 */
class T1PROJECT_API FGsPassivityManager final : public IGsManager
{
private:
	static const FString HYPERLINK_ID;
	static const FText HYPERLINK_DELIMETER;

public:
	void RegisterHyperLinkDelegate(URichTextBlock* InRichTextBlock, UObject* InOwner);
	void UnRegisterHyperLinkDelegate(URichTextBlock* InRichTextBlock, UObject* InOwner);

	// 링크 텍스트 생성
	FText MakeRichText(const FGsSchemaPassivitySet* passivitySet) const;
	FText GetSkillText(const FGsSchemaPassivitySet* passivitySet) const;
	bool IsSkillPassive(const FGsSchemaPassivitySet* passivitySet) const;

private:
	void OnClickHyperLinkId(const UGsRichTextBlockLinkDecorator::FGsOnClickParamId& InParamIdMap);
	void OnClickHyperLink(const UGsRichTextBlockLinkDecorator::FGsOnClickParamUObject& InParamUObjectMap);
};

#define GSPassivity() UGsScopeHolder::GetGameManagerFType<FGsPassivityManager>(UGsScopeGame::EManagerType::Passivity)