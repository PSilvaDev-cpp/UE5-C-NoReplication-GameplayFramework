// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeInterface.h"
#include "AttributeComponent.h"

bool IAttributeInterface::CheckAttribute_Implementation(FName AttributeName)
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

float IAttributeInterface::GetAttributePropertyValue_Implementation(FName AttributeName, EAttributePropertyName APN, EAttributePropertyType APT)
{
	AActor* Owner = GetOwnerActor();
	if (Owner != nullptr)
	{

		if (UAttributeComponent* AttComp = Owner->FindComponentByClass<UAttributeComponent>())
		{
			return AttComp->GetAttributePropertyComputedValue(AttributeName, APN, APT);
		}
		return -1.f;
	}
	return -1.f;
}

void IAttributeInterface::UpdateAttributePropertyValue_Implementation(FName AttributeName, float Value, EAttributePropertyName APN, EAttributePropertyType APT, bool bOverride)
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

AActor* IAttributeInterface::GetOwnerActor()
{
	if (AActor* ImplementingObject = Cast<AActor>(this))
	{
		return ImplementingObject;
	}
	return nullptr;
}
