// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "Components/SkeletalMeshComponent.h"
#include "ReturnoftheGorm/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShotgun::AShotgun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ShotgunWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	ShotgunWeaponMesh->SetupAttachment(Root);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(ShotgunWeaponMesh);

	ProjectileSpawnPoint2 = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint2"));
	ProjectileSpawnPoint2->SetupAttachment(ShotgunWeaponMesh);

	ProjectileSpawnPoint3 = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint3"));
	ProjectileSpawnPoint3->SetupAttachment(ShotgunWeaponMesh);
}

// Called when the game starts or when spawned
void AShotgun::BeginPlay()
{
	Super::BeginPlay();
}


void AShotgun::PullTrigger()
{
	// Effect Spawn
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, ShotgunWeaponMesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, ShotgunWeaponMesh, TEXT("MuzzleFlashSocket"));

	// Getting location and rotation informations.
	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FVector SpawnLocation2 = ProjectileSpawnPoint2->GetComponentLocation();
	FVector SpawnLocation3 = ProjectileSpawnPoint3->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	if (ProjectTileClass)
	{
		// Spawning 3 projectile different position.
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectTileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
		AProjectileBase* TempProjectile2 = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectTileClass, SpawnLocation2, SpawnRotation);
		TempProjectile2->SetOwner(this);
		AProjectileBase* TempProjectile3 = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectTileClass, SpawnLocation3, SpawnRotation);
		TempProjectile3->SetOwner(this);
	}
}
