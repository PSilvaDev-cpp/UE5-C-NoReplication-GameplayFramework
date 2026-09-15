// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemData.h"
#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API UItemObject : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	FItemData ItemData;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void UseItem();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void EquipItem();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void CallItemUsage();


};
