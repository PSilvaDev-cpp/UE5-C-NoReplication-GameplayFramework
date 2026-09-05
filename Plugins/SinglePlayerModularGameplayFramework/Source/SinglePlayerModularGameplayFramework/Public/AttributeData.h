// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AttributeData.generated.h"

/**
 * 
 */

UENUM(BlueprintType, Blueprintable)
enum class EModifierOp : uint8
{
	Add UMETA(DisplayName = "Add"),
	Multiply UMETA(DisplayName = "Multiply"),
	Override UMETA(DisplayName = "Override")
};

UENUM(BlueprintType, Blueprintable)
enum class EAttributePropertyName : uint8
{
	Default UMETA(DisplayName = "Default"),
	Regen UMETA(DisplayName = "Regen"),
	Deplet UMETA(DisplayName = "Deplet")
};

UENUM(BlueprintType, Blueprintable)
enum class EAttributePropertyType : uint8
{
	Current UMETA(DisplayName = "Current"),
	Base UMETA(DisplayName = "Base"),
	Max UMETA(DisplayName = "Max")
};

USTRUCT(BlueprintType, Blueprintable)
struct FAttributeTempModifier {
	GENERATED_BODY()

	UPROPERTY()
	FName SourceName; // Quem causou isso? (ex: "PoçãoDeForça")

	UPROPERTY()
	EModifierOp Operation = EModifierOp::Add;

	UPROPERTY()
	EAttributePropertyName PropertyName = EAttributePropertyName::Default;

	UPROPERTY()
	EAttributePropertyType PropertyType = EAttributePropertyType::Current;

	UPROPERTY()
	bool bNegative;

	UPROPERTY()
	float OpValue = 0.f;

	UPROPERTY()
	float LimitValue;

	UPROPERTY()
	float BeforeOpValue; //Before Operation Value

	//UPROPERTY()
	//float Duration = -1.f; // -1 = Permanente
};

UENUM(BlueprintType, Blueprintable)
enum class EAttributeType : uint8
{
	FloatAttribute UMETA(DisplayName = "FloatAttribute"),
	StationaryAttribute UMETA(DisplayName = "StationaryAttribute")
};

USTRUCT(BlueprintType, Blueprintable)
struct FAttributeData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FAttributeTempModifier> Modifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FName AttributeName = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float CurrentValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	EAttributeType AttributeType;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float DepleteTickAccumulator = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float RegenTickAccumulator = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float DesiredTickRate = 1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float CurrentRegenValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseRegenValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxRegenValue;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float CurrentDepletationValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseDepletationValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxDepletationValue;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float ComputedCurrentValue;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float ComputedMaxValue;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float ComputedBaseValue;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float ComputedCurrentRegenValue;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float ComputedMaxRegenValue;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float ComputedBaseRegenValue;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float ComputedCurrentDepletationValue;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float ComputedMaxDepletationValue;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float ComputedBaseDepletationValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float RegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float DepleteRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	bool bAutoRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	bool bAutoDeplete;

	float GetAttributePropertyBaseValue(EAttributePropertyName APN, EAttributePropertyType APT) const;
	float GetAttributePropertyComputedValue(EAttributePropertyName APN, EAttributePropertyType APT) const;

	void UpdateAttributePropertyValue(float Value, EAttributePropertyName APN, EAttributePropertyType APT, bool bOverride );
	void Decrease(float Value);
	void Increase(float Value);

	void AddModifier(FAttributeTempModifier& Modifier);
	void RemoveModifier(FName SourceName);
	void RecalculateModifiedProperties();

private:
	float CalculateModifiedValue(EAttributePropertyName APN, EAttributePropertyType APT, float BaseValue) const;
	void SetComputedValue(EAttributePropertyName APN, EAttributePropertyType APT, float Value);
};
