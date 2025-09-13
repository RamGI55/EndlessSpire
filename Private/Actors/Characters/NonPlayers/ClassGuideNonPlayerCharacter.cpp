// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/NonPlayers/ClassGuideNonPlayerCharacter.h"
#include "PlayerControllers/LobbyPlayerController.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"

#include "PaperSprite.h"
#include "Widgets/NicknameWidget.h"

AClassGuideNonPlayerCharacter::AClassGuideNonPlayerCharacter()
{

	// Mesh 적용
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/IdaFaber/Meshes/Girl/SK_MMO_CHAMPION_F_01.SK_MMO_CHAMPION_F_01'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		// Mesh 위치 변경
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		GetMesh()->SetCollisionProfileName(TEXT("InteractActor"));
	}

	// AnimInstance 적용
	ConstructorHelpers::FClassFinder<UAnimInstance> TempAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/IdaFaber/Demo/Animations/Girl/ABP_NonPlayer.ABP_NonPlayer_C'"));

	if (TempAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnimInstance.Class);
	}

	// 위젯 컴포넌트 생성
	NameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameWidget"));
	NameWidget->SetupAttachment(GetMesh());

	NameWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 190.0f));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);

	ConstructorHelpers::FClassFinder<UUserWidget> UI_Name(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_Nickname.WBP_Nickname_C'"));
	if (UI_Name.Succeeded())
	{
		NameWidget->SetWidgetClass(UI_Name.Class);
		NameWidget->SetDrawSize(FVector2D(350.0f, 80.0f));
	}

	// 미니맵 스프라이트
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetUsingAbsoluteRotation(true);
	SpriteComponent->SetWorldRotation(FRotator::MakeFromEuler(FVector(90.f, 180.f, -90.f)));
	SpriteComponent->SetWorldScale3D(FVector(0.2f));
	SpriteComponent->SetWorldLocation(FVector(0.f, 0.f, 300.f));
	SpriteComponent->bVisibleInSceneCaptureOnly = true;

	ConstructorHelpers::FObjectFinder<UPaperSprite> PaperSprite(TEXT("/Script/Paper2D.PaperSprite'/Game/Minimap/NPC_Sprite.NPC_Sprite'"));
	if (PaperSprite.Succeeded())
	{
		SpriteComponent->SetSprite(PaperSprite.Object);
	}

	// 블루프린트 위젯 클래스 불러오기
	static ConstructorHelpers::FClassFinder<UUserWidget> ClassSelectionAsset(TEXT("WidgetBlueprint'/Game/UI/WBP_ClassSelection.WBP_ClassSelection_C'"));

	// 템플릿 클래스에 블루프린트 클래스 넣기
	if (ClassSelectionAsset.Succeeded())
	{
		ClassSelectionClass = ClassSelectionAsset.Class;
	}
}

void AClassGuideNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UNicknameWidget* CurrentWidget = Cast<UNicknameWidget>(NameWidget->GetWidget());
	if (CurrentWidget)
	{
		CurrentWidget->SetNickname("Class Guide");
	}

	InteractableData = InstanceInteractableData;
}

void AClassGuideNonPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AClassGuideNonPlayerCharacter::BeginFocus()
{
	UE_LOG(LogTemp, Log, TEXT("Class Guide NPC"));
	if (!HasAuthority())
	{
		if (GetMesh())
		{
			GetMesh()->SetRenderCustomDepth(true);
		}
	}
}

void AClassGuideNonPlayerCharacter::EndFocus()
{
	if (!HasAuthority())
	{
		if (GetMesh())
		{
			GetMesh()->SetRenderCustomDepth(false);
		}
	}
}

void AClassGuideNonPlayerCharacter::BeginInteraction()
{
	
}

void AClassGuideNonPlayerCharacter::EndInteraction()
{
	
}

void AClassGuideNonPlayerCharacter::Interaction(ABasePlayerCharacter* Player)
{
	if (!HasAuthority())
	{
		ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(Player->GetController());
		UUserWidget* Widget = CreateWidget(LobbyPC, ClassSelectionClass);

		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}
