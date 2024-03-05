#pragma once

#include "UI/UILib/Base/GsUITooltip.h"

#include "GsUIContentsEventTooltip.generated.h"

class URichTextBlock;
class UTextBlock;

UCLASS()
class UGsUIContentsEventTooltip final : public UGsUITooltip
{

public:
	struct Parameters final
	{

		const FText _title;
		const FText _desc;
		const FText _subDesc;

		Parameters(const FText& InTitle, const FText& InDesc, const FText& InSubDesc)
			: _title(InTitle)
			, _desc(InDesc)
			, _subDesc(InSubDesc)
		{
		}

	};

	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titleTxt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _descTxt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _subDescTxt;

public:
	void SetParameters(const Parameters& InParameters);

	void Open() final;

};
