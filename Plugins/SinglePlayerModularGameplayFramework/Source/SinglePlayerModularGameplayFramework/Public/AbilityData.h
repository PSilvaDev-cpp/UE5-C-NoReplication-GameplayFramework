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
	Projectile UMETA(DisplayName = "Projectile")
};

USTRUCT(BlueprintType, Blueprintable)
struct FAbilityData
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float AttributeCost = 10.f;

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
