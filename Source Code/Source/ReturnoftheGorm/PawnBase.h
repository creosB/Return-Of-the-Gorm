// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

class UCapsuleComponent;
class AProjectileBase;
class AKillEmAllGameMode;

UCLASS()
class RETURNOFTHEGORM_API APawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	virtual void HandleDestruction();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UPROPERTY(EditAnywhere)
	bool DestroyWAttachment = false;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	float Health;

private:
	virtual void BacktoNormal();

	TArray<AActor*> Attached;
	bool ReturnGameModeDead = true;

	// SLOW MOTION
	bool SlowLogic = false;
	FTimerHandle BacktoNormalTimer;
	float TimerValue = 0.01;

	float NextShotReadyAt = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;
	UPROPERTY(EditDefaultsOnly)
	float RotateSpeed = 10.0f;
	// objenin collison'u için
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent = nullptr;

	// Static meshleri belirlemek için
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh = nullptr;

	// SpawnPoint oluşturmak için
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SlowSpawnPoint = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SpeedSpawnPoint = nullptr;

	// VARIABLES
	UPROPERTY(EditAnywhere, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectTileClass;

	// effect seçmek için
	UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DeathParticle = nullptr;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Turret hedefi gördüğünde ona dönmesi için fonksiyon
	void RotateTurret(FVector LookAtTarget);

	UPROPERTY(EditAnywhere, category = "Combat")
	float FireRateTank = 2.f;

	// Weapon type
	void SlowWeapon();
	void NormalWeapon();
	void SpeedWeapon();
};
