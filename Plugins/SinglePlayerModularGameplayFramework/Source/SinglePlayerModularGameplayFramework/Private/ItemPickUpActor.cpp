// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickUpActor.h"
#include "InventoryComponent.h"

// Sets default values
AItemPickUpActor::AItemPickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AItemPickUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemPickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemPickUpActor::PickUp(UInventoryComponent* Inventory)
{
	if (!Inventory) { return; }

	FItemRepresentation Item;

	Item.Item = ItemData;
	Item.Amount = Amount;

	Inventory->AddItemProcess(Item, Item.Amount, this);

}