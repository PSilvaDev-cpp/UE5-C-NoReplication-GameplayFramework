// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DamageData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType, Blueprintable)
struct FDamageData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Data")
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Data")
	bool bIsOverTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Data")
	bool bIsCritical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Data")
	FGameplayTagContainer DamageTypes;

};
