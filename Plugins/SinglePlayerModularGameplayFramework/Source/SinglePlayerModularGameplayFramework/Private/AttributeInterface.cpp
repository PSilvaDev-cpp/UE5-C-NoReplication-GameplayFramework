// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeInterface.h"


AActor* IAttributeInterface::GetOwnerActor()
{
	if (AActor* ImplementingObject = Cast<AActor>(this))
	{
		return ImplementingObject;
	}
	return nullptr;
}

void IAttributeInterface::UpdateAttributePropertyValue(FName AttributeName, float Value, EAttributePropertyName APN, EAttributePropertyType APT, bool bOverride)
{
	AActor* Owner = GetOwnerActor();
	if (Owner != nullptr)
	{

		if (UAttributeComponent* AttComp = Owner->FindComponentByClass<UAttributeComponent>())
		{
			AttComp->UpdateAttributePropertyValue(AttributeName, Value, APN, APT, bOverride);
			return;
		}
	}
}

bool IAttributeInterface::CheckAttribute(FName AttributeName)
{
	AActor* Owner = GetOwnerActor();
	if (Owner != nullptr)
	{
		if (UAttributeComponent* AttComp = Owner->FindComponentByClass<UAttributeComponent>())
		{
			if (AttComp->FindAttribute(AttributeName))
			{
				return true;
			}
		}
	}
	return false;
}


float IAttributeInterface::GetAttributePropertyValue(FName AttributeName, EAttributePropertyName APN, EAttributePropertyType APT)
{
	AActor* Owner = GetOwnerActor();
	if (Owner != nullptr)
	{

		if (UAttributeComponent* AttComp = Owner->FindComponentByClass<UAttributeComponent>())
		{
			if (float Result = AttComp->GetAttributePropertyComputedValue(AttributeName, APN, APT) > 0)
			{
				return Result;
			}
			return -1.f;
		}
		return -1.f;
	}
	return -1.f;
}