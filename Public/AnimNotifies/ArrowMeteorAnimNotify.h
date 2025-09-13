// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/SpawnProjectileAnimNotify.h"
#include "ArrowMeteorAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UArrowMeteorAnimNotify : public USpawnProjectileAnimNotify
{
	GENERATED_BODY()
protected :
	// 떨어지는 투사체 메쉬.
	UPROPERTY(EditAnywhere, Category = "AirDrop")
	TSubclassOf<AMMBaseProjectile> FallingProjectile;

	// Falling 투사체 개수 (에디터에서 조정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirDrop")
	int32 NumFallingProjectiles = 30;

	// Falling 투사체가 떨어질 반경 (도 단위가 아니라 거리 단위: 예, 300.f)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirDrop")
	float AirDropRadius = 300.f;

	// Falling 투사체 스폰 전 딜레이 (초 단위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirDrop")
	float AirDropDelay = 1.0f;

	// 투사체가 스폰될 때의 높이 (타겟 위치로부터 위쪽 오프셋)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirDrop")
	float AirDropHeight = 1000.f;

	// 노티파이 실행 시 부모의 단일 스폰 로직 대신 아래의 로직을 실행합니다.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	// 일정 딜레이 후 Falling 투사체들을 스폰하는 함수
	UFUNCTION()
	void SpawnFallingProjectiles(AActor* OwnerActor, FVector TargetLocation);
	
	
};
