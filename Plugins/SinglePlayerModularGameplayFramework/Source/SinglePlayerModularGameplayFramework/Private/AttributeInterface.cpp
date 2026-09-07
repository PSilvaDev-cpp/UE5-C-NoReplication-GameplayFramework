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
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ICheckAttribute Interrupted, Owner NOT Valid"));
		return false;
	}
	if (UAttributeComponent* AttComp = Owner->FindComponentByClass<UAttributeComponent>())
	{
		if (AttComp->FindAttribute(AttributeName))
		{
			return true;
		}
		UE_LOG(LogTemp, Error, TEXT("IGetAttributePropertyValue Interrupted, Attribute NOT found"));
	}
	return false;
}


float IAttributeInterface::GetAttributePropertyValue(FName AttributeName, EAttributePropertyName APN, EAttributePropertyType APT)
{
	AActor* Owner = GetOwnerActor();
	
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("IGetAttributePropertyValue Interrupted, Owner NOT Valid"));
		return -1.f;
	}
	if (UAttributeComponent* AttComp = Owner->FindComponentByClass<UAttributeComponent>())
	{
		if (float Result = AttComp->GetAttributePropertyComputedValue(AttributeName, APN, APT); Result >= 0)
		{
			return Result;
		}
		UE_LOG(LogTemp, Error, TEXT("IGetAttributePropertyValue Interrupted, Attribute Value <= 0"));
		return -1.f;
	}
	UE_LOG(LogTemp, Error, TEXT("IGetAttributePropertyValue Interrupted, AttributeComponent NOT found"));
	return -1.f;
}