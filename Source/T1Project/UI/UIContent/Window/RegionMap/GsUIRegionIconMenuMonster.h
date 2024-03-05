// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIRegionIconMenuMonster.generated.h"

class UGsButton;
class UWidgetAnimation;

UCLASS()
class UGsUIRegionIconMenuMonster : public UUserWidget
{
	GENERATED_BODY()

// [B1] | ejrrb10 | 몬스터 드랍 아이콘을 클릭했을 경우, 메뉴 아이콘 대신 바로 워프 팝업을 띄움
// 
//protected:
//	// button: auto move
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsButton* _btnAutoMove;
//	// button: teleport
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsButton* _btnTeleport;
//	// animation
//	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
//	UWidgetAnimation* _aniMenu;
//
//private:
//	int _mapId;
//
//protected:
//	virtual void NativeOnInitialized() override;
//	virtual void NativeConstruct() override;
//	virtual void NativeDestruct() override;
//
//	// logic function
//public:
//	UFUNCTION()
//		void OnClickAutoMove();
//	UFUNCTION()
//		void OnClickTeleport();
//
//	// set
//public:
//	void SetData(int In_index);

};