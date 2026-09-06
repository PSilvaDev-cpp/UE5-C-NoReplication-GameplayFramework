// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttributeComponent.h"
#include "AttributeData.h"
#include "AttributeInterface.generated.h"

/**
 * 
 */



UINTERFACE(MinimalAPI, NotBlueprintable)
class UAttributeInterface : public UInterface
{
	GENERATED_BODY()
};

class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API IAttributeInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "AttributeInterface")
	virtual bool CheckAttribute(FName AttributeName);

	UFUNCTION(BlueprintCallable, Category = "AttributeInterface")
	virtual float GetAttributePropertyValue(FName AttributeName, EAttributePropertyName APN, EAttributePropertyType APT);

	UFUNCTION(BlueprintCallable, Category = "AttributeInterface")
	virtual void UpdateAttributePropertyValue(FName AttributeName, float Value, EAttributePropertyName APN, EAttributePropertyType APT, bool bOverride);


	virtual AActor* GetOwnerActor();

};
