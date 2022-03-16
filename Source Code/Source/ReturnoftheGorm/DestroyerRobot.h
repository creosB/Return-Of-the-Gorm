// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FireObstacle.h"
#include "ProjectileBase.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Pawn.h"
#include "Components/PoseableMeshComponent.h"
#include "DestroyerRobot.generated.h"

class AContraCharacter;
class UActorMovement;

UCLASS()
class RETURNOFTHEGORM_API ADestroyerRobot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADestroyerRobot();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category= "General Settings")
	UPoseableMeshComponent* DestroyerRobot;
	UPROPERTY(EditAnywhere, Category= "General Settings")
	UStaticMeshComponent* LaserCube;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SpeedSpawnPoint1 = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SpeedSpawnPoint2 = nullptr;
	UPROPERTY(EditAnywhere, Category= "General Settings")
	ATriggerVolume* Trigger;
	UPROPERTY(EditAnywhere,Category= "Components")
	UNiagaraComponent* FlameEffect;

	AContraCharacter* ContraCharacter;

	void RotateRobot() const;
	void SpeedWeapon();
	void BackToNormal();
	void LaserTrace() const;
	bool IsDead() const;
	void CheckFireCondition();
	float ReturnDistanceToPlayer() const;
	void EffectHit() const;
	void GetLocation();

	bool CanRotate = true;
	FVector ActorLocation;

	FTimerHandle LocationTimer;

	// Health System
	float Health;
	UPROPERTY(EditAnywhere, Category= "General Settings")
	float MaxHealth;

	// SLOW MOTION
	bool SlowLogic = false;
	FTimerHandle BacktoNormalTimer;
	float TimerValue = 0.01;
	bool ReturnGameModeDead = true;

	// Dead effect
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle = nullptr;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound;

	// Weapon
	FTimerHandle FireRateTimerHandle;
	UPROPERTY(EditAnywhere, category = "Combat")
	TSubclassOf<AProjectileBase> ProjectTileClass;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRateTank = 2.f;
	UPROPERTY(EditAnywhere, category = "Combat")
	float RotateSpeed = 10.0f;
	float NextShotReadyAt = 0.f;
	UPROPERTY(EditAnywhere, category = "Combat")
	float FireRange = 2000;
	UPROPERTY(EditAnywhere, Category= "Combat")
	float FireRate = 2.0f;
	UPROPERTY(EditAnywhere, Category= "Combat")
	float SecondWeaponRange = 600;

	// Trigger
	bool TriggerLogic = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;
};
