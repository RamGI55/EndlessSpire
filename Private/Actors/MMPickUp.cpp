// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MMPickUp.h"
#include "Engine/DataTable.h"
#include "UObjects/ItemBase.h"
#include "Blueprint/UserWidget.h"
#include "PlayerControllers/LobbyPlayerController.h"
#include "ActorComponents/MMInventoryComponent.h"
#include "Interface/InteractionInterface.h"
#include "UStructs/FItem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

AMMPickUp::AMMPickUp()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickUpMesh");
	SetRootComponent(PickUpMesh);

	ConstructorHelpers::FObjectFinder<UDataTable> TempItemDataTable(TEXT("/Script/Engine.DataTable'/Game/ItemData/DT_ItemData.DT_ItemData'"));
	ItemDataTable = TempItemDataTable.Object;

	ConstructorHelpers::FClassFinder<UUserWidget> InteractionAsset(TEXT("WidgetBlueprint'/Game/UI/WBP_InteractionWidget.WBP_InteractionWidget_C'"));
	if (InteractionAsset.Succeeded())
	{
		InteractionWidgetClass = InteractionAsset.Class;
	}


	PickUpMesh->SetSimulatePhysics(true);
	PickUpMesh->SetNotifyRigidBodyCollision(true);


	if (PickUpMesh)
	{
		PickUpMesh->OnComponentHit.AddDynamic(this, &AMMPickUp::OnHit);
	}




}

void AMMPickUp::BroadcastItemDataInitialize_Implementation(const FName& ItemID, const int32 InQuantity)
{
	DesiredItemID = ItemID;
	ItemQuantity = InQuantity;
	InitializePickUp(UItemBase::StaticClass(), ItemQuantity);
}

void AMMPickUp::BroadcastDropFromTargetInventory_Implementation(ABasePlayerCharacter* Player, const FName ItemID, const int32 QuantityToDrop)
{
	UE_LOG(LogTemp, Log, TEXT("%s : %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"), *UKismetSystemLibrary::GetDisplayName(Player));
	if (!HasAuthority()) {
		UItemBase* MatchingItem = Player->GetInventory()->FindMatchingItem(ItemID);
		if (MatchingItem) {
			Player->GetInventory()->RemoveAmountOfItem(MatchingItem, QuantityToDrop);

			// 제거한 수량만큼 스폰한 액터에 아이템 데이터 설정
			InitializeDrop(MatchingItem, QuantityToDrop);
		}
	}
}

void AMMPickUp::BeginPlay()
{
	Super::BeginPlay();

	PickUpMesh->SetCollisionProfileName(TEXT("PickUpItem"));

	if (!DesiredItemID.IsNone())
	{
		InitializePickUp(UItemBase::StaticClass(), ItemQuantity);
	}
}

void AMMPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


void AMMPickUp::InitializePickUp(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;

		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->ItemStatistics = ItemData->ItemStatistics;

		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickUpMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void AMMPickUp::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	PickUpMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);

	UpdateInteractableData();
}

void AMMPickUp::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;

	InteractableData = InstanceInteractableData;
}

void AMMPickUp::BeginFocus()
{

	UE_LOG(LogTemp, Log, TEXT("Item"));

	if (!HasAuthority())
	{
		if (PickUpMesh)
		{
			PickUpMesh->SetRenderCustomDepth(true);
		}

		ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		InteractionWidget = CreateWidget(LobbyPC, InteractionWidgetClass);

		if (InteractionWidget)
		{
			InteractionWidget->AddToViewport();
		}
	}
}

void AMMPickUp::EndFocus()
{
	if (!HasAuthority())
	{
		if (PickUpMesh)
		{
			PickUpMesh->SetRenderCustomDepth(false);
		}

		InteractionWidget->RemoveFromParent();	
	}
}

void AMMPickUp::Interaction(ABasePlayerCharacter* Player)
{
	SetOwner(Player);

	if (HasAuthority())
	{
		if (Player)
		{
			BroadcastTakePickUp(Player);
		}
	}
	else
	{
		if (Player)
		{
			InteractionWidget->RemoveFromParent();
		}
	}
}

void AMMPickUp::TakePickUp(const ABasePlayerCharacter* Taker)
{
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (UMMInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				PlayerInventory->PrintLogInventory();

				switch (AddResult.OperationResult)
				{
				case EItemAddResult::IAR_NoItemAdded :
					break;
				case EItemAddResult::IAR_PartialAmountItemAdded :
					UpdateInteractableData();
					break;
				case EItemAddResult::IAR_AllItemAdded :
					if (HasAuthority())
					{
						Destroy();
					}
					UE_LOG(LogTemp, Warning, TEXT("Item Destroy"));
					break;
				}

				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PickUp internal item reference was somehow null"));
		}
	}
}
void AMMPickUp::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	ECollisionChannel OtherChannel = OtherComp->GetCollisionObjectType();
	if (OtherChannel == ECC_WorldStatic)
	{
		PickUpMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);


	}
	
}



void AMMPickUp::BroadcastTakePickUp_Implementation(ABasePlayerCharacter* Player)
{
	TakePickUp(Player);
}

#if WITH_EDITOR
void AMMPickUp::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AMMPickUp, DesiredItemID))
	{
		if (ItemDataTable)
		{
			const FString ContextString{ DesiredItemID.ToString() };

			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
			{
				PickUpMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}
#endif
