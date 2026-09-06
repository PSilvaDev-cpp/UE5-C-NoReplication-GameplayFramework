// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (FindComponentByClass<UAttributeComponent>())
	{
		AttributeComponentREF = FindComponentByClass<UAttributeComponent>();
		AttributeComponentREF->OwnerCharacter = this;
	}

	if (FindComponentByClass<UAbilityComponent>())
	{
		AbilityComponentREF = FindComponentByClass<UAbilityComponent>();
		AbilityComponentREF->OwnerCharacter = this;
	}

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


bool AMainCharacter::CheckAttribute(FName AttributeName)
{
	return IAttributeInterface::CheckAttribute(AttributeName);
}

float AMainCharacter::GetAttributePropertyValue(FName AttributeName, EAttributePropertyName APN, EAttributePropertyType APT)
{
	return IAttributeInterface::GetAttributePropertyValue(AttributeName, APN, APT);
}


void AMainCharacter::UpdateAttributePropertyValue(FName AttributeName, float Value, EAttributePropertyName APN, EAttributePropertyType APT, bool bOverride)
{
	IAttributeInterface::UpdateAttributePropertyValue(AttributeName, Value, APN, APT, bOverride);
}

void AMainCharacter::AddAura(FName AuraName, FAuraData Aura)
{
	if (!this || !AbilityComponentREF) { return; }

	ActiveAuras.FindOrAdd(AuraName, Aura);
	UpdateAuras();
}

void AMainCharacter::RemoveAura(FName AuraName)
{
	if (!this || !AbilityComponentREF) { return; }

	if (ActiveAuras.Contains(AuraName))
	{
		ActiveAuras.Remove(AuraName);
	}
	UpdateAuras();
}

void AMainCharacter::UpdateAuras()
{
	if (!this || !AbilityComponentREF) { return; }

	TArray<FName> AurasToRemove;

	for (auto& AuraPair : ActiveAuras)
	{
		FName AuraName = AuraPair.Key;
		FAuraData& AuraData = AuraPair.Value;

		if (!AurasSpheres.Contains(AuraName))
		{
			USphereComponent* NewSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), FName(*(AuraName.ToString() + "+Sphere")));
			NewSphere->RegisterComponent();
			NewSphere->SetupAttachment(RootComponent);
			NewSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
			NewSphere->SetGenerateOverlapEvents(true);
			NewSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			NewSphere->SetSphereRadius(AuraData.AuraRadius);
			AurasSpheres.FindOrAdd(AuraName, NewSphere);
		}
		else
		{
			AurasSpheres[AuraName]->SetSphereRadius(AuraData.AuraRadius);
		}
	}	

	for (auto& AuraPair : AurasSpheres)
	{
		FName AuraSphereName = AuraPair.Key;
		USphereComponent* AuraSphere = AuraPair.Value;

		if (ActiveAuras.Contains(AuraSphereName))
		{
			continue;
		}
		else
		{
			AurasToRemove.Add(AuraSphereName);
		}

	}

	for (FName AuraSphereName : AurasToRemove)
	{
		if (USphereComponent* Sphere = AurasSpheres.FindRef(AuraSphereName))
		{
			Sphere->DestroyComponent();
			AurasSpheres.Remove(AuraSphereName);
		}
	}

	if (ActiveAuras.Num() && AurasSpheres.Num() > 0)
	{
		if (!GetWorldTimerManager().IsTimerActive(AurasMasterTimerHandle))
		{
			GetWorldTimerManager().SetTimer(AurasMasterTimerHandle, this, &AMainCharacter::ProcessAurasTick, 0.1f, true);
		}
	}
	else
	{	
		GetWorldTimerManager().ClearTimer(AurasMasterTimerHandle);	
	}

}

void AMainCharacter::ProcessAurasTick()
{
	if (!this || !AbilityComponentREF) { return; }

	TArray<FName> AurasToRemove;

	for (auto& AuraPair : ActiveAuras)
	{
		FName AuraName = AuraPair.Key;
		FAuraData& Aura = AuraPair.Value;

		if (!AurasSpheres.Find(AuraName)) { continue; }

		TArray<AActor*> OverlappingActors;
		TSubclassOf<AActor> ClassFilter = AActor::StaticClass();
	
		AurasSpheres[AuraName]->GetOverlappingActors(OverlappingActors, ClassFilter);

		Aura.Accumulator += MasterTickInterval;

		if (!Aura.bPermanent && Aura.Accumulator >= Aura.Duration)
		{
			AurasToRemove.Add(AuraName);
		}
		if (Aura.Accumulator >= Aura.TickRate)
		{
			AbilityComponentREF->FindAbility(AuraName)->Targets = OverlappingActors;
			AbilityComponentREF->FindAbility(AuraName)->ApplyEffect();
			Aura.Accumulator -= Aura.TickRate;
		}

	}
	for (FName Aura : AurasToRemove)
	{
		RemoveAura(Aura);
	}
}

