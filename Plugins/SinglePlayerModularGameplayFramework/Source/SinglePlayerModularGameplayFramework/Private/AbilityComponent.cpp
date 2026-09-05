// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "GameFramework/Character.h"
#include "AttributeInterface.h"
//#include "AttributeComponent.h"
//#include "AttributeData.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UAbilityObject* UAbilityComponent::FindAbility(FName AbilityName)
{
	if(AbilityContainer.Contains(AbilityName))
	{
		return AbilityContainer[AbilityName];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability %s not found in AbilityContainer."), *AbilityName.ToString());
		return nullptr;
	}
}

void UAbilityComponent::CallAbility(FName AbilityName)
{
	if (!FindAbility(AbilityName)) { UE_LOG(LogTemp, Error, TEXT("Ability NOT found"));  return; }

	if (!VerifyCanCastAbility(FindAbility(AbilityName))) { UE_LOG(LogTemp, Warning, TEXT("Cant Cast Ability")); return; }

	UAbilityObject* Ability = FindAbility(AbilityName);
	FAbilityData Data = Ability->AbilityData;

	if(Data.bInstant)
	{
		CastAbility(AbilityName);
	}
	else
	{
		FOvertimeAbility OvertimeAbility;
		OvertimeAbility.SourceName = AbilityName;
		OvertimeAbility.TickRate = Data.AbilityTickRate;
		OvertimeAbility.Accumulator = 0.f;
		OvertimeAbility.bHasDuration = Data.bHasDuration;
		OvertimeAbility.Duration = Data.AbilityDuration;

		AddOvertimeAbilityEffect(AbilityName, OvertimeAbility);
	}
}

bool UAbilityComponent::VerifyCanCastAbility(UAbilityObject* Ability)
{
	if (!Ability || !OwnerCharacter) { return false; }

	FAbilityData Data = Ability->AbilityData;
	FName AttributeName = Data.VinculatedAttribute;
	float Cost = Data.AttributeCost;
	

	if (Ability->bIsInCooldown)
	{
		bCanCastAbility = false;
		return false;
	}

	if (Data.AttributeCost > 0.f && OwnerCharacter->GetClass()->ImplementsInterface(UAttributeInterface::StaticClass()))
	{
		
		if (!IAttributeInterface::Execute_CheckAttribute(OwnerCharacter, AttributeName))
		{
			bCanCastAbility = false;
			return false;
		}

		float AttributeCurrentValue = IAttributeInterface::Execute_GetAttributePropertyValue(OwnerCharacter, AttributeName, EAttributePropertyName::Default, EAttributePropertyType::Current);
		if (AttributeCurrentValue < Cost)
		{
			bCanCastAbility = false;
			return false;
		}
	}
	bCanCastAbility = true;
	return true;
}


void UAbilityComponent::CastAbility(FName AbilityName)
{
	if (!FindAbility(AbilityName)) { UE_LOG(LogTemp, Error, TEXT("Ability NOT found"));  return; }

	UAbilityObject* Ability = AbilityContainer[AbilityName];
	FAbilityData Data = Ability->AbilityData;

	Ability->DesiredSpawnLocation = SpawnLocation;
	Ability->Target = Target;


	if(Data.bNeedTarget && Target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability %s requires a target but none was provided."), *AbilityName.ToString());
		return;
	}

	if (Data.bSelfOrigin)
	{
		SpawnLocation = GetOwner()->GetActorLocation();
	}

	switch (Data.AbilityEffectType)
	{
		case EAbilityEffectType::Radial:
			Ability->CreateRadialEffect();
			break;
		case EAbilityEffectType::Beam:
			Ability->CreateBeamEffect();
			break;
		case EAbilityEffectType::Projectile:
			Ability->LaunchProjectile();
			break;
	}	

	FName AttributeName = Data.VinculatedAttribute;
	float Cost = Data.AttributeCost;

	if (!OwnerCharacter)
	{
		return;
	}
	if (Data.AttributeCost > 0.f && OwnerCharacter->GetClass()->ImplementsInterface(UAttributeInterface::StaticClass()))
	{
		if (IAttributeInterface::Execute_CheckAttribute(OwnerCharacter, AttributeName))
		{
			IAttributeInterface::Execute_UpdateAttributePropertyValue(OwnerCharacter, AttributeName, Cost,
				EAttributePropertyName::Default, EAttributePropertyType::Current, false);
		}
	}
		
	FCooldown AbilityCD;
	AbilityCD.SourceName = AbilityName;
	AbilityCD.CooldownTime = Data.Cooldown;
	AbilityCD.Accumulator = 0.f;

	StartCooldown(AbilityName, AbilityCD);
		
	
}

void UAbilityComponent::AddOvertimeAbilityEffect(FName AbilityName, const FOvertimeAbility& OverTimeAbility)
{
	ActiveOverTimeAbilities.FindOrAdd(AbilityName, OverTimeAbility);

	if (!GetWorld()->GetTimerManager().IsTimerActive(MasterTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(MasterTimerHandle, this, &UAbilityComponent::ProcessOvertimeAbilitiesTicks, MasterTickInterval, true);
	}

}

void UAbilityComponent::RemoveOvertimeAbilityEffect(FName AbilityName)
{
	if(ActiveOverTimeAbilities.Contains(AbilityName))
	{
		ActiveOverTimeAbilities.Remove(AbilityName);
	}

	if(ActiveOverTimeAbilities.Num() == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(MasterTimerHandle);
	}

}

void UAbilityComponent::ProcessOvertimeAbilitiesTicks()
{
	TArray<FName> AbilitiesToRemove;

	for (auto& AbilityPair : ActiveOverTimeAbilities)
	{
		FName AbilityName = AbilityPair.Key;
		FOvertimeAbility& OvertimeAbility = AbilityPair.Value;
		if (FindAbility(AbilityName))
		{
			OvertimeAbility.Accumulator += MasterTickInterval;

			if (OvertimeAbility.bHasDuration && OvertimeAbility.Accumulator >= OvertimeAbility.Duration)
			{	
				AbilitiesToRemove.Add(AbilityName);
				continue;
			}
			if (OvertimeAbility.Accumulator >= OvertimeAbility.TickRate)
			{
				CastAbility(AbilityName);
				OvertimeAbility.Accumulator -= OvertimeAbility.TickRate;
			}
		}
		else
		{
			AbilitiesToRemove.Add(AbilityName);
		}
	}
	// 3. Remove com segurança agora que o TMap não está sendo lido
	for (const FName& NameToRemove : AbilitiesToRemove)
	{
		RemoveOvertimeAbilityEffect(NameToRemove);
	}
}

void UAbilityComponent::StartCooldown(FName AbilityName, const FCooldown& CD)
{
	if (UAbilityObject* Ability = FindAbility(AbilityName))
	{
		ActiveCooldowns.FindOrAdd(AbilityName, CD);
		Ability->bIsInCooldown = true;

		if (!GetWorld()->GetTimerManager().IsTimerActive(MasterCooldownTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(MasterCooldownTimerHandle, this, &UAbilityComponent::ProcessCooldownTick, MasterTickInterval, true);
		}
	}

}

void UAbilityComponent::EndCooldown(FName AbilityName)
{
	if (ActiveCooldowns.Contains(AbilityName))
	{
		ActiveCooldowns.Remove(AbilityName);
	}

	if (ActiveCooldowns.Num() == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(MasterCooldownTimerHandle);
	}
}

void UAbilityComponent::ProcessCooldownTick()
{
	TArray<FName> CooldownsToFinish;

	for (auto& AbilityCD : ActiveCooldowns)
	{
		FName CDName = AbilityCD.Key;
		FCooldown& CD = AbilityCD.Value;

		CD.Accumulator += MasterTickInterval;

		if (CD.Accumulator >= CD.CooldownTime)
		{
			if (UAbilityObject* Ability = FindAbility(CDName))
			{
				Ability->bIsInCooldown = false;
			}
			CD.Accumulator = 0.f;
			CooldownsToFinish.Add(CDName);
		}		
	}
	for (const FName& NameToFinish : CooldownsToFinish)
	{
		EndCooldown(NameToFinish);
	}
}