// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "InputActionValue.h"
#include "MainPlayerController.h"

#include "PlayerCharacter.generated.h"

/**
 * 
 */

class UInputAction;
class UInputMappingContext;

class AMainPlayerController;


UCLASS()
class SINGLEPLAYERMODULARGAMEPLAYFRAMEWORK_API APlayerCharacter : public AMainCharacter
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditDefaultsOnly, Category = "Input/Actions")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input/Actions")
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input/Actions")
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input/Actions")
	UInputAction* IA_Interact;

	UPROPERTY(EditDefaultsOnly, Category = "Input/Actions")
	UInputAction* IA_Attack;

	UPROPERTY(EditDefaultsOnly, Category = "Input/Actions")
	UInputAction* IA_OpenCloseInventory;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();

};
