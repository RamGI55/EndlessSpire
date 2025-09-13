// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/LobbyPlayerController.h"
#include "GameModes/LobbyGameMode.h"
#include "GameStates/LobbyGameState.h"
#include "PlayerStates/LobbyPlayerState.h"
#include "HUDs/LobbyHUD.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "ActorComponents/MMInventoryComponent.h"
#include "ActorComponents/MMPlayerStatComponent.h"
#include "GameInstances/EndlessSpireGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Constructor
ALobbyPlayerController::ALobbyPlayerController()
{
	IsStatusUIVisble = false;
	IsInventoryVisble = false;
	IsEquipmentMenuVisible = false;

	// InputMappingContext ���� ������Ʈ ���ε�
	ConstructorHelpers::FObjectFinder<UInputMappingContext> CombatMappingContextObject(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputMapping/CombatNInteraction/CombatMappingContext.CombatMappingContext'"));
	if (CombatMappingContextObject.Succeeded())
	{
		CombatMappingContext = CombatMappingContextObject.Object;
	}
	// Skill InputAction ���� ������Ʈ ���ε�
	/*ConstructorHelpers::FObjectFinder<UInputAction> Skill1ActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/CombatNInteraction/IA_Skill1.IA_Skill1'"));
	if (Skill1ActionObject.Succeeded())
	{
		Skill1 = Skill1ActionObject.Object;
	}*/
	ConstructorHelpers::FObjectFinder<UInputAction> Skill2ActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/CombatNInteraction/IA_Skill2.IA_Skill2'"));
	if (Skill2ActionObject.Succeeded())
	{
		Skill2 = Skill2ActionObject.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> Skill3ActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/CombatNInteraction/IA_Skill3.IA_Skill3'"));
	if (Skill3ActionObject.Succeeded())
	{
		Skill3 = Skill3ActionObject.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> IdentitySkillActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/CombatNInteraction/IA_IdentitySkill.IA_IdentitySkill'"));
	if (IdentitySkillActionObject.Succeeded())
	{
		IdentitySkill = IdentitySkillActionObject.Object;
	}

	// Evade�� Interact InputAction ���� ������Ʈ ���ε�
	ConstructorHelpers::FObjectFinder<UInputAction> EvadeActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/CombatNInteraction/IA_Evade.IA_Evade'"));
	if (EvadeActionObject.Succeeded())
	{
		Evade = EvadeActionObject.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> InteractActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/CombatNInteraction/IA_Interact.IA_Interact'"));
	if (InteractActionObject.Succeeded())
	{
		Interact = InteractActionObject.Object;
	}

	// 0317 ������ UI Ű �߰�
	ConstructorHelpers::FObjectFinder<UInputMappingContext> ShowUIMappingContextObject(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputMapping/ShowUI/ShowUIMappingContext.ShowUIMappingContext'"));
	if (ShowUIMappingContextObject.Succeeded())
	{
		ShowUIMappingContext = ShowUIMappingContextObject.Object;
		UE_LOG(LogTemp, Log,TEXT("ShowUIMappingContextObject is Succesed"))
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ShowUIMappingContextObject is fail"))
	}
	ConstructorHelpers::FObjectFinder<UInputAction> StatusUIActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/ShowUI/IA_StatusUI.IA_StatusUI'"));
	if (StatusUIActionObject.Succeeded())
	{
		StatusUI = StatusUIActionObject.Object;
		UE_LOG(LogTemp, Log, TEXT("StatusUIActionObject is Succesed"))
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("StatusUIActionObject is fail"))
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> MenuToggleUIActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/CombatNInteraction/IA_MenuToggle.IA_MenuToggle'"));
	if (MenuToggleUIActionObject.Succeeded())
	{
		MenuToggle = MenuToggleUIActionObject.Object;
		UE_LOG(LogTemp, Log, TEXT("MenuToggleUIActionObject is Succesed"))
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("MenuToggleUIActionObject is fail"))
	}

	ConstructorHelpers::FObjectFinder<UInputAction> EquipToggleUIActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/ShowUI/IA_EquipmentMenu.IA_EquipmentMenu'"));
	if (EquipToggleUIActionObject.Succeeded())
	{
		EquipmentToggle = EquipToggleUIActionObject.Object;
		UE_LOG(LogTemp, Log, TEXT("EquipToggleUIActionObject is Succesed"))
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("EquipToggleUIActionObject is fail"))
	}
}

#pragma region Overrides
void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Set MappingContext
	if (EnhancedInputSubsystem)
	{
		EnhancedInputSubsystem->AddMappingContext(CombatMappingContext, 0);

		// 0317 ������ UI Ű �߰�
		EnhancedInputSubsystem->AddMappingContext(ShowUIMappingContext, 0);
	}

	// InputMode GameMode�θ�
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bShowMouseCursor = false;
}
// ���ø����̼� ����
void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, PlayerGuid);
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Input Action Bind
	if (EnhancedInputComponent) {
		// Skill
		EnhancedInputComponent->BindAction(Skill1, ETriggerEvent::Started, this, &ALobbyPlayerController::OnSkill1BeginClicked);
		EnhancedInputComponent->BindAction(Skill1, ETriggerEvent::Triggered, this, &ALobbyPlayerController::OnSkill1Triggered);
		EnhancedInputComponent->BindAction(Skill1, ETriggerEvent::Ongoing, this, &ALobbyPlayerController::OnSkill1Holding);
		EnhancedInputComponent->BindAction(Skill1, ETriggerEvent::Canceled, this, &ALobbyPlayerController::OnSkill1Cancled);
		EnhancedInputComponent->BindAction(Skill1, ETriggerEvent::Completed, this, &ALobbyPlayerController::OnSkill1Completed);

		EnhancedInputComponent->BindAction(Skill2, ETriggerEvent::Started, this, &ALobbyPlayerController::OnSkill2BeginClicked);
		EnhancedInputComponent->BindAction(Skill2, ETriggerEvent::Triggered, this, &ALobbyPlayerController::OnSkill2Triggered);
		EnhancedInputComponent->BindAction(Skill2, ETriggerEvent::Ongoing, this, &ALobbyPlayerController::OnSkill2Holding);
		EnhancedInputComponent->BindAction(Skill2, ETriggerEvent::Canceled, this, &ALobbyPlayerController::OnSkill2Cancled);
		EnhancedInputComponent->BindAction(Skill2, ETriggerEvent::Completed, this, &ALobbyPlayerController::OnSkill2Completed);

		EnhancedInputComponent->BindAction(Skill3, ETriggerEvent::Started, this, &ALobbyPlayerController::OnSkill3BeginClicked);
		EnhancedInputComponent->BindAction(Skill3, ETriggerEvent::Triggered, this, &ALobbyPlayerController::OnSkill3Triggered);
		EnhancedInputComponent->BindAction(Skill3, ETriggerEvent::Ongoing, this, &ALobbyPlayerController::OnSkill3Holding);
		EnhancedInputComponent->BindAction(Skill3, ETriggerEvent::Canceled, this, &ALobbyPlayerController::OnSkill3Cancled);
		EnhancedInputComponent->BindAction(Skill3, ETriggerEvent::Completed, this, &ALobbyPlayerController::OnSkill3Completed);

		EnhancedInputComponent->BindAction(IdentitySkill, ETriggerEvent::Started, this, &ALobbyPlayerController::OnIdentitySkillBeginClicked);
		EnhancedInputComponent->BindAction(IdentitySkill, ETriggerEvent::Triggered, this, &ALobbyPlayerController::OnIdentitySkillTriggered);
		EnhancedInputComponent->BindAction(IdentitySkill, ETriggerEvent::Ongoing, this, &ALobbyPlayerController::OnIdentitySkillHolding);
		EnhancedInputComponent->BindAction(IdentitySkill, ETriggerEvent::Canceled, this, &ALobbyPlayerController::OnIdentitySkillCancled);
		EnhancedInputComponent->BindAction(IdentitySkill, ETriggerEvent::Completed, this, &ALobbyPlayerController::OnIdentitySkillCompleted);
		// Evade
		EnhancedInputComponent->BindAction(Evade, ETriggerEvent::Started, this, &ALobbyPlayerController::OnEvadeBeginClicked);
		EnhancedInputComponent->BindAction(Evade, ETriggerEvent::Completed, this, &ALobbyPlayerController::OnEvadeEndClicked);
		// Interact
		EnhancedInputComponent->BindAction(Interact, ETriggerEvent::Started, this, &ALobbyPlayerController::OnInteractBeginClicked);
		EnhancedInputComponent->BindAction(Interact, ETriggerEvent::Completed, this, &ALobbyPlayerController::OnInteractEndClicked);

		// 0317 ������ UI Ű �߰�
		EnhancedInputComponent->BindAction(StatusUI, ETriggerEvent::Started, this, &ALobbyPlayerController::OnShowUIBeginClicked);

		// 0320 �輱�� �޴� ���Ű �߰�
		EnhancedInputComponent->BindAction(MenuToggle, ETriggerEvent::Started, this, &ALobbyPlayerController::ToggleMenu);
		// 0410 �輱�� ���â ���Ű �߰�
		EnhancedInputComponent->BindAction(EquipmentToggle, ETriggerEvent::Started, this, &ALobbyPlayerController::ToggleEquipmentMenu);
	}
}

#pragma endregion

#pragma region InputMappings
// �Է� �� ���� ���� -> ��� ĳ������ �Լ� ����
void ALobbyPlayerController::OnSkill1BeginClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill1BeginClick();
	}
}

void ALobbyPlayerController::OnSkill1Triggered(const FInputActionInstance& Instance)
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill1Holding(true, Instance.GetElapsedTime());
	}
}

void ALobbyPlayerController::OnSkill1Holding(const struct FInputActionInstance& Instance)
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill1Holding(false, Instance.GetElapsedTime());
	}
}

void ALobbyPlayerController::OnSkill1Cancled()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill1Released(true);
	}
}

void ALobbyPlayerController::OnSkill1Completed()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill1Released(false);
	}
}

void ALobbyPlayerController::OnSkill2BeginClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill2BeginClick();
	}
}

void ALobbyPlayerController::OnSkill2Triggered(const FInputActionInstance& Instance)
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill2Holding(true, Instance.GetElapsedTime());
	}
}

void ALobbyPlayerController::OnSkill2Holding(const struct FInputActionInstance& Instance)
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill2Holding(false, Instance.GetElapsedTime());
	}
}

void ALobbyPlayerController::OnSkill2Cancled()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill2Released(true);
	}
}

void ALobbyPlayerController::OnSkill2Completed()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill2Released(false);
	}
}

void ALobbyPlayerController::OnSkill3BeginClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill3BeginClick();
	}
}

void ALobbyPlayerController::OnSkill3Triggered(const FInputActionInstance& Instance)
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill3Holding(true, Instance.GetElapsedTime());
	}
}

void ALobbyPlayerController::OnSkill3Holding(const struct FInputActionInstance& Instance)
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill3Holding(false, Instance.GetElapsedTime());
	}
}

void ALobbyPlayerController::OnSkill3Cancled()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill3Released(true);
	}
}

void ALobbyPlayerController::OnSkill3Completed()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnSkill3Released(false);
	}
}


void ALobbyPlayerController::OnIdentitySkillBeginClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnIdentitySkillBeginClick();
	}
}

void ALobbyPlayerController::OnIdentitySkillTriggered(const FInputActionInstance& Instance)
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnIdentitySkillHolding(true, Instance.GetElapsedTime());
	}
}

void ALobbyPlayerController::OnIdentitySkillHolding(const struct FInputActionInstance& Instance)
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnIdentitySkillHolding(false, Instance.GetElapsedTime());
	}
}

void ALobbyPlayerController::OnIdentitySkillCancled()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnIdentitySkillReleased(true);
	}
}

void ALobbyPlayerController::OnIdentitySkillCompleted()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnIdentitySkillReleased(false);
	}
}

void ALobbyPlayerController::OnEvadeBeginClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnEvadeBeginClick();
	}
}

void ALobbyPlayerController::OnEvadeEndClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnEvadeReleased();
	}
}

void ALobbyPlayerController::OnInteractBeginClicked()
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnInteractBeginClick();
	}


	// 03.12 �輱�� ��ȣ�ۿ� �׽�Ʈ �ڵ�
	//AItemInteractionTestCharacter* TestCharacter = Cast<AItemInteractionTestCharacter>(GetPawn());
	//if (TestCharacter) {
	//	TestCharacter->BeginInteract();
	//}
}

// 03.20 �輱�� �޴���� �׽�Ʈ �ڵ�
void ALobbyPlayerController::ToggleMenu()
{
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController)
	{
		ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PlayerController->GetHUD());
		if (LobbyHUD)
		{
			if (IsInventoryVisble)
			{
				LobbyHUD->PlayerUI->InventoryUI->SetVisibility(ESlateVisibility::Hidden);
				const FInputModeGameOnly InputMode;
				SetInputMode(InputMode);
				SetShowMouseCursor(false);
				UE_LOG(LogTemp, Log, TEXT("SetVisibility(ESlateVisibility::Hidden)"));
			}
			else
			{
				LobbyHUD->PlayerUI->InventoryUI->SetVisibility(ESlateVisibility::Visible);
				const FInputModeGameAndUI InputMode;
				SetInputMode(InputMode);
				SetShowMouseCursor(true);
				UE_LOG(LogTemp, Log, TEXT("SetVisibility(ESlateVisibility::Visible)"));
			}

			IsInventoryVisble = !IsInventoryVisble;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LobbyHUD failed"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController failed"));
		return;
	}

}

void ALobbyPlayerController::ToggleEquipmentMenu()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController)
	{
		ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PlayerController->GetHUD());
		if (LobbyHUD)
		{
			if (IsEquipmentMenuVisible)
			{
				LobbyHUD->PlayerUI->EquipmentPanel->SetVisibility(ESlateVisibility::Hidden);
				const FInputModeGameOnly InputMode;
				SetInputMode(InputMode);
				SetShowMouseCursor(false);
				UE_LOG(LogTemp, Log, TEXT("SetVisibility(ESlateVisibility::Hidden)"));
			}
			else
			{
				LobbyHUD->PlayerUI->EquipmentPanel->SetVisibility(ESlateVisibility::Visible);
				const FInputModeGameAndUI InputMode;
				SetInputMode(InputMode);
				SetShowMouseCursor(true);
				UE_LOG(LogTemp, Log, TEXT("SetVisibility(ESlateVisibility::Visible)"));
			}

			IsEquipmentMenuVisible = !IsEquipmentMenuVisible;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LobbyHUD failed"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController failed"));
		return;
	}
}

void ALobbyPlayerController::OnInteractEndClicked()
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnInteractReleased();
	}

	// 03.12 �輱�� ��ȣ�ۿ� �׽�Ʈ �ڵ�
	//AItemInteractionTestCharacter* TestCharacter = Cast<AItemInteractionTestCharacter>(GetPawn());
	//if (TestCharacter) {
	//	TestCharacter->EndInteract();
	//}
}

// 0317 ������ UI Ű �߰�
void ALobbyPlayerController::OnShowUIBeginClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController)
	{
		ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PlayerController->GetHUD());
		if (LobbyHUD)
		{
			if (IsStatusUIVisble)
			{
				LobbyHUD->GetStatusUI()->SetVisibility(ESlateVisibility::Hidden);
				UE_LOG(LogTemp, Log, TEXT("SetVisibility(ESlateVisibility::Hidden)"));
			}
			else
			{
				LobbyHUD->GetStatusUI()->SetVisibility(ESlateVisibility::HitTestInvisible);
				UE_LOG(LogTemp, Log, TEXT("SetVisibility(ESlateVisibility::HitTestInvisible)"));
			}

			IsStatusUIVisble = !IsStatusUIVisble;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LobbyHUD failed"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController failed"));
		return;
	}

}

void ALobbyPlayerController::InitAllIMC()
{
	Super::InitAllIMC();
	if (EnhancedInputSubsystem)
	{
		if (!EnhancedInputSubsystem->HasMappingContext(CombatMappingContext)) {
			EnhancedInputSubsystem->AddMappingContext(CombatMappingContext, 0);
		}
		if (!EnhancedInputSubsystem->HasMappingContext(ShowUIMappingContext)) {
			EnhancedInputSubsystem->AddMappingContext(ShowUIMappingContext, 0);
		}
	}
}

void ALobbyPlayerController::DeleteAllIMC()
{
	Super::DeleteAllIMC();
	if (EnhancedInputSubsystem)
	{
		while (EnhancedInputSubsystem->HasMappingContext(CombatMappingContext)) {
			EnhancedInputSubsystem->RemoveMappingContext(CombatMappingContext);
		}
		while (EnhancedInputSubsystem->HasMappingContext(ShowUIMappingContext)) {
			EnhancedInputSubsystem->RemoveMappingContext(ShowUIMappingContext);
		}
	}
}



#pragma endregion



// Guid ����
void ALobbyPlayerController::SetGuid(FGuid currentguid)
{
	
	PlayerGuid = currentguid;
	UE_LOG(LogTemp, Log, TEXT("Set Guid on Server: %s"), *PlayerGuid.ToString());

	ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetPlayerState<APlayerState>());
	if (LobbyPS)
	{
		LobbyPS->PlayerGuid = PlayerGuid;
	}

	OwnerSetInfo();
}



#pragma region Client RPC
// �ڽ��� �÷��̾� ������ �����ν��Ͻ����� ������ ����
void ALobbyPlayerController::OwnerSetInfo_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("SetInfo Up"));
	UEndlessSpireGameInstance* GameInstance = Cast<UEndlessSpireGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GameInstance->Guid = PlayerGuid;

	ServerUpdateStat(GameInstance->PlayerStat);
	ServerUpdateInventory(GameInstance->PlayerItems, GameInstance->PlayerMoney);
	ServerUpdateNickname(GameInstance->Nickname);
	ServerUpdateClass(GameInstance->Class);
	UE_LOG(LogTemp, Log, TEXT("SetInfo Down"));
}

void ALobbyPlayerController::OwnerSaveInfo_Implementation(const EClass playerclass)
{
	UEndlessSpireGameInstance* GameInstance = Cast<UEndlessSpireGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetPlayerState<APlayerState>());
	ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (PlayerCharacter->GetPlayerStatComponent()->GetIsDead()) {
		LobbyPS->PlayerBaseStat.CurHp = LobbyPS->PlayerBaseStat.MaxHp;
		
	}
	GameInstance->Class = playerclass;
	GameInstance->PlayerStat = LobbyPS->PlayerBaseStat;
	GameInstance->PlayerItems = PlayerCharacter->GetInventory()->ExtractItemDatas();
	GameInstance->PlayerMoney = PlayerCharacter->GetInventory()->Money;
}

void ALobbyPlayerController::OwnerUpdatePlayerStat_Implementation(FPlayerCharacterStat OwnPlayerStat)
{
	UE_LOG(LogTemp, Log, TEXT("OwnerUpdatePlayerStat"));
	ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(this->GetHUD());
	if (LobbyHUD)
	{
		LobbyHUD->UpdatePlayerStatus(OwnPlayerStat);
	}
	UE_LOG(LogTemp, Log, TEXT("OwnerUpdatePlayerStat End"));
}

void ALobbyPlayerController::OwnerUpdateOtherPlayerStat_Implementation(FPlayerCharacterStat OtherPlayerStat, FGuid OtherPlayerGuid)
{
	UE_LOG(LogTemp, Log, TEXT("OwnerUpdateOtherPlayerStat"));
	ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(this->GetHUD());
	if (LobbyHUD)
	{
		LobbyHUD->UpdateOtherPlayerStatus(OtherPlayerStat, OtherPlayerGuid);
	}
	UE_LOG(LogTemp, Log, TEXT("OwnerUpdateOtherPlayerStat End"));
}

void ALobbyPlayerController::OwnerSetStatus_Implementation(const FString& playername, const EClass& playerclass)
{
	UE_LOG(LogTemp, Log, TEXT("OwnerSetStatus"));
	ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(this->GetHUD());
	if (LobbyHUD)
	{
		if (!LobbyHUD->PlayerUI) {

			UE_LOG(LogTemp, Warning, TEXT("Fail PlayerUI"));
			return;
		}
		LobbyHUD->PlayerUI->StatusUI->UpdateWidget(playername, playerclass);
	}
	UE_LOG(LogTemp, Log, TEXT("OwnerSetStatus End"));
}

#pragma endregion


#pragma region Server RPC

// �г��� ������Ʈ
void ALobbyPlayerController::ServerUpdateNickname_Implementation(const FString& playernickname)
{

	AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(this);
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(CurrentGameMode);

	ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());


	if (GameMode)
	{
		GameMode->SetPlayerNickname(PlayerGuid, playernickname);
	}

}

// Ŭ���� ������Ʈ
void ALobbyPlayerController::ServerUpdateClass_Implementation(const EClass& playerclass)
{
	AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(this);
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(CurrentGameMode);

	if (GameMode)
	{
		GameMode->SetPlayerClass(PlayerGuid, playerclass, this);
	}
}

// ���� ������Ʈ
void ALobbyPlayerController::ServerUpdateStat_Implementation(const FPlayerCharacterStat& playerstat)
{
	ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetPlayerState<APlayerState>());
	if (LobbyPS)
	{
		LobbyPS->PlayerBaseStat = playerstat;
	}
}

void ALobbyPlayerController::ServerUpdateInventory_Implementation(const TArray<FItemData>& playeritems, const int32& playermoney)
{
	ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetPlayerState<APlayerState>());
	if (LobbyPS)
	{
		LobbyPS->PlayerItems = playeritems;
		LobbyPS->PlayerMoney = playermoney;
	}
}

void ALobbyPlayerController::ServerShowNickname_Implementation(const FString& playernickname)
{
	ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());

	if (PlayerCharacter)
	{
		PlayerCharacter->MultiShowNickname(playernickname);
	}
}

#pragma region Vote Function

void ALobbyPlayerController::SetVote_Implementation(int32 result)
{
	ALobbyGameState* LobbyGS = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (LobbyGS)
	{
		LobbyGS->AddVoteArray(result);
	}
}

void ALobbyPlayerController::ClearVote_Implementation()
{
	ALobbyGameState* LobbyGS = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (LobbyGS)
	{
		LobbyGS->VoteArray.Empty();
	}
}

#pragma endregion

#pragma endregion

