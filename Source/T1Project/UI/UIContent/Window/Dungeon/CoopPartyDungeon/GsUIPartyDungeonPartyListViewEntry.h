#pragma once

/**
* file		GsUIPartyDungeonPartyListViewEntry.h
* @brief	Party Dungeon Party View List
* @author	PKT
* @date		2022/06/02
**/

#include "CoreMinimal.h"
#include "Dungeon/GsDungeonCommonData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsUIPartyDungeonPartyListViewEntry.generated.h"


/************************************************************************/
/* View List Entry Data                                                 */
/************************************************************************/
UCLASS()
class T1PROJECT_API UGsPartyDungeonPartyListViewEntryData : public UObject
{
	GENERATED_BODY()

protected:
	FGsPartyDungeonPartyStatusList _data;

public:
	TFunction<void(PartyId)> OnJoin;
	TFunction<void(PartyId)> OnIntrude;

public:
	void SetData(const FGsPartyDungeonPartyStatusList& InPartyStatus)
	{
		_data = InPartyStatus;
	}

	const FGsPartyDungeonPartyStatusList* GetData() { return &_data; }
};




/************************************************************************/
/* View List UI Entry                                                   */
/************************************************************************/
UCLASS()
class UGsUIPartyDungeonPartyListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:

	PartyId _partyId = INVALID_PARTY_ID;

protected:
	// 2022/05/31 PKT - 던전 난이도
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textDifficulty;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textMemberCount;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgLeaderWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textLeaderLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textLeaderName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgfirstMemberWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textfirstMemberLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgSecondMemberWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textSecondMemberLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgThirdemberWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textThirdMemberLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textInProgressState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherBtnState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnJoin;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnBreakInto;

	TFunction<void(PartyId)> OnJoin = nullptr;
	TFunction<void(PartyId)> OnIntrude = nullptr;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

	// 2022/06/08 PKT - Set Display
protected:
	void DisplayDifficulty(const struct FGsPartyDungeonPartyStatusList* InInfo);
	void DisplayLeader(const struct FGsPartyDungeonPartyStatusList* InInfo, TFunction<void(CreatureWeaponType InWeaponType, class UImage** OutImage)> InMakeWeaponImg = nullptr);
	void DisplayMember(const struct FGsPartyDungeonPartyStatusList* InInfo, TFunction<void(CreatureWeaponType InWeaponType, class UImage** OutImage)> InMakeWeaponImg = nullptr);
	void DisplayRoomCondition(const struct FGsPartyDungeonPartyStatusList* InInfo);

	// 2022/06/08 PKT - On Event
protected:
	// 2022/06/08 PKT - 참가 버튼
	UFUNCTION()
	void OnClickedJoin();

	// 2022/06/08 PKT - 난입 버튼
	UFUNCTION()
	void OnClickedBreakInto();
};