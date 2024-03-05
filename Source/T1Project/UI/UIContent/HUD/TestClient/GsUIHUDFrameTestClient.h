// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUDFrame.h"
#include "GsUIHUDFrameTestClient.generated.h"

/**
 * 메인 HUD UI 클래스
 */


UCLASS()
class T1PROJECT_API UGsUIHUDFrameTestClient : public UGsUIHUDFrame
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIHUDTestQuest* _questUI;


	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F5;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F6;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F7;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F8;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F9;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F10;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F11;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn_F12;


public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClick_F1();
	UFUNCTION()
	void OnClick_F2();
	UFUNCTION()
	void OnClick_F3();
	UFUNCTION()
	void OnClick_F4();
	UFUNCTION()
	void OnClick_F5();
	UFUNCTION()
	void OnClick_F6();
	UFUNCTION()
	void OnClick_F7();
	UFUNCTION()
	void OnClick_F8();
	UFUNCTION()
	void OnClick_F9();
	UFUNCTION()
	void OnClick_F10();
	UFUNCTION()
	void OnClick_F11();
	UFUNCTION()
	void OnClick_F12();

private:
	void KeyEventTrigger(int i);

public:
	void InvalidateAllMenu();

public:	
	class UGsUIHUDTestQuest* GetQuest() const { return _questUI; }
};
