#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GsUIGuildSnsIcon.generated.h"

class UWidgetSwitcher;

/** 테이블에 지정되지 않은 경우, 출력할 아이콘 인덱스 */
#define GUILD_SNS_ICON_UNKNOWN_INDEX 0

/** 기사단 SNS 아이콘 */
UCLASS()
class UGsUIGuildSnsIcon final : public UUserWidget
{
	GENERATED_BODY()

public:
	struct Parameters final
	{
		int32 _iconIndex = 0;
		bool _isOn = false;
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher;

private:
	Parameters _parameters;

public:
	void SetParameter(const Parameters& InParameters);
};
