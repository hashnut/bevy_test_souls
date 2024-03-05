// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrEdLIneDataLayout.h"
#include "SSearchableComboBox.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/Line/GsTerritoryLine.h"
#include "DetailLayoutBuilder.h"
#include "IDetailChildrenBuilder.h"

TSharedRef<IPropertyTypeCustomization> FGsTerrEdLIneDataLayout::MakeInstance()
{
	return MakeShareable(new FGsTerrEdLIneDataLayout());
}

void FGsTerrEdLIneDataLayout::CustomizeHeader(TSharedRef<IPropertyHandle> inPropertyHandle
	, FDetailWidgetRow& inHeaderRow
	, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle
	inHeaderRow.NameContent()
		[
			inPropertyHandle->CreatePropertyNameWidget()
		];
}


void FGsTerrEdLIneDataLayout::CustomizeChildren(TSharedRef<IPropertyHandle> inPropertyHandle, IDetailChildrenBuilder& inChildBuilder, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle		
	InvalidTIdInfos();	

	uint32 num = 0;
	inPropertyHandle->GetNumChildren(num);
	FString name;

	for (uint32 i = 0; i < num; ++i)
	{
		TSharedPtr<IPropertyHandle> handle = inPropertyHandle->GetChildHandle(i);
		if (false == handle.IsValid())
		{
			continue;
		}
		
		name = handle.Get()->GetPropertyDisplayName().ToString();
		if (name.Equals("Line ID"))
		{
			_targetIDHandle = handle;
			CreateTIdSearchComboBox(inChildBuilder);
		}
		else
		{
			inChildBuilder.AddProperty(handle.ToSharedRef());
		}
	}
}

TSharedRef<SWidget> FGsTerrEdLIneDataLayout::OnGenerateTIdWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void FGsTerrEdLIneDataLayout::OnChangedTId(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo)
{
	if (inSelectInfo != ESelectInfo::Direct)
	{
		FString value = *inNewSelection.Get();
		_selectedTId = value;
		TArray<FString> tIdParses;
		value.ParseIntoArray(tIdParses, TEXT("-"), true);
		if (tIdParses.Num() > 0)
		{			
			int64 id = FCString::Atoi64(*tIdParses[0]);
			_targetIDHandle->SetValue(id);
		}
	}
}

void FGsTerrEdLIneDataLayout::InitTId()
{
	int64 targetID;
	_targetIDHandle->GetValue(targetID);

	if (targetID > 0)
	{
		FString targetIDString = FString::Printf(TEXT("%lld"), targetID);
		_tIdText = MakeShareable<FString>(new FString(targetIDString));
		_selectedTId = targetIDString;
	}
	else
	{
		if (_tIdList.Num() > 0)
		{
			_tIdText = _tIdList[0];
			_selectedTId = (*_tIdText.Get());
			targetID = FCString::Atoi64(*_selectedTId);
			_targetIDHandle->SetValue(targetID);
		}
		else
		{
			_tIdText = MakeShareable(new FString(TEXT("No id in table")));
			_targetIDHandle->SetValue(0);
		}
	}	
}


void FGsTerrEdLIneDataLayout::CreateTIdSearchComboBox(IDetailChildrenBuilder& inChildBuilder)
{
	InitTId();

	inChildBuilder.AddProperty(_targetIDHandle.ToSharedRef())
		.CustomWidget()
		.NameContent()
		[
			_targetIDHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(500)
		[
			SAssignNew(_tIdSearchableComboBox, SSearchableComboBox)
			.OptionsSource(&_tIdList)
		.OnGenerateWidget(this, &FGsTerrEdLIneDataLayout::OnGenerateTIdWidget)
		.OnSelectionChanged(this, &FGsTerrEdLIneDataLayout::OnChangedTId)
		.InitiallySelectedItem(_tIdText)
		.ContentPadding(2)
		.Content()
		[
			SNew(STextBlock)
			.Text(this, &FGsTerrEdLIneDataLayout::GetSelectedTIdText)
		]
		];
}

FText FGsTerrEdLIneDataLayout::GetSelectedTIdText() const
{
	return FText::FromString(_selectedTId);
}

int64 FGsTerrEdLIneDataLayout::GetSelectedTId() const
{
	FString value = _selectedTId;
	TArray<FString> tIdParses;
	value.ParseIntoArray(tIdParses, TEXT("-"), true);
	if (tIdParses.Num() == 0)
	{
		return 0;
	}

	return FCString::Atoi64(*tIdParses[0]);
}

void FGsTerrEdLIneDataLayout::InvalidTIdInfos()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	_tIdList.Add(MakeShareable(new FString(TEXT("0"))));

	TArray<AActor*> lineActorArray;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryLine::StaticClass(), lineActorArray);	
	class AGsTerritoryLine* castLine = nullptr;
	FString idString;
	for (AActor* line : lineActorArray)
	{
		if (nullptr == line)
		{
			continue;
		}

		castLine = Cast<AGsTerritoryLine>(line);
		if (nullptr == castLine)
		{
			continue;
		}
		
		idString = FString::Printf(TEXT("%lld"), castLine->GetTerritoryUniqueID());
		_tIdList.Add(MakeShareable(new FString(idString)));
	}	
}

#undef LOCTEXT_NAMESPACE