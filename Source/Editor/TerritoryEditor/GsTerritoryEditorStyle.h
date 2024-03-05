#pragma once

#include "CoreMinimal.h"

class FGsTerritoryEditorStyle
{
private:
	/** Style Instance */
	static TSharedPtr<class FSlateStyleSet> _styleInstance;

public:
	/** Initialize */
	static void Initialize();

	/** Shutdown */
	static void Shutdown();

	/** Reload Textures */
	static void ReloadTextures();

	/** Get Style Set Name */
	static FName GetStyleSetName();

private:
	/** Create */
	static TSharedRef<class FSlateStyleSet> Create();

public:
	/** Get */
	static const ISlateStyle& Get();
};