// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ArrowMeteorAnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Engine/World.h"


void UArrowMeteorAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// �θ��� Notify�� ȣ���ϸ� �⺻ ���� ����ü�� ����������,
	// ���⼭�� �켱 Super::Notify() ��� ���ϴ� ������� Ŀ���͸������մϴ�.
	// (�θ��� Notify() ������ �����Ͽ� �Ϻ� �����͸� �����ɴϴ�.)

	// 1. Owner�� ĳ���� ĳ����
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

	// 2. ���� ��ġ: ������ �� ���� ��ġ
	FVector StartLocation = MeshComp->GetSocketLocation(SpawnSocket);

	// 3. Ÿ�� ��ġ: �θ� ������ ����, ����� ũ�ν���� Ʈ���̽� ����� ���
	FVector StoredTargetLocation = GetCrosshairTraceHit(Character);

	// ���� Ʈ���̽��� ���� Ÿ�� ��ġ ����
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

	// 4. ù ��° ����ü: �������� �߻� (��: ȭ���� ���� ���)
	// ĳ������ ���� ��ġ���� ����(���� Z��)���� �߻��ϵ��� ȸ���� ���
	FRotator UpwardRot = FRotator(-90.f, Character->GetActorRotation().Yaw, 0.f);
	// (-90�� pitch�� �Ϲ������� ���濡�� �������� ���ϰ� �մϴ�. �ʿ信 ���� ����)
	// Character->Server_SpawnProjectile(StartLocation, UpwardRot, UpwardRot.Vector(), AttackProjectileClass);

	// 5. ���� ������ �� Falling ����ü ����
	// Falling ����ü���� Ÿ�� ��ġ �ֺ��� �������� �����˴ϴ�.
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

	// Falling ����ü���� Ÿ�� ��ġ �ֺ��� �� ���� �������� �����մϴ�.
	for (int32 i = 0; i < NumFallingProjectiles; i++)
	{
		// �� �� ������ ��ġ (X-Y ���)
		FVector2D RandomPoint = FMath::RandPointInCircle(AirDropRadius);
		// Ÿ�� ��ġ�� X, Y�� ���� �������� �����ϰ�, Z�� AirDropHeight��ŭ �������� �������մϴ�.
		FVector SpawnLocation = TargetLocation + FVector(RandomPoint, AirDropHeight);

		// Falling ����ü�� ������ �Ʒ��� �������� �ϹǷ�, ������ �Ʒ��� ���� (��: (0, 0, -1))
		FRotator SpawnRot = UKismetMathLibrary::MakeRotFromX(FVector(0.f, 0.f, -1.f));
		// �ʿ� �� �ణ�� ���� yaw �߰�
		SpawnRot.Yaw += FMath::FRandRange(-10.f, 10.f);

		Character->Server_SpawnProjectile(SpawnLocation, SpawnRot, SpawnRot.Vector(), FallingProjectile);
	}
}

