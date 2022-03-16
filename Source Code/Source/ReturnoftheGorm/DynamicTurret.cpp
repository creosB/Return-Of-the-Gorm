// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicTurret.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ADynamicTurret::ADynamicTurret()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"));
	BoxMesh->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ADynamicTurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		FireRateTimerHandle,
		this,
		&ADynamicTurret::CheckFireCondition,
		FireRate,
		true
	);

	PlayerPawn = Cast<AContraCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void ADynamicTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange) { return; }
	RotateTurret(PlayerPawn->GetActorLocation());
	CollisionLogic();
}

// fire logic
void ADynamicTurret::CheckFireCondition()
{
	// if player == null || is dead
	if (!PlayerPawn || PlayerPawn->IsDead()) { return; }

	// if player is in range then fire
	if (ReturnDistanceToPlayer() <= FireRange && CanShoot)
	{
		SpeedWeapon();
	}
}

// calculate distance player between turret
float ADynamicTurret::ReturnDistanceToPlayer() const
{
	if (!PlayerPawn)
	{
		return 0.0f;
	}
	// it's returning 2 vector between distance.
	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

// when health <= 0
void ADynamicTurret::HandleDestruction()
{
	// pawnbase de bulunan handle destruction fonk. çağırıyor
	Super::HandleDestruction();
	Destroy();
}

// collision conduction
void ADynamicTurret::CollisionLogic()
{
	if (CollisionResult)
	{
		OnCollision();
		OnTimer = GetWorld()->GetTimeSeconds();

		if (GetWorld()->GetTimeSeconds() - OffTimer > CollisionOffDelay)
		{
			CollisionResult = false;
		}
	}
	else
	{
		OffCollision();
		OffTimer = GetWorld()->GetTimeSeconds();

		if (GetWorld()->GetTimeSeconds() - OnTimer > CollisionOnDelay)
		{
			CollisionResult = true;
		}
	}
}

// Collision settings
void ADynamicTurret::OnCollision()
{
	// Collision enabled - turret can shoot - visibility true
	BoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxMesh->SetVisibility(true);
	TurretMesh->SetVisibility(false);
	CanShoot = false;
}

void ADynamicTurret::OffCollision()
{
	// Collision disabled - turret can't shoot - visibility false
	BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxMesh->SetVisibility(false);
	TurretMesh->SetVisibility(true);
	CanShoot = true;
	if (Health < 100)
	{
		Health += 0.01;
	}
}
