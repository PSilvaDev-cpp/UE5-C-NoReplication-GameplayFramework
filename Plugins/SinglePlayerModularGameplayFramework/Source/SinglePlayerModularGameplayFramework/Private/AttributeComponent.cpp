// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	StartAutoDepletation();

}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FAttributeData* UAttributeComponent::FindAttribute(FName Attribute)
{
	for (FAttributeData& Att : Attributes)
	{
		if (Att.AttributeName == Attribute)
		{
			return &Att;
		}
	}
	return nullptr;
}

float UAttributeComponent::GetAttributeCurrentValue(FName Attribute) 
{
	return FindAttribute(Attribute)->GetCurrentValue();
}


float UAttributeComponent::GetAttributeBaseValue(FName Attribute)
{
	return FindAttribute(Attribute)->GetBaseValue();
}

float UAttributeComponent::GetAttributeMaxValue(FName Attribute)
{
	return FindAttribute(Attribute)->GetMaxValue();
}

float UAttributeComponent::GetAttributeCurrentRegenValue(FName Attribute)
{
	return FindAttribute(Attribute)->GetCurrentRegenValue();
}

float UAttributeComponent::GetAttributeBaseRegenValue(FName Attribute)
{
	return FindAttribute(Attribute)->GetBaseRegenValue();
}

float UAttributeComponent::GetAttributeMaxRegenValue(FName Attribute)
{
	return FindAttribute(Attribute)->GetMaxRegenValue();
}

float UAttributeComponent::GetAttributeCurrentDepletationValue(FName Attribute)
{
	return FindAttribute(Attribute)->GetCurrentDepletationValue();
}

float UAttributeComponent::GetAttributeBaseDepletationValue(FName Attribute)
{
	return FindAttribute(Attribute)->GetBaseDepletationValue();
}

float UAttributeComponent::GetAttributeMaxDepletationValue(FName Attribute)
{
	return FindAttribute(Attribute)->GetMaxDepletationValue();
}

void UAttributeComponent::UpdateAttributeCurrentValue(FName Attribute, FAttributeModifier& Mod)
{
	FindAttribute(Attribute)->UpdateCurrentValue(Mod);
}

void UAttributeComponent::UpdateAttributeBaseValue(FName Attribute, FAttributeModifier& Mod)
{
	FindAttribute(Attribute)->UpdateBaseValue(Mod);
}

void UAttributeComponent::UpdateAttributeMaxValue(FName Attribute, FAttributeModifier& Mod, float Limit)
{
	FindAttribute(Attribute)->UpdateMaxValue(Mod, Limit);
}

void UAttributeComponent::UpdateAttributeCurrentRegenValue(FName Attribute, FAttributeModifier& Mod)
{
	FindAttribute(Attribute)->UpdateCurrentRegenValue(Mod);
}

void UAttributeComponent::UpdateAttributeBaseRegenValue(FName Attribute, FAttributeModifier& Mod)
{
	FindAttribute(Attribute)->UpdateBaseRegenValue(Mod);
}

void UAttributeComponent::UpdateAttributeMaxRegenValue(FName Attribute, FAttributeModifier& Mod, float Limit)
{
	FindAttribute(Attribute)->UpdateMaxRegenValue(Mod, Limit);
}

void UAttributeComponent::UpdateAttributeCurrentDepletationValue(FName Attribute, FAttributeModifier& Mod)
{
	FindAttribute(Attribute)->UpdateCurrentDepletationValue(Mod);
}

void UAttributeComponent::UpdateAttributeBaseDepletationValue(FName Attribute, FAttributeModifier& Mod)
{
	FindAttribute(Attribute)->UpdateBaseDepletationValue(Mod);
}

void UAttributeComponent::UpdateAttributeMaxDepletationValue(FName Attribute, FAttributeModifier& Mod, float Limit)
{
	FindAttribute(Attribute)->UpdateMaxDepletationValue(Mod, Limit);
}

void UAttributeComponent::DecreaseAttribute(float Value, FName Attribute)
{
	FindAttribute(Attribute)->Decrease(Value);
}

void UAttributeComponent::IncreaseAttribute(float Value, FName Attribute)
{
	FindAttribute(Attribute)->Increase(Value);
}

void UAttributeComponent::DecreaseAttributeOvertime(float Value, FName Attribute, float TickRate, FName SourceName)
{
	if (!GetWorld()) { return; }

	UE_LOG(LogTemp, Warning, TEXT("DecreaseOvertime"));

	if (FAttributeData* Data = FindAttribute(Attribute))
	{
		Data->DesiredTickRate = TickRate;
	}

	ActiveDecreasings.Add(Attribute, Value);

	if (!GetWorld()->GetTimerManager().IsTimerActive(MasterTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(MasterTimerHandle, this, &UAttributeComponent::ProcessAttributeTicks, MasterTickInterval, true);
	}

}

void UAttributeComponent::IncreaseAttributeOvertime(float Value, FName Attribute, float TickRate, FName SourceName)
{
	if (!GetWorld()) return;

	if (FAttributeData* Data = FindAttribute(Attribute))
	{
		Data->DesiredTickRate = TickRate;
	}

	ActiveIncreasings.Add(Attribute, Value);

	if (!GetWorld()->GetTimerManager().IsTimerActive(MasterTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(MasterTimerHandle, this, &UAttributeComponent::ProcessAttributeTicks, MasterTickInterval, true);
	}
}

void UAttributeComponent::ProcessAttributeTicks()
{

	UE_LOG(LogTemp, Warning, TEXT("ProcessTick"));

	for (auto& Pair : ActiveDecreasings)
	{
		if (FAttributeData* Data = FindAttribute(Pair.Key))
		{
			UE_LOG(LogTemp, Warning, TEXT("FoundAttribute"));
			Data->TickAccumulator += MasterTickInterval;

			if (Data->TickAccumulator >= Data->DesiredTickRate)
			{
				UE_LOG(LogTemp, Warning, TEXT("TryCallDataDecrease"));
				Data->Decrease(Pair.Value);
				Data->TickAccumulator -= Data->DesiredTickRate;
			}

		}
	}

	for (auto& Pair : ActiveIncreasings)
	{
		if (FAttributeData* Data = FindAttribute(Pair.Key))
		{
			Data->TickAccumulator += MasterTickInterval;

			if (Data->TickAccumulator >= Data->DesiredTickRate)
			{
				Data->Increase(Pair.Value);
				Data->TickAccumulator -= Data->DesiredTickRate;
			}
		}
	}

	if (ActiveDecreasings.Num() == 0 && ActiveIncreasings.Num() == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(MasterTimerHandle);
	}

}

void UAttributeComponent::StartAutoDepletation()
{
	for (FAttributeData& Att : Attributes)
	{
		if (Att.bAutoDeplete)
		{
			UE_LOG(LogTemp, Warning, TEXT("AutoDeplete"));
			DecreaseAttributeOvertime(Att.CurrentDepletationValue, Att.AttributeName, Att.DepleteRate, "AutoDepletation");
		}
	}
}

void UAttributeComponent::StopDecreasingAttribute(FName Attribute)
{
	ActiveDecreasings.Remove(Attribute);
}

void UAttributeComponent::StopIncreasingAttribute(FName Attribute)
{
	ActiveIncreasings.Remove(Attribute);
}

int32 UAttributeComponent::GetAttributeCurrentValueAsInt(FName Attribute)
{
	return FMath::RoundToInt(FindAttribute(Attribute)->GetCurrentValue());
}

int32 UAttributeComponent::GetAttributeBaseValueAsInt(FName Attribute)
{
	return FMath::RoundToInt(FindAttribute(Attribute)->GetBaseValue());
}

int32 UAttributeComponent::GetAttributeMaxValueAsInt(FName Attribute)
{
	return FMath::RoundToInt(FindAttribute(Attribute)->GetMaxValue());
}

