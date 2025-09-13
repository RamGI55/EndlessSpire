// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/BasePlayerCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "ActorComponents/MMPlayerStatComponent.h"
#include "ActorComponents/MMInventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Actors/MMPickUp.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "PlayerControllers/PlayPlayerController.h"
#include "UObjects/ItemBase.h"

#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "Widgets/NicknameWidget.h"
#include "GameStates/PlayGameState.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


ABasePlayerCharacter::ABasePlayerCharacter() 
{
	// ���ø�����Ʈ �ֱ�
	NetUpdateFrequency = 144.0f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = NonFightSocketOffset;

	// Apply Camera Lag
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 10.0f;
	CameraBoom->CameraRotationLagSpeed = 25.0f;
	CameraBoom->CameraLagMaxDistance = 100.0f;
	CameraBoom->bUseCameraLagSubstepping = true;
	CameraBoom->CameraLagMaxTimeStep = 0.016667;

	// ���� ������Ʈ ����

	// 하드코딩들 주의.
	NicknameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NicknameWidget"));
	NicknameWidget->SetupAttachment(GetMesh());

	NicknameWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 190.0f));
	NicknameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
	ConstructorHelpers::FClassFinder<UUserWidget> UI_Nickname(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_Nickname.WBP_Nickname_C'")); // 디버그에 찍힌거 그대로 하드코딩 해서는 X, 디버그에 찍힌건 인스턴스, 여기서는 클래스를 가져와야 함.  
	if (UI_Nickname.Succeeded())
	{
		NicknameWidget->SetWidgetClass(UI_Nickname.Class);
		NicknameWidget->SetDrawSize(FVector2D(350.0f,80.0f));
	}

	// ĳ���� ���� ������Ʈ ����
	PlayerStatComponent = CreateDefaultSubobject<UMMPlayerStatComponent>(TEXT("PlayerStat"));

	// �̴ϸ� ��������Ʈ
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpriteComponent->SetUsingAbsoluteRotation(true);
	SpriteComponent->SetWorldRotation(FRotator::MakeFromEuler(FVector(90.f, 180.f, -90.f)));
	SpriteComponent->SetWorldLocation(FVector(0.f, 0.f, 300.f));
	SpriteComponent->bVisibleInSceneCaptureOnly = true;

	//  Feedback: 미니맵 Paper Sprite를 캐릭터가 들고있는 이유는? - 미니맵의 요소를 캐릭터가 들고있다??? 
	ConstructorHelpers::FObjectFinder<UPaperSprite> PaperSprite1(TEXT("PaperSprite'/Game/Minimap/First_Sprite.First_Sprite'"));
	if (PaperSprite1.Succeeded())
	{
		SpriteMap.Add(0, PaperSprite1.Object);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("PaperSprite1 Fail"));
	}
	ConstructorHelpers::FObjectFinder<UPaperSprite> PaperSprite2(TEXT("PaperSprite'/Game/Minimap/Second_Sprite.Second_Sprite'"));
	if (PaperSprite2.Succeeded())
	{
		SpriteMap.Add(1, PaperSprite2.Object);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("PaperSprite2 Fail"));
	}
	ConstructorHelpers::FObjectFinder<UPaperSprite> PaperSprite3(TEXT("PaperSprite'/Game/Minimap/Thrid_Sprite.Thrid_Sprite'"));
	if (PaperSprite3.Succeeded())
	{
		SpriteMap.Add(2, PaperSprite3.Object);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("PaperSprite3 Fail"));
	}
	ConstructorHelpers::FObjectFinder<UPaperSprite> PaperSprite4(TEXT("PaperSprite'/Game/Minimap/Fourth_Sprite.Fourth_Sprite'"));
	if (PaperSprite4.Succeeded())
	{
		SpriteMap.Add(3, PaperSprite4.Object);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("PaperSprite4 Fail"));
	}

	// Feedback: 플레이어 인벤토리 -> 컴포넌트화 되어있음. 굳이 여기서 할 이유는?, 컴포넌트가 초기화 될때 하거나 아니면 개별 캐릭터들이 초기화 값을 캐싱으로 들고있는게 정석. (만약 다르다면) 
	PlayerInventory = CreateDefaultSubobject<UMMInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);	// 매직넘버
	PlayerInventory->SetWeightCapacity(50.0f);	// 매직넘버 

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


	// ĳ���� Mesh ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Skeleton_Guard/Mesh_UE4/Full/SKM_Skeleton_Guard_Body.SKM_Skeleton_Guard_Body'"));	// 디버그 경로 하드코딩

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		// Mesh ��ġ ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));

	}
	else {
		UE_LOG(LogTemp, Log, TEXT("SetSkeletalMesh Fail"));
	}

	// ĳ���� AnimInstance ����
	ConstructorHelpers::FClassFinder<UAnimInstance> TempAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C'"));	// 디버그 경로 하드코딩 

	if (TempAnimInstance.Succeeded())
	{
		// �ִϸ��̼Ǹ�� ����
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		GetMesh()->SetAnimInstanceClass(TempAnimInstance.Class);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("SetAnimInstanceClass Fail"));
	}

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// ��ȣ�ۿ� Defalut�� ����
	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 225.0f;
}

void ABasePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}


void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetMesh()->GetAnimInstance()) {
		GetMesh()->GetAnimInstance()->OnMontageEnded.AddUniqueDynamic(this, &ABasePlayerCharacter::OnAnimationEnd);
	}
}

void ABasePlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FightCamEventDispatcher.Broadcast();

	if (!HasAuthority())
	{
		if (GetWorld()->TimeSince
		(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency) 
		{
			PerformInteractionCheck();
		}
	}

	//if (bIsRunning)
	//{
	//	PlayerStatComponent->UseStamina(0.2f);
	//}
	//else
	//{
	//	PlayerStatComponent->HealStamina(0.1f);
	//}
}

void ABasePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABasePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}

void ABasePlayerCharacter::EnterFight()
{
	bIsFighting = true;
	bUseControllerRotationYaw = bUseEightDirectionMove;
	GetCharacterMovement()->bOrientRotationToMovement = !bUseEightDirectionMove;
	FightCamEventDispatcher.Clear();
	FightCamEventDispatcher.__Internal_AddDynamic(this, &ABasePlayerCharacter::MoveToFightView, FName("MoveToFightView")); // 동적으로 디스패쳐 할당은 매우 좋음! 
}

void ABasePlayerCharacter::ExitFight()
{
	bIsFighting = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	FightCamEventDispatcher.Clear();
	FightCamEventDispatcher.__Internal_AddDynamic(this, &ABasePlayerCharacter::MoveToNonFightView, FName("MoveToNonFightView"));
}

void ABasePlayerCharacter::OnLeftMouseBeginClick()
{
	UE_LOG(LogTemp, Log, TEXT("LeftMouseBeginClick"));
	ServerCastEnterFight();
}

void ABasePlayerCharacter::OnLeftMouseHolding(bool triggered, float ElapsedTime)
{
	UE_LOG(LogTemp, Log, TEXT("LeftMouse Hold, triggered = %s"), triggered ? TEXT("True") : TEXT("False"));
}

void ABasePlayerCharacter::OnLeftMouseReleased(bool cancled)
{
	UE_LOG(LogTemp, Log, TEXT("LeftMouse Released, Cancled = %s"), cancled ? TEXT("True") : TEXT("False"));
}

void ABasePlayerCharacter::OnRightMouseBeginClick()
{
	UE_LOG(LogTemp, Log, TEXT("RightMouseBeginClick"));
	ServerCastExitFight();
}

void ABasePlayerCharacter::OnRightMouseHolding(bool triggered, float ElapsedTime)
{
	UE_LOG(LogTemp, Log, TEXT("RightMouse Hold, triggered = %s"), triggered ? TEXT("True") : TEXT("False"));
}

void ABasePlayerCharacter::OnRightMouseReleased(bool cancled)
{
	UE_LOG(LogTemp, Log, TEXT("RightMouse Released, Cancled = %s"), cancled ? TEXT("True") : TEXT("False"));
}

void ABasePlayerCharacter::OnInteractBeginClick()
{
	UE_LOG(LogTemp, Log, TEXT("Interact BeginClick"));
	BeginInteract();
}

void ABasePlayerCharacter::OnInteractReleased()
{
	UE_LOG(LogTemp, Log, TEXT("Interact Released"));

}

void ABasePlayerCharacter::ServerCastEnterFight_Implementation()
{
	BroadcastEnterFight();
}

void ABasePlayerCharacter::ServerCastExitFight_Implementation()
{
	BroadcastExitFight();
}

void ABasePlayerCharacter::BroadcastEnterFight_Implementation()
{
	EnterFight();
}

void ABasePlayerCharacter::BroadcastExitFight_Implementation()
{
	ExitFight();
}

bool ABasePlayerCharacter::IsRunning() const
{
	return bIsRunning;
}

bool ABasePlayerCharacter::IsFighting() const
{
	return bIsFighting;
}

void ABasePlayerCharacter::OnAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted) return;


	ABaseCombatCharacter* ThisCharacter = Cast<ABaseCombatCharacter>(this);
	if (ThisCharacter) {
		ThisCharacter->SetCanSwitchAnimation(true);

		if (bIsRunning) {
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		}
		else {
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}

	APlayPlayerController* ThisController = Cast<APlayPlayerController>(GetController());

	if (ThisController) {
		ThisController->InitAllIMC();
	}
}

void ABasePlayerCharacter::MoveToFightView()
{
	CameraBoom->SocketOffset = FMath::Lerp(CameraBoom->SocketOffset, FightSocketOffset, FightViewBlendAlpha);
	if ((CameraBoom->SocketOffset - FightSocketOffset).Length() < FightViewTolerance) {
		FightCamEventDispatcher.Clear();
	}
}

void ABasePlayerCharacter::MoveToNonFightView()
{
	CameraBoom->SocketOffset = FMath::Lerp(CameraBoom->SocketOffset, NonFightSocketOffset, FightViewBlendAlpha);
	if ((CameraBoom->SocketOffset - NonFightSocketOffset).Length() < FightViewTolerance) {
		FightCamEventDispatcher.Clear();
	}
}

void ABasePlayerCharacter::SetPlayerStat(FPlayerCharacterStat NewStat)
{
	if (!PlayerStatComponent) {
		UE_LOG(LogTemp, Log, TEXT("ABasePlayerCharacter PlayerStatComponent = NULL"));
		return;
	}
	PlayerStatComponent->InitStat(NewStat);
}

void ABasePlayerCharacter::SetInventory(TArray<FItemData>& NewItems, int32 NewMoney)
{
	PlayerInventory->BroadcastModifyMoney(NewMoney);

	for (const FItemData& Element : NewItems)
	{
		PlayerInventory->BroadcastNewData(Element);
	}
}

void ABasePlayerCharacter::ServerCastRun_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Start Run"));
	if (PlayerStatComponent->GetStat("CurSp") > 0.0f)
	{
		BroadCastRun();
	}
}

void ABasePlayerCharacter::ServerCastStopRunning_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Stop Run"));
	BroadCastStopRunning();
}

void ABasePlayerCharacter::BroadCastRun_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	bIsRunning = true;
}

void ABasePlayerCharacter::BroadCastStopRunning_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsRunning = false;
}

#pragma region Client RPC


#pragma endregion

#pragma region Multicast RPC

// �г��� ��Ƽ�� ���� �����ֱ�
void ABasePlayerCharacter::MultiShowNickname_Implementation(const FString& nickname)
{
	UE_LOG(LogTemp, Log, TEXT("MultiShow"));
	UNicknameWidget* CurrentWidget = Cast<UNicknameWidget>(NicknameWidget->GetWidget());
	if (CurrentWidget)
	{
		CurrentWidget->SetNickname(nickname);
	}
	UE_LOG(LogTemp, Log, TEXT("MultiShowEnd"));
}

void ABasePlayerCharacter::SetMinimap_Implementation(int32 index)
{
	UE_LOG(LogTemp, Log, TEXT("SetMinimap"));
	if (!SpriteComponent) {
		UE_LOG(LogTemp, Log, TEXT("ABasePlayerCharacter SpriteComponent = NULL"));
		return;
	}

	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ASceneCapture2D::StaticClass());
	if (Actor)
	{
		ASceneCapture2D* CaptureActor = Cast<ASceneCapture2D>(Actor);
		if (CaptureActor)
		{
			// SceneCaptureComponent2D�� �����ɴϴ�.
			USceneCaptureComponent2D* CaptureComponent = CaptureActor->GetCaptureComponent2D();
			if (CaptureComponent)
			{
				// OrthoWidth ���� �����ɴϴ�.
				float Scale = CaptureComponent->OrthoWidth / 2000.0f;

				SpriteComponent->SetSprite(*SpriteMap.Find(index));
				SpriteComponent->SetWorldScale3D(FVector(Scale));
				UE_LOG(LogTemp, Log, TEXT("SetMinimapEnd"));
			}
		}
	}
}
#pragma endregion

#pragma region Interaction

// AI Stimulus ������ �����ϴ� ������Ʈ �߰�. 
void ABasePlayerCharacter::SetStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem(); 
	}
}

void ABasePlayerCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	// ���ǹ�� ��Ȯ��
	FVector TraceStart{ GetPawnViewLocation() };
	/*FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());


	if (LookDirection > 0)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit;
		

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_GameTraceChannel7, QueryParams))
		{
			if (TraceHit.GetActor() && TraceHit.GetActor()->Implements<UInteractionInterface>())
			{

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());

					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}
	
		NoInteractableFound();
	*/
	FVector TargetLocation = GetCrosshairTraceHit();



	FHitResult TraceHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		TraceHit,
		TraceStart,
		TargetLocation,
		ECC_GameTraceChannel7,
		QueryParams
	);

	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TargetLocation, ECC_GameTraceChannel7, QueryParams))
	{
		if (TraceHit.GetActor() && TraceHit.GetActor()->Implements<UInteractionInterface>())
		{

			if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
			{
				FoundInteractable(TraceHit.GetActor());

				return;
			}

			if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
			{
				return;
			}
		}
	}

	//DrawDebugLine(GetWorld(), TraceStart, TargetLocation, FColor::Blue, false, 1.0f, 0, 2.0f);

	NoInteractableFound();
}

void ABasePlayerCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	TargetInteractable->BeginFocus();
}

void ABasePlayerCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void ABasePlayerCharacter::BeginInteract()
{
	PerformInteractionCheck();
	UE_LOG(LogTemp, Warning, TEXT("BeginInteraction"));
	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteraction();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&ABasePlayerCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}
}

void ABasePlayerCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	UE_LOG(LogTemp, Warning, TEXT("EndInteraction"));
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteraction();
	}
}

void ABasePlayerCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	UE_LOG(LogTemp, Warning, TEXT("Interaction"));
	if (IsValid(TargetInteractable.GetObject()))
	{
		AActor* InteractableObject = Cast<AActor>(TargetInteractable.GetObject());
		ServerInteract(InteractableObject,this);

		TargetInteractable->Interaction(this);
	}
}

void ABasePlayerCharacter::ServerInteract_Implementation(AActor* InteractableObject, ABasePlayerCharacter* player)
{
	TargetInteractable = InteractableObject;
	if (!TargetInteractable) return;
	TargetInteractable->Interaction(player);
}

void ABasePlayerCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		AMMPickUp* PickUp = GetWorld()->SpawnActor<AMMPickUp>(AMMPickUp::StaticClass(), SpawnTransform, SpawnParams);
		PickUp->InitializeDrop(ItemToDrop, RemovedQuantity);
		UE_LOG(LogTemp, Warning, TEXT("Server_DropItem Success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow null"));
	}
}

void ABasePlayerCharacter::DropItem(const FName ItemID, const int32 QuantityToDrop)
{
	UItemBase* MatchingItem = PlayerInventory->FindMatchingItem(ItemID);
	if (MatchingItem)
	{
		if (GetOwner()->HasAuthority()) {
			// ���� ����
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.bNoFail = true;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };
			const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

			AMMPickUp* PickUp = GetWorld()->SpawnActor<AMMPickUp>(AMMPickUp::StaticClass(), SpawnTransform, SpawnParams);

			// �κ����� �����ϰ� ������ ���� ����
			const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(MatchingItem, QuantityToDrop);

			// ������ ������ŭ ������ ���Ϳ� ������ ������ ����
			PickUp->InitializeDrop(MatchingItem, RemovedQuantity);
			PickUp->BroadcastDropFromTargetInventory(this, ItemID, RemovedQuantity);

			UE_LOG(LogTemp, Warning, TEXT("Server_DropItem Success"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow null"));
	}
}

void ABasePlayerCharacter::ServerDropItem_Implementation(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	DropItem(ItemToDrop->ID, QuantityToDrop);
}

#pragma endregion

FVector ABasePlayerCharacter::GetCrosshairTraceHit()
{
	if (!this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character : Fail"))
			return FVector::ZeroVector;
	}

	// Ʈ���̽�
	FHitResult Hit;
	FVector TraceStart = this->GetFollowCamera()->GetComponentLocation();
	FVector TraceDirection = this->GetFollowCamera()->GetForwardVector();
	FVector TraceEnd = TraceStart + TraceDirection * 625.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // �ڱ� �ڽ� ����

	bool bHit = this->GetWorld()->LineTraceSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		CameraHitLocation = Hit.ImpactPoint;
	}
	else
	{
		// �浹ü ������ �� �Ÿ���
		CameraHitLocation = TraceEnd;
	}

	//����� ���� ǥ��(�ʿ� ��)
	//DrawDebugLine(GetWorld(), TraceStart, CameraHitLocation, FColor::Green, false, 2.0f);


	return CameraHitLocation;
}