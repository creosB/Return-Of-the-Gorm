// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shotgun.generated.h"

class AProjectileBase;

UCLASS()
class RETURNOFTHEGORM_API AShotgun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShotgun();

	void PullTrigger();

	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int DefaultAmmo;
	int Ammo;
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float FireRate = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float FireRepeat = 1.0f;

private:
	// Gun
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* ShotgunWeaponMesh;
	// Projectile Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectTileClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint2 = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint3 = nullptr;
	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	USoundBase* MuzzleSound;


	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float MaxRange = 1000;
	UPROPERTY(EditAnywhere)
	float Damage = 10;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
