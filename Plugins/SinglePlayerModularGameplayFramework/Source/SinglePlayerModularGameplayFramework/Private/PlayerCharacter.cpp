// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "MainPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

APlayerCharacter::APlayerCharacter()
{

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AMainPlayerController* MPC = Cast<AMainPlayerController>(GetController()))
	{
		PC_Ref = MPC;
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Move)
		{
			EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}
		if (IA_Look)
		{
			EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}
		if (IA_Interact)
		{
			EnhancedInput->BindAction(IA_Interact, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
		}
		if (IA_Attack)
		{
			//EnhancedInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &APlayerCharacter::Attack);
		}
		if (IA_OpenCloseInventory)
		{
			//EnhancedInput->BindAction(IA_OpenCloseInventory, ETriggerEvent::Started, this, &APlayerCharacter::OpenCloseInventory);
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Interact()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (GetActorForwardVector() * 500);
	float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FRotator SweepRot = FRotator(90.f, GetActorRotation().Yaw, 0.f);
	FQuat Rotation = SweepRot.Quaternion();
	float Radius = 25.f;
	FCollisionShape Shape = FCollisionShape::MakeCapsule(Radius, HalfHeight);
	FCollisionQueryParams QueryParams;

	FHitResult HitResult;

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity, // No rotation needed for a uniform sphere
		ECC_Visibility,  // The trace channel
		Shape,
		QueryParams
	);

	if (bHit && HitResult.GetActor())
	{  /*
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			IInteractInterface::Execute_Interact(HitResult.GetActor(), this);
		}
	   */
	}

	FColor LineColor = bHit ? FColor::Green : FColor::Red;

	// Draw the starting sphere boundary
	DrawDebugSphere(GetWorld(), StartLocation, Radius, 12, LineColor, false, 2.0f, 0, 1.0f);

	if (bHit)
	{
		// Draw the trace line up to the capsule impact position
		DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, LineColor, false, 2.0f, 0, 1.0f);
		// Draw the sphere at its exact final resting point upon collision
		DrawDebugSphere(GetWorld(), HitResult.Location, Radius, 12, FColor::Magenta, false, 2.0f, 0, 1.0f);
		// Draw a small point indicating the exact impact impact surface location
		DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Cyan, false, 2.0f);
	}
	else
	{
		// Draw the full trace line to the end position
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, 2.0f, 0, 1.0f);
		// Draw the destination sphere boundary
		DrawDebugSphere(GetWorld(), EndLocation, Radius, 12, LineColor, false, 2.0f, 0, 1.0f);
	}
}