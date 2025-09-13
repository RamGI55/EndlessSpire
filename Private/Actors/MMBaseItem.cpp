// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MMBaseItem.h"

AMMBaseItem::AMMBaseItem()
{
	bReplicates = true;

}
void AMMBaseItem::BeginPlay()
{
	Super::BeginPlay();
	InteractableData = InstanceInteractableData;
}

void AMMBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AMMBaseItem::BeginFocus()
{

}

void AMMBaseItem::EndFocus()
{

}

void AMMBaseItem::BeginInteraction()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on interface test actor"));
}

void AMMBaseItem::EndInteraction()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on interface test actor"));
}

void AMMBaseItem::Interaction(ABasePlayerCharacter* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on interface test actor"));
}

