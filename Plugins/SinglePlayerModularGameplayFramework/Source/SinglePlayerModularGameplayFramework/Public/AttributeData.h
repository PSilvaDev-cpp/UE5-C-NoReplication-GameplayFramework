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
enum class EModType1 : uint8
{
	Default UMETA(DisplayName = "Default"),
	Regen UMETA(DisplayName = "Regen"),
	Deplet UMETA(DisplayName = "Deplet")
};

UENUM(BlueprintType, Blueprintable)
enum class EModType2 : uint8
{
	Current UMETA(DisplayName = "Current"),
	Base UMETA(DisplayName = "Base"),
	Max UMETA(DisplayName = "Max")
};

USTRUCT(BlueprintType, Blueprintable)
struct FAttributeModifier {
	GENERATED_BODY()

	UPROPERTY()
	FName SourceName; // Quem causou isso? (ex: "PoçãoDeForça")

	UPROPERTY()
	EModifierOp Operation = EModifierOp::Add;

	UPROPERTY()
	EModType1 ModType1 = EModType1::Default;

	UPROPERTY()
	EModType2 ModType2 = EModType2::Current;

	UPROPERTY()
	float Value = 0.f;

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
	TArray<FAttributeModifier> Modifiers;

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
	float TickAccumulator = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float DesiredTickRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseRegenValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxRegenValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float CurrentRegenValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float CurrentDepletationValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseDepletationValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxDepletationValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float RegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float DepleteRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	bool bAutoRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	bool bAutoDeplete;

	float GetCurrentValue() const;
	float GetBaseValue() const;
	float GetMaxValue() const;
	float GetCurrentRegenValue() const;
	float GetBaseRegenValue() const;
	float GetMaxRegenValue() const;
	float GetCurrentDepletationValue() const;
	float GetBaseDepletationValue() const;
	float GetMaxDepletationValue() const;

	void UpdateCurrentValue(float Value );
	void UpdateBaseValue(float Value );
	void UpdateMaxValue(float Value, float Limit);
	void UpdateCurrentRegenValue(float Value );
	void UpdateBaseRegenValue(float Value );
	void UpdateMaxRegenValue(float Value, float Limit);
	void UpdateCurrentDepletationValue(float Value );
	void UpdateBaseDepletationValue(float Value );
	void UpdateMaxDepletationValue(float Value, float Limit);
	void Decrease(float Value);
	void Increase(float Value);

	void AddModifier(const FAttributeModifier& Modifier);
	void RemoveModifier(FName SourceName);
//	void UpdateModifiers(float DeltaTime);
	float CalculateFinalValue(float Value);

	void RecalculateWithModifiers(float& TargetProperty);


};
