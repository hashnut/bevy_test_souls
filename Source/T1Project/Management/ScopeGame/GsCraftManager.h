#pragma once

#include "Classes/GsManager.h"
#include "Item/Craft/GsCraftTab.h"
#include "Item/Craft/GsCraftTarget.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageSystem.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsCraftManager.generated.h"

class FGsCostPackage;
class FGsCostPackageMultiple;
struct CraftIdAmountPair;
struct FGsSchemaItemCraftMaterial;
struct IGsMessageParam;
struct ItemDBIdAmountPair;

/**
 *	제작 매니저
 */
UCLASS()
class T1PROJECT_API UGsCraftManager final : 
	public UObject,
	public IGsManager
{

	GENERATED_BODY()

private:
	static const int32 CRAFT_FAVORITE_MAX_COUNT;

private:
	/**
	 * 최초 인게임 진입 시에는 Limit Count 정보가 업데이트되어 있지 않으므로,
	 * 1회 업데이트가 필요한데, 이 플래그로 업데이트 여부를 체크함.
	 */
	bool _limitCountInitialized = false;

	/**
	 * 최초 인게임 진입 시에는 즐겨찾기 정보가 업데이트되어 있지 않으므로,
	 * 1회 업데이트가 필요한데, 이 플래그로 업데이트 여부를 체크함.
	 */
	bool _favoriteInitialized = false;

	CraftTabId _favoriteTabId = INVALID_CRAFT_TAB_ID;

	TMap<CraftTabId, FGsCraftTab> _allTabs;

	TMap<CraftId, FGsCraftTarget> _allCraftTargets;

	TArray<TPair<MessageContentCraft, FDelegateHandle>> _craftDelegates;

	MsgSystemHandleArray _systemDelegates;

	/************************************************************************/
	/* 인스턴싱																*/
	/************************************************************************/

public:
	void Initialize() final;

	void InitializeAfterResourceDownload() final;

	void Finalize() final;
    
private:
	void RegisterMessages();

	void UnegisterMessages();

	bool InitliazeData();

public:
	bool TryToNavigate(const CraftId InCraftId, OUT FText* OutFailMessage) const;

	/************************************************************************/
	/* craft tab															*/
	/************************************************************************/

public:
	/**
	 * 제작탭 조회
	 */
	const FGsCraftTab* GetTab(const CraftTabId InCraftTabId) const;

	const FGsCraftTab* GetDisplayableTab(const CraftTabId InCraftTabId) const;

	const FGsCraftTab* GetRootTab() const;

	const FGsCraftTab* GetFavoriteTab() const;

protected:
	/**
	 * 즐겨찾기 제작탭 조회
	 */
	FGsCraftTab* GetFavoriteTabInner();

	/**
	 * 제작탭 조회
	 */
	FGsCraftTab* GetTabInner(const CraftTabId InCraftTabId);

	/**
	 * 매개변수로 입력한 탭에서 자동 선택 가능한 리프 노드를 찾아서 반환.
	 */
	const FGsCraftTab* FindDefaultLeafTab(const FGsCraftTab* InCraftTab) const;

public:
	/** 제작 제한 수량이 설정된 모든 Tab 에 대해 제한 수량 갱신 요청 */
	void RequestCraftLimitCountAll() const;

	/** 특정 Tab Id 에 대해 제작 제한 수량 서버에게 갱신 요청 */
	void RequestCraftLimitCountForTabId(const CraftTabId InCraftTabId) const;
protected:

	/** 제작 제한 수량이 설정된 모든 Tab 에 대해 제한 수량 갱신 요청 */
	void RequestCraftLimitCount() const;

	/************************************************************************/
	/* craft target															*/
	/************************************************************************/

public:
	/** 제작대상 조회 */
	const FGsCraftTarget* GetCraftTarget(const CraftId InCraftTargetId) const;

	bool IsFavorite(const CraftId InCraftTargetId) const;

	bool CanRegisterToFavorite(const bool InUseAlert) const;

	void ResetFavoriteAll(const TArray<CraftId> InCraftIds);

	void RegisterFavorite(const CraftId InCraftTargetId);

	void UnregisterFavorite(const CraftId InCraftTargetId);

	/** 가능한 적절한 제작ID를 반환. */
	CraftId FindAvaialbleCraftId(const ItemId InItemId) const;

	/** 가능한 적절한 제작대상을 반환. */
	const FGsCraftTarget* FindAvaialbleCraftTarget(const ItemId InItemId) const;

private:
	void SetCraftTargetLimitCount(const CraftTabId InCraftCategoryId, const TArray<CraftIdAmountPair>& InCraftStatListServer, const TArray<CraftIdAmountPair>& InCraftStatListServerAccount);

public:
	/**
	 * 제작대상이 포함된 탭 목록 반환. 현재는 0번이 root, 1번이 메인탭, 2번이 서브탭 순서
	 */
	TArray<const FGsCraftTab*> MakeTabRouteFromRoot(const CraftId InCraftTargetId) const;

	bool CanDisplay(const CraftId InCraftId) const;

	bool CanCraftNow(const CraftId InCraftId, const ItemAmount InCraftCount, const bool InUseUiFeedback = false) const;

	/**
	 * 기본 유효성 체크 후, 제작 요청
	 */
	bool TryCraft(const CraftId InCraftTargetId, const FGsCostPackageMultiple& InCostPackageMultiple) const;

	/************************************************************************/
	/* event																*/
	/************************************************************************/

private:
	void NotifyCraftStatListWorldChanged(const IGsMessageParam* InParam);

	void AckCraftStatList(const IGsMessageParam* InParam);

	void TryChangeFavoriteCraftTarget(const IGsMessageParam* InParam);

	void OnOpenTab(const IGsMessageParam* InParam);

	void OnResetTimeContents(const IGsMessageParam* InParam);

};

#define GSCraft() UGsScopeHolder::GetGameManager<UGsCraftManager>(UGsScopeGame::EManagerType::Craft)
