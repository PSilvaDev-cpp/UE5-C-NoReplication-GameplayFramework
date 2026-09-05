// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeData.h"
//#include "GameplayTagContainer.h"

#include "AttributeComponent.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EOvertimeEffectType : uint8
{
	Increase UMETA(DisplayName = "Increase"),
	Decrease UMETA(DisplayName = "Decrease")
};

USTRUCT(BlueprintType, Blueprintable)
struct FOvertimeEffect
{
	GENERATED_BODY()

	UPROPERTY()
	FName EffectName;

	UPROPERTY()
	float Value = 0.f;

	UPROPERTY()
	float TickRate = 0.f;

	UPROPERTY()
	float Accumulator = 0.f;

	UPROPERTY()
	EOvertimeEffectType EffectType;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	TMap<FName, FAttributeData> Attributes;


	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributePropertyBaseValue(FName Attr, EAttributePropertyName APN, EAttributePropertyType APT);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributePropertyComputedValue(FName Attr, EAttributePropertyName APN, EAttributePropertyType APT);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateAttributePropertyValue(FName Attr, float Value, bool bOverride, EAttributePropertyName APN, EAttributePropertyType APT);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void DecreaseAttributeOvertime(FName Attribute, const FOvertimeEffect& Effect);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void IncreaseAttributeOvertime(FName Attribute, const FOvertimeEffect& Effect);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void StopDecreasingAttribute(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void StopIncreasingAttribute(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void DecreaseAttribute(float Value, FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void IncreaseAttribute(float Value, FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void StartAutoDepletation();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void StartAutoRegen();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddModifier(FName Attribute, FAttributeTempModifier Modifier);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void RemoveModifier(FName Attribute, FName SourceName);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddOvertimeEffect(FName Attribute, const FOvertimeEffect& Effect);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void RemoveOvertimeEffect(FName Attribute, FName EffectName, bool bIsDeplete);

	//UFUNCTION(BlueprintCallable, Category = "Attributes")
	FAttributeData* FindAttribute(FName Attribute);

	protected:

	FTimerHandle MasterTimerHandle;

	FTimerHandle AttributeIncreaseTimerHandle;
	FTimerHandle AttributeDecreaseTimerHandle;

	TMap<FName,TArray<FOvertimeEffect>> ActiveDecreasings;
	TMap<FName, TArray<FOvertimeEffect>> ActiveIncreasings;


	void ProcessAttributeTicks();

	const float MasterTickInterval = 0.1f;

};
