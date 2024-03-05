#pragma once

#include "CoreMinimal.h"
#include "item/Collection/GsItemCollectionBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"


/**
* file		FGsGuildItemCollection.h
* @brief	기사단 전용 아이템 콜렉션, FGsItemCollectionBase를 상속 받는다.
* @author	PKT
* @date		2021/09/17
**/

class FGsGuildItemCollectionState final : public FGsItemCollectionBase
{
private:	
	// 2021/10/07 PKT - Data Schema
	const struct FGsSchemaGuildItemCollection* schemaItemCollection = nullptr;

	/************************************************************************/
	/* Constructor                                                          */
	/************************************************************************/
public:
	explicit FGsGuildItemCollectionState(const struct FGsSchemaGuildItemCollection* InSchemaItemCollection);


	/************************************************************************/
	/* Create Slot                                                          */
	/************************************************************************/
public:
	void CreateSlotList(const TArray<GuildCollectionCondition>& InGuildCollectionConditionList);

	/************************************************************************/
	/* Parser                                                               */
	/************************************************************************/
private:
	bool ParserSlotPropert(const int32 InMaxCount, OUT TArray<const struct FGsSchemaGuildItemCollectionInnerSlotProperty*>* OutSlotPropertyList);
	bool ParserAvaildItem(CollectionRandomDataId InRandomTableKey, OUT TArray<TPair<ItemId, Level>>* OutMaterialItemSet);

	/************************************************************************/
	/* Get                                                                  */
	/************************************************************************/
public:
	GuildItemCollectionType GetCollectionType() const;

	virtual void RegisterCollection(const TPair<CollectionConditionId, const FText>& InRegisterInfo);

	virtual bool IsHighlightBackGround() const override;
};