#pragma once
#include "CoreMinimal.h"
#include "Widgets/Input/SEditableTextBox.h"


class T1PROJECT_API SEditableTextBoxEx : public SEditableTextBox
{
	using Super = SEditableTextBox;
public:
	void Construct(const FArguments& InArgs);

	void SetColorAndOpacity(TAttribute<FSlateColor> Color);

protected:
	/** Text color and opacity */
	TAttribute<FSlateColor> ColorAndOpacity;

protected:
	void SetStyleEx(const FEditableTextBoxStyle* InStyle);

	void OnEditableTextChangedEx(const FText& InText);
	void OnEditableTextCommittedEx(const FText& InText, ETextCommit::Type InCommitType);

private:
	FMargin FORCEINLINE DeterminePaddingEx() const { check(Style);  return PaddingOverride.IsSet() ? PaddingOverride.Get() : Style->Padding; }
	FSlateFontInfo FORCEINLINE DetermineFontEx() const { check(Style);  return FontOverride.IsSet() ? FontOverride.Get() : Style->Font; }
	FSlateColor FORCEINLINE DetermineBackgroundColorEx() const { check(Style);  return BackgroundColorOverride.IsSet() ? BackgroundColorOverride.Get() : Style->BackgroundColor; }

	FSlateColor DetermineForegroundColorEx() const;

	/** Styling: border image to draw when not hovered or focused */
	const FSlateBrush* BorderImageNormalEx;
	/** Styling: border image to draw when hovered */
	const FSlateBrush* BorderImageHoveredEx;
	/** Styling: border image to draw when focused */
	const FSlateBrush* BorderImageFocusedEx;
	/** Styling: border image to draw when read only */
	const FSlateBrush* BorderImageReadOnlyEx;

	/** @return Border image for the text box based on the hovered and focused state */
	const FSlateBrush* GetBorderImageEx() const;
};