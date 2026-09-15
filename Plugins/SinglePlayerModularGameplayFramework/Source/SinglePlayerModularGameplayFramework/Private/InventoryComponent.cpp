// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "MainCharacter.h"
#include "ItemActor.h"
#include "ItemPickUpActor.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UInventoryComponent::FindItemByID(int32 ItemID)
{
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i].Item.ItemID == ItemID)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UInventoryComponent::FindItemByName(FName Name)
{
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i].Item.ItemName == Name)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

void UInventoryComponent::AddNewItem(FItemRepresentation& ItemData, int32 NewAmount)
{
	ItemData.Amount = NewAmount;
	Items.Add(ItemData);
}

int32 UInventoryComponent::UpdateItemAmount(int32 ItemIndex, int32 Value)
{		
	
	if (!Items.IsValidIndex(ItemIndex)) return -1;

	int32 Total = Items[ItemIndex].Amount + Value;
	int32 Max = Items[ItemIndex].Item.ItemMaxStack;
	

	// Calcula a sobra (Overflow)
	int32 Diff = (Total > Max) ? (Total - Max) : 0;

	// Atualiza a quantidade do slot atual (limitado ao MaxStack)
	if (Total <= 0)
	{
		Items[ItemIndex].Amount = FMath::Clamp(0, 0, 0);
		Items.RemoveAt(ItemIndex);
		return Diff;
	}
	Items[ItemIndex].Amount = FMath::Clamp(Total, 0, Max);

	return Diff;
}

void UInventoryComponent::AddItemProcess(FItemRepresentation Item, int32 Amount, AItemPickUpActor* ItemPickUp)
{

	FItemData Data = Item.Item;

	// Caso de item não estacável (Mantido igual)
	if (!Data.bStackable)
	{
		for (int32 i = 0; i < Amount; i++)
		{
			FItemRepresentation SingleItem = Item;
			SingleItem.Amount = 1;
			AddNewItem(SingleItem, 1);
			if (OwnerCharacter && OwnerCharacter->MainHUDInstance)
			{
				OwnerCharacter->MainHUDInstance->AddNewItem(SingleItem, 1); // Passando SingleItem para garantir a UI correta
			}
		}
		if (ItemPickUp) ItemPickUp->Destroy();
		return;
	}

	int32 RemainingAmount = Amount;

	// 1. Tentar preencher stacks já existentes no Array
	for (int32 Index = 0; Index < Items.Num(); Index++)
	{
		// Acessamos o slot atual usando o Index
		FItemRepresentation& Slot = Items[Index];

		if (Slot.Item == Data && Slot.Amount < Slot.Item.ItemMaxStack)
		{
			int32 Space = Slot.Item.ItemMaxStack - Slot.Amount;
			int32 AmountToAdd = FMath::Min(RemainingAmount, Space);

			Slot.Amount += AmountToAdd;
			RemainingAmount -= AmountToAdd;

			if (ItemPickUp) ItemPickUp->Amount = RemainingAmount;

			// Atualiza a Widget: Passamos o INDEX exato do slot, e não o FindItemByID!
			if (OwnerCharacter && OwnerCharacter->MainHUDInstance)
			{
				OwnerCharacter->MainHUDInstance->UpdateItemQuantity(Index, Slot.Amount);
			}

			if (RemainingAmount <= 0)
			{
				if (ItemPickUp) ItemPickUp->Destroy();
				return;
			}
		}
	}

	// 2. Se ainda sobrou quantidade, criar novos slots
	if (RemainingAmount > 0)
	{
		int32 FullStacks = RemainingAmount / Data.ItemMaxStack;
		int32 Leftover = RemainingAmount % Data.ItemMaxStack;

		// Spawna os slots inteiros
		for (int32 i = 0; i < FullStacks; i++)
		{
			FItemRepresentation FullSlot = Item;
			FullSlot.Amount = Data.ItemMaxStack;
			AddNewItem(FullSlot, Data.ItemMaxStack);

			if (OwnerCharacter && OwnerCharacter->MainHUDInstance)
			{
				OwnerCharacter->MainHUDInstance->AddNewItem(FullSlot, Data.ItemMaxStack);
			}
		}

		// Spawna o slot parcial
		if (Leftover > 0)
		{
			FItemRepresentation PartialSlot = Item;
			PartialSlot.Amount = Leftover;
			AddNewItem(PartialSlot, Leftover);

			if (OwnerCharacter && OwnerCharacter->MainHUDInstance)
			{
				OwnerCharacter->MainHUDInstance->AddNewItem(PartialSlot, Leftover);
			}
		}

		// Como ainda não existe um limite máximo de slots no Array (TArray Items), 
		// todo RemainingAmount foi absorvido pelo inventário. Podemos destruir o ator com segurança.
		if (ItemPickUp)
		{
			ItemPickUp->Destroy();
		}
	}
}

void UInventoryComponent::DropItem(int32 SlotIndex, int32 Amount)
{
	///*
	if (!Items.IsValidIndex(SlotIndex) || !OwnerCharacter || GetWorld()) return;

	FItemRepresentation& SlotItem = Items[SlotIndex];
	FItemData& Data = SlotItem.Item;

	// --- CASO 1: ITEM É STACKABLE ---
	if (Data.bStackable)
	{
		// Garantimos que não dropamos mais do que existe no slot atual
		int32 ActualDropAmount = FMath::Min(Amount, SlotItem.Amount);
		if (ActualDropAmount <= 0) return;

		// AQUI A LÓGICA MATEMÁTICA DE DIVISÃO:
		// Decidimos quantos Actors de "Stack Máximo" precisamos spawnar no chão
		int32 FullStacksToSpawn = ActualDropAmount / Data.ItemMaxStack;
		int32 RemainderToSpawn = ActualDropAmount % Data.ItemMaxStack;

		// 1. Spawn dos stacks cheios
		for (int32 i = 0; i < FullStacksToSpawn; i++)
		{
			SpawnItemActor(Data, Data.ItemMaxStack);
		}

		// 2. Spawn do resto (se houver)
		if (RemainderToSpawn > 0)
		{
			SpawnItemActor(Data, RemainderToSpawn);
		}

		// 3. Atualizamos a quantidade no inventário
		int32 Result = SlotItem.Amount -= ActualDropAmount;

		if (OwnerCharacter && OwnerCharacter->MainHUDInstance)
		{
			OwnerCharacter->MainHUDInstance->UpdateItemQuantity(SlotIndex, Result);
		}

		// Graças ao seu operator==, o Remove agora funciona perfeitamente
		if (SlotItem.Amount <= 0)
		{
			Items.RemoveAt(SlotIndex);
		}
	}
	// --- CASO 2: ITEM NÃO É STACKABLE ---
	else
	{
		// Para itens não stackáveis, cada unidade está em um slot diferente.
		// Precisamos dropar X slots que contenham este mesmo item.
		int32 DroppedCount = 0;

		// Iteramos de trás para frente para remover elementos do TArray com segurança
		for (int32 i = Items.Num() - 1; i >= 0; i--)
		{
			// AQUI USAMOS O SEU OPERADOR: (Slot == Item)
			// Como você definiu que FItemRepresentation == se os ItemData forem iguais,
			// isso vai encontrar todos os slots do mesmo item não stackável.
			if (Items[i] == SlotItem)
			{
				SpawnItemActor(Data, 1);
				Items.RemoveAt(i);

				if (OwnerCharacter && OwnerCharacter->MainHUDInstance)
				{
					OwnerCharacter->MainHUDInstance->UpdateItemQuantity(i, 0);
				}

				DroppedCount++;

				// Paramos quando atingirmos a quantidade desejada
				if (DroppedCount >= Amount)
				{
					break;
				}
			}
		}
	}

	
}

void UInventoryComponent::SpawnItemActor(FItemData& Data, int32 Amount)
{
	TSubclassOf<AItemPickUpActor> ClassToSpawn = Data.ItemPickUpActorRef->StaticClass();
	if (!ClassToSpawn) { UE_LOG(LogTemp, Error, TEXT("ClassToSpawn NOT Valid Low Level")); return; }

	FVector SpawnLocation = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * 500.f);
	FRotator Rotation = OwnerCharacter->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AItemPickUpActor* SpawnedItem = GetWorld()->SpawnActor<AItemPickUpActor>(ClassToSpawn, SpawnLocation, Rotation, SpawnParams);

	if (SpawnedItem)
	{
		SpawnedItem->ItemData = Data;
		// Importante: Seu AItemActor deve ter a variável 'Amount' para refletir o stack no chão
		SpawnedItem->Amount = Amount;
	}
}