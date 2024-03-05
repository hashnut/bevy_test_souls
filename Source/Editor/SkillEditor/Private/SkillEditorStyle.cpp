#include "SkillEditorStyle.h"

TSharedPtr< FSlateStyleSet > FSkillEditorStyle::StyleInstance = nullptr;

void FSkillEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FSkillEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FSkillEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SkillEditorStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef< FSlateStyleSet > FSkillEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("SkillEditorStyle"));
	Style->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));

	// Menu Icons
	//Style->Set("SkillExtensions.SkillEditor", new IMAGE_BRUSH("Icons/icon_file_switch_16px", Icon16x16));	

	// Toolbar Icons icon_eject_40x
	Style->Set("SkillToolbarIcon.SkillEditor", new IMAGE_BRUSH("Icons/icon_TextureEd_CheckeredBackground_40x", Icon40x40));	
	Style->Set("SkillToolbarIcon.AnimationMontage", new IMAGE_BRUSH("Icons/icon_eject_40x", Icon40x40));

	return Style;
}

#undef IMAGE_BRUSH

void FSkillEditorStyle::ReloadTextures()
{
	//FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

const ISlateStyle& FSkillEditorStyle::Get()
{
	return *StyleInstance;
}
