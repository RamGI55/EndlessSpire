#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UStructs/FPlayerCharacterStat.h"
#include "FItem.generated.h"

/**

*/

UENUM()
enum class EItemQuality : uint8
{
	Common UMETA(DisplayName = "Common"),
	Magic UMETA(DisplayName = "Magic"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Unique UMETA(DisplayName = "Unique"),
};

UENUM()
enum class EItemType : uint8
{
	Armor UMETA(DisplayName = "Armor"),
	Helm UMETA(DisplayName = "Helm"),
	Shoes UMETA(DisplayName = "Shoes"),
	MainWeapon UMETA(DisplayName = "MainWeapon"),
	Amulet UMETA(DisplayName = "Amulet"),
	Ring UMETA(DisplayName = "Ring"),
	Spell UMETA(DisplayName = "Spell"),
	Mundane UMETA(DisplayName = "Mundane"),
	Consumable UMETA(DisplayName = "Consumable"),
};

USTRUCT()
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()

	// �⺻ �����ڸ� ���� ��� �ʱ�ȭ
	FItemStatistics()
		: ArmorRating(0.f)
		, DamageValue(0.f)
		, RestorationAmount(0.f)
		, SellValue(0.f)
		, ItemStat() // FPlayerCharacterStat�� �⺻ ������ ȣ��
	{}

	UPROPERTY(EditAnywhere)
	float ArmorRating;

	UPROPERTY(EditAnywhere)
	float DamageValue;

	UPROPERTY(EditAnywhere)
	float RestorationAmount;

	UPROPERTY(EditAnywhere)
	float SellValue;

	UPROPERTY(EditAnywhere)
	FPlayerCharacterStat ItemStat;
};


USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	// FText �ʵ带 ��������� �ʱ�ȭ
	FItemTextData()
		: Name(FText::GetEmpty())
		, Description(FText::GetEmpty())
		, InteractionText(FText::GetEmpty())
		, UsageText(FText::GetEmpty())
	{}


	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText;
};

USTRUCT(BlueprintType, Atomic)
struct FItemNumericData : public FTableRowBase
{
	// ��ġ �����͸� �⺻������ �ʱ�ȭ (��: �⺻ ���� ������� 1��)
	FItemNumericData()
		: Weight(0.f)
		, MaxStackSize(1)
		, bIsStackable(false)
	{}

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	bool bIsStackable; 
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	// ������ Ÿ���� nullptr�� �ʱ�ȭ
	FItemAssetData()
		: Icon(nullptr)
		, Mesh(nullptr)
	{}

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// ��� ����� �⺻ �����ڿ��� �ʱ�ȭ
	FItemData()
		: ID(NAME_None)
		, ItemType(EItemType::Armor)         // �⺻������ Armor ���� (�ʿ�� �ٸ� ������ ����)
		, ItemQuality(EItemQuality::Common)    // �⺻������ Common ����
		, ItemStatistics()                     // �� ��������� �⺻ ������ ȣ��
		, TextData()
		, NumericData()
		, AssetData()
	{}

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;


};


