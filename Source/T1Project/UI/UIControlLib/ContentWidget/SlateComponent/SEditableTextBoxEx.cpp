

#include "SEditableTextBoxEx.h"
#include "SlateOptMacros.h"
#if PLATFORM_IOS
#include "T1Project.h"
#endif

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SEditableTextBoxEx::Construct(const FArguments& InArgs)
{
	SEditableTextBox::Construct(InArgs);

	ColorAndOpacity = InArgs._ForegroundColor;
	EditableText->SetColorAndOpacity(ColorAndOpacity);
}

void SEditableTextBoxEx::SetColorAndOpacity(TAttribute<FSlateColor> Color)
{
	ColorAndOpacity = Color;
	EditableText->SetColorAndOpacity(ColorAndOpacity);
}


void SEditableTextBoxEx::OnEditableTextChangedEx(const FText& InText)
{
	OnEditableTextChanged(InText);
}

void SEditableTextBoxEx::OnEditableTextCommittedEx(const FText& InText, ETextCommit::Type InCommitType)
{
#if PLATFORM_IOS
	GSLOG(Log, TEXT("*********** SEditableTextBoxEx::OnEditableTextCommittedEx [%s] [%d]"), *InText.ToString(), InCommitType);
#endif

	OnEditableTextCommitted(InText, InCommitType);
}


void SEditableTextBoxEx::SetStyleEx(const FEditableTextBoxStyle* InStyle)
{
	Style = InStyle;

	if (Style == nullptr)
	{
		FArguments Defaults;
		Style = Defaults._Style;
	}

	check(Style);

	BorderImageNormalEx = &Style->BackgroundImageNormal;
	BorderImageHoveredEx = &Style->BackgroundImageHovered;
	BorderImageFocusedEx = &Style->BackgroundImageFocused;
	BorderImageReadOnlyEx = &Style->BackgroundImageReadOnly;

	SetStyle(InStyle);
}


const FSlateBrush* SEditableTextBoxEx::GetBorderImageEx() const
{
	if (EditableText->IsTextReadOnly())
	{
		return BorderImageReadOnlyEx;
	}
	else if (EditableText->HasKeyboardFocus())
	{
		return BorderImageFocusedEx;
	}
	else
	{
		if (EditableText->IsHovered())
		{
			return BorderImageHoveredEx;
		}
		else
		{
			return BorderImageNormalEx;
		}
	}
}
FSlateColor SEditableTextBoxEx::DetermineForegroundColorEx() const
{
	check(Style);

	if (EditableText->IsTextReadOnly())
	{
		if (ReadOnlyForegroundColorOverride.IsSet())
		{
			return ReadOnlyForegroundColorOverride.Get();
		}
		if (ForegroundColorOverride.IsSet())
		{
			return ForegroundColorOverride.Get();
		}

		return Style->ReadOnlyForegroundColor;
	}
	else
	{
		return ForegroundColorOverride.IsSet() ? ForegroundColorOverride.Get() : Style->ForegroundColor;
	}
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

