// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttributeComponent.h"
#include "AbilityComponent.h"
#include "DamageInterface.h"
#include "AttributeInterface.h"
#include "MainCharacter.generated.h"

class UAttributeComponent;

UCLASS()
class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API AMainCharacter : public ACharacter, public IDamageInterface, public IAttributeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	virtual bool CheckAttribute_Implementation(FName AttributeName) override;

	virtual float GetAttributePropertyValue_Implementation(FName AttributeName, EAttributePropertyName ATN, EAttributePropertyType APT) override;

	virtual void UpdateAttributePropertyValue_Implementation(FName AttributeName, float Value, EAttributePropertyName APN, EAttributePropertyType APT, bool bOverride) override;
	*/

	UAttributeComponent* AttributeComponentREF;
	UAbilityComponent* AbilityComponentREF;

};
