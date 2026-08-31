// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeData.h"
//#include "GameplayTagContainer.h"

#include "AttributeComponent.generated.h"


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
	TArray<FAttributeData> Attributes;




	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributeCurrentValue(FName Attribute) ;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributeBaseValue(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributeMaxValue(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributeCurrentRegenValue(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributeBaseRegenValue(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributeMaxRegenValue(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributeCurrentDepletationValue(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributeBaseDepletationValue(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributeMaxDepletationValue(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateAttributeCurrentValue(FName Attribute, FAttributeModifier& Mod);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateAttributeBaseValue(FName Attribute, FAttributeModifier& Mod);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateAttributeMaxValue(FName Attribute, FAttributeModifier& Mod, float Limit);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateAttributeCurrentRegenValue(FName Attribute, FAttributeModifier& Mod);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateAttributeBaseRegenValue(FName Attribute, FAttributeModifier& Mod);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateAttributeMaxRegenValue(FName Attribute, FAttributeModifier& Mod, float Limit);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateAttributeCurrentDepletationValue(FName Attribute, FAttributeModifier& Mod);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateAttributeBaseDepletationValue(FName Attribute, FAttributeModifier& Mod);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateAttributeMaxDepletationValue(FName Attribute, FAttributeModifier& Mod, float Limit);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void DecreaseAttributeOvertime(float Value, FName Attribute, float TickRate, FName SourceName);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void IncreaseAttributeOvertime(float Value, FName Attribute, float TickRate, FName SourceName);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void StopDecreasingAttribute(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void StopIncreasingAttribute(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetAttributeCurrentValueAsInt(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetAttributeBaseValueAsInt(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetAttributeMaxValueAsInt(FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void DecreaseAttribute(float Value, FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void IncreaseAttribute(float Value, FName Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void StartAutoDepletation();

	protected:

	FTimerHandle MasterTimerHandle;

	FTimerHandle AttributeIncreaseTimerHandle;
	FTimerHandle AttributeDecreaseTimerHandle;

	TMap<FName, float> ActiveDecreasings;
	TMap<FName, float> ActiveIncreasings;

	FAttributeData* FindAttribute(FName Attribute);

	void ProcessAttributeTicks();

	const float MasterTickInterval = 0.1f;

};
