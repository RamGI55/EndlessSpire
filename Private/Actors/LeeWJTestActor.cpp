// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeWJTestActor.h"

#include "Kismet/KismetSystemLibrary.h"

ALeeWJTestActor::ALeeWJTestActor()
{
	bReplicates = true;

	scene = CreateDefaultSubobject<USceneComponent>(FName("aerkjgherjk"));
	RootComponent = scene;
}

void ALeeWJTestActor::BeginPlay()
{
	if (HasAuthority()) {
		AActor* tmp = Cast<AActor>(GetWorld()->GetAuthGameMode());
		if (tmp) {
			SetOwner(tmp);
			ForceNetUpdate();
			UE_LOG(LogTemp, Log, TEXT("%s"), *UKismetSystemLibrary::GetDisplayName(tmp->GetOwner()));
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("GetAuthGameMode NULL"));
		}
	}
	else {
		AActor* tmp = GetWorld()->GetFirstPlayerController();
		if (tmp) {
			SetOwner(tmp);
			ForceNetUpdate();
			UE_LOG(LogTemp, Log, TEXT("%s"), *UKismetSystemLibrary::GetDisplayName(tmp->GetOwner()));
		}
	}
}

void ALeeWJTestActor::Fuck_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("eraoiughlseiurhgliuesrhguilh"));
}
