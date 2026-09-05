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
	}

	if (FindComponentByClass<UAbilityComponent>())
	{
		AbilityComponentREF = FindComponentByClass<UAbilityComponent>();
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

/*
bool AMainCharacter::CheckAttribute_Implementation(FName AttributeName)
{
	if (!this || !AttributeComponentREF) { return false; }

	if (!AttributeComponentREF->FindAttribute(AttributeName)) { return false; }

	return true;
}

float AMainCharacter::GetAttributePropertyValue_Implementation(FName AttributeName, EAttributePropertyName APN, EAttributePropertyType APT)
{
	if (!this || CheckAttribute_Implementation(AttributeName)) { return NULL; }

	

}
*/