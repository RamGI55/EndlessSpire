// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ArrowMeteorAnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Engine/World.h"


void UArrowMeteorAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// 부모의 Notify를 호출하면 기본 단일 투사체가 스폰되지만,
	// 여기서는 우선 Super::Notify() 대신 원하는 방식으로 커스터마이즈합니다.
	// (부모의 Notify() 로직을 참고하여 일부 데이터를 가져옵니다.)

	// 1. Owner와 캐릭터 캐스팅
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	ABaseCombatCharacter* Character = Cast<ABaseCombatCharacter>(Owner);
	if (!Character || !MeshComp)
	{
		return;
	}

	UWorld* World = Character->GetWorld();
	if (!World)
	{
		return;
	}

	// 2. 시작 위치: 오른손 끝 소켓 위치
	FVector StartLocation = MeshComp->GetSocketLocation(SpawnSocket);

	// 3. 타겟 위치: 부모 로직과 같이, 저장된 크로스헤어 트레이스 결과를 사용
	FVector StoredTargetLocation = GetCrosshairTraceHit(Character);

	// 라인 트레이스로 최종 타겟 위치 결정
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		StoredTargetLocation,
		ECC_Visibility,
		QueryParams
	);
	FVector FinalTargetLocation = bHit ? HitResult.ImpactPoint : StoredTargetLocation;

	// 4. 첫 번째 투사체: 위쪽으로 발사 (예: 화살을 위로 쏘기)
	// 캐릭터의 소켓 위치에서 위쪽(월드 Z축)으로 발사하도록 회전값 계산
	FRotator UpwardRot = FRotator(-90.f, Character->GetActorRotation().Yaw, 0.f);
	// (-90도 pitch는 일반적으로 전방에서 위쪽으로 향하게 합니다. 필요에 따라 조정)
	// Character->Server_SpawnProjectile(StartLocation, UpwardRot, UpwardRot.Vector(), AttackProjectileClass);

	// 5. 일정 딜레이 후 Falling 투사체 스폰
	// Falling 투사체들은 타겟 위치 주변에 무작위로 스폰됩니다.
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("SpawnFallingProjectiles"), Owner, FinalTargetLocation);

	FTimerHandle TimerHandle;
	World->GetTimerManager().SetTimer(TimerHandle, TimerDel, AirDropDelay, false);
}

void UArrowMeteorAnimNotify::SpawnFallingProjectiles(AActor* OwnerActor, FVector TargetLocation)
{
	if (!OwnerActor)
	{
		return;
	}

	ABaseCombatCharacter* Character = Cast<ABaseCombatCharacter>(OwnerActor);
	if (!Character)
	{
		return;
	}

	UWorld* World = Character->GetWorld();
	if (!World)
	{
		return;
	}

	// Falling 투사체들을 타겟 위치 주변의 원 내에 무작위로 스폰합니다.
	for (int32 i = 0; i < NumFallingProjectiles; i++)
	{
		// 원 내 무작위 위치 (X-Y 평면)
		FVector2D RandomPoint = FMath::RandPointInCircle(AirDropRadius);
		// 타겟 위치의 X, Y에 랜덤 오프셋을 적용하고, Z는 AirDropHeight만큼 위쪽으로 오프셋합니다.
		FVector SpawnLocation = TargetLocation + FVector(RandomPoint, AirDropHeight);

		// Falling 투사체는 위에서 아래로 떨어져야 하므로, 방향은 아래쪽 벡터 (예: (0, 0, -1))
		FRotator SpawnRot = UKismetMathLibrary::MakeRotFromX(FVector(0.f, 0.f, -1.f));
		// 필요 시 약간의 랜덤 yaw 추가
		SpawnRot.Yaw += FMath::FRandRange(-10.f, 10.f);

		Character->Server_SpawnProjectile(SpawnLocation, SpawnRot, SpawnRot.Vector(), FallingProjectile);
	}
}

