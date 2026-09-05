// Fill out your copyright notice in the Description page of Project Settings.

#define ECC_AbilityTrace ECC_GameTraceChannel1
#define ECC_ShootTrace ECC_GameTraceChannel2

#include "AbilityObject.h"
#include "ProjectileActor.h"
#include "GameFramework/Character.h"
#include "DamageInterface.h"
#include "AttributeComponent.h"

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
	
	if (!OwnerComponent) {return;}


	ECollisionChannel TraceChannel = ECC_AbilityTrace;

	FCollisionQueryParams Params;
	if (AbilityData.bIgnoreCaster)
	{
		Params.AddIgnoredActor(OwnerCharacter);
	}

	TArray<FHitResult> HitResults;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		DesiredSpawnLocation,
		DesiredSpawnLocation,
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
				}
				else
				{
					if (AbilityData.bBlock)
					{
						Targets.Empty();
						if(Targets.Num() == 0)
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
}

void UAbilityObject::CreateBeamEffect()
{
	if (!OwnerComponent) { return; }


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
}

void UAbilityObject::LaunchProjectile()
{
	if (!OwnerComponent) { return; }

	if (ProjectileClass)
	{
		AProjectileActor* SpawnedProjectile = GetWorld()->SpawnActor<AProjectileActor>(ProjectileClass, DesiredSpawnLocation, FRotator::ZeroRotator);
		
		if (SpawnedProjectile)
		{
			UE_LOG(LogTemp, Log, TEXT("Projectile spawned successfully."));

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

void UAbilityObject::ApplyEffect()
{
	if (!OwnerComponent) { return; }

	if (AbilityData.bNeedTarget)
	{
		if(Target == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Target is null. Cannot apply effect."));
			return;
		}

		if(Target->GetClass()->ImplementsInterface(UDamageInterface::StaticClass()))
		{
			IDamageInterface::Execute_TakeDamage(Target, AbilityData.DamageData);
			return;
		}
	}
	else
	{
		for (AActor* TargetActor : Targets)
		{
			if(TargetActor && TargetActor->GetClass()->ImplementsInterface(UDamageInterface::StaticClass()))
			{
				IDamageInterface::Execute_TakeDamage(TargetActor, AbilityData.DamageData);
			}
		}

	}
}

void UAbilityObject::UpdateAttribute(FName AttributeName, float Value,  UAttributeComponent* AttributeComponent, EAttributePropertyName APN, EAttributePropertyType APT, bool bOverride)
{
	if (FAttributeData* AttData = AttributeComponent->FindAttribute(AttributeName))
	{	
		AttData->UpdateAttributePropertyValue(Value, APN, APT, bOverride);
	}
}