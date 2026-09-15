// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/Texture2D.h"

#include "ItemData.generated.h"

/**
 * 
 */

UENUM(BlueprintType, Blueprintable)
enum class EItemTypeUsage : uint8
{
	None UMETA(DisplayName = "None"),
	Use UMETA(DisplayName = "Use"),
	Equip UMETA(DisplayName = "Equip")
};

class AItemActor;
class AItemPickUpActor;

USTRUCT(BlueprintType, Blueprintable)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID = 1;

	bool operator==(const FItemData& Other) const
	{
		// Define que dois itens são iguais se tiverem o mesmo ID
		return ItemID == Other.ItemID;
	}

	bool operator!=(const FItemData& Other) const
	{
		return !(*this == Other);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName = "None";

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	//int32 Amount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FGameplayTagContainer ItemTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ItemWeight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ItemValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bStackable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemMaxStack = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemTypeUsage ItemTypeUsage = EItemTypeUsage::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float CurrentDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float MaxDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AItemPickUpActor> ItemPickUpActorRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AItemActor> ItemActorRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemIcon = nullptr;

};
