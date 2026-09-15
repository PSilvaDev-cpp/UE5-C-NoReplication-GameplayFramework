// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "ItemPickUpActor.generated.h"

class UInventoryComponent;

UCLASS()
class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API AItemPickUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	int32 Amount;

	UFUNCTION(BlueprintCallable, Category = "PickUp")
	void PickUp(UInventoryComponent* Inventory);

};
