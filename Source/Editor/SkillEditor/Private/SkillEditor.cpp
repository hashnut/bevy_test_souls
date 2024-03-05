// Copyright/2019/04/19 - BSAM

#include "SkillEditor.h"
#include "Widgets/SkillEditorPathViewTypes.h"
#include "Widgets/SkillAssetViewWidgets.h"
#include "Data/SkillEditorDataAsset.h"
#include "SkillEditorViewport.h"
#include "SkillNotify/CollisionSkillNotify.h"
#include "SkillNotify/GuideEffectSkillNotify.h"
#include "SkillNotify/ProjectileSkillNotify.h"
#include "SkillNotify/CancleSkillNotify.h"
#include "SkillNotify/GroupCollisionSkillNotify.h"
#include "SkillNotify/RandomCollisionSkillNotify.h"
#include "Data/SkillEditorDataAssetContainerFactoryNew.h"
#include "T1Project.h"
#include "SkillEditorTools.h"
#include "ActorComponentEx/EGsMeshShaderEffect.h"

#include "Framework/Application/SlateApplication.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SScrollBar.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Framework/Docking/TabManager.h"
#include "HAL/PlatformApplicationMisc.h"
#include "ContentBrowserModule.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSplitter.h"
#include "EditorStyleSet.h"
#include "Widgets/Layout/SSeparator.h"
#include "AssetRegistryModule.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "FileHelpers.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "EditorWidgetsModule.h"
#include "Widgets/Layout/SScrollBorder.h"
#include "Widgets/Input/SButton.h"
#include "Factories/DataAssetFactory.h"
#include "AssetToolsModule.h"
#include "IContentBrowserSingleton.h"
#include "Editor.h"
#include "PropertyEditorModule.h"
#include "PersonaModule.h"
#include "IPersonaToolkit.h"
#include "AdvancedPreviewSceneModule.h"
#include "IPersonaPreviewScene.h"
#include "ISkeletonTree.h"
#include "ISkeletonEditorModule.h"
#include "Misc/MessageDialog.h"
#include "PackageTools.h"
#include "ObjectTools.h"
#include "Runtime/Json/Public/Serialization/JsonWriter.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "DrawTools.h"
#include "SkillSection/SkillSectionData.h"
#include "SkillNotify/CutremineSkillNotify.h"
#include "SkillNotify/HitSkillNotify.h"
#include "SkillSection/CommonSectionData.h"
#include "SkillSection/ReactionSectionData.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Factories/AnimSequenceFactory.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

#include "Animation/AnimSequenceSkill.h"
#include "Animation/AnimMontageSkill.h"

#include "Persona/Private/AnimationEditorPreviewScene.h"
#include "Persona/Private/PersonaPreviewSceneDescription.h"
#include "Persona/Private/TabSpawners.h"
#include "Persona/Private/PreviewSceneCustomizations.h"

#include "Widgets/Input/SEditableTextBox.h"

#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/GsSchemaSkillReaction.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "GsTableManagerEditor.h"

#include "Data/GsGlobalConstant.h"
#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "ActorComponentEx/GsHitEffectComponent.h"
#include "ActorEx/GsCharacterBase.h"


#define LOCTEXT_NAMESPACE "SkillEditor"

const FName FSkillEditor::SkillSetTabID(TEXT("SkillSetTab"));
const FName FSkillEditor::SkillNotifyTabID(TEXT("SkillNotifyTab"));
const FName FSkillEditor::MontageSectionTabID(TEXT("MontageSectionTab"));

FSkillEditor::FSkillEditor()
{
	// If Tab Mabager is not valid, it will be obvious ... But after creating the major tab(skill data browser), manager is created.
	if (!TabManager.IsValid())
	{
		const TSharedRef<SDockTab> MajorTab = SNew(SDockTab)
			.TabRole(ETabRole::MajorTab);

		TabManager = FGlobalTabmanager::Get()->NewTabManager(MajorTab);
	}

	RegisterTabSpawner();
}

FSkillEditor::~FSkillEditor()
{
	UnregisterTabSpawner();
}

// Skill browser other than the skill browser before creating and selecting the data is not a dummy tab default settings.
// Once registered, it will not be released until the module is terminated.FName, ID.
void FSkillEditor::RegisterTabSpawner()
{
	TabManager->RegisterTabSpawner("SkillEditorDataBrowserTab", FOnSpawnTab::CreateRaw(this, &FSkillEditor::SpawnSkillEditorDataBrowserTab));
	TabManager->RegisterTabSpawner("SkillEditorViewportTab", FOnSpawnTab::CreateRaw(this, &FSkillEditor::SpawnSkillEditorViewportTab));
	TabManager->RegisterTabSpawner("MontageTab", FOnSpawnTab::CreateRaw(this, &FSkillEditor::SpawnMontageTab));
	TabManager->RegisterTabSpawner(SkillNotifyTabID, FOnSpawnTab::CreateRaw(this, &FSkillEditor::SpawnSkillNotifyTab));
	TabManager->RegisterTabSpawner(SkillSetTabID, FOnSpawnTab::CreateRaw(this, &FSkillEditor::SpawnSkillSetTab));
	TabManager->RegisterTabSpawner(MontageSectionTabID, FOnSpawnTab::CreateRaw(this, &FSkillEditor::SpawnMontageSectionTab));
	TabManager->RegisterTabSpawner("SkillEditorDataPropertyTab", FOnSpawnTab::CreateRaw(this, &FSkillEditor::SpawnSkillEditorDataPropertyTab));
	TabManager->RegisterTabSpawner("PreviewSceneTab", FOnSpawnTab::CreateRaw(this, &FSkillEditor::SpawnPreViewSceneTab));
	TabManager->RegisterTabSpawner("AnimationAssetTab", FOnSpawnTab::CreateRaw(this, &FSkillEditor::SpawnAnimationAssetTab));
}

void FSkillEditor::UnregisterTabSpawner()
{
	TabManager->UnregisterTabSpawner("SkillEditorDataBrowserTab");
	TabManager->UnregisterTabSpawner("SkillEditorViewportTab");
	TabManager->UnregisterTabSpawner("MontageTab");
	TabManager->UnregisterTabSpawner(SkillNotifyTabID);
	TabManager->UnregisterTabSpawner(SkillSetTabID);
	TabManager->UnregisterTabSpawner(MontageSectionTabID);
	TabManager->UnregisterTabSpawner("SkillEditorDataPropertyTab");
	TabManager->UnregisterTabSpawner("PreviewSceneTab");
	TabManager->UnregisterTabSpawner("AnimationAssetTab");
}

FReply FSkillEditor::KeyDownEventHandler(const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.IsControlDown() && InKeyEvent.GetCharacter() == 'S')	{
		
		OnSaveSkillEditorDataAsset();
	}

	return FReply::Handled();
}

// You need to be aware that it can be called multiple times (first and every time you open the Editor).
void FSkillEditor::InitSkillEditor()
{
	if (RootWindow.IsValid())
	{
		RootWindow->GetNativeWindow()->SetWindowFocus();
		return;
	}

	SkillEditorDataAsset = nullptr;
	FVector2D WindowSize = FVector2D(1100, 740);

	// Position the center of the desktop screen.
	FDisplayMetrics DisplayMetrics;
	FSlateApplication::Get().GetCachedDisplayMetrics(DisplayMetrics);
	const FVector2D DisplayTopLeft(DisplayMetrics.PrimaryDisplayWorkAreaRect.Left, DisplayMetrics.PrimaryDisplayWorkAreaRect.Top);
	const FVector2D DisplaySize(DisplayMetrics.PrimaryDisplayWorkAreaRect.Right - DisplayMetrics.PrimaryDisplayWorkAreaRect.Left,
		DisplayMetrics.PrimaryDisplayWorkAreaRect.Bottom - DisplayMetrics.PrimaryDisplayWorkAreaRect.Top);

	// Monitor's DPI scale factor in monitor coordinates (displayed in pixels)
	const float DPIScale = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(DisplayMetrics.PrimaryDisplayWorkAreaRect.Left, DisplayMetrics.PrimaryDisplayWorkAreaRect.Top);

	// Position the center of the desktop and set the scale(ratio) to 1. If the window is too big than the actual screen
	const float CenterScale = 0.85f;
	WindowSize = (CenterScale * DisplaySize) / DPIScale;

	FVector2D ScreenPos = DisplayTopLeft + (DisplaySize - WindowSize) * 0.5f;

	// Create Root Window
	RootWindow = SNew(SWindow)
		.AutoCenter(EAutoCenter::None)
		.Title(LOCTEXT("SkillEditorWindowTitle", "Skill Editor Window"))
		.IsInitiallyMaximized(false)
		.ScreenPosition(ScreenPos)
		.ClientSize(WindowSize)
		.CreateTitleBar(true)
		.SizingRule(ESizingRule::UserSized)
		.SupportsMinimize(true)
		.SupportsMaximize(true);

	RootWindow->SetRequestDestroyWindowOverride(FRequestDestroyWindowOverride::CreateRaw(this, &FSkillEditor::RequestDestroyWindowOverride));
	FSlateApplication::Get().AddWindow(RootWindow.ToSharedRef(), false);
	FSlateApplication::Get().SetUnhandledKeyDownEventHandler(FOnKeyEvent::CreateRaw(this, &FSkillEditor::KeyDownEventHandler));

	SkillEditorLayout = FTabManager::NewLayout("SkillEditor_Layout_v0.01")
		->AddArea
		(			
			FTabManager::NewPrimaryArea()
			->Split
			(
				FTabManager::NewSplitter()
				->SetSizeCoefficient(0.2f)
				->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewSplitter()
					->SetSizeCoefficient(0.2f)
					->SetOrientation(Orient_Vertical)
					->Split
					(						
						FTabManager::NewStack()
						->SetHideTabWell(true)
						->AddTab("SkillEditorDataBrowserTab", ETabState::OpenedTab)
					)
				)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetSizeCoefficient(0.6f)
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.65f)
					->SetHideTabWell(true)
					->AddTab("SkillEditorViewportTab", ETabState::ClosedTab)
				)
				->Split
				(					
					FTabManager::NewStack()
					->SetSizeCoefficient(0.35f)
					->SetHideTabWell(true)
					->AddTab("MontageTab", ETabState::ClosedTab)
				)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetSizeCoefficient(0.20f)
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.35f)
					->SetHideTabWell(true)
					->AddTab("SkillEditorDataPropertyTab", ETabState::ClosedTab)
					->AddTab("PreviewSceneTab", ETabState::ClosedTab)
					->AddTab("AnimationAssetTab", ETabState::ClosedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.65f)
					->SetHideTabWell(true)
					->AddTab(SkillNotifyTabID, ETabState::ClosedTab)
					->AddTab(SkillSetTabID, ETabState::ClosedTab)
					->AddTab(MontageSectionTabID, ETabState::ClosedTab)
				)
			)
		);
	
	// Connect layout to window, then widgetize, interlock
	TSharedPtr<SWidget> MainFrameContent;
	MainFrameContent = TabManager->RestoreFrom(SkillEditorLayout.ToSharedRef(), RootWindow, false);
	RootWindow->SetContent(MainFrameContent.ToSharedRef());

	// Actual tab creation
	TabManager->TryInvokeTab(FTabId("SkillEditorDataBrowserTab"));
	TabManager->TryInvokeTab(FTabId("SkillEditorViewportTab"));
	TabManager->TryInvokeTab(FTabId("MontageTab"));
	TabManager->TryInvokeTab(FTabId("PreviewSceneTab"));
	TabManager->TryInvokeTab(FTabId("AnimationAssetTab"));
	TabManager->TryInvokeTab(SkillNotifyTabID);
	TabManager->TryInvokeTab(SkillSetTabID);
	TabManager->TryInvokeTab(MontageSectionTabID);
	TabManager->TryInvokeTab(FTabId("SkillEditorDataPropertyTab"));

	RootWindow->ShowWindow();

	// Import the PropertyEditor module to create a detail view.
	const bool bIsUpdatable = false;
	const bool bAllowFavorites = true;
	const bool bIsLockable = false;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs(bIsUpdatable, bIsLockable, true, FDetailsViewArgs::ObjectsUseNameArea, false);
	DataActorComponentView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	NoitfyDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	SkillSetDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	OriginamAnimationAssetView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	if (nullptr == GlobalConstant)
	{
		// 기본 피격 정보 불러오기
		GlobalConstant = LoadObject<UGsGlobalConstant>(nullptr, TEXT("/Game/Data/GlobalConstant/GlobalConstant.GlobalConstant"));
	}

	if (nullptr == CurveFloat)
	{
		// 기본 RimLight효과 커브 데이터 불러오기
		CurveFloat = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/Data/CH/RimLightCurve.RimLightCurve"));
	}
}

void FSkillEditor::OnChangeAnimNotifyVisible()
{
	/*if (MontageEditor.IsValid())
	{
		MontageEditor->GetAnimNotifyPanel()->SkillModeAnimNotifyVisible(true);
	}*/
}

void FSkillEditor::OnChangeEdited()
{
	/*const FAssetData& AssetViewAsset = StaticCastSharedPtr<FSkillAssetViewAsset>(SelectedSkillAssetViewItemItem)->Data;
	SkillEditorDataAsset = Cast<USkillEditorDataAsset>(AssetViewAsset.GetAsset());
	if (SkillEditorDataAsset == nullptr) return;*/
	
	//SkillEditorDataPropertyTab->SetContent(SNullWidget::NullWidget);

	/*static FName FlagPropertyName = TEXT("AnimMontage");
	if (UClass* cl = SkillEditorDataAsset->GetClass())
	{
		if (UProperty* Property = cl->FindPropertyByName(FlagPropertyName))
		{
			const static EPropertyFlags OriginalPropertyFlags = Property->PropertyFlags;
			static bool test = true;

			if (test)
			{
				Property->PropertyFlags = CPF_None;
				test = false;
			}
			else
			{
				Property->PropertyFlags = OriginalPropertyFlags;
				test = true;
			}
		}
	}

	DataActorComponentView->SetObject(SkillEditorDataAsset, true);
	SkillEditorDataPropertyTab->SetContent(DataActorComponentView.ToSharedRef());*/

	//if (SelectedSkillAssetViewItemItem.IsValid())
	OnListMouseButtonDoubleClick(SelectedSkillAssetViewItemItem);
}

void FSkillEditor::OnChangeSkillEffectInfoEdited()
{
	if (SkillEditorDataAsset != nullptr && AnimMontage != nullptr)
	{
		// 스킬 노티파이 이벤트를 가져온다.
		TArray< FAnimNotifyEvent* > SkillNotifyEvent;
		for (int32 index = 0; index < AnimMontage->GetNotifies().Num(); ++index)
		{
			if (AnimMontage->GetNotifies()[index].SkillNotify != nullptr)
				SkillNotifyEvent.Add(&AnimMontage->GetNotifies()[index]);
		}		
		
		TArray<USkillNotify*> listSkillNotify;
		for (int32 index = 0; index < SkillNotifyEvent.Num(); ++index)
		{
			// 충돌 노티파이
			UCollisionSkillNotify* CollsionSkillNotify = Cast< UCollisionSkillNotify >(SkillNotifyEvent[index]->SkillNotify);
			if (CollsionSkillNotify != nullptr)
			{				
				listSkillNotify.Add(SkillNotifyEvent[index]->SkillNotify);
				continue;
			}

			// 캔슬 노티파이
			UCancleSkillNotify* CancleSkillNotify = Cast< UCancleSkillNotify >(SkillNotifyEvent[index]->SkillNotify);
			if (CancleSkillNotify != nullptr)
			{
				listSkillNotify.Add(SkillNotifyEvent[index]->SkillNotify);
				continue;
			}

			// 가이드 이펙트 노티파이
			UGuideEffectSkillNotify* GuideEffectSkillNotify = Cast< UGuideEffectSkillNotify >(SkillNotifyEvent[index]->SkillNotify);
			if (GuideEffectSkillNotify != nullptr)
			{
				listSkillNotify.Add(SkillNotifyEvent[index]->SkillNotify);
				continue;
			}

			// 프로젝트 타일 노티파이
			UProjectileSkillNotify* ProjectileSkillNotify = Cast< UProjectileSkillNotify >(SkillNotifyEvent[index]->SkillNotify);
			if (ProjectileSkillNotify != nullptr)
			{
				listSkillNotify.Add(SkillNotifyEvent[index]->SkillNotify);
				continue;
			}
		}
	}
}

void FSkillEditor::OnChangeSkillEditorAssetName(const FName& Name)
{
	AssetRenameCommit(Name);
}

void FSkillEditor::OnChangeSkillEditorAsset(const FGsSkillSetData *SkillSetData)
{
	/*
	1. 현재 선택된 폴더의 모든 스킬 에디터 데이터 애셋의 정보를 가져온다
	2. 받은 skillid와 동일한게 있는지 체크한다.고
	3. 있다면 원래대로 돌리고 실패 다이얼로그를 출력한다.
	*/
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));

	const FString rootPaths = FString(TEXT("/Game/SkillEditor/SkillEditorData"));
	TArray<FString> outSubPaths;
	AssetRegistryModule.Get().GetSubPaths(FPaths::GetPath(rootPaths), outSubPaths, true);

	TArray<FAssetData> AllAssets;
	FARFilter Filter;
	for (int32 PathIdx = 0; PathIdx < outSubPaths.Num(); ++PathIdx)
	{
		Filter.PackagePaths.Add(*outSubPaths[PathIdx]);
	}

	AssetRegistryModule.Get().GetAssets(Filter, AllAssets);
	for (int32 AssetIdx = 0; AssetIdx < AllAssets.Num(); ++AssetIdx)
	{
		const FAssetData& AssetData = AllAssets[AssetIdx];
		USkillEditorDataAsset* SkillEditorData = Cast<USkillEditorDataAsset>(AssetData.GetAsset());
		if (SkillEditorData != nullptr)
		{
			/*for (int32 SkillSetIdx = 0; SkillSetIdx < SkillEditorData->ListSkillSet.Num(); ++SkillSetIdx)
			{
				const FGsSkillSetData * NewSkillSetData = &(SkillEditorData->ListSkillSet[SkillSetIdx]);
				if (NewSkillSetData != SkillSetData)
				{
					if (NewSkillSetData->SkillId == SkillSetData->SkillId)
					{
						FText Message(LOCTEXT("SkillEditor", "Duplicate Skill ID!!!"));
						FMessageDialog::Open(EAppMsgType::Ok, Message);

						return;
					}					
				}
			}*/
		}
	}
}

void FSkillEditor::RequestDestroyWindowOverride(const TSharedRef<SWindow>& window)
{
	FSlateApplicationBase::Get().RequestDestroyWindow(window);
	
	SkeletonTree = nullptr;

	TabManager->CloseAllAreas();

	RootWindow = nullptr;

	SkillEditorDataPropertyTab = nullptr;
	PreviewSceneTab = nullptr;
	AnimationAssetTab = nullptr;
	MontageTab = nullptr;
	SkillEditorViewportTab = nullptr;
	SkillNotifyTab = nullptr;
	SkillSetTab = nullptr;
	MontageSectionTab = nullptr;
	PersonaToolkit = nullptr;
	Viewport = nullptr;		

	SkillEditorDataAsset = nullptr;
	SkillEditorSetData = nullptr;	
}

TSharedRef<SDockTab> FSkillEditor::SpawnMontageSectionTab(const FSpawnTabArgs& InArgs)
{
	MontageSectionTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			SNullWidget::NullWidget
		];

	return MontageSectionTab.ToSharedRef();
}

TSharedRef<SDockTab> FSkillEditor::SpawnSkillNotifyTab(const FSpawnTabArgs& InArgs)
{
	SkillNotifyTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			SNullWidget::NullWidget
		];

	return SkillNotifyTab.ToSharedRef();
}

TSharedRef<SDockTab> FSkillEditor::SpawnSkillSetTab(const FSpawnTabArgs& InArgs)
{
	SkillSetTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			SNullWidget::NullWidget
		];

	return SkillSetTab.ToSharedRef();
}

TSharedRef<SDockTab> FSkillEditor::SpawnSkillEditorViewportTab(const FSpawnTabArgs& InArgs)
{
	SkillEditorViewportTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			SNullWidget::NullWidget
		];

	return SkillEditorViewportTab.ToSharedRef();
}

TSharedRef<SDockTab> FSkillEditor::SpawnAnimationAssetTab(const FSpawnTabArgs& InArgs)
{
	AnimationAssetTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			SNullWidget::NullWidget
		];

	return AnimationAssetTab.ToSharedRef();
}

TSharedRef<SDockTab> FSkillEditor::SpawnPreViewSceneTab(const FSpawnTabArgs& InArgs)
{
	PreviewSceneTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			SNullWidget::NullWidget
		];

	return PreviewSceneTab.ToSharedRef();
}

TSharedRef<SDockTab> FSkillEditor::SpawnMontageTab(const FSpawnTabArgs& InArgs)
{
	MontageTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			SNullWidget::NullWidget
		];

	return MontageTab.ToSharedRef();
}

TSharedRef<SWidget> FSkillEditor::CreateSkillEditorDataAssetVeiw()
{
	TSharedRef<SVerticalBox> MainVerticalBox = SNew(SVerticalBox);
	MainVerticalBox->AddSlot()
		.FillHeight(1)
		.Padding(2, 1, 2, 1)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.Visibility(EVisibility::Visible)
				.ContentPadding(FMargin(8, 2))
				.Text(LOCTEXT("SaveSkillEditorDataAsset", "Save SkillEditorData Asset"))
				.OnClicked(this, &FSkillEditor::OnSaveSkillEditorDataAsset)
			]

			+ SVerticalBox::Slot()
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				[
					DataActorComponentView->AsShared()
				]
			]
		];

	return MainVerticalBox;
}


TSharedRef<SDockTab> FSkillEditor::SpawnSkillEditorDataPropertyTab(const FSpawnTabArgs& InArgs)
{	
	SkillEditorDataPropertyTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			SNullWidget::NullWidget
		];
	
	return SkillEditorDataPropertyTab.ToSharedRef();
}

TSharedRef<SDockTab> FSkillEditor::SpawnSkillEditorDataBrowserTab(const FSpawnTabArgs& InArgs)
{
	TSharedRef<SDockTab> SkillEditorDataBrowserTab = SNew(SDockTab)
		.TabRole(ETabRole::MajorTab);

	// Path/Asset view
	TSharedRef<SVerticalBox> MainVerticalBox = SNew(SVerticalBox);
	MainVerticalBox->AddSlot()
		.FillHeight(1)
		.Padding(0, 0, 0, 4)
		[
			SNew(SSplitter)

			+ SSplitter::Slot()
			.Value(0.4f)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				[
					CreatePathWidget()
				]
			]

			+ SSplitter::Slot()
			.Value(0.6f)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				[
					CreateAssetWidget()
				]
			]
		];

	SkillEditorDataBrowserTab->SetContent(MainVerticalBox);

	return SkillEditorDataBrowserTab;
}

TSharedRef<SDockTab> FSkillEditor::SpawnDummyTab(const FSpawnTabArgs& InArgs)
{
	TSharedRef<SDockTab> DummyTab = SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		.ContentPadding(FMargin(0));

	return DummyTab;	
}

void FSkillEditor::PathPickerPathSelected(const FString& FolderPath)
{
	RefreshSourceItems();
}

void FSkillEditor::AssetSelected(const FAssetData& AssetData)
{
}

void FSkillEditor::RecursiveExpandParents(const TSharedPtr<FSkillTreeItem>& Item)
{
	if (Item->Parent.IsValid())
	{
		RecursiveExpandParents(Item->Parent.Pin());
		TreeViewPtr->SetItemExpansion(Item->Parent.Pin(), true);
	}
}

void FSkillEditor::TreeItemScrolledIntoView(TSharedPtr<FSkillTreeItem> TreeItem, const TSharedPtr<ITableRow>& Widget)
{
	if (TreeItem->bNamingFolder && Widget.IsValid() && Widget->GetContent().IsValid())
	{
		TreeItem->OnRenamedRequestEvent.Broadcast();
	}
}

bool FSkillEditor::DoesFolderExist(const FString& FolderPath)
{
	TArray<FString> SubPaths;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	AssetRegistryModule.Get().GetSubPaths(FPaths::GetPath(FolderPath), SubPaths, false);

	for (auto SubPathIt(SubPaths.CreateConstIterator()); SubPathIt; SubPathIt++)
	{
		if (*SubPathIt == FolderPath)
		{
			return true;
		}
	}

	return false;
}

UObject* FSkillEditor::CreateNewAsset(UClass* AssetClass, const FString& TargetPath, const FString& DesiredName, EObjectFlags Flags)
{
	FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

	// Create a unique package name and asset name for the frame
	const FString TentativePackagePath = UPackageTools::SanitizePackageName(TargetPath + TEXT("/") + DesiredName);
	FString DefaultSuffix;
	FString AssetName;
	FString PackageName;
	AssetToolsModule.Get().CreateUniqueAssetName(TentativePackagePath, DefaultSuffix, /*out*/ PackageName, /*out*/ AssetName);

	// Create a package for the asset
	UObject* OuterForAsset = CreatePackage(*PackageName);

	// Create a frame in the package
	UObject* NewAsset = NewObject<UObject>(OuterForAsset, AssetClass, *AssetName, Flags);
	FAssetRegistryModule::AssetCreated(NewAsset);

	return NewAsset;
}

void FSkillEditor::TransferToDatacenter()
{
	if (nullptr == SkillEditorDataAsset)
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "not select DataAsset"));
		return;
	}

	SkillEditorDataAsset->Export();
}

void FSkillEditor::Converting()
{
	// Load the asset registry module
	static const FName AssetRegistryName(TEXT("AssetRegistry"));
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryName);
	const TArray<FString> SelectedPaths = GetSelectedPaths();
	FString FolderPath = SelectedPaths[0];
	TArray<FString> Outlist;

	// 1. 선택한 폴더의 모든 하위폴더를 찾는다.(담는다)
	AssetRegistryModule.Get().GetSubPaths(FolderPath, Outlist, true);

	// 2. 각 폴더별로 SkillAsset을 찾는다.(담는다)
	TArray<FAssetData>& Items = AssetItems;
	for (int32 i = 0; i < Outlist.Num(); ++i)
	{
		FARFilter Filter;
		Filter.PackagePaths.Add(*Outlist[i]);
		AssetRegistryModule.Get().GetAssets(Filter, Items);
	}

	// 3. SkillAsset의 Convert 함수를 호출한다.
	for (int32 AssetIdx = 0; AssetIdx < AssetItems.Num(); ++AssetIdx)
	{
		const FAssetData& AssetData = AssetItems[AssetIdx];
		USkillEditorDataAsset* SkillEditorData = Cast<USkillEditorDataAsset>(AssetData.GetAsset());
		if (SkillEditorData)
			SkillEditorData->Convert();
	}
}

void FSkillEditor::AllTransferToDatacenter()
{
	// Load the asset registry module
	static const FName AssetRegistryName(TEXT("AssetRegistry"));
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryName);
	const TArray<FString> SelectedPaths = GetSelectedPaths();
	FString FolderPath = SelectedPaths[0];
	TArray<FString> Outlist;

	// 1. 선택한 폴더의 모든 하위폴더를 찾는다.(담는다)
	AssetRegistryModule.Get().GetSubPaths(FolderPath, Outlist, true);

	// 2. 각 폴더별로 SkillAsset을 찾는다.(담는다)
	TArray<FAssetData>& Items = AssetItems;
	for (int32 i = 0; i < Outlist.Num(); ++i)
	{
		FARFilter Filter;
		Filter.PackagePaths.Add(*Outlist[i]);
		AssetRegistryModule.Get().GetAssets(Filter, Items);
	}

	// [B.Y] 3. Export전에 최상위 Root 폴더에서 실행은 기존 테이블을 비워주는 작업을 한다.
	if (FolderPath.Equals((TEXT("/Game/SkillEditor/SkillEditorData"))))
	{
		USkillEditorDataAsset::ClearTable();
	}

	// 4. SkillAsset의 Convert 함수를 호출한다.
	for (int32 AssetIdx = 0; AssetIdx < AssetItems.Num(); ++AssetIdx)
	{
		const FAssetData& AssetData = AssetItems[AssetIdx];
		USkillEditorDataAsset* SkillEditorData = Cast<USkillEditorDataAsset>(AssetData.GetAsset());
		if (SkillEditorData)
			SkillEditorData->Export();
	}
}

void FSkillEditor::OnRenameFolder()
{
}

void FSkillEditor::OnCreateFolder()
{
	const TArray<FString> SelectedPaths = GetSelectedPaths();
	const FString& FolderPath = SelectedPaths[0];

	FText DefaultFolderBaseName = LOCTEXT("DefaultFolderName", "NewFolder");
	FText DefaultFolderName = DefaultFolderBaseName;
	int32 NewFolderPostfix = 1;

	while (DoesFolderExist(FolderPath / DefaultFolderName.ToString()))
	{
		DefaultFolderName = FText::Format(LOCTEXT("DefaultFolderNamePattern", "{0}{1}"), DefaultFolderBaseName, FText::AsNumber(NewFolderPostfix));
		NewFolderPostfix++;
	}	
	
	const FString& FolderName = DefaultFolderName.ToString();	

	FString Path = FolderPath;
	Path.RemoveAt(0, FString(TEXT("/Game/SkillEditor/")).Len());

	// Create subpaths, connect them as children, and return the corresponding items
	TSharedPtr<FSkillTreeItem> Item = AddPath(Path / FolderName); 
	if (Item.IsValid())
	{
		if (!TreeViewPtr->IsItemExpanded(Item))
		{
			TreeViewPtr->SetItemExpansion(Item, true);
			TreeViewPtr->SetSelection(Item);
		}
	}	

	const FString NewPath = FolderPath / FolderName;
	FString NewPathOnDisk;
	bool bSuccess = FPackageName::TryConvertLongPackageNameToFilename(NewPath, NewPathOnDisk) && IFileManager::Get().MakeDirectory(*NewPathOnDisk, true);

	if (bSuccess)
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		bSuccess = AssetRegistryModule.Get().AddPath(NewPath);
	}
}

void FSkillEditor::OnDeleteFolder()
{
	const TArray<FString> SelectedPaths = GetSelectedPaths();

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");	
	for (const FString& PathToDelete : SelectedPaths)
	{
		if (DeleteEmptyFolderFromDisk(*PathToDelete))
		{
			AssetRegistryModule.Get().RemovePath(PathToDelete);
			TSharedPtr<FSkillTreeItem> ItemToRemove = FindItemRecursive(PathToDelete);
			if(ItemToRemove.IsValid())
			{
				if (ItemToRemove->Parent.IsValid())
				{
					ItemToRemove->Parent.Pin()->Children.Remove(ItemToRemove);
				}
				else
				{
					TreeRootItems.Remove(ItemToRemove);
				}

				TreeViewPtr->RequestTreeRefresh();
			}
		}
	}
}

TSharedPtr<FSkillTreeItem> FSkillEditor::FindItemRecursive(const FString& Path) const
{
	for (auto TreeItemIt = TreeRootItems.CreateConstIterator(); TreeItemIt; ++TreeItemIt)
	{
		if ((*TreeItemIt)->FolderPath == Path)
		{
			// This root item is the path
			return *TreeItemIt;
		}

		// Try to find the item under this root
		TSharedPtr<FSkillTreeItem> Item = (*TreeItemIt)->FindItemRecursive(Path);
		if (Item.IsValid())
		{
			// The item was found under this root
			return Item;
		}
	}

	return TSharedPtr<FSkillTreeItem>();
}

bool FSkillEditor::DeleteEmptyFolderFromDisk(const FString& InPathToDelete)
{
	struct FEmptyFolderVisitor : public IPlatformFile::FDirectoryVisitor
	{
		bool bIsEmpty;

		FEmptyFolderVisitor()
			: bIsEmpty(true)
		{
		}

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (!bIsDirectory)
			{
				bIsEmpty = false;
				return false; // abort searching
			}

			return true; // continue searching
		}
	};

	FString PathToDeleteOnDisk;
	if (FPackageName::TryConvertLongPackageNameToFilename(InPathToDelete, PathToDeleteOnDisk))
	{
		// Look for files on disk in case the folder contains things not tracked by the asset registry
		FEmptyFolderVisitor EmptyFolderVisitor;
		IFileManager::Get().IterateDirectoryRecursively(*PathToDeleteOnDisk, EmptyFolderVisitor);

		if (EmptyFolderVisitor.bIsEmpty)
		{
			return IFileManager::Get().DeleteDirectory(*PathToDeleteOnDisk, false, true);
		}
	}

	return false;
}

TSharedPtr<SWidget> FSkillEditor::MakePathViewContextMenu()
{
	const bool bShouldCloseWindowAfterMenuSelection = true;
	TSharedPtr<FUICommandList> CommandList;
	CommandList = MakeShareable(new FUICommandList());
	FMenuBuilder MenuBuilder(bShouldCloseWindowAfterMenuSelection, CommandList);

	FSlateIcon DummyIcon(NAME_None, NAME_None);

	MenuBuilder.BeginSection("SkillEditorDataFolder", LOCTEXT("SkillEditorDataFolderHeader", "SkillEditorDataFolder"));
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("SkillEditorDataFolderCreate", "Create Folder"),
			LOCTEXT("SkillEditorDataFolderCreate_ToolTip", "Create Skill Data Folder"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FSkillEditor::OnCreateFolder),
				FCanExecuteAction()
			)
		);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("SkillEditorDataFolderDelete", "Delete Folder"),
			LOCTEXT("SkillEditorDataFolderDelete_ToolTip", "Delete Skill Data Folder"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FSkillEditor::OnDeleteFolder),
				FCanExecuteAction()
			)
		);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("SkillEditorDataFolderAllTransferToDatacenter", "AllTransferToDatacenter"),
			LOCTEXT("SkillEditorDataFolderAllTransferToDatacenter_ToolTip", "Transfer to Data Center"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FSkillEditor::AllTransferToDatacenter),
				FCanExecuteAction()
			)
		);

		// 클라이언트 개발자 전용 - Converting - 2020/10/26 - BSAM
#if defined(GAMEVIL_ENGINE_SKILL_EDITOR_CONVERTING_BSAM)
		MenuBuilder.AddMenuEntry(
			LOCTEXT("SkillEditorDataFolderConverting", "Converting"),
			LOCTEXT("SkillEditorDataFolderConverting_ToolTip", "클라이언트 개발자 전용 - Converting - 2020/10/26 - BSAM"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FSkillEditor::Converting),
				FCanExecuteAction()
			)
		);
#endif
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

TArray<FString> FSkillEditor::GetSelectedPaths() const
{
	TArray<FString> RetArray;

	TArray<TSharedPtr<FSkillTreeItem>> Items = TreeViewPtr->GetSelectedItems();
	for (int32 ItemIdx = 0; ItemIdx < Items.Num(); ++ItemIdx)
	{
		RetArray.Add(Items[ItemIdx]->FolderPath);
	}
	return RetArray;
}

void FSkillEditor::TreeSelectionChanged(TSharedPtr< FSkillTreeItem > TreeItem, ESelectInfo::Type /*SelectInfo*/)
{
	//if (ShouldAllowTreeItemChangedDelegate())
	const TArray<TSharedPtr<FSkillTreeItem>> SelectedItems = TreeViewPtr->GetSelectedItems();
	if (SelectedItems.IsValidIndex(0))
	{
		const TSharedPtr<FSkillTreeItem> Item = SelectedItems[0];
		if (!ensure(Item.IsValid())) return;

		if (TreeItem.IsValid())
		{
			PathPickerPathSelected(TreeItem->FolderPath);
		}
	}
}

void FSkillEditor::OnAssetNameCommitted(const FText& NewText, ETextCommit::Type CommitType)
{
}

FReply FSkillEditor::OnKeyDownHandler(const FGeometry&, const FKeyEvent& KeyEvent)
{
	//return KeyEvent.GetKey() == EKeys::Escape ? OnDecline() : FReply::Unhandled();
	return FReply::Unhandled();
}

bool FSkillEditor::SearchChildren(const TSharedPtr<FSkillTreeItem> InSkillTreeItem, TArray<TSharedPtr<FSkillTreeItem>>& InSearchTreeItems)
{
	if (InSkillTreeItem->Children.Num() > 0)
	{
		for (int32 i = 0; InSkillTreeItem->Children.Num() > i; ++i)
		{
			SearchChildren(InSkillTreeItem->Children[i], InSearchTreeItems);
		}

		return true;
	}
	else
	{
		InSearchTreeItems.Add(InSkillTreeItem);
		return false;
	}
}

void FSkillEditor::OnAssetNameChanged(const FText& InText)
{
	SearchString = InText.ToString();
	RefreshSourceItems();
}

void FSkillEditor::RefreshSourceItems()
{
	if (!TreeViewPtr.IsValid()) return;

	const TArray<TSharedPtr<FSkillTreeItem>> SelectedItems = TreeViewPtr->GetSelectedItems();
	const TSharedPtr<FSkillTreeItem> RootSkillTreeItem = SelectedItems[0];
	if (RootSkillTreeItem == nullptr) 
		return;

	TArray<TSharedPtr<FSkillTreeItem>> OutChildren;
	GetChildrenForTree(RootSkillTreeItem, OutChildren);

	const TArray<FString> SelectedPaths = GetSelectedPaths();
	TArray<TSharedPtr<FSkillTreeItem>> SearchTreeItems;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	for (const FString& PathToSearch : SelectedPaths)
	{
		TSharedPtr<FSkillTreeItem> RootItemToSearch = FindItemRecursive(PathToSearch);
		SearchChildren(RootItemToSearch, SearchTreeItems);
	}

	FARFilter Filter;
	for (int32 i = 0; i < SearchTreeItems.Num(); ++i)
	{
		Filter.PackagePaths.Add(*SearchTreeItems[i]->FolderPath);
	}

	AssetItems.Empty();
	FilteredAssetItems.Empty();

	TArray<FAssetData>& Items = AssetItems;
	AssetRegistryModule.Get().GetAssets(Filter, Items);

	if (SearchString.IsEmpty())
	{
		for (int32 i = 0; i < Items.Num(); ++i)
		{
			const FAssetData& AssetData = Items[i];
			if (AssetData.IsValid())
				FilteredAssetItems.Add(MakeShareable(new FSkillAssetViewAsset(AssetData)));
		}
	}
	else
	{
		for (int32 i = 0; i < Items.Num(); ++i)
		{
			const FAssetData& AssetData = Items[i];
			int32 result = AssetData.AssetName.ToString().Find(SearchString);
			if (result >= 0)
			{
				if (AssetData.IsValid())
					FilteredAssetItems.Add(MakeShareable(new FSkillAssetViewAsset(AssetData)));
			}
		}
	}

	ListView->RequestListRefresh();

}

bool FSkillEditor::OnValidatingAssetName(const FText& NewSessionName, FText& OutError)
{
	OutError = ValidateAssetName(NewSessionName.ToString());
	return OutError.IsEmpty();
}

FText FSkillEditor::ValidateAssetName(const FString& Name)
{
	constexpr int32 MaxSessionNameLen = 128;

	if (Name.IsEmpty())
	{
		return LOCTEXT("SessionName_EmptyNameError", "Name cannot be left blank!");
	}
	else if (FChar::IsWhitespace(Name[0]))
	{
		return LOCTEXT("SessionName_IllegalBlankLeading", "Illegal leading white space!");
	}
	else if (FChar::IsWhitespace(Name[Name.Len() - 1]))
	{
		return LOCTEXT("SessionName_IllegalBlankTrailing", "Illegal trailing white space!");
	}
	else if (Name.Len() > MaxSessionNameLen)
	{
		return FText::Format(LOCTEXT("SessionName_TooLong", "Too long. Limited to {0} characters!"), MaxSessionNameLen);
	}

	FText Reason;
	if (!FPaths::ValidatePath(Name, &Reason))
	{
		return FText::Format(LOCTEXT("IllegalNameError", "{0}"), Reason);
	}

	return FText::GetEmpty(); // No Error.
}

TSharedRef<SWidget> FSkillEditor::CreatePathWidget()
{
	SAssignNew(TreeViewPtr, STreeView< TSharedPtr<FSkillTreeItem> >)
		.TreeItemsSource(&TreeRootItems)
		.OnGenerateRow(this, &FSkillEditor::GenerateTreeRow)
		.OnItemScrolledIntoView(this, &FSkillEditor::TreeItemScrolledIntoView)
		.ItemHeight(18)
		.OnSelectionChanged(this, &FSkillEditor::TreeSelectionChanged)
		.OnGetChildren(this, &FSkillEditor::GetChildrenForTree)
		.OnContextMenuOpening(this, &FSkillEditor::MakePathViewContextMenu)
		.ClearSelectionOnClick(false);

	TSharedRef<SVerticalBox> PathBrowser = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.VAlign(VAlign_Center)
			.Padding(FMargin(0, 0, 2, 0))
			[
				SAssignNew(EditableAssetName, SEditableTextBox)
				.HintText(LOCTEXT("EnterAssetNameHint", "Enter a asset name"))
				//.OnTextCommitted(this, &FSkillEditor::OnAssetNameCommitted)
				//.OnKeyDownHandler(this, &FSkillEditor::OnKeyDownHandler)
				//.OnVerifyTextChanged(this, &FSkillEditor::OnValidatingAssetName)
				.OnTextChanged(this, &FSkillEditor::OnAssetNameChanged)
			]
			//.Font(FEditorStyle::GetFontStyle("ContentBrowser.SourceTitleFont"))
			//.Visibility(EVisibility::Visible)
		]

		// Separator
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 1)
		[
			SNew(SSeparator)
			.Visibility(EVisibility::Visible)
		]

		// Tree
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[
			TreeViewPtr.ToSharedRef()
		];

	// Path Tree creation and setting
	// 0. Initialize UI and associated data
	// 1. Assign Game / Data / SkillEditorData as root in the skill browser path.
	// 2. If there is a child folder of Game / Data / SkillEditorData, connect it.
	TreeRootItems.Empty();
	TreeViewPtr->ClearSelection();

	// Root path, tree item creation and setting
	TSharedPtr<struct FSkillTreeItem> RootItem = nullptr;
	const FText DisplayName = FText::FromString(TEXT("SkillEditorDataRoot"));
	RootItem = MakeShareable(new FSkillTreeItem(DisplayName, FString(TEXT("SkillEditorData")), FString(TEXT("/Game/SkillEditor/SkillEditorData")), TSharedPtr<FSkillTreeItem>()));
	TreeRootItems.Add(RootItem);
	TreeViewPtr->RequestTreeRefresh();

	// Search subfolders (search all subordinate to subordinate)
	TArray<FString> SubPaths;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));

	AssetRegistryModule.Get().GetSubPaths(FPaths::GetPath(FString(TEXT("/Game/SkillEditor/SkillEditorData"))), SubPaths, true);

	// Path connection and tree item setting according to depth of sub folder.
	for (int32 PathIdx = 0; PathIdx < SubPaths.Num(); ++PathIdx)
	{
		// "SkillEditorData" The folder is the main root and only the subfolder is set, and the folder locations above it are removed.
		SubPaths[PathIdx].RemoveAt(0, FString(TEXT("/Game/SkillEditor")).Len());

		const FString& Path = SubPaths[PathIdx];

		// Create subpaths, connect them as children, and return the corresponding items
		TSharedPtr<FSkillTreeItem> Item = AddPath(Path);
		if (Item.IsValid())
		{
			if (!TreeViewPtr->IsItemExpanded(Item))
			{
				TreeViewPtr->SetItemExpansion(Item, true);
			}
		}
	}

	TreeViewPtr->SetSelection(RootItem);

	return PathBrowser;
}

TSharedRef<ITableRow> FSkillEditor::GenerateTreeRow(TSharedPtr<FSkillTreeItem> TreeItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	check(TreeItem.IsValid());	

	return
		SNew(STableRow< TSharedPtr<FSkillTreeItem> >, OwnerTable)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(0, 0, 2, 0)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.IsEnabled(true)
				.Text(FText::FromString(TreeItem.Get()->FolderName))
			]
		];
}

void FSkillEditor::GetChildrenForTree(TSharedPtr<FSkillTreeItem> TreeItem, TArray<TSharedPtr<FSkillTreeItem>>& OutChildren)
{
	TreeItem->SortChildrenIfNeeded();
	OutChildren = TreeItem->Children;
}

TSharedPtr<FSkillTreeItem> FSkillEditor::AddPath(const FString& Path)
{
	if (!ensure(TreeViewPtr.IsValid()))
	{
		return TSharedPtr<FSkillTreeItem>();
	}

	TArray<FString> PathItemList;
	Path.ParseIntoArray(PathItemList, TEXT("/"), /*InCullEmpty=*/true);

	if (PathItemList.Num() && TreeRootItems.Num())
	{
		// "/Game/SkillEditor/"
		TSharedPtr<FSkillTreeItem> CurrentItem = TreeRootItems[0]; 

		if (CurrentItem.IsValid())
		{
			for (int32 PathItemIdx = 1; PathItemIdx < PathItemList.Num(); ++PathItemIdx)
			{
				const FString& PathItemName = PathItemList[PathItemIdx];
				TSharedPtr<FSkillTreeItem> ChildItem = CurrentItem->GetChild(PathItemName);

				if (!ChildItem.IsValid())
				{
					const FString FolderName = PathItemName;
					const FString FolderPath = CurrentItem->FolderPath + "/" + PathItemName;

					ChildItem = MakeShareable(new FSkillTreeItem(FText::FromString(FolderName), FolderName, FolderPath, CurrentItem));
					CurrentItem->Children.Add(ChildItem);					
					CurrentItem->RequestSortChildren();
					TreeViewPtr->RequestTreeRefresh();
				}
				else
				{					
					ChildItem->FolderPath = CurrentItem->FolderPath + "/" + PathItemName;
				}

				CurrentItem = ChildItem;
			}

			RecursiveExpandParents(CurrentItem);
			TreeViewPtr->RequestScrollIntoView(CurrentItem);
			//TreeViewPtr->SetSelection(CurrentItem);
		}

		return CurrentItem;
	}

	return TSharedPtr<FSkillTreeItem>();
}

TSharedRef<SWidget> FSkillEditor::CreateAssetWidget()
{
	TSharedRef<SScrollBar> VerticalScrollBar = SNew(SScrollBar)
		.Orientation(Orient_Vertical)
		.Thickness(FVector2D(12.0f, 12.0f));

	TSharedRef<SVerticalBox> AssertBrowser = SNew(SVerticalBox);
	AssertBrowser->AddSlot()
		.FillHeight(1.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.Visibility(EVisibility::Visible)
				.ContentPadding(FMargin(8, 2))
				.Text(LOCTEXT("CreateSkillEditorDataAsset", "Create SkillEditorData Asset"))
				.OnClicked(this, &FSkillEditor::OnCreateSkillEditorDataAsset)
			]

			/*+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.Visibility(EVisibility::Visible)
				.ContentPadding(FMargin(8, 2))
				.Text(LOCTEXT("CopySkillEditorDataAsset", "Copy SkillEditorData Asset"))
				.OnClicked(this, &FSkillEditor::OnCopySkillEditorDataAsset)
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.Visibility(EVisibility::Visible)
				.ContentPadding(FMargin(8, 2))
				.Text(LOCTEXT("PateSkillEditorDataAsset", "Pate SkillEditorData Asset"))
				.OnClicked(this, &FSkillEditor::OnPateSkillEditorDataAsset)
			]*/

			+ SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SAssignNew(ViewContainer, SBorder)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					VerticalScrollBar
				]
			]			
		];

	ListView.Reset();
	TSharedRef<SWidget> NewView = SNullWidget::NullWidget;
	ListView = CreateListView();
	NewView = CreateShadowOverlay(ListView.ToSharedRef());
	ViewContainer->SetContent(NewView);

	//RefreshSourceItems();

	return AssertBrowser;
}

void FSkillEditor::CopySkillEditorDataAsset()
{
	if (!SelectedSkillAssetViewItemItem.IsValid()) return;

	TempSkillAssetViewItem = SelectedSkillAssetViewItemItem;
}

void FSkillEditor::PateSkillEditorDataAsset()
{
	if (!SelectedSkillAssetViewItemItem.IsValid() || !TempSkillAssetViewItem.IsValid()) return;

	OnListMouseButtonDoubleClick(SelectedSkillAssetViewItemItem);

	TempSkillEditorDataAsset = nullptr;
	const FAssetData& CopyAsset = StaticCastSharedPtr<FSkillAssetViewAsset>(TempSkillAssetViewItem)->Data;
	TempSkillEditorDataAsset = Cast<USkillEditorDataAsset>(CopyAsset.GetAsset());
	SkillEditorDataAsset->Pate(TempSkillEditorDataAsset);

	if (SkillEditorDataAsset)
	{
		MontageTab->SetContent(SNullWidget::NullWidget);
		PreviewSceneTab->SetContent(SNullWidget::NullWidget);
		AnimationAssetTab->SetContent(SNullWidget::NullWidget);
		SkillEditorViewportTab->SetContent(SNullWidget::NullWidget);
		SkillNotifyTab->SetContent(SNullWidget::NullWidget);
		SkillSetTab->SetContent(SNullWidget::NullWidget);
		MontageSectionTab->SetContent(SNullWidget::NullWidget);

		// Event Bind
		SkillEditorDataAsset->OnChangeEdited.Unbind();
		SkillEditorDataAsset->OnChangeSectionCategoryEdited.Unbind();
		SkillEditorDataAsset->OnNotifyIdChangedEx.Unbind();

		SkillEditorDataAsset->OnChangeEdited.BindRaw(this, &FSkillEditor::RefreshSkillEditorTab);
		SkillEditorDataAsset->OnChangeSectionCategoryEdited.BindRaw(this, &FSkillEditor::RefreshSkillSetTab);
		SkillEditorDataAsset->OnNotifyIdChangedEx.BindRaw(this, &FSkillEditor::HandleNotifiesChangedEx);

		SkillEditorDataAsset->OnSpawnBPDataGsCharacterBaseSettingUpdate.BindRaw(this, &FSkillEditor::SpawnBPDataGsCharacterBaseSettingUpdate);
		SkillEditorDataAsset->OnSpawnBPDataGsCharacterBaseRemoveUpdate.BindRaw(this, &FSkillEditor::SpawnBPDataGsCharacterBaseRemoveUpdate);
		SkillEditorDataAsset->OnSpawnBPDataGsCharacterBaseRemoveAllUpdate.BindRaw(this, &FSkillEditor::SpawnBPDataGsCharacterBaseRemoveAllUpdate);

		SkillEditorDataAsset->OnSpawnTransformUpdate.BindRaw(this, &FSkillEditor::SpawnTransformUpdate);
		SkillEditorDataAsset->OnSpawnBPDataRendering.BindRaw(this, &FSkillEditor::SpawnBPDataRendering);

		SkillEditorDataAsset->OnActorScaleUpdate.BindRaw(this, &FSkillEditor::ActorScaleUpdate);

		RefreshSourceItems();
		RefreshSkillEditorTab();

		ResetSpawnBPDataGsCharacterBaseSettingUpdate();
		SpawnBPDataGsCharacterBaseSettingUpdateNotInitPos();

		AddSkillCollisionComponent();

		ActorScaleUpdate();
	}
}

FReply FSkillEditor::OnSaveSkillEditorDataAsset()
{
	if (SkillEditorDataAsset != nullptr)
	{
		const bool bPromptUserToSave = true;
		const bool bSaveMapPackages = true;
		const bool bSaveContentPackages = true;
		const bool bFastSave = false;
		const bool bNotifyNoPackagesSaved = false;
		const bool bCanBeDeclined = false;

		FEditorFileUtils::SaveDirtyPackage(SkillEditorDataAsset->GetOutermost(), bPromptUserToSave, bSaveMapPackages, bSaveContentPackages, bFastSave, bNotifyNoPackagesSaved, bCanBeDeclined);
	}

	return FReply::Handled();
}

FReply FSkillEditor::OnCreateSkillEditorDataAsset()
{
	// 신규 기능 요청 및 추가 수정으로 인해 리펙토링 작업을 겸해서 작업
	/*
	A. 기능 순서
		1. 생성 이름 입력 가능한 상태 표시
		2. 이름 저장 완료(중복체크)
		3. 리스트에 출력

	B. 내부 로직
		1. SkillEditorData Asset 생성
		2. 언리얼 엔진에 등록
		3. 에디터에서 이름 작성 및 저장
		4. 언리얼에 통보
		5. 에디터에서 사용
	*/

	const FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	const FString seletedPath = GetSelectedPaths()[0];
	FString PathNameWithFilename = seletedPath + TEXT("/") + LOCTEXT("NewSkillEditorDataName_Default", "NewSkillEditorData").ToString();

	FString PackageName = seletedPath + TEXT("/");
	FString DefaultSuffix = TEXT("_");
	FString AssetName;

	// 유니크 이름 생성
	AssetToolsModule.Get().CreateUniqueAssetName(PathNameWithFilename, DefaultSuffix, PackageName, AssetName);
	UPackage* Package = CreatePackage(*PackageName);
	if (Package != nullptr)
	{
		auto SkillDataContaineFactory = NewObject<USkillEditorDataAssetContainerFactoryNew>();
		SkillDataContaineFactory->FactoryCreateNew(USkillEditorDataAsset::StaticClass(), Package, *AssetName, RF_Standalone | RF_Public, NULL, GWarn);
	}

	RefreshSourceItems();

	// 생성 한걸 Pick한것 처럼 셋팅하기.("DoubleClick" is same)
	SelectedSkillAssetViewItemItem = nullptr;
	for (int32 i=0; i < FilteredAssetItems.Num(); ++i)
	{
		//AssetItem->
		const FAssetData& AssetViewAsset = StaticCastSharedPtr<FSkillAssetViewAsset>(FilteredAssetItems[i])->Data;
		SkillEditorDataAsset = Cast<USkillEditorDataAsset>(AssetViewAsset.GetAsset());
		if (SkillEditorDataAsset)
		{
			if (*FPackageName::GetShortName(*SkillEditorDataAsset->GetOutermost()->GetPathName()) == AssetName)
			{
				SelectedSkillAssetViewItemItem = FilteredAssetItems[i];
				break;
			}
		}
	}	
	
	// 빈거를 생성 or 선택했으니 초기화 상태를 보여주자, Event등록도
	SetSkillEditorDataAsset();
	
	return FReply::Handled();
}

void FSkillEditor::SetSkillEditorDataAsset()
{
	MontageTab->SetContent(SNullWidget::NullWidget);
	PreviewSceneTab->SetContent(SNullWidget::NullWidget);
	AnimationAssetTab->SetContent(SNullWidget::NullWidget);
	SkillEditorViewportTab->SetContent(SNullWidget::NullWidget);
	SkillNotifyTab->SetContent(SNullWidget::NullWidget);
	SkillSetTab->SetContent(SNullWidget::NullWidget);
	MontageSectionTab->SetContent(SNullWidget::NullWidget);

	if (SelectedSkillAssetViewItemItem.IsValid())
	{
		const FAssetData& AssetViewAsset = StaticCastSharedPtr<FSkillAssetViewAsset>(SelectedSkillAssetViewItemItem)->Data;
		SkillEditorDataAsset = Cast<USkillEditorDataAsset>(AssetViewAsset.GetAsset());
		DataActorComponentView->SetObject(SkillEditorDataAsset, true);		
		SkillEditorDataPropertyTab->SetContent(CreateSkillEditorDataAssetVeiw());

		// Event Bind
		SkillEditorDataAsset->OnChangeEdited.Unbind();
		SkillEditorDataAsset->OnChangeSectionCategoryEdited.Unbind();
		SkillEditorDataAsset->OnNotifyIdChangedEx.Unbind();

		SkillEditorDataAsset->OnChangeEdited.BindRaw(this, &FSkillEditor::RefreshSkillEditorTab);
		SkillEditorDataAsset->OnChangeSectionCategoryEdited.BindRaw(this, &FSkillEditor::RefreshSkillSetTab);
		SkillEditorDataAsset->OnNotifyIdChangedEx.BindRaw(this, &FSkillEditor::HandleNotifiesChangedEx);

		SkillEditorDataAsset->OnSpawnBPDataGsCharacterBaseSettingUpdate.BindRaw(this, &FSkillEditor::SpawnBPDataGsCharacterBaseSettingUpdate);
		SkillEditorDataAsset->OnSpawnBPDataGsCharacterBaseRemoveUpdate.BindRaw(this, &FSkillEditor::SpawnBPDataGsCharacterBaseRemoveUpdate);
		SkillEditorDataAsset->OnSpawnBPDataGsCharacterBaseRemoveAllUpdate.BindRaw(this, &FSkillEditor::SpawnBPDataGsCharacterBaseRemoveAllUpdate);

		SkillEditorDataAsset->OnSpawnTransformUpdate.BindRaw(this, &FSkillEditor::SpawnTransformUpdate);
		SkillEditorDataAsset->OnSpawnBPDataRendering.BindRaw(this, &FSkillEditor::SpawnBPDataRendering);

		SkillEditorDataAsset->OnActorScaleUpdate.BindRaw(this, &FSkillEditor::ActorScaleUpdate);

		ResetSpawnBPDataGsCharacterBaseSettingUpdate();		
	}
}

void FSkillEditor::InitSpawnBPDataGsCharacterBaseSettingUpdate()
{
	if(PersonaToolkit.IsValid())
	{
		UWorld* World = PersonaToolkit->GetPreviewScene()->GetWorld();

		for (int32 i = 0;i < SkillEditorDataAsset->SpawnBPData.Num(); ++i)
		{
			UClass* bpClass = nullptr;
			FString BPPath = SkillEditorDataAsset->SpawnBPData[i].GsCharacterBase.ToString();
			bpClass = LoadObject<UClass>(nullptr, *BPPath);

			if (bpClass != nullptr)
			{
				FActorSpawnParameters spawnParam = FActorSpawnParameters();
				spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				spawnParam.Name = NAME_None;

				FTransform SpawnTranform = SkillEditorDataAsset->SpawnBPData[i].TM;

				SkillEditorDataAsset->SpawnBPData[i].SpawnActor = World->SpawnActor(bpClass, &SpawnTranform, spawnParam);
				SkillEditorDataAsset->SpawnBPData[i].PreGsCharacterBase = SkillEditorDataAsset->SpawnBPData[i].GsCharacterBase;

				if (SkillEditorDataAsset->SpawnBPData[i].SpawnActor != nullptr)
					SpawnActors.Add(SkillEditorDataAsset->SpawnBPData[i].SpawnActor);
			}
		}
	}
}

void FSkillEditor::ResetSpawnBPDataGsCharacterBaseSettingUpdate()
{
	for (int32 i = 0; i < SpawnActors.Num(); ++i)
	{
		SpawnActors[i]->Destroy();
		SpawnActors[i] = nullptr;
	}

	SpawnActors.Reset();

	if (PersonaToolkit.IsValid())
	{
		UWorld* World = PersonaToolkit->GetPreviewScene()->GetWorld();

		for (int32 i = 0;i < SkillEditorDataAsset->SpawnBPData.Num(); ++i)
		{
			SkillEditorDataAsset->SpawnBPData[i].PreGsCharacterBase.Reset();

			if (SkillEditorDataAsset->SpawnBPData[i].SpawnActor != nullptr)
			{
				SkillEditorDataAsset->SpawnBPData[i].SpawnActor->Destroy();
				SkillEditorDataAsset->SpawnBPData[i].SpawnActor = nullptr;
			}
		}
	}
}

void FSkillEditor::SpawnBPDataRendering()
{
	if (SkillEditorDataAsset == nullptr) return;

	if (PersonaToolkit.IsValid())
	{
		if (SkillEditorDataAsset->SpawnBPDataRendering)
		{
			InitSpawnBPDataGsCharacterBaseSettingUpdate();
		}
		else
		{
			ResetSpawnBPDataGsCharacterBaseSettingUpdate();
		}
	}
}

void FSkillEditor::SpawnTransformUpdate()
{
	if (SkillEditorDataAsset == nullptr) return;

	if (SkillEditorDataAsset->SpawnBPDataRendering == false) return;

	if (PersonaToolkit.IsValid())
	{
		UWorld* World = PersonaToolkit->GetPreviewScene()->GetWorld();

		for (int32 i = 0;i < SkillEditorDataAsset->SpawnBPData.Num(); ++i)
		{
			if (SkillEditorDataAsset->SpawnBPData[i].SpawnActor != nullptr)
				SkillEditorDataAsset->SpawnBPData[i].SpawnActor->SetActorTransform(SkillEditorDataAsset->SpawnBPData[i].TM);
		}
	}
}

void FSkillEditor::SpawnBPDataGsCharacterBaseRemoveAllUpdate()
{
	if (SkillEditorDataAsset == nullptr) return;

	if (SkillEditorDataAsset->SpawnBPDataRendering == false) return;

	for (int32 i = 0; i < SpawnActors.Num(); ++i)
	{
		SpawnActors[i]->Destroy();
		SpawnActors[i] = nullptr;
	}

	SpawnActors.Reset();
}

void FSkillEditor::SpawnBPDataGsCharacterBaseRemoveUpdate()
{
	if (SkillEditorDataAsset == nullptr) return;	
	
	if (PersonaToolkit.IsValid())
	{
		// 지워지고 남은 새로운 액터들을 찾는다.
		TArray<AActor*> NewActors;
		for (int32 i = 0; i < SpawnActors.Num(); ++i)
		{
			for (int32 j = 0; j < SkillEditorDataAsset->SpawnBPData.Num(); ++j)
			{
				if (SkillEditorDataAsset->SpawnBPData[j].SpawnActor == nullptr)
					continue;

				if (SpawnActors[i] == SkillEditorDataAsset->SpawnBPData[j].SpawnActor)
				{
					NewActors.Add(SpawnActors[i]);
					break;
				}
			}
		}

		// 기존에 스폰된 엑터리스트에서 새로운 액터 리스트에 없는것들은 삭제한다.
		for (int32 i = 0; i < SpawnActors.Num(); ++i)
		{
			int32 findIndex = NewActors.Find(SpawnActors[i]);
			if (findIndex < 0)
			{
				SpawnActors[i]->Destroy();
				SpawnActors[i] = nullptr;
			}
		}

		SpawnActors = NewActors;
	}
}

void FSkillEditor::SpawnBPDataGsCharacterBaseSettingUpdateNotInitPos()
{
	if (SkillEditorDataAsset == nullptr) return;

	if (PersonaToolkit.IsValid())
	{
		UWorld* World = PersonaToolkit->GetPreviewScene()->GetWorld();

		for (int32 i = 0;i < SkillEditorDataAsset->SpawnBPData.Num(); ++i)
		{
			// 1. 비어 있고, sspawn 했었으면 삭제하기.
			if (false == SkillEditorDataAsset->SpawnBPData[i].GsCharacterBase.IsValid())
			{
				if (SkillEditorDataAsset->SpawnBPData[i].SpawnActor != nullptr)
				{
					int32 findIndex = SpawnActors.Find(SkillEditorDataAsset->SpawnBPData[i].SpawnActor);
					if (0 <= findIndex)
					{
						SpawnActors.RemoveAt(findIndex);

						SkillEditorDataAsset->SpawnBPData[i].SpawnActor->Destroy();
						SkillEditorDataAsset->SpawnBPData[i].SpawnActor = nullptr;
						SkillEditorDataAsset->SpawnBPData[i].PreGsCharacterBase.Reset();
						SkillEditorDataAsset->SpawnBPData[i].TM = FTransform::Identity;
					}
				}
			}
			else
			{
				// BP Class same check 다르면
				if (SkillEditorDataAsset->SpawnBPData[i].GsCharacterBase.ToString() != SkillEditorDataAsset->SpawnBPData[i].PreGsCharacterBase.ToString())
				{
					int32 findIndex = SpawnActors.Find(SkillEditorDataAsset->SpawnBPData[i].SpawnActor);
					if (0 <= findIndex)
					{
						SpawnActors.RemoveAt(findIndex);

						SkillEditorDataAsset->SpawnBPData[i].SpawnActor->Destroy();
						SkillEditorDataAsset->SpawnBPData[i].SpawnActor = nullptr;
						SkillEditorDataAsset->SpawnBPData[i].PreGsCharacterBase.Reset();
						SkillEditorDataAsset->SpawnBPData[i].TM = FTransform::Identity;
					}

					UClass* bpClass = nullptr;
					FString BPPath = SkillEditorDataAsset->SpawnBPData[i].GsCharacterBase.ToString();
					bpClass = LoadObject<UClass>(nullptr, *BPPath);

					if (bpClass != nullptr)
					{
						FActorSpawnParameters spawnParam = FActorSpawnParameters();
						spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						spawnParam.Name = NAME_None;

						FTransform SpawnTranform = SkillEditorDataAsset->SpawnBPData[i].TM;

						if (SkillEditorDataAsset->SpawnBPDataRendering == true)
							SkillEditorDataAsset->SpawnBPData[i].SpawnActor = World->SpawnActor(bpClass, &SpawnTranform, spawnParam);

						SkillEditorDataAsset->SpawnBPData[i].PreGsCharacterBase = SkillEditorDataAsset->SpawnBPData[i].GsCharacterBase;

						if (SkillEditorDataAsset->SpawnBPData[i].SpawnActor != nullptr)
						{
							SpawnActors.Add(SkillEditorDataAsset->SpawnBPData[i].SpawnActor);

							// 초기 값 셋팅
							ACharacter* character = Cast<ACharacter>(SkillEditorDataAsset->SpawnBPData[i].SpawnActor);
							if (character)
							{
								UCapsuleComponent* capsuleComponent = character->GetCapsuleComponent();
								if (capsuleComponent)
								{
									SkillEditorDataAsset->SpawnBPData[i].SpawnActor->SetActorTransform(SkillEditorDataAsset->SpawnBPData[i].TM);
								}
							}
						}
						else
						{
							SkillEditorDataAsset->SpawnBPData[i].TM.SetRotation(FQuat(FRotator(0.0f, -90.0f, 0.0f)));
						}
					}
				}
			}
		}
	}
}

void FSkillEditor::SpawnBPDataGsCharacterBaseSettingUpdate()
{
	if (SkillEditorDataAsset == nullptr) return;
	
	if (PersonaToolkit.IsValid())
	{
		UWorld* World = PersonaToolkit->GetPreviewScene()->GetWorld();

		for (int32 i = 0;i < SkillEditorDataAsset->SpawnBPData.Num(); ++i)
		{
			// 1. 비어 있고, sspawn 했었으면 삭제하기.
			if (false == SkillEditorDataAsset->SpawnBPData[i].GsCharacterBase.IsValid())
			{
				if (SkillEditorDataAsset->SpawnBPData[i].SpawnActor != nullptr)
				{
					int32 findIndex = SpawnActors.Find(SkillEditorDataAsset->SpawnBPData[i].SpawnActor);
					if (0 <= findIndex)
					{
						SpawnActors.RemoveAt(findIndex);

						SkillEditorDataAsset->SpawnBPData[i].SpawnActor->Destroy();
						SkillEditorDataAsset->SpawnBPData[i].SpawnActor = nullptr;
						SkillEditorDataAsset->SpawnBPData[i].PreGsCharacterBase.Reset();
						SkillEditorDataAsset->SpawnBPData[i].TM = FTransform::Identity;
					}
				}
			}
			else
			{
				// BP Class same check 다르면
				if (SkillEditorDataAsset->SpawnBPData[i].GsCharacterBase.ToString() != SkillEditorDataAsset->SpawnBPData[i].PreGsCharacterBase.ToString())
				{
					int32 findIndex = SpawnActors.Find(SkillEditorDataAsset->SpawnBPData[i].SpawnActor);
					if (0 <= findIndex)
					{
						SpawnActors.RemoveAt(findIndex);

						SkillEditorDataAsset->SpawnBPData[i].SpawnActor->Destroy();
						SkillEditorDataAsset->SpawnBPData[i].SpawnActor = nullptr;
						SkillEditorDataAsset->SpawnBPData[i].PreGsCharacterBase.Reset();
						SkillEditorDataAsset->SpawnBPData[i].TM = FTransform::Identity;
					}

					UClass* bpClass = nullptr;
					FString BPPath = SkillEditorDataAsset->SpawnBPData[i].GsCharacterBase.ToString();
					bpClass = LoadObject<UClass>(nullptr, *BPPath);

					if (bpClass != nullptr)
					{
						FActorSpawnParameters spawnParam = FActorSpawnParameters();
						spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						spawnParam.Name = NAME_None;

						FTransform SpawnTranform = SkillEditorDataAsset->SpawnBPData[i].TM;

						if (SkillEditorDataAsset->SpawnBPDataRendering == true)
							SkillEditorDataAsset->SpawnBPData[i].SpawnActor = World->SpawnActor(bpClass, &SpawnTranform, spawnParam);

						SkillEditorDataAsset->SpawnBPData[i].PreGsCharacterBase = SkillEditorDataAsset->SpawnBPData[i].GsCharacterBase;

						if (SkillEditorDataAsset->SpawnBPData[i].SpawnActor != nullptr)
						{
							SpawnActors.Add(SkillEditorDataAsset->SpawnBPData[i].SpawnActor);

							// 초기 값 셋팅
							ACharacter* character = Cast<ACharacter>(SkillEditorDataAsset->SpawnBPData[i].SpawnActor);
							if (character)
							{
								UCapsuleComponent* capsuleComponent = character->GetCapsuleComponent();
								if (capsuleComponent)
								{
									SkillEditorDataAsset->SpawnBPData[i].TM.SetLocation(FVector(0.0f, 500.0f, capsuleComponent->GetScaledCapsuleHalfHeight()));
									SkillEditorDataAsset->SpawnBPData[i].TM.SetRotation(FQuat(FRotator(0.0f, -90.0f, 0.0f)));

									SkillEditorDataAsset->SpawnBPData[i].SpawnActor->SetActorTransform(SkillEditorDataAsset->SpawnBPData[i].TM);
								}
							}
						}
						else
						{
							// actor을 스폰하기전이라 ScaledCapsuleHalfHeight값을 알수가 없다 제일 보편적인 평균값인 90.0f를 셋팅한다.
							SkillEditorDataAsset->SpawnBPData[i].TM.SetLocation(FVector(0.0f, 500.0f, 90.0f));
							SkillEditorDataAsset->SpawnBPData[i].TM.SetRotation(FQuat(FRotator(0.0f, -90.0f, 0.0f)));
						}
					}
				}
			}
		}
	}
}

void FSkillEditor::ActorScaleUpdate()
{
	if (SkillEditorDataAsset == nullptr) return;

	if (PersonaToolkit.IsValid())
	{
		AActor* Actor = PersonaToolkit->GetPreviewScene()->GetActor();
		if (Actor != nullptr)
		{
			FVector ScaleVec = FVector(1, 1, 1) * SkillEditorDataAsset->ActorScale;
			Actor->SetActorScale3D(ScaleVec);
		}
	}
}

// AnimSequence전용 Montage는 Section클릭으로 보여지며, AnimSequence는 애초에 Section이 없는걸 만들기에...트리거가 없다.
void FSkillEditor::RefreshSkillSetTabEx(bool InTryInvokeTab)
{	
	SkillSetTab->SetContent(SNullWidget::NullWidget);
	SelectedSkillSet = nullptr;

	if (SkillEditorDataAsset)
	{
		if (SkillEditorDataAsset->ListSkillSetData.Num() > 0)			
		{
			if (SkillEditorDataAsset->AnimMontage != nullptr
				|| SkillEditorDataAsset->AnimSequence != nullptr)
			{
				if (SkillEditorDataAsset->ActionCategory == ActionCategory::SKILL)
				{
					USkillSectionData* SkillSectionData = Cast<USkillSectionData>(SkillEditorDataAsset->ListSkillSetData[0]);
					SkillSetDetailsView->SetObject(SkillSectionData, true);
				}
				else if (SkillEditorDataAsset->ActionCategory == ActionCategory::COMMON)
				{
					UCommonSectionData* CommonSectionData = Cast<UCommonSectionData>(SkillEditorDataAsset->ListSkillSetData[0]);
					SkillSetDetailsView->SetObject(CommonSectionData, true);
				}
				else if (SkillEditorDataAsset->ActionCategory == ActionCategory::REACTION)
				{
					UReactionSectionData* ReactionSectionData = Cast<UReactionSectionData>(SkillEditorDataAsset->ListSkillSetData[0]);
					SkillSetDetailsView->SetObject(ReactionSectionData, true);
				}

				SelectedSkillSet = SkillEditorDataAsset->ListSkillSetData[0];
				SkillSetTab->SetContent(SkillSetDetailsView.ToSharedRef());
				if(InTryInvokeTab)
					TabManager->TryInvokeTab(SkillSetTabID);
			}
		}
	}
}

void FSkillEditor::RefreshSkillSetTab()
{
	SkillSetTab->SetContent(SNullWidget::NullWidget);
	SelectedSkillSet = nullptr;

	if (SkillEditorDataAsset)
	{
		if (SkillEditorDataAsset->ListSkillSetData.Num() > 0)
		{
			if (SkillEditorDataAsset->AnimMontage != nullptr
				|| SkillEditorDataAsset->AnimSequence != nullptr)
			{
				if (SkillEditorDataAsset->ActionCategory == ActionCategory::SKILL)
				{
					USkillSectionData* SkillSectionData = Cast<USkillSectionData>(SkillEditorDataAsset->ListSkillSetData[0]);
					SkillSetDetailsView->SetObject(SkillSectionData, true);
				}
				else if (SkillEditorDataAsset->ActionCategory == ActionCategory::COMMON)
				{
					UCommonSectionData* CommonSectionData = Cast<UCommonSectionData>(SkillEditorDataAsset->ListSkillSetData[0]);
					SkillSetDetailsView->SetObject(CommonSectionData, true);
				}
				else if (SkillEditorDataAsset->ActionCategory == ActionCategory::REACTION)
				{
					UReactionSectionData* ReactionSectionData = Cast<UReactionSectionData>(SkillEditorDataAsset->ListSkillSetData[0]);
					SkillSetDetailsView->SetObject(ReactionSectionData, true);
				}

				SelectedSkillSet = SkillEditorDataAsset->ListSkillSetData[0];
				SkillSetTab->SetContent(SkillSetDetailsView.ToSharedRef());
				TabManager->TryInvokeTab(SkillSetTabID);
			}
		}
	}
}

void FSkillEditor::RefreshSkillDataAssetTab()
{
	if (SkillEditorDataAsset)
	{
		DataActorComponentView->SetObject(SkillEditorDataAsset, true);
		SkillEditorDataPropertyTab->SetContent(CreateSkillEditorDataAssetVeiw());
	}
}

void FSkillEditor::RefreshMontageTab()
{
	MontageTab->SetContent(SNullWidget::NullWidget);

	if (SkillEditorDataAsset)
	{		
		AnimationAsset = nullptr;
		if (SkillEditorDataAsset->AnimMontageSkill != nullptr)
			AnimationAsset = SkillEditorDataAsset->AnimMontageSkill;
		else
		if (SkillEditorDataAsset->AnimSequenceSkill != nullptr)
			AnimationAsset = SkillEditorDataAsset->AnimSequenceSkill;

		if (AnimationAsset != nullptr)
		{
			FPersonaModule& PersonaModule = FModuleManager::LoadModuleChecked<FPersonaModule>("Persona");
			PersonaToolkit = PersonaModule.CreatePersonaToolkit(AnimationAsset);

			PersonaToolkit->GetPreviewScene()->SetDefaultAnimationMode(EPreviewSceneDefaultAnimationMode::Animation);
			PersonaToolkit->GetPreviewScene()->SetSkillMode(true);

			// Montage setting
			FSkeletonTreeArgs SkeletonTreeArgs;
			SkeletonTreeArgs.PreviewScene = PersonaToolkit->GetPreviewScene();
			ISkeletonEditorModule& SkeletonEditorModule = FModuleManager::GetModuleChecked<ISkeletonEditorModule>("SkeletonEditor");
			SkeletonTree = SkeletonEditorModule.CreateSkeletonTree(PersonaToolkit->GetSkeleton(), SkeletonTreeArgs);

			FString	DocumentLink;
			FAnimDocumentArgs Args(PersonaToolkit->GetPreviewScene(), GetPersonaToolkit(), SkeletonTree->GetEditableSkeleton(), OnPostUndo, OnSectionsChanged);
			Args.OnDespatchObjectsSelected = FOnObjectsSelected::CreateSP(this, &FSkillEditor::HandleObjectsSelected);
			Args.OnDespatchNotifiesChanged = FSimpleDelegate::CreateSP(this, &FSkillEditor::HandleNotifiesChanged);			
			Args.OnDespatchNotifiesChangedEx = FOnObjectsSelected::CreateSP(this, &FSkillEditor::HandleNotifiesChangedEx);

			ToolkitCommands = MakeShareable(new FUICommandList());

			TSharedRef<SWidget> Contents = PersonaModule.CreateEditorWidgetForAnimDocument(ToolkitCommands->AsShared(), AnimationAsset, Args, DocumentLink);
			MontageTab->SetContent(Contents);

			TSharedRef<SWidget> MontageSectionContents = PersonaModule.CreateAnimMontageSectionsTabFactory(GetPersonaToolkit(), OnSectionsChanged);
			MontageSectionTab->SetContent(MontageSectionContents);
		}
	}
}

void FSkillEditor::RefreshAnimationAssetTab()
{
	//AnimationAssetTab->SetContent(SNullWidget::NullWidget);

	//if (SkillEditorDataAsset)
	//{
	//	if (SkillEditorDataAsset->AnimMontageSkill != nullptr || SkillEditorDataAsset->AnimSequenceSkill != nullptr)
	//	{
	//		//SkillEditorDataAsset->AnimMontage->GetPathName

	//		UAnimationAsset* originamAnimationAsset = PersonaToolkit.Get()->GetAnimationAsset();			

	//		UObject* Obj = originamAnimationAsset;
	//		if (Obj == nullptr) return;
	//		if (Obj->IsA(UAnimationAsset::StaticClass()))
	//		{
	//			OriginamAnimationAssetView->SetObject(AnimationAsset);
	//			AnimationAssetTab->SetContent(OriginamAnimationAssetView.ToSharedRef());
	//		}
	//	}
	//}
}

void FSkillEditor::RefreshPreviewSceneTab()
{	
	//PreviewSceneTab->SetContent(SNullWidget::NullWidget);

	//if (SkillEditorDataAsset)
	//{
	//	if (SkillEditorDataAsset->AnimMontageSkill != nullptr || SkillEditorDataAsset->AnimSequenceSkill != nullptr)
	//	{
	//		FPersonaModule& PersonaModule = FModuleManager::GetModuleChecked<FPersonaModule>("Persona");
	//		TSharedRef<SWidget> TabContents = PersonaModule.CreateAdvancedPreviewSceneTabFactory(PersonaToolkit->GetPreviewScene());

	//		PreviewSceneTab->SetContent(TabContents);
	//	}
	//}
}

//TSharedRef<class IDetailCustomization> FSkillEditor::CustomizePreviewSceneDescription()
//{
//	TSharedRef<IPersonaPreviewScene> PreviewSceneRef = PersonaToolkit.Get()->GetPreviewScene();
//	FString SkeletonName;
//	TSharedPtr<IEditableSkeleton> EditableSkeleton = PreviewSceneRef->GetPersonaToolkit()->GetEditableSkeleton();
//	if (EditableSkeleton.IsValid())
//	{
//		SkeletonName = FAssetData(&EditableSkeleton->GetSkeleton()).GetExportTextName();
//	}
//	return MakeShareable(new FPreviewSceneDescriptionCustomization(SkeletonName, PreviewSceneRef->GetPersonaToolkit()));
//}

//TSharedRef<class IPropertyTypeCustomization> FSkillEditor::CustomizePreviewMeshCollectionEntry()
//{
//	return MakeShareable(new FPreviewMeshCollectionEntryCustomization(PreviewScene.Pin().ToSharedRef()));
//}

void FSkillEditor::RefreshViewportTab()
{
	SkillEditorViewportTab->SetContent(SNullWidget::NullWidget);

	if (SkillEditorDataAsset)
	{
		if (SkillEditorDataAsset->AnimMontageSkill != nullptr || SkillEditorDataAsset->AnimSequenceSkill != nullptr)
		{
			Viewport = SNew(SSkillEditorViewport)
				.ParentMyEditor(SharedThis(this))
				.PreviewScene(PersonaToolkit.Get()->GetPreviewScene());

			SkillEditorViewportTab->SetContent(Viewport.ToSharedRef());
		}
	}
}

void FSkillEditor::RefreshSkillEditorTab()
{
	MontageTab->SetContent(SNullWidget::NullWidget);
	PreviewSceneTab->SetContent(SNullWidget::NullWidget);
	AnimationAssetTab->SetContent(SNullWidget::NullWidget);
	SkillEditorViewportTab->SetContent(SNullWidget::NullWidget);
	SkillNotifyTab->SetContent(SNullWidget::NullWidget);

	RefreshSkillDataAssetTab();

	RefreshMontageTab();

	RefreshPreviewSceneTab();

	RefreshAnimationAssetTab();

	RefreshViewportTab();

	RefreshSkillSetTab();

	RootWindow->GetNativeWindow()->SetWindowFocus();
}

// 컬리전 체크할 컴퍼넌트 생성 및 add
void FSkillEditor::AddSkillCollisionComponent()
{
	if (false == PersonaToolkit.IsValid()) return;

	if (SkillEditorDataAsset)
	{
		AActor* actor = GetPersonaToolkit()->GetPreviewScene()->GetPreviewMeshComponent()->GetOwner();
		if (actor)
		{
			UGsSkillCollisionComponent* component = Cast<UGsSkillCollisionComponent>(actor->GetComponentByClass(UGsSkillCollisionComponent::StaticClass()));
			if (nullptr == component)
			{
				component = NewObject<UGsSkillCollisionComponent>(actor);
				component->SetupAttachment(actor->GetRootComponent());
				component->RegisterComponent();
				component->_useDebugLine = true;
				component->_defaultRotator = FRotator(0.f, 90.f, 0.f);
			}
		}
	}
}

// 체크할 컬리전 정보로 타겟 캐릭터 체크(다수임, 개당)
void FSkillEditor::SpawnActorCollisionUpdate()
{
	if (false == PersonaToolkit.IsValid()) return;

	AActor* actor = GetPersonaToolkit()->GetPreviewScene()->GetPreviewMeshComponent()->GetOwner();
	if (actor)
	{
		UGsSkillCollisionComponent* component = Cast<UGsSkillCollisionComponent>(actor->GetComponentByClass(UGsSkillCollisionComponent::StaticClass()));
		if (component)
		{
			for (int32 i = 0; i < SkillEditorDataAsset->ListSkillSetData.Num(); ++i)
			{
				USkillMetaData*& SkillMetaData = SkillEditorDataAsset->ListSkillSetData[i];
				USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);

				if (SkillSectionData != nullptr)
				{
					for (int32 j = 0; j < SkillSectionData->SkillSetData.collisionInfoList.Num(); ++j)
					{
						component->Set(SkillSectionData->SkillSetData.collisionInfoList[j]);
						// 타켓 캐릭터 충돌시 반응
						for (int32 k = 0; k < SkillEditorDataAsset->SpawnBPData.Num(); ++k)
						{
							if (AGsCharacterBase* castActor = Cast<AGsCharacterBase>(SkillEditorDataAsset->SpawnBPData[k].SpawnActor))
							{
								// 충돌시 효과
								if (component->IsInsideCheck(castActor->GetActorLocation(), castActor->GetCapsuleComponent()->GetUnscaledCapsuleRadius()))
								{
									UGsMeshShaderControlComponent* meshShaderComponent = castActor->GetMeshShaderComponent();
									meshShaderComponent->Add(EGsMeshShaderEffect::Fresnel, castActor->GetMesh(), CurveFloat);
									meshShaderComponent->Start();

									castActor->OnHit(HitResultType::NORMAL, EGsStateBase::Idle, nullptr);
									castActor->OnHitEffect(actor->GetActorLocation(), HitResultType::NORMAL,
										nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
									UGsHitEffectComponent* hitComponent = castActor->GetHitEffect();
									const FGsSchemaSoundResData* soundInfo = GlobalConstant->HitSoundData.GetRow();
									hitComponent->OverrideData(GlobalConstant->HitEffectData, soundInfo, 0.f, 0.f);
									hitComponent->OnHitEffect(actor->GetActorLocation());
								}
							}
						}
					}
				}
			}
		}
	}
}

void FSkillEditor::OnListMouseButtonDoubleClick(TSharedPtr<FSkillAssetViewItem> AssetItem)
{
	if (!AssetItem.IsValid())
	{
		MontageTab->SetContent(SNullWidget::NullWidget);
		PreviewSceneTab->SetContent(SNullWidget::NullWidget);
		AnimationAssetTab->SetContent(SNullWidget::NullWidget);
		SkillEditorViewportTab->SetContent(SNullWidget::NullWidget);
		SkillNotifyTab->SetContent(SNullWidget::NullWidget);
		SkillSetTab->SetContent(SNullWidget::NullWidget);
		MontageSectionTab->SetContent(SNullWidget::NullWidget);

		return;
	}

	if (AssetItem->GetType() == ESkillAssetItemType::Folder) return;
	if (AssetItem->IsTemporaryItem()) return;	

	ResetSpawnBPDataGsCharacterBaseSettingUpdate();
	SpawnBPDataGsCharacterBaseSettingUpdateNotInitPos();

	const FAssetData& AssetViewAsset = StaticCastSharedPtr<FSkillAssetViewAsset>(AssetItem)->Data;
	SkillEditorDataAsset = Cast<USkillEditorDataAsset>(AssetViewAsset.GetAsset());
	if (SkillEditorDataAsset == nullptr) return;	

	CurrentFAssetData = AssetViewAsset;
	CurrentSkillAssetViewItem = AssetItem;
	CurrentPackageName = AssetViewAsset.PackageName;

	// Event Bind
	SkillEditorDataAsset->OnChangeEdited.Unbind();
	SkillEditorDataAsset->OnChangeSectionCategoryEdited.Unbind();
	SkillEditorDataAsset->OnNotifyIdChangedEx.Unbind();

	SkillEditorDataAsset->OnChangeEdited.BindRaw(this, &FSkillEditor::RefreshSkillEditorTab);
	SkillEditorDataAsset->OnChangeSectionCategoryEdited.BindRaw(this, &FSkillEditor::RefreshSkillSetTab);
	SkillEditorDataAsset->OnNotifyIdChangedEx.BindRaw(this, &FSkillEditor::HandleNotifiesChangedEx);

	SkillEditorDataAsset->OnSpawnBPDataGsCharacterBaseSettingUpdate.BindRaw(this, &FSkillEditor::SpawnBPDataGsCharacterBaseSettingUpdate);
	SkillEditorDataAsset->OnSpawnBPDataGsCharacterBaseRemoveUpdate.BindRaw(this, &FSkillEditor::SpawnBPDataGsCharacterBaseRemoveUpdate);
	SkillEditorDataAsset->OnSpawnBPDataGsCharacterBaseRemoveAllUpdate.BindRaw(this, &FSkillEditor::SpawnBPDataGsCharacterBaseRemoveAllUpdate);

	SkillEditorDataAsset->OnSpawnTransformUpdate.BindRaw(this, &FSkillEditor::SpawnTransformUpdate);
	SkillEditorDataAsset->OnSpawnBPDataRendering.BindRaw(this, &FSkillEditor::SpawnBPDataRendering);

	SkillEditorDataAsset->OnActorScaleUpdate.BindRaw(this, &FSkillEditor::ActorScaleUpdate);

	SelectedSkillAssetViewItemItem = AssetItem;

	SkillEditorDataAsset->Selected();
	//SkillEditorDataAsset->Modify();

	// 애셋 리스트 뷰에서 선택한 애셋만 표시 기능(선택, 수정)보이게 수정
	SelectedSkillAssetViewItemItem->OnAssetDataChanged.Broadcast();
	ListView->RequestListRefresh();

	RefreshSkillEditorTab();

	ResetSpawnBPDataGsCharacterBaseSettingUpdate();
	SpawnBPDataGsCharacterBaseSettingUpdateNotInitPos();

	AddSkillCollisionComponent();

	ActorScaleUpdate();
}

TSharedRef<SWidget> FSkillEditor::CreateShadowOverlay(TSharedRef<STableViewBase> Table)
{
	return SNew(SScrollBorder, Table)
		[
			Table
		];
}

TSharedRef<SSkillAssetListView> FSkillEditor::CreateListView()
{
	return SNew(SSkillAssetListView)
		.SelectionMode(ESelectionMode::Single)
		.ListItemsSource(&FilteredAssetItems)
		.OnGenerateRow(this, &FSkillEditor::MakeListViewWidget)
		.OnItemScrolledIntoView(this, &FSkillEditor::ItemScrolledIntoView)
		//.OnContextMenuOpening(this, &FSkillEditor::OnGetContextMenuContent)
		.OnContextMenuOpening(this, &FSkillEditor::MakeExportContextMenu)
		.OnMouseButtonDoubleClick(this, &FSkillEditor::OnListMouseButtonDoubleClick)
		.OnSelectionChanged(this, &FSkillEditor::AssetSelectionChanged)
		.ItemHeight(this, &FSkillEditor::GetListViewItemHeight);
}

float FSkillEditor::GetListViewItemHeight() const
{
	return 16.0f;
	// Get desktop metrics
	//FDisplayMetrics DisplayMetrics;
	//FSlateApplication::Get().GetCachedDisplayMetrics(DisplayMetrics);

	//const FVector2D DisplaySize(
	//	DisplayMetrics.PrimaryDisplayWorkAreaRect.Right - DisplayMetrics.PrimaryDisplayWorkAreaRect.Left,
	//	DisplayMetrics.PrimaryDisplayWorkAreaRect.Bottom - DisplayMetrics.PrimaryDisplayWorkAreaRect.Top);

	//const float ThumbnailScaleRangeScalar = (DisplaySize.Y / 1080);
	////return (ListViewThumbnailSize + ListViewThumbnailPadding * 2) * FMath::Lerp(MinThumbnailScale, MaxThumbnailScale, GetThumbnailScale());
	//return 1.0f;// (64 + 4 * 2) * FMath::Lerp(0.2f * ThumbnailScaleRangeScalar, 2.0f * ThumbnailScaleRangeScalar, 1.0f);
}

void FSkillEditor::AssetSelectionChanged(TSharedPtr< struct FSkillAssetViewItem > AssetItem, ESelectInfo::Type SelectInfo)
{
	if(AssetItem != nullptr)
		SelectedSkillAssetViewItemItem = AssetItem;	
}

void FSkillEditor::PostUndo(bool bSuccess)
{
	OnPostUndo.Broadcast();
}

void FSkillEditor::PostRedo(bool bSuccess)
{
	OnPostUndo.Broadcast();
}

void FSkillEditor::CollisionInfoPrint()
{
	if (Viewport.IsValid())
	{
		FString strResult;// = TEXT("CollisionInfo : Empty");

		if (SkillEditorDataAsset != nullptr)
		{
			if (SkillEditorDataAsset->ActionCategory == ActionCategory::SKILL)
			{
				// 1. 충돌 정보 출력
				strResult.Append(TEXT("\n"));

				for (int32 i = 0; i < SkillEditorDataAsset->ListSkillSetData.Num(); ++i)
				{
					USkillSectionData* SkillSectionData = Cast<USkillSectionData>(SkillEditorDataAsset->ListSkillSetData[i]);
					if (SkillSectionData != nullptr)
					{
						strResult += FString::Printf(TEXT("Section : %d\n"), i);

						for (int32 j = 0; j < SkillSectionData->SkillSetData.collisionInfoList.Num(); ++j)
						{
							strResult += FString::Printf(TEXT("collisionInfoList : %d\n"), j);
							strResult.Append(TEXT("Type : "));
							switch (SkillSectionData->SkillSetData.collisionInfoList[j].type)
							{
							case SkillCollisionType::CIRCLE:
								strResult.Append(TEXT("CIRCLE\n"));
								break;
							case SkillCollisionType::FAN:
								strResult.Append(TEXT("FAN\n"));
								break;
							case SkillCollisionType::RECT:
								strResult.Append(TEXT("RECT\n"));
								break;
							case SkillCollisionType::NONE:
								strResult.Append(TEXT("NONE\n"));
								break;
							}

							strResult += FString::Printf(TEXT("PosOffset : %d\n"), SkillSectionData->SkillSetData.collisionInfoList[j].posOffset);
							strResult += FString::Printf(TEXT("offsetX : %d\n"), SkillSectionData->SkillSetData.collisionInfoList[j].offsetX);
							strResult += FString::Printf(TEXT("offsetY : %d\n"), SkillSectionData->SkillSetData.collisionInfoList[j].offsetY);
							strResult += FString::Printf(TEXT("AngleOffset : %d\n"), SkillSectionData->SkillSetData.collisionInfoList[j].angleOffset);
							strResult += FString::Printf(TEXT("AngleOffset2 : %d\n"), SkillSectionData->SkillSetData.collisionInfoList[j].angleOffset2);
							strResult += FString::Printf(TEXT("Width : %d\n"), SkillSectionData->SkillSetData.collisionInfoList[j].width);
							strResult += FString::Printf(TEXT("Height : %d\n"), SkillSectionData->SkillSetData.collisionInfoList[j].height);
							strResult += FString::Printf(TEXT("MinRadius : %d\n"), SkillSectionData->SkillSetData.collisionInfoList[j].minRadius);
							strResult += FString::Printf(TEXT("MaxRadius : %d\n"), SkillSectionData->SkillSetData.collisionInfoList[j].maxRadius);
							strResult += FString::Printf(TEXT("Angle : %d\n"), SkillSectionData->SkillSetData.collisionInfoList[j].angle);

							strResult.Append(TEXT("\n"));
						}

						strResult.Append(TEXT("\n"));
					}
				}
			}
		}

		Viewport->PrintText(strResult);
	}
}

void FSkillEditor::Tick(float DeltaTime)
{
	if(PersonaToolkit.IsValid())
		GetPersonaToolkit()->GetPreviewScene()->InvalidateViews();

	CollisionInfoPrint();

	SpawnActorCollisionUpdate();
}

TStatId FSkillEditor::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FSkillEditor, STATGROUP_Tickables);
}

void FSkillEditor::OnChangeSkillSetData()
{
	//RefreshEditor();//RefreshSkillAssetData();
}

void FSkillEditor::HandleNotifiesChangedEx(const TArray<UObject*>& InObjects)
{
	SkillEditorDataAsset->NotifiesChangedEx(InObjects);

	RefreshSkillSetTabEx(false);
}

void FSkillEditor::HandleNotifiesChanged()
{
	if (SkillEditorDataAsset)
	{
		// 만약에 노티파이가 없다면 View초기화
		if (SkillEditorDataAsset->SkillNotifies.Num() < 1)
			SkillNotifyTab->SetContent(SNullWidget::NullWidget);
	}

	if (SkillEditorDataAsset->ActionCategory == ActionCategory::SKILL)
	{
		USkillSectionData* SkillSectionData = Cast<USkillSectionData>(SelectedSkillSet);
		SkillSetDetailsView->SetObject(SkillSectionData, true);
	}
	else if (SkillEditorDataAsset->ActionCategory == ActionCategory::COMMON)
	{
		UCommonSectionData* CommonSectionData = Cast<UCommonSectionData>(SelectedSkillSet);
		SkillSetDetailsView->SetObject(CommonSectionData, true);
	}
	else if (SkillEditorDataAsset->ActionCategory == ActionCategory::REACTION)
	{
		UReactionSectionData* ReactionSectionData = Cast<UReactionSectionData>(SelectedSkillSet);
		SkillSetDetailsView->SetObject(ReactionSectionData, true);
	}
	
	SkillSetTab->SetContent(SkillSetDetailsView.ToSharedRef());
	SkillSetTab->SetContent(SkillSetDetailsView.ToSharedRef());
}

void FSkillEditor::HandleSectionsChanged()
{
	//if (SkillEditorDataAsset)
	//{
	//	SkillEditorDataAsset->SectionsChanged();
	//}
}

// 스킬 노티파이가 추가 되거나 삭제 될때 호출
void FSkillEditor::RefreshEditor()
{/*
	if (SkillEditorDataAsset != nullptr && SkillEditorDataAsset->AnimMontage != nullptr)
	{
		SkillEditorTools::MontageEditorDataToEditorData(SkillEditorDataAsset);
		UAnimMontage* AnimMontage = Cast<UAnimMontage>(SkillEditorDataAsset->AnimMontage);
		if (AnimMontage == nullptr) return;

		TArray<USkillNotify*> SkillNotifys = SkillEditorTools::GetSkillNotifys(AnimMontage);
		for (int32 index = 0; index < SkillNotifys.Num(); ++index)
		{
			if (SkillNotifys[index] != nullptr)
			{
				if (!SkillNotifys[index]->OnChangeSkillNotify.IsBound())
				{
					SkillNotifys[index]->OnChangeSkillNotify = FSimpleDelegate::CreateRaw(this, &FSkillEditor::HandleNotifiesChanged);
				}
			}
		}
	}*/
}

// 매칭되는 SkillSet Data UI 활성화
// 언리얼 에디터에서 지원하는 셀렉트한 오브젝트 지원하는 콜백함수를 사용했는데(엔진 빌드 최소화 하기위해)
// 해당 데이터가 현재 선택된 데이터가 오는게 아니라 전데이터가 오는 부분을 확인했다.다만 받고 나면(해당
// 오브젝트랑 UI연결된 포인터로 갱신을 다시 해주는 시스템) 전데이터가 오고 한번더 클릭해야지 본데이터가
// 오는 문제가 있다.이건 일단 엔진, 엔진에디터 수정이 필요한것으로 판단이 榮.다만 지금 하면 엔진 빌드
// 시간의 문제로 인해 일단 작업을 모두 마친후 다시 엔진 수정, 콜백 연동 작업을 다시 할 예정이다.)
void FSkillEditor::HandleObjectsSelected(const TArray<UObject*>& InObjects)
{
	if (InObjects.Num() < 1) return;	

	UObject* Obj = InObjects[0];
	if (Obj == nullptr) return;
	//SelectedObj = InObjects[0];
	if (Obj->IsA(USkillNotify::StaticClass()))
	{		
		NoitfyDetailsView->SetObjects(InObjects);		
		SkillNotifyTab->SetContent(NoitfyDetailsView.ToSharedRef());
		TabManager->TryInvokeTab(SkillNotifyTabID);
	}
	// 현재는 스킬 노티파이와 섹션밖에 없다. 다만 섹션외에 추가가되면 섹션은 오브젝트 상속구조가 아니라 위와같은
	// 체크방식은 불가능하기에 다른 방법을 강구해야된다.
	else
	{		
		UEditorCompositeSection* EditorCompositeSection = Cast<UEditorCompositeSection>(Obj);
		if (EditorCompositeSection)
		{
			int32 SectionIndex = SkillEditorDataAsset->IsDivide() == true ? EditorCompositeSection->SectionIndex : 0;
			if (SkillEditorDataAsset->ListSkillSetData.IsValidIndex(SectionIndex))
			{
				if (SkillEditorDataAsset->ActionCategory == ActionCategory::SKILL)
				{
					USkillSectionData* SkillSectionData = Cast<USkillSectionData>(SkillEditorDataAsset->ListSkillSetData[SectionIndex]);
					SkillSetDetailsView->SetObject(SkillSectionData, true);
				}
				else if (SkillEditorDataAsset->ActionCategory == ActionCategory::COMMON)
				{
					UCommonSectionData* CommonSectionData = Cast<UCommonSectionData>(SkillEditorDataAsset->ListSkillSetData[SectionIndex]);
					SkillSetDetailsView->SetObject(CommonSectionData, true);
				}
				else if (SkillEditorDataAsset->ActionCategory == ActionCategory::REACTION)
				{
					UReactionSectionData* ReactionSectionData = Cast<UReactionSectionData>(SkillEditorDataAsset->ListSkillSetData[SectionIndex]);
					SkillSetDetailsView->SetObject(ReactionSectionData, true);
				}

				SelectedSkillSet = SkillEditorDataAsset->ListSkillSetData[SectionIndex];
				SkillSetTab->SetContent(SkillSetDetailsView.ToSharedRef());
				TabManager->TryInvokeTab(SkillSetTabID);
			}						
		}
	}
}

FReply FSkillEditor::RefreshSkillAssetData()
{
	// 몽타주 에디터 정보로 갱신(실시간 반영)
	if (SkillEditorDataAsset != nullptr && SkillEditorDataAsset->AnimMontage != nullptr)
	{
		SkillEditorTools::MontageEditorDataToEditorData(SkillEditorDataAsset);
	}	

	return FReply::Handled();
}

void FSkillEditor::AssetItemWidgetDestroyed(const TSharedPtr<FSkillAssetViewItem>& Item)
{
	//AssetItems.Remove(Item);

	//Item
	//const FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	//const FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	//const FString seletedPath = GetSelectedPaths()[0];
	//FString PathNameWithFilename = seletedPath + TEXT("/") + LOCTEXT("NewSkillEditorDataName_Default", "NewSkillEditorData").ToString();

	//FString PackageName = seletedPath + TEXT("/");
	//FString DefaultSuffix = TEXT("_");
	//FString AssetName;

	//// 유니크 이름 생성
	//AssetToolsModule.Get().CreateUniqueAssetName(PathNameWithFilename, DefaultSuffix, PackageName, AssetName);

	//UPackage* Package = CreatePackage(NULL, *PackageName);

	//if (Package != nullptr)
	//{
	//	auto SkillDataContaineFactory = NewObject<USkillEditorDataAssetContainerFactoryNew>();
	//	USkillEditorDataAsset* skillEditorDataAsset = (USkillEditorDataAsset*)SkillDataContaineFactory->FactoryCreateNew(USkillEditorDataAsset::StaticClass(), Package, *AssetName, RF_Standalone | RF_Public, NULL, GWarn);
	//	if (skillEditorDataAsset != nullptr)
	//		AssetRegistryModule.AssetCreated(skillEditorDataAsset);
	//}

	//const FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	//const FAssetData& AssetViewAsset = StaticCastSharedPtr<FSkillAssetViewAsset>(Item)->Data;

	//DataActorComponentView->SetObject(nullptr);
	//SkillEditorDataPropertyTab->SetContent(SNullWidget::NullWidget);

	//// Asset skill editor data to use
	//SkillEditorDataAsset = Cast<USkillEditorDataAsset>(AssetViewAsset.GetAsset());

	//SkillEditorDataAsset->OnChangeEdited.Unbind();
	//SkillEditorDataAsset->OnChangeSkillEffectInfo.Unbind();
	//SkillEditorDataAsset->OnChangeSkillId.Unbind();

	//AssetRegistryModule.AssetDeleted(SkillEditorDataAsset);

	//RefreshSourceItems();
	/*if (Item.IsValid() && Item->IsTemporaryItem() && Item->GetType() != EAssetItemType::Folder)
	{
		const TSharedPtr<FAssetViewAsset>& ItemAsAsset = StaticCastSharedPtr<FAssetViewAsset>(Item);
	}*/
}

/**
* Nulls out references to a given object
*
* @param InObject - Object to null references to
*/
void FSkillEditor::NullReferencesToObject(UObject* InObject)
{
	TArray<UObject*> ReplaceableObjects;
	TMap<UObject*, UObject*> ReplacementMap;
	ReplacementMap.Add(InObject, NULL);
	ReplacementMap.GenerateKeyArray(ReplaceableObjects);

	// Find all the properties (and their corresponding objects) that refer to any of the objects to be replaced
	TMap< UObject*, TArray<FProperty*> > ReferencingPropertiesMap;
	for (FThreadSafeObjectIterator ObjIter; ObjIter; ++ObjIter)
	{
		UObject* CurObject = *ObjIter;

		// Find the referencers of the objects to be replaced
		FFindReferencersArchive FindRefsArchive(CurObject, ReplaceableObjects);

		// Inform the object referencing any of the objects to be replaced about the properties that are being forcefully
		// changed, and store both the object doing the referencing as well as the properties that were changed in a map (so that
		// we can correctly call PostEditChange later)
		TMap<UObject*, int32> CurNumReferencesMap;
		TMultiMap<UObject*, FProperty*> CurReferencingPropertiesMMap;
		if (FindRefsArchive.GetReferenceCounts(CurNumReferencesMap, CurReferencingPropertiesMMap) > 0)
		{
			TArray<FProperty*> CurReferencedProperties;
			CurReferencingPropertiesMMap.GenerateValueArray(CurReferencedProperties);
			ReferencingPropertiesMap.Add(CurObject, CurReferencedProperties);
			for (TArray<FProperty*>::TConstIterator RefPropIter(CurReferencedProperties); RefPropIter; ++RefPropIter)
			{
				CurObject->PreEditChange(*RefPropIter);
			}
		}

	}

	// Iterate over the map of referencing objects/changed properties, forcefully replacing the references and then
	// alerting the referencing objects the change has completed via PostEditChange
	int32 NumObjsReplaced = 0;
	for (TMap< UObject*, TArray<FProperty*> >::TConstIterator MapIter(ReferencingPropertiesMap); MapIter; ++MapIter)
	{
		++NumObjsReplaced;

		UObject* CurReplaceObj = MapIter.Key();
		const TArray<FProperty*>& RefPropArray = MapIter.Value();

		FArchiveReplaceObjectRef<UObject> ReplaceAr(CurReplaceObj, ReplacementMap, false, true, false);

		for (TArray<FProperty*>::TConstIterator RefPropIter(RefPropArray); RefPropIter; ++RefPropIter)
		{
			FPropertyChangedEvent PropertyEvent(*RefPropIter);
			CurReplaceObj->PostEditChangeProperty(PropertyEvent);
		}

		if (!CurReplaceObj->HasAnyFlags(RF_Transient) && CurReplaceObj->GetOutermost() != GetTransientPackage())
		{
			if (!CurReplaceObj->RootPackageHasAnyFlags(PKG_CompiledIn))
			{
				CurReplaceObj->MarkPackageDirty();
			}
		}
	}
}

void FSkillEditor::AssetRenameCommit(const FName& NewName)
{
	//if(SelectedSkillAssetViewItemItem)
	//UObject* Asset = nullptr;
	if (SelectedSkillAssetViewItemItem == nullptr) return;

	USkillEditorDataAsset* Asset = nullptr;
	const TSharedPtr<FSkillAssetViewAsset>& ItemAsAsset = StaticCastSharedPtr<FSkillAssetViewAsset>(SelectedSkillAssetViewItemItem);

	// 이름이 같은가?
	if (NewName.ToString().Equals(ItemAsAsset->Data.AssetName.ToString(), ESearchCase::CaseSensitive))
	//if(NewName == ItemAsAsset->Data.AssetName)
		return;

	Asset = Cast<USkillEditorDataAsset>(ItemAsAsset->Data.GetAsset());
	//Asset->Name = NewName;

	const FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	TArray<FAssetRenameData> AssetsAndNames;
	const FString PackagePath = FPackageName::GetLongPackagePath(Asset->GetOutermost()->GetName());
	new(AssetsAndNames) FAssetRenameData(Asset, PackagePath, NewName.ToString());
	AssetToolsModule.Get().RenameAssetsWithDialog(AssetsAndNames);

	RefreshSourceItems();
}

void FSkillEditor::AssetRenameCommit(const TSharedPtr<FSkillAssetViewItem>& Item, const FString& NewName, const FSlateRect& MessageAnchor, const ETextCommit::Type CommitType)
{
	//UObject* Asset = nullptr;
	USkillEditorDataAsset* Asset = nullptr;
	const TSharedPtr<FSkillAssetViewAsset>& ItemAsAsset = StaticCastSharedPtr<FSkillAssetViewAsset>(Item);

	// 이름이 같은가?
	if (NewName.Equals(ItemAsAsset->Data.AssetName.ToString(), ESearchCase::CaseSensitive))
		return;

	Asset = Cast<USkillEditorDataAsset>(ItemAsAsset->Data.GetAsset());
	//Asset->Name = *NewName;

	const FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	TArray<FAssetRenameData> AssetsAndNames;
	const FString PackagePath = FPackageName::GetLongPackagePath(Asset->GetOutermost()->GetName());
	new(AssetsAndNames) FAssetRenameData(Asset, PackagePath, NewName);
	AssetToolsModule.Get().RenameAssetsWithDialog(AssetsAndNames);

	RefreshSourceItems();
}

void FSkillEditor::DeleteSkillEditorDataAsset()
{	
	if (!SelectedSkillAssetViewItemItem.IsValid()) return;

	TArray<UObject*> ObjectsToDelete;
	const FAssetData& AssetViewAsset = StaticCastSharedPtr<FSkillAssetViewAsset>(SelectedSkillAssetViewItemItem)->Data;
	UObject *ObjectToDelete = AssetViewAsset.GetAsset();
	if (ObjectToDelete != nullptr)
	{
		ObjectsToDelete.Add(ObjectToDelete);
	}

	int32 NumObjectsToDelete = ObjectsToDelete.Num();
	if (NumObjectsToDelete > 0)
	{
		ObjectTools::ForceDeleteObjects(ObjectsToDelete, false);
	}

	MontageTab->SetContent(SNullWidget::NullWidget);
	PreviewSceneTab->SetContent(SNullWidget::NullWidget);
	AnimationAssetTab->SetContent(SNullWidget::NullWidget);
	SkillEditorViewportTab->SetContent(SNullWidget::NullWidget);

	RefreshSourceItems();	
}

void FSkillEditor::RenameSkillEditorDataAsset()
{
	SelectedSkillAssetViewItemItem->RenamedRequestEvent.ExecuteIfBound();
}

TSharedPtr<SWidget> FSkillEditor::MakeExportContextMenu()
{
	const bool bShouldCloseWindowAfterMenuSelection = true;
	TSharedPtr<FUICommandList> CommandList;
	CommandList = MakeShareable(new FUICommandList());
	FMenuBuilder MenuBuilder(bShouldCloseWindowAfterMenuSelection, CommandList);

	FSlateIcon DummyIcon(NAME_None, NAME_None);

	MenuBuilder.BeginSection("SkillEditorDataAsset", LOCTEXT("SkillEditorDataAssetHeader", "SkillEditorDataAssetFolder"));
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("SkillEditorAssetData_Delete", "Delete"),
			LOCTEXT("SkillEditorAssetDataExport_ToolTip", "Delete SkillEditorDataAsset"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FSkillEditor::DeleteSkillEditorDataAsset),
				FCanExecuteAction()
			)
		);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("SkillEditorAssetData_Copy", "Copy"),
			LOCTEXT("SkillEditorAssetDataExport_ToolTip", "Copy SkillEditorDataAsset"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FSkillEditor::CopySkillEditorDataAsset),
				FCanExecuteAction()
			)
		);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("SkillEditorAssetData_Pate", "Pate"),
			LOCTEXT("SkillEditorAssetDataExport_ToolTip", "Pate SkillEditorDataAsset"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FSkillEditor::PateSkillEditorDataAsset),
				FCanExecuteAction()
			)
		);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("SkillEditorAssetData_Rename", "Rename"),
			LOCTEXT("SkillEditorAssetDataExport_ToolTip", "Rename SkillEditorDataAsset"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FSkillEditor::RenameSkillEditorDataAsset),
				FCanExecuteAction()
			)
		);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("Transfer to DataCenter", "Transfer to DataCenter"),
			LOCTEXT("SkillEditorAssetDataTransferToDataCenter_ToolTip", "Transfer to DataCenter"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FSkillEditor::TransferToDatacenter),
				FCanExecuteAction()
			)
		);
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

TSharedPtr<SWidget> FSkillEditor::OnGetContextMenuContent()
{
	/*if (CanOpenContextMenu())
	{
		if (IsRenamingAsset())
		{
			RenamingAsset.Pin()->RenameCanceledEvent.ExecuteIfBound();
			RenamingAsset.Reset();
		}

		const TArray<FString> SelectedFolders = GetSelectedFolders();
		if (SelectedFolders.Num() > 0)
		{
			return OnGetFolderContextMenu.Execute(SelectedFolders, OnGetPathContextMenuExtender, FOnCreateNewFolder::CreateSP(this, &FSkillEditor::OnCreateNewFolder));
		}
		else
		{
			return OnGetAssetContextMenu.Execute(GetSelectedAssets());
		}
	}*/

	return NULL;
}

void FSkillEditor::ItemScrolledIntoView(TSharedPtr<struct FSkillAssetViewItem> AssetItem, const TSharedPtr<ITableRow>& Widget)
{
	if (AssetItem->bRenameWhenScrolledIntoview)
	{
		// Make sure we have window focus to avoid the inline text editor from canceling itself if we try to click on it
		// This can happen if creating an asset opens an intermediary window which steals our focus, 
		// eg, the blueprint and slate widget style class windows (TTP# 314240)
		/*TSharedPtr<SWindow> OwnerWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
		if (OwnerWindow.IsValid())
		{
			OwnerWindow->BringToFront();
		}*/

		//AwaitingRename = AssetItem;
	}
}

TSharedRef<ITableRow> FSkillEditor::MakeListViewWidget(TSharedPtr<FSkillAssetViewItem> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!ensure(AssetItem.IsValid()))
	{
		return SNew(STableRow<TSharedPtr<FSkillAssetViewAsset>>, OwnerTable);
	}

	//VisibleItems.Add(AssetItem);
	//bPendingUpdateThumbnails = true;

	if (AssetItem->GetType() == ESkillAssetItemType::Folder)
	{
		TSharedPtr< STableRow<TSharedPtr<FSkillAssetViewItem>> > TableRowWidget;
		SAssignNew(TableRowWidget, STableRow<TSharedPtr<FSkillAssetViewItem>>, OwnerTable)
			.Style(FEditorStyle::Get(), "ContentBrowser.AssetListView.TableRow")
			.Cursor(EMouseCursor::Default);
			//.OnDragDetected(this, &FSkillEditor::OnDraggingAssetItem);

		TSharedRef<SSkillAssetListItem> Item =
			SNew(SSkillAssetListItem)
			.AssetItem(AssetItem);
			//.ItemHeight(this, &FSkillEditor::GetListViewItemHeight);
			//.OnRenameBegin(this, &FSkillEditor::AssetRenameBegin)
			//.OnRenameCommit(this, &FSkillEditor::AssetRenameCommit)
			//.OnVerifyRenameCommit(this, &FSkillEditor::AssetVerifyRenameCommit)
			//.OnItemDestroyed(this, &FSkillEditor::AssetItemWidgetDestroyed)
			//.ShouldAllowToolTip(this, &FSkillEditor::ShouldAllowToolTips)
			//.HighlightText(HighlightedText)
			//.IsSelected(FIsSelected::CreateSP(TableRowWidget.Get(), &STableRow<TSharedPtr<FSkillAssetViewAsset>>::IsSelectedExclusively));
			//.OnAssetsOrPathsDragDropped(this, &FSkillEditor::OnAssetsOrPathsDragDropped)
			//.OnFilesDragDropped(this, &FSkillEditor::OnFilesDragDropped);

		TableRowWidget->SetContent(Item);

		return TableRowWidget.ToSharedRef();
	}
	else
	{
		//TSharedPtr<FSkillAssetViewAsset> AssetItemAsAsset = StaticCastSharedPtr<FAssetViewAsset>(AssetItem);

		//TSharedPtr<FAssetThumbnail>* AssetThumbnailPtr = RelevantThumbnails.Find(AssetItemAsAsset);
		//TSharedPtr<FAssetThumbnail> AssetThumbnail;
		//if (AssetThumbnailPtr)
		//{
		//	AssetThumbnail = *AssetThumbnailPtr;
		//}
		//else
		//{
		//	const float ThumbnailResolution = ListViewThumbnailResolution;
		//	AssetThumbnail = MakeShareable(new FAssetThumbnail(AssetItemAsAsset->Data, ThumbnailResolution, ThumbnailResolution, AssetThumbnailPool));
		//	RelevantThumbnails.Add(AssetItemAsAsset, AssetThumbnail);
		//	AssetThumbnail->GetViewportRenderTargetTexture(); // Access the texture once to trigger it to render
		//}

		TSharedPtr< STableRow<TSharedPtr<FSkillAssetViewItem>> > TableRowWidget;
		SAssignNew(TableRowWidget, STableRow<TSharedPtr<FSkillAssetViewItem>>, OwnerTable)
			.Style(FEditorStyle::Get(), "ContentBrowser.AssetListView.TableRow")
			.Cursor(EMouseCursor::Default);
			//.OnDragDetected(this, &FSkillEditor::OnDraggingAssetItem);

		TSharedRef<SSkillAssetListItem> Item =
			SNew(SSkillAssetListItem)
			//.AssetThumbnail(AssetThumbnail)
			.AssetItem(AssetItem)
			//.ThumbnailPadding(ListViewThumbnailPadding)
			.ItemHeight(this, &FSkillEditor::GetListViewItemHeight)
			//.OnRenameBegin(this, &FSkillEditor::AssetRenameBegin)
			.OnRenameCommit(this, &FSkillEditor::AssetRenameCommit);
			//.OnVerifyRenameCommit(this, &FSkillEditor::AssetVerifyRenameCommit)
			//.OnItemDestroyed(this, &FSkillEditor::AssetItemWidgetDestroyed);
			//.ShouldAllowToolTip(this, &FSkillEditor::ShouldAllowToolTips)
			//.HighlightText(HighlightedText)
			//.ThumbnailEditMode(this, &FSkillEditor::IsThumbnailEditMode)
			//.ThumbnailLabel(ThumbnailLabel)
			//.ThumbnailHintColorAndOpacity(this, &SAssetView::GetThumbnailHintColorAndOpacity)
			//.AllowThumbnailHintLabel(AllowThumbnailHintLabel)
			//.IsSelected(FIsSelected::CreateSP(TableRowWidget.Get(), &STableRow<TSharedPtr<FSkillAssetViewItem>>::IsSelectedExclusively))
			//.OnIsAssetValidForCustomToolTip(OnIsAssetValidForCustomToolTip)
			//.OnGetCustomAssetToolTip(OnGetCustomAssetToolTip)
			//.OnVisualizeAssetToolTip(OnVisualizeAssetToolTip)
			//.OnAssetToolTipClosing(OnAssetToolTipClosing);

		TableRowWidget->SetContent(Item);

		return TableRowWidget.ToSharedRef();
	}
}

#undef LOCTEXT_NAMESPACE 