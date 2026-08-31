// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeData.h"

float FAttributeData::GetCurrentValue() const
{
	return CurrentValue;
}

float FAttributeData::GetBaseValue()const
{
	return BaseValue;
}

float FAttributeData::GetMaxValue()const
{
	return MaxValue;
}

float FAttributeData::GetCurrentRegenValue()const
{
	return CurrentRegenValue;
}

float FAttributeData::GetBaseRegenValue()const
{
	return BaseRegenValue;
}

float FAttributeData::GetMaxRegenValue()const
{
	return MaxRegenValue;
}

float FAttributeData::GetCurrentDepletationValue()const
{
	return CurrentDepletationValue;
}

float FAttributeData::GetBaseDepletationValue()const
{
	return BaseDepletationValue;
}

float FAttributeData::GetMaxDepletationValue()const
{
	return MaxDepletationValue;
}

void FAttributeData::UpdateCurrentValue(float Value)
{
	CurrentValue = FMath::Clamp(CurrentValue + Value, 0.f, MaxValue);
	//CurrentValue = FMath::Clamp(CalculateFinalValue(CurrentValue), 0.f, MaxValue);
}

void FAttributeData::UpdateBaseValue(float Value)
{
	BaseValue = FMath::Clamp(BaseValue + Value, 0.f, MaxValue);
	//BaseValue = FMath::Clamp(CalculateFinalValue(BaseValue), 0.f, MaxValue);
}

void FAttributeData::UpdateMaxValue(float Value, float Limit)
{
	MaxValue = FMath::Clamp(MaxValue + Value, 0.f, Limit);
	//MaxValue = FMath::Clamp(CalculateFinalValue(MaxValue), 0.f, Limit);
}

void FAttributeData::UpdateCurrentRegenValue(float Value)
{
	CurrentRegenValue = FMath::Clamp(CurrentRegenValue + Value, 0.f, MaxValue);
	//CurrentRegenValue = FMath::Clamp(CalculateFinalValue(CurrentRegenValue), 0.f, MaxValue);
}

void FAttributeData::UpdateBaseRegenValue(float Value)
{
	BaseRegenValue = FMath::Clamp(BaseRegenValue + Value, 0.f, MaxValue);
	//BaseRegenValue = FMath::Clamp(CalculateFinalValue(BaseRegenValue), 0.f, MaxValue);
}

void FAttributeData::UpdateMaxRegenValue(float Value, float Limit)
{
	MaxRegenValue = FMath::Clamp(MaxRegenValue + Value, 0.f, Limit);
	//MaxRegenValue = FMath::Clamp(CalculateFinalValue(MaxRegenValue), 0.f, MaxValue);
}

void FAttributeData::UpdateCurrentDepletationValue(float Value)
{
	CurrentDepletationValue = FMath::Clamp(CurrentDepletationValue + Value, 0.f, MaxValue);
	//CurrentDepletationValue = FMath::Clamp(CalculateFinalValue(CurrentDepletationValue), 0.f, MaxValue);
}

void FAttributeData::UpdateBaseDepletationValue(float Value)
{
	BaseDepletationValue = FMath::Clamp(BaseDepletationValue + Value, 0.f, MaxValue);
	//BaseDepletationValue = FMath::Clamp(CalculateFinalValue(BaseDepletationValue), 0.f, MaxValue);
}

void FAttributeData::UpdateMaxDepletationValue(float Value, float Limit)
{
	MaxDepletationValue = FMath::Clamp(MaxDepletationValue + Value, 0.f, MaxValue);
	//MaxDepletationValue = FMath::Clamp(CalculateFinalValue(MaxDepletationValue), 0.f, MaxValue);
}

void FAttributeData::Decrease(float Value)
{
	UpdateCurrentValue(-Value);
}

void FAttributeData::Increase(float Value)
{
	UpdateCurrentValue(Value);
}

void FAttributeData::AddModifier(const FAttributeModifier& Modifier)
{
	Modifiers.Add(Modifier);

}

void FAttributeData::RemoveModifier(FName SourceName)
{
	Modifiers.RemoveAll([SourceName](const FAttributeModifier& Mod)
		{
			return Mod.SourceName == SourceName;
		});	
}

/* 
void FAttributeData::UpdateModifiers(float DeltaTime)
{
	for (int32 i = Modifiers.Num() - 1; i >= 0; --i)
	{
		FAttributeModifier& Mod = Modifiers[1];
		if (Mod.Duration > 0.f)
		{
			Mod.RemainingTime -= DeltaTime;
		}
	}
}
*/

float FAttributeData::CalculateFinalValue(float Value)
{
	float AdditiveSum = 0.f;
	float MultiplicativeProduct = 1.0f;
	float OverrideValue = Value;
	bool bHasOverride = false;

	for (const FAttributeModifier& Mod : Modifiers)
	{
		switch (Mod.Operation)
		{
		case EModifierOp::Add:
			AdditiveSum += Mod.Value;
			break;
		case EModifierOp::Multiply:
			MultiplicativeProduct *= (1.0f + Mod.Value);
			break;
		case EModifierOp::Override:
			if (!bHasOverride)
			{
				OverrideValue = Mod.Value;
				bHasOverride = true;
			}
			break;
		}
	}
	float Result = bHasOverride ? OverrideValue : Value;
	Result = (Result + AdditiveSum) * MultiplicativeProduct;
	return Result;
}

void FAttributeData::RecalculateWithModifiers(float& TargetProperty)
{
	TargetProperty = CalculateFinalValue(TargetProperty);
}