// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttributeComponent.h"
#include "AbilityComponent.h"
#include "DamageInterface.h"
#include "AttributeInterface.h"
#include "Components/SphereComponent.h"

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

	UFUNCTION(BlueprintCallable, Category = "AttributeInterface")
	virtual bool CheckAttribute(FName AttributeName) override;

	UFUNCTION(BlueprintCallable, Category = "AttributeInterface")
	virtual float GetAttributePropertyValue(FName AttributeName, EAttributePropertyName ATN, EAttributePropertyType APT) override;

	UFUNCTION(BlueprintCallable, Category = "AttributeInterface")
	virtual void UpdateAttributePropertyValue(FName AttributeName, float Value, EAttributePropertyName APN, EAttributePropertyType APT, bool bOverride) override;
	

	UAttributeComponent* AttributeComponentREF;
	UAbilityComponent* AbilityComponentREF;

	TMap<FName, USphereComponent*> AurasSpheres;

	void UpdateAuras();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void AddAura(FName AuraName, FAuraData Aura);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAura(FName AuraName);

	

	TMap<FName, FAuraData> ActiveAuras;
	void ProcessAurasTick();
	FTimerHandle AurasMasterTimerHandle;
	const float MasterTickInterval = 0.1f;

};
