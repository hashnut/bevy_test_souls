// Copyright/2019/05/02 - BSAM
// Declaring a structure that contains information for each folder in the Path View(folder) of the Skill Editor(see ContentBrowser Module)

#pragma once

#include "CoreMinimal.h"

#include "ContentBrowserDelegates.h"

struct FSkillTreeItem : public TSharedFromThis<FSkillTreeItem>
{
	FText DisplayName;
	FString FolderName;
	FString FolderPath;
	bool bNamingFolder;

	TArray< TSharedPtr<FSkillTreeItem> > Children;

	TWeakPtr<FSkillTreeItem> Parent;

	DECLARE_MULTICAST_DELEGATE( FRenamedRequestEvent )

	FRenamedRequestEvent OnRenamedRequestEvent;

	FSkillTreeItem(FText InDisplayName, FString InFolderName, FString InFolderPath, TSharedPtr<FSkillTreeItem> InParent, bool InNamingFolder = false)
		: DisplayName(MoveTemp(InDisplayName))
		, FolderName(MoveTemp(InFolderName))
		, FolderPath(MoveTemp(InFolderPath))
		, bNamingFolder(InNamingFolder)
		, Parent(MoveTemp(InParent))
		, bChildrenRequireSort(false)
	{}

	bool IsChildOf(const FSkillTreeItem& InParent)
	{
		auto CurrentParent = Parent.Pin();
		while (CurrentParent.IsValid())
		{
			if (CurrentParent.Get() == &InParent)
			{
				return true;
			}

			CurrentParent = CurrentParent->Parent.Pin();
		}

		return false;
	}
	TSharedPtr<FSkillTreeItem> GetChild(const FString& InChildFolderName) const
	{
		for (const auto& Child : Children)
		{
			if (Child->FolderName == InChildFolderName)
			{
				return Child;
			}
		}

		return nullptr;
	}

	TSharedPtr<FSkillTreeItem> FindItemRecursive(const FString& InFullPath)
	{
		if (InFullPath == FolderPath)
		{
			return SharedThis(this);
		}

		for (const auto& Child : Children)
		{
			const TSharedPtr<FSkillTreeItem>& Item = Child->FindItemRecursive(InFullPath);
			if (Item.IsValid())
			{
				return Item;
			}
		}

		return nullptr;
	}

	void RequestSortChildren()
	{
		bChildrenRequireSort = true;
	}

	void SortChildrenIfNeeded()
	{
		if (bChildrenRequireSort)
		{
			Children.Sort([](TSharedPtr<FSkillTreeItem> A, TSharedPtr<FSkillTreeItem> B) -> bool
			{
				return A->DisplayName.ToString() < B->DisplayName.ToString();
			});

			bChildrenRequireSort = false;
		}
	}

private:
	bool bChildrenRequireSort;
};

namespace ESkillAssetItemType
{
	enum Type
	{
		Normal,
		Folder,
		Creation,
		Duplication
	};
}

struct FSkillAssetViewItem
{
	FSkillAssetViewItem()
		: bRenameWhenScrolledIntoview(false)
	{
	}

	virtual ~FSkillAssetViewItem() {}

	virtual ESkillAssetItemType::Type GetType() const = 0;

	virtual bool IsTemporaryItem() const = 0;

	virtual void CacheCustomColumns(const TArray<FAssetViewCustomColumn>& CustomColumns, bool bUpdateSortData, bool bUpdateDisplayText, bool bUpdateExisting) {}

	FSimpleDelegate RenamedRequestEvent;

	FSimpleDelegate RenameCanceledEvent;

	DECLARE_MULTICAST_DELEGATE(FOnAssetDataChanged);
	FOnAssetDataChanged OnAssetDataChanged;

	bool bRenameWhenScrolledIntoview;
};

struct FSkillAssetViewAsset : public FSkillAssetViewItem
{
	FAssetData Data;

	TMap<FName, FString> CustomColumnData;

	TMap<FName, FText> CustomColumnDisplayText;

	explicit FSkillAssetViewAsset(const FAssetData& AssetData)
		: Data(AssetData)
	{}

	void SetAssetData(const FAssetData& NewData)
	{
		Data = NewData;
		OnAssetDataChanged.Broadcast();
	}

	bool GetTagValue(FName Tag, FString& OutString) const
	{
		const FString* FoundString = CustomColumnData.Find(Tag);

		if (FoundString)
		{
			OutString = *FoundString;
			return true;
		}

		return Data.GetTagValue(Tag, OutString);
	}

	virtual ESkillAssetItemType::Type GetType() const override
	{
		return ESkillAssetItemType::Normal;
	}

	virtual bool IsTemporaryItem() const override
	{
		return false;
	}

	virtual void CacheCustomColumns(const TArray<FAssetViewCustomColumn>& CustomColumns, bool bUpdateSortData, bool bUpdateDisplayText, bool bUpdateExisting) override
	{
		for (const FAssetViewCustomColumn& Column : CustomColumns)
		{
			if (bUpdateSortData)
			{
				if (bUpdateExisting ? CustomColumnData.Contains(Column.ColumnName) : !CustomColumnData.Contains(Column.ColumnName))
				{
					CustomColumnData.Add(Column.ColumnName, Column.OnGetColumnData.Execute(Data, Column.ColumnName));
				}
			}

			if (bUpdateDisplayText)
			{
				if (bUpdateExisting ? CustomColumnDisplayText.Contains(Column.ColumnName) : !CustomColumnDisplayText.Contains(Column.ColumnName))
				{
					if (Column.OnGetColumnDisplayText.IsBound())
					{
						CustomColumnDisplayText.Add(Column.ColumnName, Column.OnGetColumnDisplayText.Execute(Data, Column.ColumnName));
					}
					else
					{
						CustomColumnDisplayText.Add(Column.ColumnName, FText::AsCultureInvariant(Column.OnGetColumnData.Execute(Data, Column.ColumnName)));
					}
				}
			}
		}
	}
};

struct FSkillAssetViewFolder : public FSkillAssetViewItem
{
	/** The folder this item represents */
	FString FolderPath;

	/** The folder this item represents, minus the preceding path */
	FText FolderName;

	/** Whether this is a developer folder */
	bool bDeveloperFolder;

	/** Whether this is a collection folder */
	bool bCollectionFolder;

	/** Whether this folder is a new folder */
	bool bNewFolder;

	FSkillAssetViewFolder(const FString& InPath)
		: FolderPath(InPath)
		, bNewFolder(false)
	{
		FolderName = FText::FromString(FPaths::GetBaseFilename(FolderPath));
		bDeveloperFolder = false;// ContentBrowserUtils::IsDevelopersFolder(FolderPath);
		bCollectionFolder = false;// ContentBrowserUtils::IsCollectionPath(FolderPath);
	}

	/** Set the name of this folder (without path) */
	void SetFolderName(const FString& InName)
	{
		FolderPath = FPaths::GetPath(FolderPath) / InName;
		FolderName = FText::FromString(InName);
		OnAssetDataChanged.Broadcast();
	}

	// FAssetViewItem interface
	virtual ESkillAssetItemType::Type GetType() const override
	{
		return ESkillAssetItemType::Folder;
	}

	virtual bool IsTemporaryItem() const override
	{
		return false;
	}
};

struct FSkillAssetViewCreation : public FSkillAssetViewAsset, public FGCObject
{
	/** The class to use when creating the asset */
	UClass* AssetClass;

	/** The factory to use when creating the asset. */
	UFactory* Factory;

	FSkillAssetViewCreation(const FAssetData& AssetData, UClass* InAssetClass, UFactory* InFactory)
		: FSkillAssetViewAsset(AssetData)
		, AssetClass(InAssetClass)
		, Factory(InFactory)
	{}

	// FAssetViewItem interface
	virtual ESkillAssetItemType::Type GetType() const override
	{
		return ESkillAssetItemType::Creation;
	}

	virtual bool IsTemporaryItem() const override
	{
		return true;
	}

	// FGCObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		Collector.AddReferencedObject(AssetClass);
		Collector.AddReferencedObject(Factory);
	}
};