// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageData.h"
#include "AbilityData.generated.h"

/**
 * 
 */

UENUM(BlueprintType, Blueprintable)
enum class EAbilityEffectType :uint8
{
	Radial UMETA(DisplayName = "Radial"),
	Beam UMETA(DisplayName = "Beam"),
	Projectile UMETA(DisplayName = "Projectile"),
	InnerTarget UMETA(DisplayName = "InnerTarget"),
	Aura UMETA(DisplayName = "Aura")
};

USTRUCT(BlueprintType, Blueprintable)
struct FAuraData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	float AuraRadius = 500.f;

	UPROPERTY()
	bool bPermanent = true;

	UPROPERTY()
	float Duration = 10.f;

	UPROPERTY()
	float TickRate = 1.f;

	UPROPERTY()
	float TickAccumulator = 0.f;

	UPROPERTY()
	float DurationAccumulator = 0.f;

};

USTRUCT(BlueprintType, Blueprintable)
struct FAbilityData
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float AttributeCost = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FAuraData Aura;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FName VinculatedAttribute = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float Cooldown = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FDamageData DamageData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bNeedTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bBlock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	EAbilityEffectType AbilityEffectType = EAbilityEffectType::Radial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bSelfOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bInstant = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float AbilityRadius = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float AbilityRange = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float AbilityTickRate = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float AbilityDuration = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bHasDuration = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bIgnoreCaster = true;

};
