// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/SpawnEffectAnimNotify.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "NiagaraSystem.h"



void USpawnEffectAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	// ��������Ƽ�� �������� �������� �ʵ���
	if (Owner->HasAuthority()) return;

	UWorld* World = Owner->GetWorld();
	if (!World) return;

	FVector SpawnLocation;

	if (SpawnSocket == "None")
	{
		SpawnLocation = Owner->GetActorLocation() + ( Owner->GetActorRotation().Vector() * ForwardDistance );
	}
	else
	{
		SpawnLocation = MeshComp->GetSocketLocation(SpawnSocket);
	}

	const FRotator SpawnRotation = Owner->GetActorRotation();

	if (!bFollowToPlayer)
	{
		if (NiagaraEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation
			(
				World,
				NiagaraEffect,
				SpawnLocation,
				FRotator::ZeroRotator,     // ȸ�� �ʿ� �� SpawnRotation���� �ٲ㵵 ��
				SpawnScale,             // ������ (�����Ϳ��� ���� �����ص� ��)
				true,                      // bAutoDestroy
				true,                      // bAutoActivate
				ENCPoolMethod::None,
				true                       // bPreCullCheck
			);
		}


		if (CacheEffect)
		{
			UParticleSystemComponent* ParticleComp = UGameplayStatics::SpawnEmitterAtLocation(
				World,
				CacheEffect,
				SpawnLocation,
				SpawnRotation,
				true                       // bAutoDestroy
			);

			ParticleComp->SetRelativeScale3D(SpawnScale);
		}
	}
	else
	{
		if (NiagaraEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached
			(
				NiagaraEffect,
				MeshComp,                   // ������ ������Ʈ
				SpawnSocket,                // ������ ���� �̸�
				FVector::ZeroVector,        // ���� ���� ������ (�ʿ� �� ����)
				FRotator::ZeroRotator,      // ȸ���� (�ʿ� �� ����)
				EAttachLocation::SnapToTarget, // ���� ��ġ�� ���� ����
				true                        // �ڵ� �Ҹ� ����
			);
		}


		if (CacheEffect)
		{
			UParticleSystemComponent* ParticleComp = UGameplayStatics::SpawnEmitterAttached(
				CacheEffect,
				MeshComp,                   // ������ ������Ʈ
				SpawnSocket,                // ������ ���� �̸�
				FVector::ZeroVector,        // ������ (�ʿ� �� ����)
				FRotator::ZeroRotator,      // ȸ���� (�ʿ� �� ����)
				EAttachLocation::SnapToTarget,
				true                        // �ڵ� �Ҹ� ����
			);

			ParticleComp->SetRelativeScale3D(SpawnScale);
		}
	}

	



}
