// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class AProjectileBase;

UCLASS()
class RETURNOFTHEGORM_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	USkeletalMeshComponent* NormalWeaponMesh;
	// Projectile Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectTileClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint = nullptr;
	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	USoundBase* MuzzleSound;


	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float MaxRange = 1000;
	UPROPERTY(EditAnywhere)
	float Damage = 10;

};
