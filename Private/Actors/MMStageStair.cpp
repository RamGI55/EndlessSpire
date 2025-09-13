// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MMStageStair.h"
#include "Kismet/GameplayStatics.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "Components/WidgetComponent.h"

#include "PaperSprite.h"

AMMStageStair::AMMStageStair()
{
	bReplicates = true;

	// StaticMeshComponent 초기화
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetWorldScale3D(FVector(2.0f, 1.0f, 3.0f));
	RootComponent = StaticMeshComponent;

	// Mesh 적용
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/ModularDungeon2/StaticMesh/Stair/SM_Stair_Straight_Base.SM_Stair_Straight_Base'"));

	if (TempMesh.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(TempMesh.Object);
	}

	// 미니맵 스프라이트
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetUsingAbsoluteRotation(true);
	SpriteComponent->SetWorldRotation(FRotator::MakeFromEuler(FVector(90.f, 180.f, -90.f)));
	SpriteComponent->SetWorldScale3D(FVector(0.2f));
	SpriteComponent->SetWorldLocation(FVector(0.f, 0.f, 300.f));
	SpriteComponent->bVisibleInSceneCaptureOnly = true;

	//ConstructorHelpers::FObjectFinder<UPaperSprite> PaperSprite(TEXT("/Script/Paper2D.PaperSprite'/Game/Minimap/NPC_Sprite.NPC_Sprite'"));
	//if (PaperSprite.Succeeded())
	//{
	//	SpriteComponent->SetSprite(PaperSprite.Object);
	//}

	// 블루프린트 위젯 클래스 불러오기
	static ConstructorHelpers::FClassFinder<UUserWidget> VoteAsset(TEXT("WidgetBlueprint'/Game/UI/WBP_Vote.WBP_Vote_C'"));

	// 템플릿 클래스에 블루프린트 클래스 넣기
	if (VoteAsset.Succeeded())
	{
		VoteWidgetClass = VoteAsset.Class;
	}
}

void AMMStageStair::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->SetCollisionProfileName(TEXT("InteractActor"));
	InteractableData = InstanceInteractableData;
}

void AMMStageStair::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AMMStageStair::BeginFocus()
{
	UE_LOG(LogTemp, Log, TEXT("Next Stage Stair"));
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetRenderCustomDepth(true);
	}
}

void AMMStageStair::EndFocus()
{
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetRenderCustomDepth(false);
	}
}

void AMMStageStair::BeginInteraction()
{
}

void AMMStageStair::EndInteraction()
{
}

void AMMStageStair::Interaction(ABasePlayerCharacter* Player)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Display, TEXT("Server"));
		VoteForNextStage();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Client"));

	}
}

void AMMStageStair::VoteForNextStage_Implementation()
{
	if (!HasAuthority())
	{
		UUserWidget* Widget = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0), VoteWidgetClass);

		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}