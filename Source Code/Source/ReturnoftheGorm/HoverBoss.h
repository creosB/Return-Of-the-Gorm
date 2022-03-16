// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContraCharacter.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Pawn.h"
#include "HoverBoss.generated.h"

class AKillEmAllGameMode;

UCLASS()
class RETURNOFTHEGORM_API AHoverBoss : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHoverBoss();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

private:
	UPROPERTY(EditAnywhere, Category= "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category= "Components")
	TSubclassOf<AActor> Hover;
	UPROPERTY(EditAnywhere, Category= "Components")
	TSubclassOf<AActor> Bomb;
	UPROPERTY(EditAnywhere, Category= "Components")
	USceneComponent* HoverSpawnPoint;
	UPROPERTY(EditAnywhere, Category= "Components")
	USceneComponent* BombSpawnPoint;

	void HoverMovement(float DeltaTime);
	void BombSpawn();
	void HoverSpawn();
	void SetAllTimer();
	void BacktoNormal();
	void UpdateTargetLocation();

	UPROPERTY(EditAnywhere, Category= "Components")
	ATriggerVolume* Trigger = nullptr;
	// Timer
	FTimerHandle UpdateTimer;
	FTimerHandle BombTimer;
	FTimerHandle HoverTimer;

	bool IsTimerSet = true;

	// Movement
	FVector Forward;
	FVector MovementInitial, MovementCurrent;
	UPROPERTY(EditAnywhere, Category= "Spawner Settings")
	FVector MovementTarget;

	UPROPERTY(EditAnywhere, Category= "Components")
	float MovementSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category= "Spawner Settings")
	float HoverHeight = 800.0f;
	UPROPERTY(EditAnywhere, Category= "Spawner Settings")
	float BombRepeatTime = 5.0f;
	UPROPERTY(EditAnywhere, Category= "Spawner Settings")
	float HoverRepeatTimer = 7.0f;

	AContraCharacter* Player;

	//Health
	bool IsDead() const;
	float Health;
	UPROPERTY(EditAnywhere, Category= "Components")
	float MaxHealth = 100.0f;

	// SLOW MOTION
	bool SlowLogic = false;
	FTimerHandle BacktoNormalTimer;
	float TimerValue = 0.01;
	bool ReturnGameModeDead = true;

	// Effect
	UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DeathParticle = nullptr;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
