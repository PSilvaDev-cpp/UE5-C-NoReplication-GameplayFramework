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

	for(FAttributeData& Att : Attributes)
	{
		Att.RecalculateModifiedProperties();
	}
	StartAutoDepletation();
	StartAutoRegen();
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

float UAttributeComponent::GetAttributePropertyBaseValue(FName Attr, EAttributePropertyName APN, EAttributePropertyType APT)
{
	return FindAttribute(Attr)->GetAttributePropertyBaseValue(APN, APT);
}

float UAttributeComponent::GetAttributePropertyComputedValue(FName Attr, EAttributePropertyName APN, EAttributePropertyType APT)
{
	return FindAttribute(Attr)->GetAttributePropertyComputedValue(APN, APT);
}

void UAttributeComponent::UpdateAttributePropertyValue(FName Attr, float Value, float Limit, bool bOverride, EAttributePropertyName APN, EAttributePropertyType APT)
{
	FindAttribute(Attr)->UpdateAttributePropertyValue(Value, APN, APT, Limit, bOverride);
}

void UAttributeComponent::DecreaseAttribute(float Value, FName Attribute)
{
	FindAttribute(Attribute)->Decrease(Value);
}

void UAttributeComponent::IncreaseAttribute(float Value, FName Attribute)
{
	FindAttribute(Attribute)->Increase(Value);
}

void UAttributeComponent::DecreaseAttributeOvertime(FName Attribute, const FOvertimeEffect& Effect)
{
	if (!GetWorld()) { return; }

	AddOvertimeEffect(Attribute, Effect);

}

void UAttributeComponent::IncreaseAttributeOvertime(FName Attribute, const FOvertimeEffect& Effect)
{
	if (!GetWorld()) { return; }

	AddOvertimeEffect(Attribute, Effect);

}

void UAttributeComponent::ProcessAttributeTicks()
{

	UE_LOG(LogTemp, Warning, TEXT("ProcessTick"));

	for (auto& Pair : ActiveDecreasings)
	{
		FName AttributeName = Pair.Key;
		TArray<FOvertimeEffect>& Effects = Pair.Value;

		if (FAttributeData* Data = FindAttribute(AttributeName))
		{
			UE_LOG(LogTemp, Warning, TEXT("FoundAttribute"));
			
			for (int32 i = Effects.Num() - 1; i >= 0; --i)
			{
				FOvertimeEffect& Effect = Effects[i];
				Effect.Accumulator += MasterTickInterval;

				if (Effect.Accumulator >= Effect.TickRate)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Decrease: %s on %s (Value: %f)"), *Effect.SourceName.ToString(), *AttributeName.ToString(), Effect.Value);
					Data->Decrease(Effect.Value);
					Effect.Accumulator -= Effect.TickRate;
				}
			}
		}
		else
		{
			ActiveDecreasings.Remove(AttributeName);
		}
	}

	for (auto& AttrPair : ActiveIncreasings)
	{
		FName AttributeName = AttrPair.Key;
		TArray<FOvertimeEffect>& Effects = AttrPair.Value;

		if (FAttributeData* Data = FindAttribute(AttributeName))
		{
			for (int32 i = Effects.Num() - 1; i >= 0; --i)
			{
				FOvertimeEffect& Effect = Effects[i];
				Effect.Accumulator += MasterTickInterval;

				if (Effect.Accumulator >= Effect.TickRate)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Increase: %s on %s (Value: %f)"), *Effect.SourceName.ToString(), *AttributeName.ToString(), Effect.Value);
					Data->Increase(Effect.Value);
					Effect.Accumulator -= Effect.TickRate;
				}
			}
		}
		else
		{
			// Attribute no longer exists, clean up
			ActiveIncreasings.Remove(AttributeName);
		}
	}

	if(ActiveIncreasings.Num() == 0 && ActiveDecreasings.Num() == 0)
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

			FOvertimeEffect AutoDepleteEffect;
			AutoDepleteEffect.EffectName = "AutoDeplete";
			AutoDepleteEffect.EffectType = EOvertimeEffectType::Decrease;
			AutoDepleteEffect.Value = Att.CurrentDepletationValue;
			AutoDepleteEffect.TickRate = Att.DepleteRate;
			AutoDepleteEffect.Accumulator = 0.f;

			DecreaseAttributeOvertime(Att.AttributeName, AutoDepleteEffect);
		}
	}
}

void UAttributeComponent::StartAutoRegen()
{
	for (FAttributeData& Att : Attributes)
	{
		if (Att.bAutoRegen)
		{
			FOvertimeEffect AutoRegenEffect;
			AutoRegenEffect.EffectName = "AutoRegen";
			AutoRegenEffect.EffectType = EOvertimeEffectType::Increase;
			AutoRegenEffect.Value = Att.CurrentRegenValue;
			AutoRegenEffect.TickRate = Att.RegenRate;
			AutoRegenEffect.Accumulator = 0.f;

			IncreaseAttributeOvertime(Att.AttributeName, AutoRegenEffect);
		}
	}
}

void UAttributeComponent::StopDecreasingAttribute(FName Attribute)
{
	if (ActiveDecreasings.Remove(Attribute) > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stopped ALL depletion effects on %s"), *Attribute.ToString());
	}

	if (ActiveDecreasings.Num() == 0 && ActiveIncreasings.Num() == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(MasterTimerHandle);
	}
}

void UAttributeComponent::StopIncreasingAttribute(FName Attribute)
{
	if (ActiveIncreasings.Remove(Attribute) > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stopped ALL regeneration effects on %s"), *Attribute.ToString());
	}

	if (ActiveDecreasings.Num() == 0 && ActiveIncreasings.Num() == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(MasterTimerHandle);
	}
}

void UAttributeComponent::AddModifier(FName Attribute, FAttributeTempModifier Modifier)
{
	FindAttribute(Attribute)->AddModifier(Modifier);
}

void UAttributeComponent::RemoveModifier(FName Attribute, FName SourceName)
{
	FindAttribute(Attribute)->RemoveModifier(SourceName);
}

void UAttributeComponent::AddOvertimeEffect(FName Attribute, const FOvertimeEffect& Effect)
{
	EOvertimeEffectType Type = Effect.EffectType;

	switch (Type)
	{
	case EOvertimeEffectType::Increase:
		ActiveIncreasings.FindOrAdd(Attribute).Add(Effect);
		break;
	case EOvertimeEffectType::Decrease:
		ActiveDecreasings.FindOrAdd(Attribute).Add(Effect);
		break;
	default:
		break;
	}

	if (!GetWorld()->GetTimerManager().IsTimerActive(MasterTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(MasterTimerHandle, this, &UAttributeComponent::ProcessAttributeTicks, MasterTickInterval, true);
	}

}

void UAttributeComponent::RemoveOvertimeEffect(FName Attribute, FName EffectName, bool bIsDeplete)
{
	TMap<FName, TArray<FOvertimeEffect>>& EffectMap = bIsDeplete ? ActiveDecreasings : ActiveIncreasings;

	if (TArray<FOvertimeEffect>* Effects = EffectMap.Find(Attribute))
	{
		Effects->RemoveAll([EffectName](const FOvertimeEffect& E) { return E.EffectName == EffectName; });

		if (Effects->Num() == 0)
		{
			EffectMap.Remove(Attribute);
		}
	}

	if (ActiveDecreasings.Num() == 0 && ActiveIncreasings.Num() == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(MasterTimerHandle);
	}

}