// Fill out your copyright notice in the Description page of Project Settings.

#define ECC_AbilityTrace ECC_GameTraceChannel1
#define ECC_ShootTrace ECC_GameTraceChannel2

#include "AbilityObject.h"
#include "ProjectileActor.h"
#include "GameFramework/Character.h"
#include "DamageInterface.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.h"
#include "Engine/OverlapResult.h"
//#include "AttributeComponent.h"

#include "GameFramework/ProjectileMovementComponent.h" 

UAbilityObject::UAbilityObject()
{Target = nullptr;
	OwnerComponent = nullptr;
	OwnerCharacter = nullptr;
	DesiredSpawnLocation = FVector::ZeroVector;
	ProjectileClass = nullptr;
	bIsInCooldown = false;
}

void UAbilityObject::CreateRadialEffect()
{
	if (!OwnerComponent || !OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateRadialEffect interrupted -> OwnerComponent OR OwnerCharacter NOT Valid"))
			return;
	}

	ECollisionChannel TraceChannel = ECC_AbilityTrace;

	FCollisionQueryParams Params;
	if (AbilityData.bIgnoreCaster)
	{
		Params.AddIgnoredActor(OwnerCharacter);
	}

	TArray<FOverlapResult> OverlapResults;
	Targets.Empty();

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		DesiredSpawnLocation,
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(AbilityData.AbilityRadius),
		Params
	);

	if (bHit)
	{
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			AActor* HitActor = Overlap.GetActor();

			if (HitActor)
			{
				if (AbilityData.bNeedTarget)
				{
					Targets.Empty();
				}
				else
				{
					if (AbilityData.bBlock)
					{
						Targets.Empty();
						if (Targets.Num() == 0)
						{
							Targets.Add(HitActor);
						}
					}
					else
					{
						
						Targets.Add(HitActor);
					}
				}
			}
		}
		ApplyEffect();
	}

	// Visual Debug atualizado para Overlaps
#if !UE_BUILD_SHIPPING
	if (GetWorld())
	{
		FColor DebugColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), DesiredSpawnLocation, AbilityData.AbilityRadius, 16, DebugColor, false, 0.1f);
		if (bHit)
		{
			for (const FOverlapResult& Overlap : OverlapResults)
			{
				if (AActor* OverlappedActor = Overlap.GetActor())
				{
					DrawDebugPoint(GetWorld(), OverlappedActor->GetActorLocation(), 10.f, FColor::Yellow, false, 0.1f);
					DrawDebugLine(GetWorld(), DesiredSpawnLocation, OverlappedActor->GetActorLocation(), FColor::Cyan, false, 0.f);
				}
			}
		}
	}
#endif
}

void UAbilityObject::CreateBeamEffect()
{
	if (!OwnerComponent) { return; }

	if (AbilityData.AbilityRadius <= 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("Radius <= 0. Abortando para evitar crash."));
		return;
	}

	ECollisionChannel TraceChannel = ECC_AbilityTrace;

	FCollisionQueryParams Params;
	if (AbilityData.bIgnoreCaster)
	{
		Params.AddIgnoredActor(OwnerCharacter);
	}

	TArray<FHitResult> HitResults;
	FVector ForwardVector = OwnerCharacter->GetActorForwardVector();
	FVector EndLocation = DesiredSpawnLocation + ForwardVector * AbilityData.AbilityRange;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		DesiredSpawnLocation,
		EndLocation,
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(AbilityData.AbilityRadius),
		Params
	);

	if (bHit)
	{


		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();

			if (HitActor)
			{
				if (AbilityData.bNeedTarget)
				{
					Targets.Empty();
					if(Targets.Num() == 0)
					{
						Targets.Add(HitActor);
					}
					Target = HitActor;
				}
				else
				{
					if (AbilityData.bBlock)
					{
						Targets.Empty();
						if (Targets.Num() == 0)
						{
							Targets.Add(HitActor);
						}
					}
					else
					{
						Targets.Empty();
						Targets.Add(HitActor);
					}
				}
			}
		}
		ApplyEffect();
	}
	//Visual Debug
#if !UE_BUILD_SHIPPING
	if (GetWorld())
	{
		FColor DebugColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), DesiredSpawnLocation, AbilityData.AbilityRadius, 16, DebugColor, false, 0.1f);
		if (bHit)
		{
			for (const FHitResult& Hit : HitResults)
			{
				DrawDebugPoint(GetWorld(), Hit.Location, 10.f, FColor::Yellow, false, 0.1f);
				DrawDebugLine(GetWorld(), DesiredSpawnLocation, Hit.Location, FColor::Cyan, false, 0.f);
			}
		}
	}
#endif
}

void UAbilityObject::LaunchProjectile()
{
	if (!OwnerComponent) { return; }

	if (ProjectileClass)
	{
		AProjectileActor* SpawnedProjectile = GetWorld()->SpawnActor<AProjectileActor>(ProjectileClass, DesiredSpawnLocation, FRotator::ZeroRotator);
		
		if (SpawnedProjectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("Projectile spawned successfully."));

			if (AbilityData.bNeedTarget)
			{
				Targets.Empty();
				SpawnedProjectile->ProjectileMovementComponent->bIsHomingProjectile = true;
				SpawnedProjectile->ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
			}
			else
			{
				Target = nullptr;
				Targets.Empty();
				SpawnedProjectile->ProjectileMovementComponent->bIsHomingProjectile = false;
			}
		}
	}


}

void UAbilityObject::CreateAura(FName AuraName)
{
	if (!OwnerCharacter || !OwnerComponent) { UE_LOG(LogTemp, Error, TEXT("CreateAura FAILED")); return; }

	OwnerCharacter->AddAura(AuraName, AbilityData.Aura);
}

void UAbilityObject::ApplyEffect_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ApplyEffect Called"));
	
	if (!OwnerComponent || !OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyEffect interrupted -> OwnerComponent OR OwnerCharacter NOT Valid"))
		return;
	}


	if (AbilityData.bNeedTarget)
	{
		if(Target == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Target is null. Cannot apply effect."));
			return;
		}
	}
}
