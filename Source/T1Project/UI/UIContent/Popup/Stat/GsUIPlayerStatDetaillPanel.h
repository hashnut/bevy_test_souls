// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageUI.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedStatEnum.h"
#include "GsUIPlayerStatDetaillPanel.generated.h"

/**
 * 
 */
 
UCLASS()
class T1PROJECT_API UGsUIPlayerStatDetaillPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* _bookmarkStatCountTextBlock;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UListView* _statListView;

private:
	TWeakObjectPtr<class UGsGameObjectLocalPlayer> _localPlayer;

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UGsBookmarkStatListViewEntry> _entryTemplate;

private:	
	MsgUIHandleArray _uiMsgHandleArray;
	MsgGameObjHandleArray _gameObjMsgHandleArray;
	class FGsPlayerBookmarkStatController* _bookmarkStatControl;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void BindMessage();
	void UnbindMessage();
	void InitStatList();	
	void InvalidBookmarkStatCount();
	void CreateStatItems(const TArray<StatType>& inStatList, TArray<class UGsBookmarkStatListViewData*>& inStatEntryList);
	void AddLine(TArray<class UGsBookmarkStatListViewData*>& inStatEntryList);
	void InvalidWidget();

private:
	void OnInvalidWidget(const struct IGsMessageParam* inParam);	
};
