/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "DataSchema/SubDialog/GsSchemaSubDialogActorData.h"
FGsSchemaSubDialogActorData::FGsSchemaSubDialogActorData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSubDialogActorData)
	
	Id = 0;
	facetypeList = TArray<FSoftObjectPath> ();
	facetypeList.Reserve(4);
	facetypeList.Add(FSoftObjectPath(TEXT("/Game/UI/Texture/DialogNPC/SPR_IMG_SubDialogDummyNormal.SPR_IMG_SubDialogDummyNormal")));
	facetypeList.Add(FSoftObjectPath(TEXT("/Game/UI/Texture/DialogNPC/SPR_IMG_SubDialogDummyAngry.SPR_IMG_SubDialogDummyAngry")));
	facetypeList.Add(FSoftObjectPath(TEXT("/Game/UI/Texture/DialogNPC/SPR_IMG_SubDialogDummySmile.SPR_IMG_SubDialogDummySmile")));
	facetypeList.Add(FSoftObjectPath(TEXT("/Game/UI/Texture/DialogNPC/SPR_IMG_SubDialogDummySad.SPR_IMG_SubDialogDummySad")));
}
