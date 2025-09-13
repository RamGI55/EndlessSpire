// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actors/Characters/BaseCharacter.h"
#include "Interface/InteractionInterface.h"
#include "UStructs/FInteractionData.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayerCharacter.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFightCamDelegate);

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
class UMMPlayerStatComponent;
class UMMInventoryComponent;
class UPaperSpriteComponent;
class UInputMappingContext;
class UInputAction;
class UPaperSprite;
class UAIPerceptionStimuliSourceComponent;
struct FInputActionValue;

UCLASS()
class ENDLESSSPIRE_API ABasePlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ABasePlayerCharacter();

protected:
	UPROPERTY(BlueprintAssignable, Category = "EventDispatcher")
	FFightCamDelegate FightCamEventDispatcher;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	// To add mapping context
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns PlayerStatComponent subobject **/
	FORCEINLINE class UMMPlayerStatComponent* GetPlayerStatComponent() const { return PlayerStatComponent; }

	/** Possess **/
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void EnterFight();
	virtual void ExitFight();

#pragma region InputResponse
public:
	UFUNCTION(Category = "InputResponse")
	virtual void OnLeftMouseBeginClick();

	UFUNCTION(Category = "InputResponse")
	virtual void OnLeftMouseHolding(bool triggered, float ElapsedTime);

	UFUNCTION(Category = "InputResponse")
	virtual void OnLeftMouseReleased(bool cancled);
	
	UFUNCTION(Category = "InputResponse")
	virtual void OnRightMouseBeginClick();

	UFUNCTION(Category = "InputResponse")
	virtual void OnRightMouseHolding(bool triggered, float ElapsedTime);

	UFUNCTION(Category = "InputResponse")
	virtual void OnRightMouseReleased(bool cancled);

	UFUNCTION(Category = "InputResponse")
	virtual void OnInteractBeginClick();

	UFUNCTION(Category = "InputResponse")
	virtual void OnInteractReleased();

#pragma endregion

#pragma region RPCs
public:
	UFUNCTION(Server, Reliable)
	virtual void ServerCastEnterFight();
	UFUNCTION(Server, Reliable)
	virtual void ServerCastExitFight();

	UFUNCTION(Server, Reliable)
	virtual void ServerCastRun();

	UFUNCTION(Server, Reliable)
	virtual void ServerCastStopRunning();

	UFUNCTION(NetMulticast, Reliable)
	virtual void BroadcastEnterFight();
	UFUNCTION(NetMulticast, Reliable)
	virtual void BroadcastExitFight();

	UFUNCTION(NetMulticast, Reliable)
	virtual void BroadCastRun();

	UFUNCTION(NetMulticast, Reliable)
	virtual void BroadCastStopRunning();
#pragma endregion

public:
	bool IsRunning() const;
	bool IsFighting() const;

protected:
	UFUNCTION(Category = "FightCamera")
	void MoveToFightView();

	UFUNCTION(Category = "FightCamera")
	void MoveToNonFightView();

private:
	UFUNCTION()
	void OnAnimationEnd(UAnimMontage* Montage, bool bInterrupted);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseEightDirectionMove = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FightSocketOffset = FVector(0, 150, 75);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector NonFightSocketOffset = FVector(0, 0, 75);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FightViewBlendAlpha = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FightViewTolerance = 0.1f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRunning;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFighting;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Sprite **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	UMMPlayerStatComponent* PlayerStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	TMap<int32, UPaperSprite*> SpriteMap;

#pragma region InputProperties	//인풋엑션을 도대체 누가 들고있나요? 
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* NicknameWidget;

	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* StimulusSource; 

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	UMMInventoryComponent* PlayerInventory;


public:
	
	UFUNCTION()
	void SetPlayerStat(FPlayerCharacterStat NewStat);

	UFUNCTION()
	void SetInventory(TArray<FItemData>& NewItems, int32 NewMoney);

	UFUNCTION(NetMulticast, Reliable)
	void SetMinimap(int32 index);

	UFUNCTION(NetMulticast, Reliable)
	void MultiShowNickname(const FString& playernickname);

	UFUNCTION()
	void SetStimulusSource();
	

#pragma region Interaction

public:

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };
	FORCEINLINE UMMInventoryComponent* GetInventory() const { return PlayerInventory; };


	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

	void DropItem(const FName ItemID, const int32 QuantityToDrop);

	UFUNCTION(Server, Reliable)
	void ServerDropItem(UItemBase* ItemToDrop, const int32 RemovedQuantity);


	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InteractableObject, ABasePlayerCharacter* player);

	FVector GetCrosshairTraceHit();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData1 InteractionData;

	FVector CameraHitLocation;
#pragma endregion 

};
