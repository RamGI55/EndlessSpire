// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteractActors/InteractChest.h"
#include "UStructs/FItem.h"
#include "Actors/MMPickUp.h"
#include "GameStates/PlayGameState.h"
#include "Net/UnrealNetwork.h"



AInteractChest::AInteractChest()
{
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName("InteractActor");

	ConstructorHelpers::FObjectFinder<UStaticMesh> CloseMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Assets/AncientTreasures/Meshes/SM_Chest_01a.SM_Chest_01a'"));
	if (CloseMeshAsset.Succeeded())
	{
		CloseMesh = CloseMeshAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> OpenMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Assets/AncientTreasures/Meshes/SM_Chest_01b.SM_Chest_01b'"));
	if (OpenMeshAsset.Succeeded())
	{
		OpenedMesh = OpenMeshAsset.Object;
	}

	bOpened = false;

	// 아이템 데이터 테이블
	ConstructorHelpers::FObjectFinder<UDataTable> TempItemDataTable(TEXT("/Script/Engine.DataTable'/Game/ItemData/DT_ItemData.DT_ItemData'"));
	if (TempItemDataTable.Succeeded())
	{
		ItemDataTable = TempItemDataTable.Object;
	}

}

void AInteractChest::BeginPlay()
{
	if (CloseMesh)
	{
		Mesh->SetStaticMesh(CloseMesh);
	}

	if (HasAuthority())
	{
		TArray<FName> RowNames = ItemDataTable->GetRowNames();
		int32 RandomCount = 3; //FMath::RandRange(1, 5);

		for (int i = 0; i < RandomCount; i++)
		{
			if (RowNames.Num() > 0)
			{
				// 랜덤한 인덱스 선택
				int32 RandomIndex = FMath::RandRange(0, RowNames.Num() - 1);
				FName RandomRowName = RowNames[RandomIndex];

				// 랜덤한 행 가져오기
				FItemData* RandomRow = ItemDataTable->FindRow<FItemData>(RandomRowName, TEXT("RandomRow"));
				if (RandomRow) 
				{
					SpawnItemsID.Add(RandomRowName);
				}
			}
		}
	}
}

void AInteractChest::BeginFocus()
{
	if (!HasAuthority())
	{
		if (!bOpened)
		{
			if (Mesh)
			{
				Mesh->SetRenderCustomDepth(true);
			}
		}
	
	}
}

void AInteractChest::EndFocus()
{
	if (!HasAuthority())
	{
		if (Mesh)
		{
			Mesh->SetRenderCustomDepth(false);
		}
	}
}

void AInteractChest::BeginInteraction()
{


}

void AInteractChest::EndInteraction()
{
}

void AInteractChest::Interaction(ABasePlayerCharacter* Player)
{
	if (HasAuthority())
	{
		if (!bOpened)
		{
			Multicast_OpenChest();

			SpawnRandomItem();
		}

	}

}


void AInteractChest::SpawnRandomItem()
{
	// 데이터 테이블에서 모든 행을 가져오기
	if (!HasAuthority())
	{
		return;
	}

	for (FName Element : SpawnItemsID)
	{
		// 랜덤 오프셋으로 위치 조정
		FVector SpawnLocation = GetActorLocation() + FVector(FMath::FRandRange(-50.f, 50.f), FMath::FRandRange(-50.f, 50.f), 100.f);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AMMPickUp* SpawnedItem = GetWorld()->SpawnActor<AMMPickUp>(AMMPickUp::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedItem)
		{
			SpawnedItem->BroadcastItemDataInitialize(Element, 1);
		}

		if (SpawnedItem && SpawnedItem->GetRootComponent())
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnedItem  && SpawnedItem->GetRootComponent(): cast Success"));

			UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(SpawnedItem->GetRootComponent());
			if (PrimComp)
			{
				UE_LOG(LogTemp, Warning, TEXT("PrimComp : cast Success"));

				// 예제: 위쪽으로 500.f 만큼의 임펄스를 추가하여 발사 효과 부여
				FVector LaunchImpulse = FVector(0.f, 0.f, 500.f);
				PrimComp->AddImpulse(LaunchImpulse);

				

			}
		}
	}
}



void AInteractChest::Multicast_OpenChest_Implementation()
{
	if (OpenedMesh)
	{
		Mesh->SetStaticMesh(OpenedMesh);
		UE_LOG(LogTemp, Warning, TEXT("Multicast_OpenChest_Implementation() : success"))

		if (Mesh)
		{
			Mesh->SetRenderCustomDepth(false);
		}
		bOpened = true;
	}
}

void AInteractChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractChest, bOpened);
	DOREPLIFETIME(AInteractChest, SpawnItemsID);
}