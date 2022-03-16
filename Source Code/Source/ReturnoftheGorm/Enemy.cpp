// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Gun.h"
#include "ReturnoftheGorm/Boomer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#define OUT

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Eyes = CreateDefaultSubobject<UBoxComponent>("Eyes");
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Eyes->SetWorldLocation(Gun->GetActorLocation() + FVector(0, 100, 0));
	// Getting player information.
	Player = UGameplayStatics::GetPlayerCharacter(this, 0);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EnemySight();
	// setting AI rotation per sec
	if (Player && Health > 0) { SetRotationAI(); }
	if (GetCharacterMovement()->Velocity.Z > 0.0f)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(BossShake, 0.025f);
		if (GetCharacterMovement()->Velocity.Z >= 2230.0f)
		{
			GetCharacterMovement()->DisableMovement();
		}
	}

}

// Calculating AI rotation
void AEnemy::CalculateRotationAI()
{
	// Getting target info in child and calculating rotation value.
	RotationCalculate = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(),
	                                                           Player->GetActorLocation());
	// Getting calculated value and applying 2 between value.
	AIRotation = FMath::RInterpTo(GetOwner()->GetActorRotation(), RotationCalculate,
	                              GetWorld()->GetDeltaSeconds(), AIRotationValue);
}

// Setting AI rotation
void AEnemy::SetRotationAI()
{
	CalculateRotationAI();
	// Setting turret rotation value
	SetActorRotation(AIRotation);
}

bool AEnemy::EnemySight()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());;


	// sending line trace between end-start
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		Eyes->GetComponentLocation(),
		GetActorForwardVector() * 1000 + Eyes->GetComponentLocation(),
		ECC_Pawn,
		TraceParams
	);


	AActor* ActorHit = Hit.GetActor();

	// when hit the pawn and if pawn tag is player, returning true.
	if (ActorHit && ActorHit->ActorHasTag("Player"))
	{
		return true;
	}
	else
	{
		StopShooting();
		return false;
	}
}

void AEnemy::BossAIGrenade()
{
	if (!BossJumpAnim || !Drone) { return; }
	if (!AnimIsPlaying)
	{
		GetMesh()->PlayAnimation(BossJumpAnim, false);
		ABoomer* SpawnedActor = GetWorld()->SpawnActor<ABoomer>(
			Drone, BossDroneSpawnLocation - FVector(0.f, 200.f, 0.f), FRotator(180, 0, 0));
		ABoomer* SpawnedActor2 = GetWorld()->SpawnActor<ABoomer>(
			Drone, BossDroneSpawnLocation - FVector(0.f, 800.f, 0.f), FRotator(180, 0, 0));
	}
	else
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
	AnimIsPlaying = !AnimIsPlaying;
}
