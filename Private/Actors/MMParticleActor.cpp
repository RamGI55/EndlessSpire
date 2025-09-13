// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MMParticleActor.h"

#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AMMParticleActor::AMMParticleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
}

// Called when the game starts or when spawned
void AMMParticleActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMMParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FollowTarget) return;

	ACharacter* Character = Cast<ACharacter>(FollowTarget);
	FVector Location = FollowTarget->GetActorLocation();
	FRotator Rotation = FollowTarget->GetActorRotation();

	if (Character) {
		Location = Character->GetMesh()->GetSocketLocation(FollowSocketName);
		Rotation = Character->GetMesh()->GetSocketRotation(FollowSocketName);
	}

	if (FollowTarget) {
		SetActorLocation(Location);
		SetActorRotation(Rotation);
	}
}

UParticleSystemComponent* AMMParticleActor::GetParticleSystemComponent() const
{
	return ParticleSystemComponent;
}

