// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIControlLib/TextAnimation/GsTextAnimation.h"
#include "TextBlock.h"
#include "RichTextBlock.h"
#include "Engine/DataTable.h"
#include "T1Project.h"
#include "TimerManager.h"

//#define LOG_DEBUG_TEXTANI

void UGsTextAnimation::BeginDestroy()
{
	_styleNameList.Empty();
	Super::BeginDestroy();
}

void UGsTextAnimation::Initialize(float inVelocity, UDataTable* inRichTextStyle)
{	
	_velocity = inVelocity;

	_styleNameList.Empty();
	if (nullptr != inRichTextStyle)
	{
		for (const auto& entry : inRichTextStyle->GetRowMap())
		{
			_styleNameList.Emplace(FString::Format(TEXT("{0}{1}{2}"), { TEXT("<"), *entry.Key.ToString(), TEXT(">") }));
		}
	}

	GSLOG(Log, TEXT("[GsTextAnimation] Initialize styleNameListNum : %d"), _styleNameList.Num());
}

void UGsTextAnimation::Finalize()
{
	if (_timerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
		_timerHandle.Invalidate();
	}
}

void UGsTextAnimation::SetData(class UTextBlock* inTextBlock, FString inString)
{
	// 타이머 설정
	UWorld* world = GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().PauseTimer(_timerHandle);
	}

	_isFindTag = false;
	_currPos = 0;
	_richTextBlock = nullptr;
	_textBlock = inTextBlock;	
	_textBlock->SetText(FText::FromString(""));
	_strScript = inString;
	_textLen = _strScript.Len();

	StartAnimation();
}

void UGsTextAnimation::SetData(class URichTextBlock* inTextBlock, FString inString)
{
	// 타이머 설정
	UWorld* world = GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().PauseTimer(_timerHandle);
	}

	_isFindTag = false;
	_currPos = 0;
	_textBlock = nullptr;
	_richTextBlock = inTextBlock;
	_richTextBlock->SetText(FText::FromString(""));
	_strScript = inString;
	_textLen = _strScript.Len();

	StartAnimation();
}

void UGsTextAnimation::SetPostion(int inPos)
{
	_currPos = inPos;
}

void UGsTextAnimation::Update(float InDeltaTime)
{

}

void UGsTextAnimation::StartAnimation()
{
	// 타이머 설정
	UWorld* world = GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().UnPauseTimer(_timerHandle);
		return;
	}

	world->GetTimerManager().SetTimer(_timerHandle, FTimerDelegate::CreateLambda([this]()
		{
			SetTextAnimation();

		}), _velocity, true);
}

void UGsTextAnimation::StopAnimation()
{
	_currPos = _textLen;

	SetText(_strScript);

	// 타이머 설정
	UWorld* world = GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().PauseTimer(_timerHandle);
	}
}

void UGsTextAnimation::SetTextAnimation()
{
	if (_textLen > _currPos)
	{
		FString strTalk;
		if (true == _isFindTag)
		{
#ifdef LOG_DEBUG_TEXTANI
			GSLOG(Log, TEXT("[GsTextAnimation] 145 SetTextAnimation FindTag(true) _currPos : %d"), _currPos);
#endif

			// 명령어가 시작 된 후 : 뒤에 끝나는 명령어를 넣어준다.		
			int32 findNum = _strScript.Find(*TAG_CLOSE, ESearchCase::IgnoreCase, ESearchDir::FromStart, _currPos);
			if (-1 < findNum)
			{
#ifdef LOG_DEBUG_TEXTANI			
				GSLOG(Log, TEXT("[GsTextAnimation] 153 SetTextAnimation TAG_CLOSE Find currPos : %d, findNum : %d "), _currPos, findNum);
#endif
				if (_currPos == findNum)
				{
					strTalk = FString::Format(TEXT("{0}"), { _strScript.Left(_currPos += TAG_CLOSE.Len()) });
					_isFindTag = false;
				}
				else
				{
					strTalk = FString::Format(TEXT("{0}{1}"), { _strScript.Left(_currPos), TAG_CLOSE });
				}
			}
#ifdef LOG_DEBUG_TEXTANI
			GSLOG(Log, TEXT("[GsTextAnimation] 166 SetTextAnimation FindTag(true) strTalk : %s"), *strTalk);
#endif
			SetText(strTalk);
		}
		else
		{
#ifdef LOG_DEBUG_TEXTANI
			GSLOG(Log, TEXT("[GsTextAnimation] 173 SetTextAnimation FindTag(false) _currPos : %d"), _currPos);
#endif

			// 명령어가 없거나, 끝났다.
			for (FString& key : _styleNameList)
			{
				int currPos = _currPos;
				int32 findNum = _strScript.Find(*key, ESearchCase::IgnoreCase, ESearchDir::FromStart, currPos);
#ifdef LOG_DEBUG_TEXTANI		
				GSLOG(Log, TEXT("[GsTextAnimation] 181 SetTextAnimation strScript : %s, key : %s, Find currPos : %d, findNum : %d, _currPos : %d "), 
					*_strScript, *key, currPos, findNum, _currPos);
#endif
				if (-1 < findNum)
				{
					if (_currPos == findNum)
					{
						int32 findTagCloseNum = _strScript.Find(*TAG_CLOSE, ESearchCase::IgnoreCase, ESearchDir::FromStart, _currPos);

						_isFindTag = true; 
						_currPos = findNum + key.Len()/* + 1*/;
						strTalk = FString::Format(TEXT("{0}{1}"), { _strScript.Left(_currPos), TAG_CLOSE });

						GSLOG(Log, TEXT("[GsTextAnimation] 191 SetTextAnimation Find(true) strTalk : %s, currPos : %d, findNum : %d "), 
							*strTalk, _currPos, findNum);

						SetText(strTalk);
						return;
					}
				}
			}
#ifdef LOG_DEBUG_TEXTANI
			GSLOG(Log, TEXT("[GsTextAnimation] 200 SetTextAnimation Find(false) strTalk : %s, currPos : %d"), 
				*_strScript.Left(_currPos), _currPos);
#endif
			SetText(_strScript.Left(_currPos));
		}
	}
	else
	{
#ifdef LOG_DEBUG_TEXTANI
		GSLOG(Log, TEXT("[GsTextAnimation] 209 SetTextAnimation EndText"));
#endif

		SetText(_strScript);

		// 타이머 설정
		UWorld* world = GetWorld();
		if (nullptr == world)
			return;

		if (_timerHandle.IsValid())
		{
			world->GetTimerManager().PauseTimer(_timerHandle);
		}
	}
}

void UGsTextAnimation::SetText(FString inValue)
{
	if (nullptr != _textBlock)
	{
		_textBlock->SetText(FText::FromString(inValue));
	}

	if (nullptr != _richTextBlock)
	{
		_richTextBlock->SetText(FText::FromString(inValue));
	}

	++_currPos;
}

bool UGsTextAnimation::IsAnimation()
{
	if (_timerHandle.IsValid())
	{
		// 타이머 설정
		UWorld* world = GetWorld();
		if (nullptr == world)
			return false;

		return world->GetTimerManager().IsTimerActive(_timerHandle);
	}
	
	return false;
}