/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "DataSchema/Item/GsSchemaItemIngredientDetail.h"
#include "UI/UIContent/Define/EGsIngredientUse_UIType.h"
FGsSchemaItemIngredientDetail::FGsSchemaItemIngredientDetail()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaItemIngredientDetail)
	
	id = 0;
	isUseItem = false;
	contentsType = EGsIngredientUse_UIType::ContentsType_Max;
	ingredientDecrement = 0;
	cooldownTime = 0.000000f;
	cooldownGroupId = 0;
	effectType = ItemEffectType::NONE;
	tooltipDurationTime = 0;
	isAllowedAutoUse = false;
	isAllowedAutoSell = false;
}
