// Copyright/2019/04/19 - BSAM
// Button etc.Command registration(Do not use at present)

#pragma once

#include "Define.h"
#include "SkillEditorStyle.h"

class FSkillEditorCommands : public TCommands<FSkillEditorCommands>
{
public:
	FORCEINLINE FSkillEditorCommands()
		: TCommands<FSkillEditorCommands>(TEXT("SkillExtensions"),
			NSLOCTEXT("SkillEditor", "SkillEditor", "SkillEditor"),
			NAME_None, FSkillEditorStyle::GetStyleSetName())
	{
	}	

	TSharedPtr< FUICommandInfo > Save;
	static TSharedRef<FUICommandList> _actionList;
	static void OnShowTerritoryInfo();
	
	//TSharedPtr< FUICommandInfo > CommandOpenSkillWidget;
	//TSharedPtr< FUICommandInfo > CommandOpenAnimationMontage;

	//// import animation
	//TSharedPtr<FUICommandInfo> ImportAnimation;
	//// reimport animation
	//TSharedPtr<FUICommandInfo> ReimportAnimation;
	//// apply compression
	//TSharedPtr<FUICommandInfo> ApplyCompression;
	//// export to FBX
	//TSharedPtr<FUICommandInfo> ExportToFBX_AnimData;
	//// export to FBX
	//TSharedPtr<FUICommandInfo> ExportToFBX_PreviewMesh;
	//// Add looping interpolation
	//TSharedPtr<FUICommandInfo> AddLoopingInterpolation;
	//// set key for bone track
	//TSharedPtr<FUICommandInfo> SetKey;
	//// bake bone track curves to animation
	//TSharedPtr<FUICommandInfo> ApplyAnimation;
	//// Remove bone tracks
	//TSharedPtr<FUICommandInfo> RemoveBoneTracks;

public:
	virtual void RegisterCommands() override;
};

class FSkillEditorActions
{
public:
	/*static void OpenSkillWidget();
	static void OpenAnimationMontage();*/

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	//static UAnimMontage* AttackMontage;
};