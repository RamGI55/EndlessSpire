// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/NonPlayers/WanderingTraderNonPlayer.h"
#include "PlayerControllers/LobbyPlayerController.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Widgets/ShopWidget.h"
#include "Net/UnrealNetwork.h"

AWanderingTraderNonPlayer::AWanderingTraderNonPlayer()
{
	// Mesh ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/IdaFaber/Meshes/Girl/SK_MMO_CHAMPION_F_04.SK_MMO_CHAMPION_F_04'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		// Mesh ��ġ ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		GetMesh()->SetCollisionProfileName(TEXT("InteractActor"));
	}

	// AnimInstance ����
	ConstructorHelpers::FClassFinder<UAnimInstance> TempAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/IdaFaber/Demo/Animations/Girl/ABP_NonPlayer.ABP_NonPlayer_C'"));

	if (TempAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnimInstance.Class);
	}

	// ������ ������ ���̺�
	ConstructorHelpers::FObjectFinder<UDataTable> TempItemDataTable(TEXT("/Script/Engine.DataTable'/Game/ItemData/DT_ItemData.DT_ItemData'"));
	ItemDataTable = TempItemDataTable.Object;


	// �������Ʈ ���� Ŭ���� �ҷ�����
	static ConstructorHelpers::FClassFinder<UUserWidget> ShopAsset(TEXT("WidgetBlueprint'/Game/UI/WBP_Shop.WBP_Shop_C'"));

	// ���ø� Ŭ������ �������Ʈ Ŭ���� �ֱ�
	if (ShopAsset.Succeeded())
	{
		ShopClass = ShopAsset.Class;
	}
}

void AWanderingTraderNonPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCollisionProfileName(TEXT("InteractActor"));

	if (HasAuthority())
	{
		// ������ ���̺��� ��� ���� ��������
		TArray<FName> RowNames = ItemDataTable->GetRowNames();
		TSet<int32> UsedNumbers;
		int32 RandomCount = 10; //FMath::RandRange(1, 5);

		if (RowNames.Num() < RandomCount)
		{
			RandomCount = RowNames.Num();
		}

		for (int i = 0; i < RandomCount; i++)
		{
			if (RowNames.Num() > 0)
			{
				// ������ �ε��� ����
				int32 RandomIndex = FMath::RandRange(0, RowNames.Num() - 1);

				// �ߺ��� ���ڰ� ������ �ٽ� ����
				while (UsedNumbers.Contains(RandomIndex))
				{
					RandomIndex = FMath::RandRange(0, RowNames.Num() - 1);  // ���� ���� �����
				}

				// �ߺ����� �ʴ� ������ ��� �¿� �߰�
				UsedNumbers.Add(RandomIndex);

				FName RandomRowName = RowNames[RandomIndex];

				// ������ �� ��������
				FItemData* RandomRow = ItemDataTable->FindRow<FItemData>(RandomRowName, TEXT("RandomRow"));

				if (RandomRow)
				{
					// ������ ���� ������ ���
					UE_LOG(LogTemp, Warning, TEXT("RandomItem: %s "), *RandomRowName.ToString());
					ShopItems.Add(*RandomRow);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("RandomRow not Found"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No RandomRow in the ItemDataTable"));
			}
		}

	}

	InteractableData = InstanceInteractableData;
}

void AWanderingTraderNonPlayer::Tick(float DeltaSeconds)
{
}

void AWanderingTraderNonPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWanderingTraderNonPlayer, ShopItems);
}

void AWanderingTraderNonPlayer::BeginFocus()
{
	UE_LOG(LogTemp, Log, TEXT("WanderingTrader NPC"));
	if (!HasAuthority())
	{
		if (GetMesh())
		{
			GetMesh()->SetRenderCustomDepth(true);
		}
	}
}

void AWanderingTraderNonPlayer::EndFocus()
{
	if (!HasAuthority())
	{
		if (GetMesh())
		{
			GetMesh()->SetRenderCustomDepth(false);
		}
	}
}

void AWanderingTraderNonPlayer::BeginInteraction()
{
}

void AWanderingTraderNonPlayer::EndInteraction()
{
}

void AWanderingTraderNonPlayer::Interaction(ABasePlayerCharacter* Player)
{
	if (HasAuthority())
	{
		for (const FItemData& Element : ShopItems)
		{
			UE_LOG(LogTemp, Warning, TEXT("Shop Item: %s "), *Element.TextData.Name.ToString());
		}
	}
	else
	{
		ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(Player->GetController());
		UUserWidget* Widget = CreateWidget(LobbyPC, ShopClass);

		if (Widget)
		{
			Widget->AddToViewport();
			ShopWidget = Cast<UShopWidget>(Widget);
			if (ShopWidget)
			{
				ShopWidget->SetOwner(this);
				ShopWidget->UpdateItems(ShopItems);
			}
		}
	}
}
