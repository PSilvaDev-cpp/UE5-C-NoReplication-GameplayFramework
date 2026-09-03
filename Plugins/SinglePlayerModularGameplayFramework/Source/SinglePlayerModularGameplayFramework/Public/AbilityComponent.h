// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityObject.h"
#include "AbilityComponent.generated.h"

USTRUCT(BlueprintType)
struct FOvertimeAbility
{
	GENERATED_BODY()

	UPROPERTY()
	FName SourceName = "Name";

	UPROPERTY()
	float TickRate = 0.f;

	UPROPERTY()
	float Accumulator = 0.f;

	UPROPERTY()
	bool bHasDuration = false;

	UPROPERTY()
	float Duration = 0.f;

};

USTRUCT(BlueprintType)
struct FCooldown
{
	GENERATED_BODY()

	UPROPERTY()
	FName SourceName = "Name";

	UPROPERTY()
	float CooldownTime = 0.f;

	UPROPERTY()
	float Accumulator = 0.f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TMap<FName, UAbilityObject*> AbilityContainer;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	UAbilityObject* FindAbility(FName AbilityName);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void CallAbility(FName AbilityName);

	TMap<FName, FOvertimeAbility> ActiveOverTimeAbilities;
	TMap<FName, FCooldown> ActiveCooldowns;

	FTimerHandle AbilityCooldownTimerHandle;
	FTimerHandle MasterCooldownTimerHandle;
	FTimerHandle MasterTimerHandle;
	const float MasterTickInterval = 0.1f;

	void AddOvertimeAbilityEffect(FName AbilityName, const FOvertimeAbility& OverTimeAbility);
	void RemoveOvertimeAbilityEffect(FName AbilityName);
	void ProcessOvertimeAbilitiesTicks();
	
	void StartCooldown(FName AbilityName, const FCooldown& CD);
	void EndCooldown(FName AbilityName);
	void ProcessCooldownTick();

	void CastAbility(FName AbilityName);

	AActor* Target;
	FVector SpawnLocation;

};
