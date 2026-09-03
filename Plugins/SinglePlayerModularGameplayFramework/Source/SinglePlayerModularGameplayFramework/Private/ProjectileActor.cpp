// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileActor.h"
#include "GameFramework/ProjectileMovementComponent.h" 

// Sets default values
AProjectileActor::AProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // (Opcional) Geralmente projéteis têm uma colisão raiz
     CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
     RootComponent = CollisionComponent;

    // 1. Cria o componente na memória
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

    // 2. Garante que o movimento atualize a raiz do ator (a colisão)
    // ProjectileMovement->SetUpdatedComponent(CollisionComp);

    // 3. Configurações de Física e Velocidade
    ProjectileMovementComponent->InitialSpeed = 2000.f;
    ProjectileMovementComponent->MaxSpeed = 3000.f;

    // O projétil gira automaticamente para a direção em que está voando
    ProjectileMovementComponent->bRotationFollowsVelocity = true;

    // O projétil sofre gravidade? (0.f para atirar reto como um laser)
    ProjectileMovementComponent->ProjectileGravityScale = 1.0f;

    // Ative se quiser que a magia/flecha quique no chão
    ProjectileMovementComponent->bShouldBounce = false;

}

// Called when the game starts or when spawned
void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

