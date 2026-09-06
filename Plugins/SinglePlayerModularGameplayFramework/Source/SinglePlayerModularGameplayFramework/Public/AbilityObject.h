// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbilityData.h"
//#include "Components/SphereComponent.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	AActor* Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	TArray<AActor*> Targets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bIsInCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UActorComponent* OwnerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	class AMainCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FVector DesiredSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<AProjectileActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool VisualbDebug = true;

	FTimerHandle AbilityEffectTimerHandle;
	FTimerHandle AbilityCooldownTimerHandle;

	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	void ApplyEffect();

	void CreateRadialEffect();
	void CreateBeamEffect();
	void LaunchProjectile();
	void CreateAura(FName AuraName);



};
