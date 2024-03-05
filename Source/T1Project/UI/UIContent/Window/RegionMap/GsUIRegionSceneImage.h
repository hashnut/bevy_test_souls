#pragma once

#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "SlateCore/Public/Layout/Geometry.h"
#include "SlateCore/Public/Input/Events.h"
#include "SlateCore/Public/Input/Reply.h"

#include "GsUIRegionSceneImage.generated.h"

class UGsWidgetPoolManager;

class UWidget;
class UCanvasPanel;

UCLASS()
class T1PROJECT_API UGsUIRegionSceneImage : public UUserWidget
{
	GENERATED_BODY()
protected:

	// 씬 이미지 로드 해서 붙일곳
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _sceneImageRoot;

	// 로드한 지도 이미지
	UPROPERTY()
	UUserWidget* _loadedSceneImage;

	// 씬 이미지 풀매니저
	TWeakObjectPtr<UGsWidgetPoolManager> _sceneImagePoolManager;

private:
	TFunction<void(FVector)> _callbackClickImage;
	TFunction<void()> _callbackClickCantMoveZone;
public:
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	// 씬 이미지 로드
	void LoadSceneImage();

	// 이미지 해제
	void ReleaseImage(UUserWidget* In_image);

	// 맵 이미지 위젯 만들기
	UUserWidget* MakeSceneImageWidget(const FSoftObjectPath& In_path);
	UUserWidget* Claim(UClass* InClass);

	FVector Get3DPos(FVector2D In_2dPos);


	void ClickImage(const FVector2D In_touchPos);
		
	// set
public:
	void SetCallbackClickImage(TFunction<void(FVector)> In_func)
	{
		_callbackClickImage = In_func;
	}
	void SetCallbackClickCantMoveZone(TFunction<void()> In_func)
	{
		_callbackClickCantMoveZone = In_func;
	}
};