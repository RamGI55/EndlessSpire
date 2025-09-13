// Fill out your copyright notice in the Description page of Project Settings.


#include "Miscellaneous/HitReactionLegacyCameraShake.h"
#include "Camera/CameraShakeSourceComponent.h"

UHitReactionShakePattern::UHitReactionShakePattern(const FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer)
{
	
	// Set up legacy-style parameters
	Duration = 0.25f;
	BlendInTime = 0.2f;
	BlendOutTime = 0.25f;
    
	ElapsedTime = 0.0f;
	bIsActive = false;
    
	// X-axis oscillation
	XOscillation.Amplitude = 2.0f;
	XOscillation.Frequency = 50.0f;
	XOscillation.Phase = 0.0f; // Zero offset
    
	// Y-axis oscillation  
	YOscillation.Amplitude = 2.0f;
	YOscillation.Frequency = 50.0f;
	YOscillation.Phase = FMath::RandRange(0.0f, 2.0f * PI); // Random offset
    
	// Z-axis oscillation
	ZOscillation.Amplitude = 3.0f;
	ZOscillation.Frequency = 500.0f;
	ZOscillation.Phase = FMath::RandRange(0.0f, 2.0f * PI); // Random offset;
	
}

void UHitReactionShakePattern::GetShakePatternInfoImpl(FCameraShakeInfo& OutInfo) const
{
	OutInfo.Duration = FCameraShakeDuration(Duration);
	OutInfo.BlendIn = BlendInTime;
	OutInfo.BlendOut = BlendOutTime;
	
}

void UHitReactionShakePattern::StartShakePatternImpl(const FCameraShakePatternStartParams& Params)
{
	ElapsedTime = 0.0f;
	bIsActive = true;
    
	// Regenerate random phases for Y and Z if restarting
	if (Params.bIsRestarting)
	{
		YOscillation.Phase = FMath::RandRange(0.0f, 2.0f * PI);
		ZOscillation.Phase = FMath::RandRange(0.0f, 2.0f * PI);
	}
}

void UHitReactionShakePattern::UpdateShakePatternImpl(const FCameraShakePatternUpdateParams& Params,
	FCameraShakePatternUpdateResult& OutResult)
{
	if (!bIsActive)
	{
		return;
	}
    
	ElapsedTime += Params.DeltaTime;
    
	// Calculate sine wave oscillations
	float XOffset = XOscillation.Amplitude * FMath::Sin(2.0f * PI * XOscillation.Frequency * ElapsedTime + XOscillation.Phase);
	float YOffset = YOscillation.Amplitude * FMath::Sin(2.0f * PI * YOscillation.Frequency * ElapsedTime + YOscillation.Phase);
	float ZOffset = ZOscillation.Amplitude * FMath::Sin(2.0f * PI * ZOscillation.Frequency * ElapsedTime + ZOscillation.Phase);
    
	// Apply the location offset
	OutResult.Location = FVector(XOffset, YOffset, ZOffset);
    
	// The base class will handle blending and scaling automatically
}

bool UHitReactionShakePattern::IsFinishedImpl() const
{
	return ElapsedTime >= Duration;
}

UHitReactionLegacyCameraShake::UHitReactionLegacyCameraShake(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}
