#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GsUISkillDetailInfoRichEntry.generated.h"

class URichTextBlock;

/**
 * 스킬 상세정보 UI
 */
UCLASS()
class T1PROJECT_API UGsUISkillDetailInfoRichEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockTitle;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _textBlockDesc;

	void NativeConstruct() final;

public:
	void SetTitle(const FText& InText);
	void SetDesc(const FText& InText);
	URichTextBlock* GetDescRichTextBlock();
};
