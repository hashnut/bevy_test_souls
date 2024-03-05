#include "SkillEditorCommands.h"

#include "SkillEditor.h"

#include "IAnimationEditorModule.h"

#define LOCTEXT_NAMESPACE "SkillEditorCommand"

//UAnimMontage* FSkillEditorActions::AttackMontage;

TSharedRef<FUICommandList> FSkillEditorCommands::_actionList(new FUICommandList());

void FSkillEditorCommands::RegisterCommands()
{
	/*UI_COMMAND(CommandOpenSkillWidget, "Skill Editor", "Editor to develop skills ", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(CommandOpenAnimationMontage, "Animaion Montage", "Animaion Montage Ex", EUserInterfaceActionType::Button, FInputGesture());

	UI_COMMAND(ImportAnimation, "Import Animation", "Import new animation for the skeleton.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ReimportAnimation, "Reimport Animation", "Reimport current animation.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ApplyCompression, "Apply Compression", "Apply compression to current animation", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ExportToFBX_AnimData, "Animation Data", "Export current animation source data to FBX", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ExportToFBX_PreviewMesh, "Preview Mesh", "Export current animation by playing on the current preview mesh, including retargeting, post process graph, or anything you see on the preview mesh.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AddLoopingInterpolation, "Add Looping Interpolation", "Add an extra first frame at the end of the animation to create interpolation when looping", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenAnimationMontage, "Set Key", "Add Bone Transform to Additive Layer Tracks", EUserInterfaceActionType::Button, FInputChord(EKeys::S));
	UI_COMMAND(ApplyAnimation, "Apply Animation", "Apply Additive Layer Tracks to Runtime Animation Data", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(RemoveBoneTracks, "Remove All Bone Tracks", "Remove all bone tracks. This doesn't remove notifies or curves.", EUserInterfaceActionType::Button, FInputChord());*/

	UI_COMMAND(Save, "Save Current", "Saves the current level to disk", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control, EKeys::U));

	//_actionList->MapAction(Save, FExecuteAction::CreateStatic(&FSkillEditorCommands::OnShowTerritoryInfo), FCanExecuteAction::CreateStatic(&FSkillEditorCommands::OnCanShowTerritoryInfo));
	_actionList->MapAction(Save, FExecuteAction::CreateStatic(&FSkillEditorCommands::OnShowTerritoryInfo));
	FLevelEditorModule& levelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	TSharedRef<FUICommandList> levelEditorCommandBindings = levelEditor.GetGlobalLevelEditorActions();

	levelEditorCommandBindings->Append(_actionList);
}

void FSkillEditorCommands::OnShowTerritoryInfo()
{

}

//void FSkillEditorActions::OpenSkillWidget()
//{
//	//TSharedRef<FSkillEditorMainWidget> NewMainWidget(new FSkillEditorMainWidget());
//	//NewMainWidget->InitMainEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), NewObject<USkillEditorObject>());	
//}
//
//void FSkillEditorActions::OpenAnimationMontage()
//{	
//	//FSkillEditorActions::AttackMontage = CreateDefaultSubObject<UAnimMontage>(TEXT("TESTMONTAGE"));
//
//	/*static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/NewAnimMontage.NewAnimMontage"));
//	if (ATTACK_MONTAGE.Succeeded())
//	{
//		FSkillEditorActions::AttackMontage = ATTACK_MONTAGE.Object;
//	}*/
//	FSkillEditorActions::AttackMontage = Cast< UAnimMontage >(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/NewAnimMontage.NewAnimMontage")));
//
//	EToolkitMode::Type Mode = EToolkitMode::Standalone;//EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
//
//	//AnimMontage'/Game/InfinityBladeWarriors/Character/CompleteCharacters/NewAnimMontage.NewAnimMontage'
//	IAnimationEditorModule& AnimationEditorModule = FModuleManager::LoadModuleChecked<IAnimationEditorModule>("AnimationEditor");	
//	AnimationEditorModule.CreateAnimationEditor(Mode, nullptr, FSkillEditorActions::AttackMontage);
//}

#undef LOCTEXT_NAMESPACE