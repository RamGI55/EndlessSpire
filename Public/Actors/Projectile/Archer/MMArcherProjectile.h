// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MMBaseProjectile.h"
#include "MMArcherProjectile.generated.h"

/**
 * 
 */

UCLASS()
class ENDLESSSPIRE_API AMMArcherProjectile : public AMMBaseProjectile
{
	GENERATED_BODY()
	
	
public: AMMArcherProjectile();
	
protected :
	void EnemyOnHit();

	

	
};
