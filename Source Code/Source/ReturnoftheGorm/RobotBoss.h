// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/TriggerVolume.h"
#include "RobotBoss.generated.h"

class UCapsuleComponent;
class AProjectileBase;
class AKillEmAllGameMode;
class AContraCharacter;

UCLASS()
class RETURNOFTHEGORM_API ARobotBoss : public APawn
{
	GENERATED_BODY()


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this pawn's properties
	ARobotBoss();

	// taking damage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
						AActor* DamageCauser) override;

	// Check Dead
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

private:
	// Slow Motion function
	virtual void BackToNormal();
	// Destroy
	void HandleDestruction();
	// Target System
	void CheckFireCondition();
	float ReturnDistanceToPlayer() const;
	void RotateTurret(FVector LookAtTarget) const;
	// Weapon type
	void ElectricGun();
	// Material
	void ChangeMaterial();
	void NormalMaterial();
	void DissolveMaterial();

	// Game Mode Logic
	bool ReturnGameModeDead = true;

	// SLOW MOTION
	bool SlowLogic = false;
	FTimerHandle BacktoNormalTimer;
	float TimerValue = 0.01;
	// Gun Settings
	float NextShotReadyAt = 0.f;
	UPROPERTY(EditAnywhere, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.0f;
	UPROPERTY(EditAnywhere, Category= "Gun Settings")
	float FireRange = 3000;
	// Timer
	FTimerHandle FireRateTimerHandle;

	// Health System
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;
	float Health;
	// Rotate Settings
	UPROPERTY(EditAnywhere, Category= "Gun Settings")
	float RotateSpeed = 10.0f;

	// Object collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent = nullptr;

	// Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RobotMesh = nullptr;

	// Creating spawn point for electric gun
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ElectricGunSpawnPoint = nullptr;

	// Gun Type
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectTileClass;

	// Dead effect
	UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DeathParticle = nullptr;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound;

	// Trigger Area
	UPROPERTY(EditAnywhere)
	ATriggerVolume* Trigger = nullptr; // Trigger Area
	bool TriggerLogic = true; // check conduction

	// Cast Player
	AContraCharacter* PlayerPawn = nullptr;

	// Material Settings
	bool CanChangeMaterial = true; // material change logic
	float NormalMaterialLast; // last change time
	float DissolveMaterialLast;
	UPROPERTY(EditAnywhere)
	class UMaterial* Metal; // default material
	UPROPERTY(EditAnywhere)
	class UMaterialInstance* MetalInstance; // customized material
	UPROPERTY(EditAnywhere)
	class UMaterial* Body; // default material
	UPROPERTY(EditAnywhere)
	class UMaterialInstance* BodyInstance; // customized material

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category= "Gun Settings")
	float FireRateTank = 2.f;
};
