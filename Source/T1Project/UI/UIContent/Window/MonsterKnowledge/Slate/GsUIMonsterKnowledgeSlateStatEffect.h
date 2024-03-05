#pragma once

#include "CoreMinimal.h"
#include "../Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GsUIMonsterKnowledgeSlateStatEffect.generated.h"


class UGsUIVFX;

UCLASS()
class T1PROJECT_API UGsUIMonsterKnowledgeSlateStatEffect : public UUserWidget
{
	GENERATED_BODY()

		/************************************************************************/
		/* BP Bind                                                              */
		/************************************************************************/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _effectVFX;

protected:
	void NativeOnInitialized() override;
	void BeginDestroy() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	void Reset();
	void Finish();
public:
	void Show();
	void Hide();
};