// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttributeData.h"
#include "AttributeInterface.generated.h"

/**
 * 
 */



UINTERFACE(MinimalAPI, Blueprintable)
class UAttributeInterface : public UInterface
{
	GENERATED_BODY()
};

class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API IAttributeInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attribute")
	bool CheckAttribute(FName AttributeName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attribute")
	void UpdateAttributePropertyValue(FName AttributeName, float Value, EAttributePropertyName APN, EAttributePropertyType APT, bool bOverride);

};
