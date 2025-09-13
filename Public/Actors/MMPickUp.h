// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MMBaseItem.h"
#include "Interface/InteractionInterface.h"
#include "MMPickUp.generated.h"

/**
 * 
 */
class UItemBase;
class UDataTable;
class UMMInventoryComponent;
class ABasePlayerCharacter;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ENDLESSSPIRE_API AMMPickUp : public AMMBaseItem
{
	GENERATED_BODY()


public :
	AMMPickUp();

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastItemDataInitialize(const FName& ItemID, const int32 InQuantity);
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastDropFromTargetInventory(ABasePlayerCharacter* Player, const FName ItemID, const int32 QuantityToDrop);

	void InitializePickUp(const TSubclassOf<UItemBase> baseClass, const int32 ItemQuantity);

	void InitializeDrop(UItemBase* ItemDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemDAta() { return ItemReference; }

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

protected:	

	UPROPERTY(VisibleAnywhere, Category = "PickUp | Components")
	UStaticMeshComponent* PickUpMesh;

	UPROPERTY(EditInstanceOnly, Category = "PickUp | Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "PickUp | Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "PickUp | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "PickUp | Item Initialization")
	int32 ItemQuantity;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction(ABasePlayerCharacter* Player) override;
	void UpdateInteractableData();

	void TakePickUp(const ABasePlayerCharacter* Taker);


	UFUNCTION(NetMulticast, Reliable)
	void BroadcastTakePickUp(ABasePlayerCharacter* Player);

	UUserWidget* InteractionWidget;

	// 템플릿 클래스 생성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InteractionWidgetClass;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);



};


