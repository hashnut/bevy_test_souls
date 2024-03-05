// Copyright/2019/04/19 - BSAM

#include "SkillEditorModule.h"
#include "SkillEditorStyle.h"
#include "SkillEditorCommands.h"
#include "SkillEditor.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "SkillEditorModule"

void FSkillEditorModule::StartupModule()
{
	FSkillEditorStyle::Initialize();
	FSkillEditorCommands::Register();

	/*TSharedPtr<FUICommandList> PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FSkillEditorCommands::Get().Save,
		FExecuteAction::CreateRaw(this, &FSkillEditorModule::Save));*/

	// Create a skill editor button in the Unreal Level Editor (main editor).
	SkillEditorBtnExtender = MakeShareable(new FExtender());
	SkillEditorBtnExtender->AddToolBarExtension("Settings",
		EExtensionHook::After,
		TSharedPtr<FUICommandList>(),
		FToolBarExtensionDelegate::CreateRaw(this, &FSkillEditorModule::ExtendToolbar));

	// Add skill editor button widget to level editor.
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(SkillEditorBtnExtender);	
}

void FSkillEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorModule.GetMenuExtensibilityManager()->RemoveExtender(SkillEditorBtnExtender);
	}
		
	FSkillEditorCommands::Unregister();	
	FSkillEditorStyle::Shutdown();
}

void FSkillEditorModule::ExtendToolbar(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.BeginSection("Skill");
	{
		ToolbarBuilder.AddToolBarButton
		(
			FUIAction
			(
				FExecuteAction::CreateRaw(this, &FSkillEditorModule::OpenSkillEditor)
			),
			NAME_None,
			LOCTEXT("OpenSkillEditor", "Skill Editotr"),
			LOCTEXT("OpenSkillEditorToolTip", "Opens the SkillEditor."),
			FSlateIcon(FSkillEditorStyle::GetStyleSetName(), "SkillToolbarIcon.SkillEditor")
		);
	}
	ToolbarBuilder.EndSection();
}

//void FSkillEditorModule::Save()
//{
//	if (SkillEditor.IsValid())
//	{
//		SkillEditor->OnSaveSkillEditorDataAsset();
//	}
//}

void FSkillEditorModule::OpenSkillEditor()
{
	if (!SkillEditor.IsValid())
	{
		TSharedPtr<FSkillEditor> NewSkillEditor(new FSkillEditor());
		NewSkillEditor->InitSkillEditor();
		SkillEditor = NewSkillEditor;
	}
	else
		SkillEditor->InitSkillEditor();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSkillEditorModule, SkillEditor);