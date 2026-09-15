// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"

#include "MainUserWidget.generated.h"

/**
 * 
 */

struct FItemRepresentation;

UCLASS(Blueprintable, BlueprintType)
class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MainWidget")
	class AMainCharacter* OwnerCharacter;

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void AddNewItem(FItemRepresentation Item, int32 Amount);

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void UpdateItemQuantity(int32 ItemIndex, int32 Value);

};
