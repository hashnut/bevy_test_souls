// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISampleRegex.h"
#include "Components/MultiLineEditableText.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Internationalization/Regex.h"


void UGsUISampleRegex::OnClickApply()
{
	FString strTarget = _inputTarget->GetText().ToString();
	FString strPattrn = _inputRegex->GetText().ToString();
	FRegexPattern pattern(strPattrn);

	FString strResult;

	FRegexMatcher matcher(pattern, strTarget);
	int32 beginIndex = INDEX_NONE;

	while (matcher.FindNext())
	{
		int32 currBegin = matcher.GetMatchBeginning();
		if (INDEX_NONE != currBegin)
		{
			if (INDEX_NONE == beginIndex)
			{
				strResult += strTarget.Mid(0, currBegin);
			}
			else
			{
				int32 count = currBegin - beginIndex;
				if (0 < count)
				{
					strResult += strTarget.Mid(beginIndex, count);
				}
			}
		}

		beginIndex = matcher.GetMatchEnding();
	}

	if (INDEX_NONE == beginIndex)
	{
		strResult = strTarget;
	}
	else
	{
		int32 count = strTarget.Len() - beginIndex;
		if (0 < count)
		{
			strResult += strTarget.Mid(beginIndex, count);
		}
	}

	_textBlockResult->SetText(FText::FromString(strResult));
}
