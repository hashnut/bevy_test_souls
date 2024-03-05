// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "T1Project.h"
#include "GsScrollBox.generated.h"

class UPanelWidget;
class UGsScrollBox;
struct FWidgetItem;

#pragma region FGsChatObjectPool
UCLASS()
class T1PROJECT_API UUserObjectScrollEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetListInfo(TWeakPtr<FWidgetItem> item) { _item = item; }
	TWeakPtr<FWidgetItem> GetItem() { return  _item; }

	void Release() { _lastDesiredSize = FVector2D::ZeroVector; };
	void InvalidateLayout() { InvalidateLayoutAndVolatility(); _lastDesiredSize = GetDesiredSize(); }
	FVector2D GetLastDesiredSize() { return _lastDesiredSize; }

	virtual void SetIsSelected(bool bInIsSelected) { if (_toggleCallback) _toggleCallback(this, bInIsSelected); };
	virtual bool GetIsSelected() const { return _isSelected; }
	
protected:
	/** Follows the same pattern as the NativeOn[X] methods in UUserWidget - super calls are expected in order to route the event to BP. */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) { InvalidateLayout(); };
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) {};

	void SetToggleCallback(TFunction<void(UUserObjectScrollEntry*, bool)> InToggleCallback) { _toggleCallback = InToggleCallback; }
private:
	TWeakPtr<FWidgetItem> _item;
	FVector2D _lastDesiredSize{ FVector2D::ZeroVector };

	bool _isSelected{ false };
	TFunction<void(UUserObjectScrollEntry*, bool)> _toggleCallback;

public:
	friend class UGsScrollBox;
};

USTRUCT()
struct T1PROJECT_API FWidgetItem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TWeakObjectPtr<UUserObjectScrollEntry> _widget{ nullptr };

public:
	void Init()
	{
		
	}
	void Release() 
	{ 
		if (_widget.IsValid())
		{
			_widget->Release();
		}
	}
	TWeakObjectPtr<UUserObjectScrollEntry>
		CreateWidgetItem(UWorld* world, TSubclassOf<UUserObjectScrollEntry> entryWidgetClass, TWeakPtr<FWidgetItem> item)
	{
		static int64 made = 0;
		if (_widget.IsValid() == false)
		{
			GSLOG(Log, TEXT("UGsScrollBox::CreateWidget : %d"), made++);
			_widget = Cast<UUserObjectScrollEntry>(CreateWidget(world, entryWidgetClass));
			_widget->SetListInfo(item);
		}
		return _widget;
	}
	TWeakObjectPtr<UUserObjectScrollEntry> GetWidget() { return  _widget; }

	void Reset() { if (_widget.IsValid()) _widget = nullptr; }
};


template<typename E, typename U>
struct T1PROJECT_API FGsChatObjectPool
{
	using TYPE = typename TEnableIf<TIsDerivedFrom<E, FWidgetItem>::IsDerived, E>::Type;
	using UTYPE = typename TEnableIf<TIsDerivedFrom<U, UUserObjectScrollEntry>::IsDerived, U>::Type;
private:
	TArray<TYPE*> _pool;

private:
	TYPE* Get() {
		if (_pool.Num() == 0)
		{
			_pool.Push(new TYPE());
		}
		return _pool.Pop();
	}
	void Release(TYPE* e) { e->Release(); _pool.Push(e); }

public:
	TSharedPtr<TYPE> GetItem(UWorld* world, TSubclassOf<UTYPE> entryWidgetClass)
	{
		TSharedPtr<TYPE> data = MakeShareable(Get(), [this](TYPE* item) { Release(item); });
		auto widget = data->CreateWidgetItem(world, entryWidgetClass, data);
		data->Init();
		return data;
	}

	void Flush() 
	{
		while (_pool.Num() > 0)
		{
			if (auto e = _pool.Pop())
				delete e;
		}
		_pool.Reset();
	}

	~FGsChatObjectPool() { Flush(); }
};
#pragma endregion

/**
 * Listbox 처럼 사용하기 위한 scrollbox  SetListItems
 */
UCLASS()
class T1PROJECT_API UGsScrollBox : public UScrollBox
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(Transient)
	TArray<UObject*> ListItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float toEndTolerance = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
	bool useSelection{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserObjectScrollEntry> EntryWidgetClass;

	UUserObjectScrollEntry* _selectEntry{ nullptr };

public:
	virtual void Destruct();

	template <typename ItemObjectT, typename AllocatorType = FDefaultAllocator>
	void SetListItems(const TArray<ItemObjectT, AllocatorType>& InListItems)
	{
		ClearListItems();
		ListItems.Append(InListItems);

		RequestRefresh();
	}

	EOrientation GetOrientation() const { return Orientation; }

	/**
	 * Gets the list of all items in the list.
	 * Note that each of these items only has a corresponding entry widget when visible. Use GetDisplayedEntryWidgets to get the currently displayed widgets.
	 */
	UFUNCTION(BlueprintCallable, Category = ListView)
	const TArray<UObject*>& GetListItems() const { return ListItems; }

	/** Adds an the item to the list */
	UFUNCTION(BlueprintCallable, Category = ListView)
	void AddItem(UObject* Item);

	/** Removes an the item from the list */
	UFUNCTION(BlueprintCallable, Category = ListView)
	void RemoveItem(UObject* Item);

	/** Returns the item at the given index */
	UFUNCTION(BlueprintCallable, Category = ListView)
	UObject* GetItemAt(int32 Index) const;

	/** Returns the total number of items */
	UFUNCTION(BlueprintCallable, Category = ListView)
	int32 GetNumItems() const { return ListItems.Num();  }

	/** Returns the index that the specified item is at. Will return the first found, or -1 for not found */
	UFUNCTION(BlueprintCallable, Category = ListView)
	int32 GetIndexForItem(UObject* Item) const;

	void ClearListItems();

	TSubclassOf<UUserObjectScrollEntry> GetEntryWidgetClass() const { return EntryWidgetClass; }

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

public:
	virtual void BeginDestroy() override;

	void InitializeData();

	DECLARE_EVENT_OneParam(UGsScrollBox, FOnScrollEntryGenerated, UUserObjectScrollEntry&);
	FOnScrollEntryGenerated& OnEntryWidgetGenerated() { return OnEntryWidgetGeneratedEvent; }

	DECLARE_EVENT_OneParam(UGsScrollBox, FOnScrollEntryWidgetReleased, UUserObjectScrollEntry&);
	FOnScrollEntryWidgetReleased& OnEntryWidgetReleased() { return OnEntryWidgetReleasedEvent; }

	void SetScrollIndex(int32 topIndex);
	void SetScrollEnd();

	bool IsScrollEndPosition();

	int GetItemIndexByOffset(float offset);

	void SetSelectIndex(int32 index);

	void SetSelectEntry(const UUserObjectScrollEntry* selectEntry, bool forced = false);
	UUserObjectScrollEntry* GetSelectEntry() const { return _selectEntry; }

	void RefreshAll();

protected:
	void RequestRefresh();
	void SlateHandleTick(const FGeometry& Geometry, const float DeltaTime);

	FORCEINLINE float GetScrollComponentFromVector(FVector2D Vector) const
	{
		return Orientation == EOrientation::Orient_Vertical ? Vector.Y : Vector.X;
	}

protected:
	FOnScrollEntryGenerated OnEntryWidgetGeneratedEvent;
	FOnScrollEntryWidgetReleased OnEntryWidgetReleasedEvent;

private: // guild chat scroll 처리
	bool _toBottomScrolled{ false };

	TArray<TSharedPtr<FWidgetItem>> _widgets;
	FGsChatObjectPool<FWidgetItem, UUserObjectScrollEntry> _entryPool;
};

