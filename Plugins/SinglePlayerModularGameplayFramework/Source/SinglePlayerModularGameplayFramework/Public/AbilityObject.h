// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbilityData.h"
#include "AbilityObject.generated.h"

/**
 * 
 */

class ACharacter;
class AProjectileActor;
class UAttributeComponent;
struct FAttributeData;

UCLASS(BlueprintType, Blueprintable)
class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API UAbilityObject : public UObject
{
	GENERATED_BODY()
	
public:

	UAbilityObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FAbilityData AbilityData;

	UPROPERTY()
	AActor* Target;

	UPROPERTY()
	TArray<AActor*> Targets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bIsInCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UActorComponent* OwnerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	ACharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FVector DesiredSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<AProjectileActor> ProjectileClass;

	FTimerHandle AbilityEffectTimerHandle;
	FTimerHandle AbilityCooldownTimerHandle;

	

	void ApplyEffect();
	void CreateRadialEffect();
	void CreateBeamEffect();
	void LaunchProjectile();


};
