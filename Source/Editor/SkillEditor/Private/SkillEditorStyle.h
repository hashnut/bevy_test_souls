// Copyright/2019/04/19 - BSAM
// Creation and application management of slate to show in editor

#pragma once

#include "Define.h"

class FSkillEditorStyle
{
public:
	static void Initialize();
	static void Shutdown();
	static void ReloadTextures();
	static const ISlateStyle& Get();
	static FName GetStyleSetName();

private:
	static TSharedRef< class FSlateStyleSet > Create();
	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};
