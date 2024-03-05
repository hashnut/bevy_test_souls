/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/EventProgress/GsSchemaEventProgress.h"
FGsSchemaEventProgress::FGsSchemaEventProgress()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaEventProgress)
	
	_type = EGsEventProgressType::NONE;
	_order = 0;
	_lowerOrder = EGsEventProgressWorkType::NOT_DO;
	_higherOrder = EGsEventProgressWorkType::NOT_DO;
	_isDirect = false;
}
