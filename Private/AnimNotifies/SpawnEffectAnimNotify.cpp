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

	// 데디케이티드 서버에서 실행하지 않도록
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
				FRotator::ZeroRotator,     // 회전 필요 시 SpawnRotation으로 바꿔도 됨
				SpawnScale,             // 스케일 (에디터에서 따로 설정해도 됨)
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
				MeshComp,                   // 부착할 컴포넌트
				SpawnSocket,                // 부착할 소켓 이름
				FVector::ZeroVector,        // 소켓 기준 오프셋 (필요 시 조정)
				FRotator::ZeroRotator,      // 회전값 (필요 시 조정)
				EAttachLocation::SnapToTarget, // 소켓 위치에 맞춰 부착
				true                        // 자동 소멸 여부
			);
		}


		if (CacheEffect)
		{
			UParticleSystemComponent* ParticleComp = UGameplayStatics::SpawnEmitterAttached(
				CacheEffect,
				MeshComp,                   // 부착할 컴포넌트
				SpawnSocket,                // 부착할 소켓 이름
				FVector::ZeroVector,        // 오프셋 (필요 시 조정)
				FRotator::ZeroRotator,      // 회전값 (필요 시 조정)
				EAttachLocation::SnapToTarget,
				true                        // 자동 소멸 여부
			);

			ParticleComp->SetRelativeScale3D(SpawnScale);
		}
	}

	



}
