// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UILib/Interface/GsEditableInterface.h"
#include "GameFramework/PlayerController.h"
#include "UserWidget.h"
#include "WidgetTree.h"
#include "UI/UIControlLib/ContentWidget/GsEditableTextBox.h"
#include "UMG/Public/Components/EditableText.h"
#include "UMG/Public/Components/EditableTextBox.h"
#include "UMG/Public/Components/MultiLineEditableText.h"
#include "UMG/Public/Components/MultiLineEditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Management/GsScopeHolder.h"
#include "DataSchema/KeyMapping/GsSchemaKeyMapping.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "ForEach.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

void IGsEditableInterface::Initialize()
{
	_widget = nullptr;
	_editableWidget.Reset();
}

void IGsEditableInterface::Finalize()
{
	InputFinalize(nullptr);
	_widget = nullptr;
	_inputComponent = nullptr;
	_editableWidget.Reset();
}
// Add default functionality here for any IGsEditableInterface functions that are not pure virtual.
void IGsEditableInterface::InputInitialize(TWeakObjectPtr<UGsUIWidgetBase> ownRoot)
{
	if (_useEditableCheck == false)
	{
		_editableWidget.Reset();

		if (ownRoot.IsValid())
		{
			_includeEditableWidget = FindEditableWidgetResursive(ownRoot);
			_useEditableCheck = true;
		}
	}

	if (IsEditableWidgetInclude())
	{
		KeyProcInitialize(ownRoot);
	}
}

void IGsEditableInterface::InputFinalize(TWeakObjectPtr<UGsUIWidgetBase> ownRoot)
{
	DisableEditableInput();
}

void IGsEditableInterface::EnableEditableInput()
{
#if PLATFORM_WINDOWS || WITH_EDITOR
	if (auto widget = _widget.Get())
	{	
		widget->RegisterInputComponent();
	}
#endif
}

void IGsEditableInterface::DisableEditableInput()
{
#if PLATFORM_WINDOWS || WITH_EDITOR
	if (auto widget = _widget.Get())
	{
		widget->UnregisterInputComponent();
	}
#endif
}

bool IGsEditableInterface::IsEditableWidgetInclude()
{
	return _useEditableCheck && _includeEditableWidget; // _editableWidget.Num() > 0;
}

void IGsEditableInterface::KeyProcInitialize(TWeakObjectPtr<UGsUIWidgetBase> ownRoot)
{	
	if (auto widget = ownRoot.Get())
	{
		_widget = widget;
#if PLATFORM_WINDOWS || WITH_EDITOR
		if (widget->InputComponent == nullptr)
		{
			widget->InitializeInputComponent();
		}
		_inputComponent = widget->InputComponent;
#endif
		SetInputComponent(_inputComponent.Get());
	}
}

void IGsEditableInterface::SetInputComponent(UInputComponent* inputComponent)
{
	if (inputComponent == nullptr) return;

#if PLATFORM_WINDOWS || WITH_EDITOR
	if (auto widget = _widget.Get())
	{
		TArray<const FGsSchemaKeyMapping*> keyMapList;
		if (const UGsTable* table = FGsSchemaKeyMapping::GetStaticTable())
		{
			table->GetAllRows<FGsSchemaKeyMapping>(keyMapList);

			auto weakWidget = _widget;

			Algo::ForEachIf(keyMapList, [](const auto e) { return e != nullptr && e->keyBindType == EGsOptionKeyBindType::Action;
				}, [weakWidget, inputComponent](const auto e) {
					Algo::ForEach(e->keyInputEvents, [weakWidget, inputComponent, e](auto ev) {

						if (auto widget = weakWidget.Get())
						{
							inputComponent->BindAction<TDelegate<void(EGsKeyMappingType, EInputEvent)>>(e->keyName, ev.GetValue(),
								widget, &UGsUIWidgetBase::InputMapKeyAction, e->keyMappingType, ev.GetValue());
						}});
				});
		}
	}
#endif
}

TArray<UWidget*> IGsEditableInterface::LoadEditableWidgetRecursive(TWeakObjectPtr<UUserWidget> ownRoot)
{
	TArray<UWidget*> localEditableWidget;
	if (ownRoot.IsValid())
	{
		TArray<UWidget*> Children;
		ownRoot->WidgetTree->GetAllWidgets(Children);

		auto recursiveRoot = Children.FilterByPredicate([](const UWidget* e) {
				return e->IsA<UUserWidget>();
			});
		
		for (auto root : recursiveRoot)
		{
			localEditableWidget.Append(LoadEditableWidgetRecursive(Cast<UUserWidget>(root)));
		}
		
		localEditableWidget.Append(Children.FilterByPredicate([](const UWidget* e) {
			return (e->IsA<UGsEditableTextBox>() ||
				e->IsA<UEditableText>() ||
				e->IsA<UEditableTextBox>() ||
				e->IsA<UMultiLineEditableText>() ||
				e->IsA<UMultiLineEditableTextBox>());
			}));
	}
	return localEditableWidget;
}

bool IGsEditableInterface::FindEditableWidgetResursive(TWeakObjectPtr<UUserWidget> ownRoot) // _includeEditableWidget
{
	bool includeEdiableWidget{ false };
	if (ownRoot.IsValid())
	{
		TArray<UWidget*> Children;
		ownRoot->WidgetTree->GetAllWidgets(Children);

		includeEdiableWidget = Children.FindByPredicate([](const UWidget* e) {
			return (e->IsA<UGsEditableTextBox>() ||
				e->IsA<UEditableText>() ||
				e->IsA<UEditableTextBox>() ||
				e->IsA<UMultiLineEditableText>() ||
				e->IsA<UMultiLineEditableTextBox>());
			}) != nullptr;

		if (!includeEdiableWidget)
		{
			auto recursiveRoot = Children.FilterByPredicate([](const UWidget* e) {
				return e->IsA<UUserWidget>();
				});

			includeEdiableWidget = recursiveRoot.FindByPredicate([this](auto root) { 
				return FindEditableWidgetResursive(Cast<UUserWidget>(root)); }) != nullptr;
		}
	}
	return includeEdiableWidget;
}