// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MMParticleActor.generated.h"

class UParticleSystemComponent;

UCLASS()
class ENDLESSSPIRE_API AMMParticleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMParticleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UParticleSystemComponent* GetParticleSystemComponent() const;

	AActor* FollowTarget;
	FName FollowSocketName;
	
private:
	UParticleSystemComponent* ParticleSystemComponent;

};
