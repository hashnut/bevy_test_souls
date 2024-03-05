// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdToolLevelViewSettingCompoundWidget.h"
#include "LogMacros.h"
#include "TabManager.h"
#include "SHeaderRow.h"
#include "EditorStyleSet.h"
#include "STextBlock.h"
#include "SButton.h"
#include "Reply.h"
#include "Util/GsTerrEdRecoveryUtil.h"
#include "Data/GsTerritoryDataManager.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "TerritoryEditor.h"
#include "SNumericEntryBox.h"
#include "DetailLayoutBuilder.h"

#define LOCTEXT_NAMESPACE "TerritoryEditor"

#define LAMDA_OPTIMIZE false

void SGsTerrEdToolLevelViewSettingCompoundWidget::Construct(const FArguments& inArgs)
{
	_showTypeList.Empty();
	_showTypeList.Add(MakeShareable(new FString("HideAll")));
	_showTypeList.Add(MakeShareable(new FString("ShowOnlySelectActor")));
	_showTypeList.Add(MakeShareable(new FString("ShowAllActor")));

	_height = 25.f;
	_descriptionColumnSize = 800;
	_excuteColumnSize = 400;
	_padding = 0;

	//widget show hide
	TSharedPtr<SWidget> setWidgetDistanceWidget = CreateWidgetDistance();
	TSharedPtr<SWidget> setWidgetShowTypeWidget = CreateWidgetShow();

	//shpae show hide
	TSharedPtr<SWidget> setShapeDistanceWidget = CreateShapeDistance();
	TSharedPtr<SWidget> setShapeShowTypeWidget = CreateShapeShow();

	TSharedPtr<SWidget> setSightRadiusDistanceWidget = CreateSightRadiusDistance();
	TSharedPtr<SWidget> setSightRadiusTypeWidget = CreateSightShow();

	TSharedPtr<SWidget> setTransparentFloorWidget = CreateTransparentFloorWidget();
	TSharedPtr<SWidget> setTransparentFloorValueWidget = CreateTransparentFloorValueWidget();

	TSharedPtr<SWidget> checkNavmeshWidget = CreateCheckNavmeshWidget();
	TSharedPtr<SWidget> scalabilityQualityLowWidget = CreateScalabilityQualityLowWidget();


	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			SNew(SHeaderRow)
			+ SHeaderRow::Column(TEXT("Description"))
		.FillWidth(0.8f)
		.DefaultLabel(LOCTEXT("Description", "Description"))

		+ SHeaderRow::Column(TEXT("Excute"))
		.FillWidth(0.2f)
		.DefaultLabel(LOCTEXT("Excute", "Excute"))
		]
	//npc id range column
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			setWidgetDistanceWidget.ToSharedRef()
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			setWidgetShowTypeWidget.ToSharedRef()
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			setShapeDistanceWidget.ToSharedRef()
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			setShapeShowTypeWidget.ToSharedRef()
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			setSightRadiusDistanceWidget.ToSharedRef()
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			setSightRadiusTypeWidget.ToSharedRef()
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			setTransparentFloorWidget.ToSharedRef()
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			setTransparentFloorValueWidget.ToSharedRef()
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			checkNavmeshWidget.ToSharedRef()
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			scalabilityQualityLowWidget.ToSharedRef()
		]
		];
}

TSharedRef<SDockTab> SGsTerrEdToolLevelViewSettingCompoundWidget::HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs
	, const FName inTabIdentifier)
{
	TSharedRef<SDockTab> tab = SNew(SDockTab).TabRole(ETabRole::PanelTab);

	return tab;
}

TSharedPtr<SWidget> SGsTerrEdToolLevelViewSettingCompoundWidget::CreateWidgetDistance()
{
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(_descriptionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Widget view distance")))
		]

		]
	+ SSplitter::Slot()
		.Value(_excuteColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.MaxWidth(400)
		[
			SNew(SSpinBox<float>)
			.Value_Lambda([this]()->float {return FGsTerritoryLevelManager::_widgetVisibleDistance; })
		.OnValueCommitted_Lambda([this](float value, ETextCommit::Type commitType) {
		if (commitType == ETextCommit::Type::OnEnter) {
			FGsTerritoryLevelManager::_widgetVisibleDistance = value;
			UE_LOG(LogTemp, Log, TEXT("test : %f"), value);
		}}
		)
		.MinValue(0.001f)
			.MaxValue(TNumericLimits<float>::Max())
			//.Style(&FGameplayInsightsStyle::Get().GetWidgetStyle<FSpinBoxStyle>("TransportControls.HyperlinkSpinBox"))
			.ClearKeyboardFocusOnCommit(true)
			.Delta(0.01f)
			.LinearDeltaSensitivity(25)
			//.TypeInterface(MakeShared<TNumericUnitTypeInterface<float>>(EUnit::Multiplier))
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelViewSettingCompoundWidget::CreateWidgetShow()
{
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(_descriptionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Widget view type(W+Shift+Alt)")))
		]

		]
	+ SSplitter::Slot()
		.Value(_excuteColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.MaxWidth(400)
		[
			SNew(SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&_showTypeList)
		.OnSelectionChanged(this, &SGsTerrEdToolLevelViewSettingCompoundWidget::WidgetShowTypeChanged)
		.OnGenerateWidget(this, &SGsTerrEdToolLevelViewSettingCompoundWidget::GenerateComboWidget)
		.InitiallySelectedItem(FindShowTypeString(FGsTerritoryLevelManager::_widgetShowType))
		.Content()
		[
			SNew(STextBlock).Text_Lambda([this]()->FText {
		return FText::FromString(FGsTerritoryLevelManager::ConvertShowTypeString(FGsTerritoryLevelManager::_widgetShowType));
				})
		]
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelViewSettingCompoundWidget::CreateShapeDistance()
{
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(_descriptionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Shape view distance")))
		]

		]
	+ SSplitter::Slot()
		.Value(_excuteColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.MaxWidth(_excuteColumnSize)
		[
			SNew(SSpinBox<float>)
			.Value_Lambda([this]()->float {return FGsTerritoryLevelManager::_shapeVisibleDistance; })
		.OnValueCommitted_Lambda([this](float value, ETextCommit::Type commitType) {
		if (commitType == ETextCommit::Type::OnEnter) { FGsTerritoryLevelManager::_shapeVisibleDistance = value; }}
		)
		.MinValue(0.001f)
			.MaxValue(TNumericLimits<float>::Max())
			//.Style(&FGameplayInsightsStyle::Get().GetWidgetStyle<FSpinBoxStyle>("TransportControls.HyperlinkSpinBox"))
			.ClearKeyboardFocusOnCommit(true)
			.Delta(0.01f)
			.LinearDeltaSensitivity(25)
			//.TypeInterface(MakeShared<TNumericUnitTypeInterface<float>>(EUnit::Multiplier))
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelViewSettingCompoundWidget::CreateShapeShow()
{
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(_descriptionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Shape view type(S+Shift+Alt)")))
		]

		]
	+ SSplitter::Slot()
		.Value(_excuteColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.MaxWidth(400)
		[
			SNew(SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&_showTypeList)
		.OnSelectionChanged(this, &SGsTerrEdToolLevelViewSettingCompoundWidget::ShapeShowTypeChanged)
		.OnGenerateWidget(this, &SGsTerrEdToolLevelViewSettingCompoundWidget::GenerateComboWidget)
		.InitiallySelectedItem(FindShowTypeString(FGsTerritoryLevelManager::_shapeShowType))
		.Content()
		[
			SNew(STextBlock).Text_Lambda([this]()->FText {
		return FText::FromString(FGsTerritoryLevelManager::ConvertShowTypeString(FGsTerritoryLevelManager::_shapeShowType));
				})
		]
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelViewSettingCompoundWidget::CreateSightRadiusDistance()
{	
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(_descriptionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Sight radius view distance")))
		]

		]
	+ SSplitter::Slot()
		.Value(_excuteColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.MaxWidth(_excuteColumnSize)
		[
			SNew(SSpinBox<float>)
			.Value_Lambda([this]()->float {return FGsTerritoryLevelManager::_sightRadiusVisibleDistance; })
		.OnValueCommitted_Lambda([this](float value, ETextCommit::Type commitType) {
		if (commitType == ETextCommit::Type::OnEnter) { FGsTerritoryLevelManager::_sightRadiusVisibleDistance = value; }}
		)
		.MinValue(0.001f)
			.MaxValue(TNumericLimits<float>::Max())
			//.Style(&FGameplayInsightsStyle::Get().GetWidgetStyle<FSpinBoxStyle>("TransportControls.HyperlinkSpinBox"))
			.ClearKeyboardFocusOnCommit(true)
			.Delta(0.01f)
			.LinearDeltaSensitivity(25)
			//.TypeInterface(MakeShared<TNumericUnitTypeInterface<float>>(EUnit::Multiplier))
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelViewSettingCompoundWidget::CreateSightShow()
{
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(_descriptionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Sight radius view type(D+Shift+Alt)")))
		]

		]
	+ SSplitter::Slot()
		.Value(_excuteColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.MaxWidth(400)
		[
			SNew(SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&_showTypeList)
		.OnSelectionChanged(this, &SGsTerrEdToolLevelViewSettingCompoundWidget::SightRadiusTypeChanged)
		.OnGenerateWidget(this, &SGsTerrEdToolLevelViewSettingCompoundWidget::GenerateComboWidget)
		.InitiallySelectedItem(FindShowTypeString(FGsTerritoryLevelManager::_sightRadiusShowType))
		.Content()
		[
			SNew(STextBlock).Text_Lambda([this]()->FText {
		return FText::FromString(FGsTerritoryLevelManager::ConvertShowTypeString(FGsTerritoryLevelManager::_sightRadiusShowType));
				})
		]
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelViewSettingCompoundWidget::CreateTransparentFloorWidget()
{
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(_descriptionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Zone floor transparent active(T+Shift+Alt)")))
		]
		]
	+ SSplitter::Slot()
		.Value(_excuteColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.MaxWidth(_excuteColumnSize)
		[
			SNew(SCheckBox)
			.IsChecked_Lambda([](void)->ECheckBoxState {return FGsTerritoryLevelManager::IsTransparentFloor() == true ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
		.OnCheckStateChanged_Lambda([](ECheckBoxState inState) {FGsTerritoryLevelManager::SetTransparentFloor(inState == ECheckBoxState::Checked); })
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelViewSettingCompoundWidget::CreateTransparentFloorValueWidget()
{
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(_descriptionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Zone floor transparent value")))
		]
		]
	+ SSplitter::Slot()
		.Value(_excuteColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.MaxWidth(_excuteColumnSize)
		[
			SNew(SNumericEntryBox<float>)
			.AllowSpin(true)
		.MinValue(TOptional<float>(0.0f))
		.MaxValue(TOptional<float>(1.0f))
		.Value(this, &SGsTerrEdToolLevelViewSettingCompoundWidget::GetTransparentValue)
		.OnValueCommitted(this, &SGsTerrEdToolLevelViewSettingCompoundWidget::OnSetransparentValue)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelViewSettingCompoundWidget::CreateCheckNavmeshWidget()
{
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(_descriptionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Check navmesh exist")))
		]
		]
	+ SSplitter::Slot()
		.Value(_excuteColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.MaxWidth(_excuteColumnSize)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Check navmesh exist")))
		.OnClicked_Lambda([]()->FReply {
		UWorld* world = GEditor->GetEditorWorldContext().World();
		FString mapName = UGameplayStatics::GetCurrentLevelName(world);
		if (false == FGsTerritoryLevelManager::IsNavmeshExist(world))
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("Navmesh"), TEXT("Navmesh is missing - map name : ") + mapName);
		}
		else
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("Navmesh"), TEXT("Navmesh is exist - map name : ") + mapName);
		}

		return FReply::Handled();
			})
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelViewSettingCompoundWidget::CreateScalabilityQualityLowWidget()
{
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(_descriptionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("ScalabilityQaulityLow(Q+Shift+Alt)")))
		]
		]
	+ SSplitter::Slot()
		.Value(_excuteColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.MaxWidth(_excuteColumnSize)
		[
			SNew(SCheckBox)
			.IsChecked_Lambda([](void)->ECheckBoxState {return FGsTerritoryLevelManager::_isScalabilityQualityLow == true ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
		.OnCheckStateChanged_Lambda([](ECheckBoxState inState) {FGsTerritoryLevelManager::SetScalabilityQaulityLow(inState == ECheckBoxState::Checked); })
		]
		];
}

TSharedPtr<FString> SGsTerrEdToolLevelViewSettingCompoundWidget::FindShowTypeString(EGsTerrEdShowType inShowType)
{
	for (int i = 0; i < _showTypeList.Num(); ++i)
	{
		bool isEqual = FGsTerritoryLevelManager::ConvertShowTypeString(inShowType).Equals(*_showTypeList[i]);
		if (isEqual)
		{
			return _showTypeList[i];
		}
	}

	return _showTypeList[0];
}

void SGsTerrEdToolLevelViewSettingCompoundWidget::WidgetShowTypeChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	FGsTerritoryLevelManager::_widgetShowType = FGsTerritoryLevelManager::ConvertShowTypeEnum(*InItem);
	FGsTerritoryLevelManager::ActiveTerritoryWidget(FGsTerritoryLevelManager::_widgetShowType);
}

void SGsTerrEdToolLevelViewSettingCompoundWidget::ShapeShowTypeChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	FGsTerritoryLevelManager::_shapeShowType = FGsTerritoryLevelManager::ConvertShowTypeEnum(*InItem);
	FGsTerritoryLevelManager::ActiveTerritoryShape(FGsTerritoryLevelManager::_shapeShowType);
}

void SGsTerrEdToolLevelViewSettingCompoundWidget::SightRadiusTypeChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	FGsTerritoryLevelManager::_sightRadiusShowType = FGsTerritoryLevelManager::ConvertShowTypeEnum(*InItem);
	FGsTerritoryLevelManager::ActiveTerritoryShape(FGsTerritoryLevelManager::_sightRadiusShowType);
}

TSharedRef<SWidget> SGsTerrEdToolLevelViewSettingCompoundWidget::GenerateComboWidget(TSharedPtr<FString> InItem)
{
	return
		SNew(STextBlock)
		.Text(FText::FromString(*InItem))
		.Font(IDetailLayoutBuilder::GetDetailFont());
}

TOptional<float> SGsTerrEdToolLevelViewSettingCompoundWidget::GetTransparentValue() const
{
	return TOptional<float>(FGsTerritoryLevelManager::_transparentFloorAlpha);
}

#undef LOCTEXT_NAMESPACE