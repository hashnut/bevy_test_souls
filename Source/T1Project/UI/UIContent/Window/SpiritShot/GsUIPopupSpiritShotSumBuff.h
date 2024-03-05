// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "SpiritShot/GsSpiritShotPassivityCollection.h"

#include "GsUIPopupSpiritShotSumBuff.generated.h"

class UListView;
class UGsButton;
class UCanvasPanel;

class FGsSpiritShotPassivityCollection;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupSpiritShotSumBuff : public UGsUIPopup
{
	GENERATED_BODY()
	
public:
	struct Parameters
	{
		FGsSpiritShotPassivityCollection plusPassivityCollection;
		TArray<TPair<Level, PassivityId>> levelPassivityIdPairList;
	};


	/************************************************************************/
	/* 위젯                                                                 */
	/************************************************************************/
protected:
	/** 좌측의 '천마석 추가 효과' 가 비어 있을 경우, 예외처리를 위한 WidgetSwitcher */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panelNoPlusStatSum;

	/** 좌측의 '천마석 추가 효과' 목록을 띄워줄 리스트뷰 (각 카테고리별 레벨 합산) */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _listViewPlusStatSum;

	/** 우측의 '잠식 레벨 효과' 목록을 띄워줄 리스트뷰 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _listViewLevelStatSum;

	/** 닫기 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClose;
	
private:
	/** 파싱에 사용할 Parameter */
	Parameters _parameters;


	/************************************************************************/
	/* Overrides                                                            */
	/************************************************************************/
protected:
	virtual void NativeOnInitialized() final;


	/************************************************************************/
	/* Setter                                                               */
	/************************************************************************/
public:
	/** Parameters 를 받아서, 각 ListView 에 알맞은 데이터를 파싱하여 전달할 것이다 */
	void SetParameters(Parameters&& InParameters);


	/************************************************************************/
	/* 이벤트                                                               */
	/************************************************************************/
protected:
	/** 닫기 버튼 클릭 */
	UFUNCTION()
	void OnClickClose();

	/** 키보드 입력으로 닫는 기능 구현을 위함 */
	virtual void OnInputCancel() final;
};
