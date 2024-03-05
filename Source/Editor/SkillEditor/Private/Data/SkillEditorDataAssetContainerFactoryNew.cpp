#include "SkillEditorDataAssetContainerFactoryNew.h"
#include "SkillEditorDataAsset.h"

#include "AssetRegistryModule.h"

//#include "GameObject/Skill/Container/GsSkillDataContainerBase.h"

USkillEditorDataAssetContainerFactoryNew::USkillEditorDataAssetContainerFactoryNew(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	SupportedClass = USkillEditorDataAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}


UObject* USkillEditorDataAssetContainerFactoryNew::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	USkillEditorDataAsset* SkillEditorDataAssetContainer = NewObject<USkillEditorDataAsset>(InParent, Class, Name, Flags);	

	if (SkillEditorDataAssetContainer != nullptr)
	{
		const FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		AssetRegistryModule.AssetCreated(SkillEditorDataAssetContainer);
	}

	//SkillEditorDataAssetContainer->Modify();
	//SkillEditorDataAssetContainer->Name = Name;
	//SkillEditorDataAssetContainer->PostEditChange();
	SkillEditorDataAssetContainer->MarkPackageDirty();

	return SkillEditorDataAssetContainer;
}