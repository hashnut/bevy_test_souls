#pragma once

#include "ContentsEvent/GsContentsEventGroup.h"
#include "GsUIIconDefault.h"
#include "GsUIIconContentsEvent.generated.h"

class UGsButton;
class UGsUITooltip;

/**
 * 컨텐츠이벤트 아이콘
 */
UCLASS()
class UGsUIIconContentsEvent final : public UGsUIIconDefault
{

	GENERATED_BODY()

public:
	struct Parameters final
	{
		const FSoftObjectPath _iconPath;
		const FText _title;
		const FText _desc;
		const FText _subDesc;

		Parameters(const FGsContentsEventGroup& InContentsEventGroup)
			: _iconPath(InContentsEventGroup.GetIcon())
			, _title(InContentsEventGroup.GetTitle())
			, _desc(InContentsEventGroup.GetDescription())
			, _subDesc(InContentsEventGroup.GetAltDescription())
		{
		}
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UGsButton* _button;

public:
	FSimpleDelegate OnClickIcon;

protected:
	void NativeOnInitialized() final;
	void NativeConstruct() final;
	void NativeDestruct() final;

public:
	void SetParameters(const UGsUIIconContentsEvent::Parameters& InParameters);

protected:
	UFUNCTION()
	void OnClickedButton();

};
