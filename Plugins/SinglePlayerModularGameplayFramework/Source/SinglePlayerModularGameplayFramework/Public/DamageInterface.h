// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "DamageData.h"
#include "DamageInterface.generated.h"

/**
 * 
 */

class AMainCharacter;

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API IDamageInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void TakeDamage(FDamageData DamageData);

};
