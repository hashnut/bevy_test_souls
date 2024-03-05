#include "GsInputEventValidator.h"

FGsInputKeyEventValidator::FGsInputKeyEventValidator()
{
	InitializeValidator();
}

void FGsInputKeyEventValidator::InitializeValidator()
{
	if (!_validatorMap.IsSet())
	{
		_validatorMap.Emplace();
	}

	// EGsKeyMappingType 과 EGsKeyMappingInputFlags 의 flags 들을 매핑
	for (const auto& key : TEnumRange<EGsKeyMappingType>())
	{
		switch (key)
		{
		case EGsKeyMappingType::NONE:
			_validatorMap->Emplace(key, EGsKeyMappingInputFlags::INPUT_NONE);
			break;
		case EGsKeyMappingType::KEY_MAPPING_MOVE_UP:
		case EGsKeyMappingType::KEY_MAPPING_MOVE_DOWN:
		case EGsKeyMappingType::KEY_MAPPING_MOVE_LEFT:
		case EGsKeyMappingType::KEY_MAPPING_MOVE_RIGHT:
			_validatorMap->Emplace(key, EGsKeyMappingInputFlags::INPUT_KEY_MOVE);
			break;
		case EGsKeyMappingType::KEY_MAPPING_OK:
			_validatorMap->Emplace(key, EGsKeyMappingInputFlags::INPUT_KEY_OK);
			break;
		case EGsKeyMappingType::KEY_MAPPING_NO:
		case EGsKeyMappingType::KEY_MAPPING_NO_ANDROID_BACK:
		case EGsKeyMappingType::KEY_MAPPING_NO_ANDROID_BACKSPACE:
			_validatorMap->Emplace(key, EGsKeyMappingInputFlags::INPUT_KEY_NO);
			break;
		case EGsKeyMappingType::TOUCH_MOVE:
			_validatorMap->Emplace(key, EGsKeyMappingInputFlags::INPUT_TOUCH_MOVE);
			break;
		case EGsKeyMappingType::TOUCH_INTERACTION:
			_validatorMap->Emplace(key, EGsKeyMappingInputFlags::INPUT_TOUCH_INTERACTION);
			break;
		default:
			_validatorMap->Emplace(key, EGsKeyMappingInputFlags::INPUT_KEY_MISC);
			break;
		}
	}
}

bool FGsInputKeyEventValidator::IsValidEventToState(EGsKeyMappingType InEvent, EGsKeyMappingInputFlags InState) const
{
	// Function Test
	//bool testBool = this->IsValidEventToState(InEvent, InState);

	if (!_validatorMap.IsSet())
	{
		return false;
	}

	// InEvent 로 들어온 키 입력이 현재 InState 에서 막고 있는 것인지 체크
	if (const EGsKeyMappingInputFlags* flag = _validatorMap->Find(InEvent))
	{
		return EnumHasAnyFlags(InState, *flag);
	}

	return false;
}

/*
* 임시로 원하는 키에 원하는 플래그를 추가 -> Input 에 한해서는 사용할 일 없을 것으로 보임
*/  
void FGsInputKeyEventValidator::AddValidEventToState(EGsKeyMappingType InEvent, EGsKeyMappingInputFlags InState)
{
	TGsEnumEventValidatorBase::AddValidEventToState(InEvent, InState);

	// 구조 변경으로 아래 코드는 폐기
	//if (!_validatorMap.IsSet())
	//{
	//	_validatorMap.Emplace();
	//}

	//if (_validatorMap->Contains(InEvent))
	//{
	//	EnumAddFlags(_validatorMap.GetValue()[InEvent], InState);
	//}
	//else
	//{
	//	_validatorMap->Emplace(InEvent, InState);
	//}
}
