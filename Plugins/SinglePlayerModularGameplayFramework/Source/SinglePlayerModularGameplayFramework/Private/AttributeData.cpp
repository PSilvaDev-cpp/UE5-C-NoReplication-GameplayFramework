// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeData.h"

float FAttributeData::GetAttributePropertyBaseValue(EAttributePropertyName APN, EAttributePropertyType APT) const
{
	switch (APN)
	{
	case EAttributePropertyName::Default:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			return CurrentValue;
			break;
		case EAttributePropertyType::Base:
			return BaseValue;
			break;
		case EAttributePropertyType::Max:
			return MaxValue;
			break;
		default:
			return 0.f;
			break;
		}
		break;
	case EAttributePropertyName::Regen:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			return CurrentRegenValue;
			break;
		case EAttributePropertyType::Base:
			return BaseRegenValue;
			break;
		case EAttributePropertyType::Max:
			return MaxRegenValue;
			break;
		default:
			return 0.f;
			break;
		}
		break;
	case EAttributePropertyName::Deplet:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			return CurrentDepletationValue;
			break;
		case EAttributePropertyType::Base:
			return BaseDepletationValue;
			break;
		case EAttributePropertyType::Max:
			return MaxDepletationValue;
			break;
		default:
			return 0.f;
			break;
		}
		break;
	default:
		return 0.f;
		break;
	}
}

float FAttributeData::GetAttributePropertyComputedValue(EAttributePropertyName APN, EAttributePropertyType APT) const
{
	switch (APN)
	{
	case EAttributePropertyName::Default:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			return ComputedCurrentValue;
		case EAttributePropertyType::Base:
			return ComputedBaseValue;
		case EAttributePropertyType::Max:
			return ComputedMaxValue;
		default:
			return 0.f;
		}
	case EAttributePropertyName::Regen:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			return ComputedCurrentRegenValue;
		case EAttributePropertyType::Base:
			return ComputedBaseRegenValue;
		case EAttributePropertyType::Max:
			return ComputedMaxRegenValue;
		default:
			return 0.f;
		}
	case EAttributePropertyName::Deplet:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			return ComputedCurrentDepletationValue;
		case EAttributePropertyType::Base:
			return ComputedBaseDepletationValue;
		case EAttributePropertyType::Max:
			return ComputedMaxDepletationValue;
		default:
			return 0.f;
		}
	default:
		return 0.f;
	}
}

void FAttributeData::SetComputedValue(EAttributePropertyName APN, EAttributePropertyType APT, float Value)
{
	float MaxBase = GetAttributePropertyBaseValue(APN, EAttributePropertyType::Max);
	switch (APN)
	{
	case EAttributePropertyName::Default:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			ComputedCurrentValue = FMath::Max(0.f, Value);
			break;
		case EAttributePropertyType::Base:
			ComputedBaseValue = FMath::Max(0.f, Value);
			break;
		case EAttributePropertyType::Max:
			ComputedMaxValue = FMath::Max(0.f, Value);
			break;
		default:
			break;
		}
		break;
	case EAttributePropertyName::Regen:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			ComputedCurrentRegenValue = FMath::Clamp(Value, 0.f, ComputedMaxValue > 0.f ? ComputedMaxValue : MaxBase);
			break;
		case EAttributePropertyType::Base:
			ComputedBaseRegenValue = FMath::Clamp(Value, 0.f, ComputedMaxValue > 0.f ? ComputedMaxValue : MaxBase);
			break;
		case EAttributePropertyType::Max:
			ComputedMaxRegenValue = FMath::Clamp(Value, 0.f, ComputedMaxValue > 0.f ? ComputedMaxValue : MaxBase);
			break;
		default:
			break;
		}
		break;
	case EAttributePropertyName::Deplet:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			ComputedCurrentDepletationValue = FMath::Clamp(Value, 0.f, ComputedMaxValue > 0.f ? ComputedMaxValue : MaxBase);
			break;
		case EAttributePropertyType::Base:
			ComputedBaseDepletationValue = FMath::Clamp(Value, 0.f, ComputedMaxValue > 0.f ? ComputedMaxValue : MaxBase);
			break;
		case EAttributePropertyType::Max:
			ComputedMaxDepletationValue = FMath::Clamp(Value, 0.f, ComputedMaxValue > 0.f ? ComputedMaxValue : MaxBase);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void FAttributeData::Decrease(float Value)
{
	UpdateAttributePropertyValue(-Value, EAttributePropertyName::Default, EAttributePropertyType::Current, false);
}

void FAttributeData::Increase(float Value)
{
	UpdateAttributePropertyValue(Value, EAttributePropertyName::Default, EAttributePropertyType::Current, false);
}

void FAttributeData::AddModifier(FAttributeTempModifier& Modifier)
{
	Modifiers.Add(Modifier);
	RecalculateModifiedProperties();
}

void FAttributeData::RemoveModifier(FName SourceName)
{
    Modifiers.RemoveAll([SourceName](const FAttributeTempModifier& Mod)
    {
        return Mod.SourceName == SourceName;
    });

	RecalculateModifiedProperties();

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

void FAttributeData::UpdateAttributePropertyValue(float Value, EAttributePropertyName APN, EAttributePropertyType APT, bool bOverride)
{
	switch (APN)
	{
	case EAttributePropertyName::Default:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			if (bOverride) { CurrentValue = Value; break; }
			CurrentValue = FMath::Clamp(CurrentValue + Value, 0.f, MaxValue);
			break;
		case EAttributePropertyType::Base:
			if (bOverride) { BaseValue = Value; break; }
			BaseValue = FMath::Clamp(BaseValue + Value, 0.f, MaxValue);
			break;
		case EAttributePropertyType::Max:
			if (bOverride) { MaxValue = Value; break; }
			MaxValue = FMath::Max(0.f, MaxValue + Value);
			break;
		default:
			break;
		}
		break;
	case EAttributePropertyName::Regen:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			if (bOverride) { CurrentRegenValue = Value; break; }
			CurrentRegenValue = FMath::Clamp(CurrentRegenValue + Value, 0.f, MaxRegenValue);
			break;
		case EAttributePropertyType::Base:
			if (bOverride) { BaseRegenValue = Value; break; }
			BaseRegenValue = FMath::Clamp(BaseRegenValue + Value, 0.f, MaxRegenValue);
			break;
		case EAttributePropertyType::Max:
			if (bOverride) { MaxRegenValue = Value; break; }
			MaxRegenValue = FMath::Max(0.f, MaxRegenValue + Value);
			break;
		default:
			break;
		}
		break;
	case EAttributePropertyName::Deplet:
		switch (APT)
		{
		case EAttributePropertyType::Current:
			if (bOverride) { CurrentDepletationValue = Value; break; }
			CurrentDepletationValue = FMath::Clamp(CurrentDepletationValue + Value, 0.f, MaxDepletationValue);
			break;
		case EAttributePropertyType::Base:
			if (bOverride) { BaseDepletationValue = Value; break; }
			BaseDepletationValue = FMath::Clamp(BaseDepletationValue + Value, 0.f, MaxDepletationValue);
			break;
		case EAttributePropertyType::Max:
			if (bOverride) { MaxDepletationValue = Value; break; }
			MaxDepletationValue = FMath::Max(0.f, MaxDepletationValue + Value);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	RecalculateModifiedProperties();
}

void FAttributeData::RecalculateModifiedProperties()
{
	const TArray<EAttributePropertyName> PropertyNames =
	{
		EAttributePropertyName::Default,
		EAttributePropertyName::Regen,
		EAttributePropertyName::Deplet
	};

	const TArray<EAttributePropertyType> PropertyTypes =
	{
		EAttributePropertyType::Max,
		EAttributePropertyType::Base,
		EAttributePropertyType::Current
	};

	for (EAttributePropertyName APN : PropertyNames)
	{
		for (EAttributePropertyType APT : PropertyTypes)
		{
			float NewBaseValue = GetAttributePropertyBaseValue(APN, APT);
			float ComputedValue = CalculateModifiedValue(APN, APT, NewBaseValue);
			SetComputedValue(APN, APT, ComputedValue);
		}
	}
}

float FAttributeData::CalculateModifiedValue(EAttributePropertyName APN, EAttributePropertyType APT, float NewBaseValue) const
{
	float AdditiveSum = 0.f;
	float MultiplierSum = 0.f;
	float OverrideValue = NewBaseValue;
	bool bHasOverride = false;

	for (const FAttributeTempModifier& Mod : Modifiers)
	{
		if (Mod.PropertyName == APN && Mod.PropertyType == APT)
		{
			float OpValue = Mod.bNegative ? -Mod.OpValue : Mod.OpValue;

			switch (Mod.Operation)
			{
			case EModifierOp::Add:
				AdditiveSum += OpValue;
				break;
			case EModifierOp::Multiply:
				MultiplierSum += OpValue;
				break;
			case EModifierOp::Override:
				OverrideValue = OpValue;
				bHasOverride = true;
				break;
			}
		}
	}

	float Result = bHasOverride ? OverrideValue : NewBaseValue;
	float FinalMultiplier = FMath::Max(0.f, 1.f + MultiplierSum);
	Result = (Result + AdditiveSum) * FinalMultiplier;

	return Result;
}