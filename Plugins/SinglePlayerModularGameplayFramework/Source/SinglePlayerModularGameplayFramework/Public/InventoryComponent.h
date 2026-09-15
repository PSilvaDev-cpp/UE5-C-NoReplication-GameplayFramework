// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemObject.h"
#include "ItemData.h"
#include "InventoryComponent.generated.h"

class AItemPickUpActor;

USTRUCT(BlueprintType, Blueprintable)
struct FItemRepresentation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemRepresentation")
	FItemData Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemRepresentation")
	int32 Amount;

	bool operator==(const FItemRepresentation& Other) const
	{
		// Agora duas representações são iguais apenas se o ID for o mesmo
		return Item.ItemID == Other.Item.ItemID;
	}

	bool operator!=(const FItemRepresentation& Other) const
	{
		return !(*this == Other);
	}

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class AMainCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemRepresentation> Items;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 FindItemByID(int32 ItemID);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 FindItemByName(FName Name);
 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemProcess(FItemRepresentation Item, int32 Amount, AItemPickUpActor* ItemPickUp);

	void AddNewItem(FItemRepresentation& ItemData, int32 NewAmount);

	int32 UpdateItemAmount(int32 ItemIndex, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropItem(int32 SlotIndex, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SpawnItemActor(FItemData& Data, int32 Amount);

};
