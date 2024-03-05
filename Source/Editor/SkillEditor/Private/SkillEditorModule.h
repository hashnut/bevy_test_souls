// Copyright/2019/04/19 - BSAM
// Create Skill Editor module, add buttons to Unreal Level Editor, link Skill Editor creation to buttons, register basic information of editor module

#pragma once

#include "ISkillEditorModule.h"

class FSkillEditor;
class FToolBarBuilder;
class FSpawnTabArgs;
class FExtender;

class FSkillEditorModule : public ISkillEditorModule
{
private:
	TSharedPtr<FExtender> SkillEditorBtnExtender;
	TSharedPtr<FSkillEditor> SkillEditor;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;	

private:
	void ExtendToolbar(FToolBarBuilder& ToolbarBuilder);
	void OpenSkillEditor();
	//void Save();

	TSharedRef<SDockTab> SpawnDummyTab(const FSpawnTabArgs& InArgs);
};