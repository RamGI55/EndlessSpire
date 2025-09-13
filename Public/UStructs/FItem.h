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

	// 기본 생성자를 통해 멤버 초기화
	FItemStatistics()
		: ArmorRating(0.f)
		, DamageValue(0.f)
		, RestorationAmount(0.f)
		, SellValue(0.f)
		, ItemStat() // FPlayerCharacterStat의 기본 생성자 호출
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

	// FText 필드를 명시적으로 초기화
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
	// 수치 데이터를 기본값으로 초기화 (예: 기본 스택 사이즈는 1로)
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

	// 포인터 타입은 nullptr로 초기화
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

	// 모든 멤버를 기본 생성자에서 초기화
	FItemData()
		: ID(NAME_None)
		, ItemType(EItemType::Armor)         // 기본값으로 Armor 설정 (필요시 다른 값으로 수정)
		, ItemQuality(EItemQuality::Common)    // 기본값으로 Common 설정
		, ItemStatistics()                     // 각 구성요소의 기본 생성자 호출
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


