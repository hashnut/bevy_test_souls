// Copyright/2019/04/19 - BSAM
// Main Frame Class in Skill Editor
// Skill Editor main window widget creation and editor tabs, basic layout design and setting of function viewers
// The role of the Manager in the Skill Editor (I did not create it because there are not many roles yet.

///////////////////////////////////////////////////////////////////////////////////////
// BSAM - I decided to create a new test with a lot of tests (I have a big limit on what I get from Unreal.
//        I can only get a part of it according to my situation and there are many.It gets too complicated.)
///////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EngineMinimal.h"
#include "Widgets/Views/STreeView.h"
#include "EditorUndoClient.h"
#include "Animation/EditorCompositeSection.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "GameObject/Define/GsGameObjectDefine.h"


struct FSkillTreeItem;
struct FSkillAssetViewItem;
struct FGsSkillSetData;

class SMontageEditor;
class SSkillAssetListView;
class IPersonaToolkit;
class UAnimationAsset;
class ISkeletonTree;
class UEditorCompositeSection;
class USkillEditorDataAsset;
class USkillEditorSetData;

class UGsGlobalConstant;
class UCurveFloat;

class FSkillEditor : public TSharedFromThis<FSkillEditor>, public FEditorUndoClient, public FTickableEditorObject
{
private:
	static const FName SkillSetTabID;
	static const FName SkillNotifyTabID;
	static const FName MontageSectionTabID;

	TSharedPtr<SWindow> RootWindow;
	TSharedPtr<FTabManager::FLayout> SkillEditorLayout;
	TSharedPtr<FTabManager> TabManager;

	// Skill Editor Data Path Browser
	TSharedPtr<STreeView<TSharedPtr<FSkillTreeItem>>> TreeViewPtr;
	TArray< TSharedPtr<FSkillTreeItem> > TreeRootItems;
	//TArray< TSharedPtr<FSkillTreeItem> > SearchTreeItems;

	// Skill Editor Data Asset Browser
	TSharedPtr<SBorder> ViewContainer;
	TSharedPtr<class SSkillAssetListView> ListView;
	TArray<TSharedPtr<FSkillAssetViewItem>> FilteredAssetItems;
	TArray<FAssetData> AssetItems;
	TSharedPtr<SDockTab> SkillEditorDataPropertyTab;
	TSharedPtr<class IDetailsView> DataActorComponentView;
	//FOnAssetsActivated OnAssetsActivated;
	//ESelectionMode::Type SelectionMode;
	//TSharedPtr<class SSkillAssetColumnView> ColumnView;

	// Preview Gadget, Preview Port, Montage, Montage Detail Tab
	TSharedPtr<class IPersonaToolkit> PersonaToolkit;
	//TSharedPtr<class IPersonaViewport> Viewport;
	TSharedPtr<SDockTab> SkillNotifyTab;
	TSharedPtr<SDockTab> SkillSetTab;
	TSharedPtr<SDockTab> MontageSectionTab;
	TSharedPtr<SDockTab> PreviewSceneTab;
	TSharedPtr<SDockTab> AnimationAssetTab;
	TSharedPtr<SDockTab> MontageTab;
	TSharedPtr<SDockTab> SkillEditorViewportTab;
	TSharedPtr<class SSkillEditorViewport> Viewport;
	TSharedPtr<class ISkeletonTree> SkeletonTree;
	FSimpleMulticastDelegate OnPostUndo;
	FSimpleMulticastDelegate OnSectionsChanged;	
	TSharedPtr<class IDetailsView> NoitfyDetailsView;
	TSharedPtr<class IDetailsView> SkillSetDetailsView;
	TSharedPtr<class IDetailsView> OriginamAnimationAssetView;

	UAnimationAsset* AnimationAsset;
	UAnimMontage* AnimMontage;
	TSharedPtr<FSkillAssetViewItem> SelectedSkillAssetViewItemItem;
	TSharedPtr<class SMontageEditor> MontageEditor;
	//SMontageEditor* MontageEditor;

	USkillEditorDataAsset* SkillEditorDataAsset;
	USkillEditorSetData* SkillEditorSetData;
	USkillEditorDataAsset *TempSkillEditorDataAsset;

	FName TempPackageName;
	FName CurrentPackageName;

	FAssetData TempFAssetData;
	TSharedPtr<FSkillAssetViewItem> TempSkillAssetViewItem;

	FAssetData CurrentFAssetData;
	TSharedPtr<FSkillAssetViewItem> CurrentSkillAssetViewItem;

	UObject* SelectedObj = nullptr;
	USkillMetaData* SelectedSkillSet = nullptr;

	TSharedPtr<FUICommandList> ToolkitCommands;

	TSharedPtr<SEditableTextBox> EditableAssetName;

	FString SearchString;

	TArray<AActor*> SpawnActors;

	UGsGlobalConstant* GlobalConstant = nullptr;
	UCurveFloat* CurveFloat = nullptr;

public:
	FSkillEditor();
	~FSkillEditor();

public:
	static TSharedRef<FSkillEditor> MakeInstance()
	{
		return MakeShareable(new FSkillEditor);
	}	

private:
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }

private:
	void SpawnActorCollisionUpdate();
	void AddSkillCollisionComponent();
	void CollisionInfoPrint();
	// 이번에 신규 추가하는 작업들
	FReply KeyDownEventHandler(const FKeyEvent& InKeyEvent);
	FReply OnSaveSkillEditorDataAsset();
	void SetSkillEditorDataAsset();	
	void RefreshSkillDataAssetTab();	
	void RefreshSkillEditorTab();
	void RefreshMontageTab();
	void RefreshPreviewSceneTab();
	void RefreshAnimationAssetTab();
	void RefreshViewportTab();
	void RefreshSkillSetTab();
	void RefreshSkillSetTabEx(bool InTryInvokeTab = true);
	void RefreshSourceItems();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void RegisterTabSpawner();
	void UnregisterTabSpawner();
	void RequestDestroyWindowOverride(const TSharedRef<SWindow>& window);
	
	TSharedRef<SDockTab> SpawnDummyTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> SpawnSkillEditorDataBrowserTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> SpawnSkillEditorDataPropertyTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> SpawnSkillEditorViewportTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> SpawnPreViewSceneTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> SpawnMontageTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> SpawnSkillNotifyTab(const FSpawnTabArgs& InArgs);	
	TSharedRef<SDockTab> SpawnSkillSetTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> SpawnMontageSectionTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> SpawnAnimationAssetTab(const FSpawnTabArgs& InArgs);

	// Skill Editor Data Path Browser
	TSharedRef<SWidget> CreateSkillEditorDataAssetVeiw();
	void PathPickerPathSelected(const FString& FolderPath);
	void AssetSelected(const FAssetData& AssetData);
	TSharedRef<SWidget> CreatePathWidget();
	TSharedRef<ITableRow> GenerateTreeRow(TSharedPtr<FSkillTreeItem> TreeItem, const TSharedRef<STableViewBase>& OwnerTable);
	void GetChildrenForTree(TSharedPtr<FSkillTreeItem> TreeItem, TArray< TSharedPtr<FSkillTreeItem> >& OutChildren);
	TSharedPtr<FSkillTreeItem> AddPath(const FString& Path);
	void TreeItemScrolledIntoView(TSharedPtr<FSkillTreeItem> TreeItem, const TSharedPtr<ITableRow>& Widget);
	TSharedPtr<SWidget> MakePathViewContextMenu();
	TArray<FString> GetSelectedPaths() const;
	void OnCreateFolder();
	void OnDeleteFolder();
	void OnRenameFolder();
	bool DeleteEmptyFolderFromDisk(const FString& InPathToDelete);
	void RecursiveExpandParents(const TSharedPtr<FSkillTreeItem>& Item);
	bool DoesFolderExist(const FString& FolderPath);
	TSharedPtr<FSkillTreeItem> FindItemRecursive(const FString& Path) const;
	void TreeSelectionChanged(TSharedPtr< FSkillTreeItem > TreeItem, ESelectInfo::Type /*SelectInfo*/);

	// Skill Editor Data Asset Browser
	TSharedRef<SWidget> CreateAssetWidget();
	TSharedRef<SSkillAssetListView> CreateListView();
	TSharedRef<SWidget> CreateShadowOverlay(TSharedRef<STableViewBase> Table);
	TSharedRef<ITableRow> MakeListViewWidget(TSharedPtr<FSkillAssetViewItem> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);
	void ItemScrolledIntoView(TSharedPtr<struct FSkillAssetViewItem> AssetItem, const TSharedPtr<ITableRow>& Widget);
	TSharedPtr<SWidget> OnGetContextMenuContent();
	void OnListMouseButtonDoubleClick(TSharedPtr<FSkillAssetViewItem> AssetItem);
	void AssetSelectionChanged(TSharedPtr< struct FSkillAssetViewItem > AssetItem, ESelectInfo::Type SelectInfo);
	float GetListViewItemHeight() const;	
	FReply OnCreateSkillEditorDataAsset();
	TSharedPtr<SWidget> MakeExportContextMenu();
	void TransferToDatacenter();
	void AllTransferToDatacenter();
	void Converting();
	UObject* CreateNewAsset(UClass* AssetClass, const FString& TargetPath, const FString& DesiredName, EObjectFlags Flags);

	// 몽타주 섹션과 연결된 스킬 셋 위젯을 활성화
	void ActivateWidget(TWeakObjectPtr<UEditorCompositeSection> EditorCompositeSection);
	void ActivateWidgets(const TArray<UObject*>& InObjects);

	// etc
	TSharedRef<class IPersonaToolkit> GetPersonaToolkit() const { return PersonaToolkit.ToSharedRef(); }
	void OnChangeEdited();
	void OnChangeAnimNotifyVisible();
	void OnChangeSkillEditorAsset(const FGsSkillSetData* SkillSetData);
	void HandleObjectsSelected(const TArray<UObject*>& InObjects);
	void HandleNotifiesChanged();
	void HandleNotifiesChangedEx(const TArray<UObject*>& InObjects);
	void HandleSectionsChanged();
	FReply RefreshSkillAssetData();
	void RefreshEditor();
	void OnChangeSkillSetData();
	void OnChangeSkillEffectInfoEdited();
	void DeleteSkillEditorDataAsset();
	void CopySkillEditorDataAsset();
	void PateSkillEditorDataAsset();
	void RenameSkillEditorDataAsset();
	void AssetItemWidgetDestroyed(const TSharedPtr<FSkillAssetViewItem>& Item);
	void NullReferencesToObject(UObject* InObject);
	void AssetRenameCommit(const TSharedPtr<FSkillAssetViewItem>& Item, const FString& NewName, const FSlateRect& MessageAnchor, const ETextCommit::Type CommitType);
	void AssetRenameCommit(const FName& Name);
	void OnChangeSkillEditorAssetName(const FName& Name);

	void OnAssetNameCommitted(const FText& NewText, ETextCommit::Type CommitType);
	FReply OnKeyDownHandler(const FGeometry&, const FKeyEvent& KeyEvent);
	void OnAssetNameChanged(const FText& InText);
	bool OnValidatingAssetName(const FText& NewSessionName, FText& OutError);
	FText ValidateAssetName(const FString& Name);
	bool SearchChildren(const TSharedPtr<FSkillTreeItem> InSkillTreeItem, TArray<TSharedPtr<FSkillTreeItem>>& InSearchTreeItems);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SpawnBPData Event
	void SpawnBPDataGsCharacterBaseSettingUpdateNotInitPos();
	void SpawnBPDataGsCharacterBaseSettingUpdate();
	void SpawnBPDataGsCharacterBaseRemoveUpdate();
	void SpawnBPDataGsCharacterBaseRemoveAllUpdate();
	void SpawnTransformUpdate();
	void SpawnBPDataRendering();

	void InitSpawnBPDataGsCharacterBaseSettingUpdate();
	void ResetSpawnBPDataGsCharacterBaseSettingUpdate();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ActorScaleUpdate();

public: 
	void InitSkillEditor();

	// 충돌 관련 FSkillEditorViewportClient 에서 호출

	UAnimMontage* GetCurrentMontage() { return AnimMontage; }	
	USkillEditorDataAsset* GetSkillEditorDataAsset() { return SkillEditorDataAsset; }
};