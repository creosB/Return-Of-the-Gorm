// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NiagaraSystem.h"
#include "Engine/TriggerVolume.h"
#include "BallBoss.generated.h"

USTRUCT()
struct FShamanEffect
{
	GENERATED_BODY()
public:
	// Niagara Effect
	UPROPERTY(EditDefaultsOnly, Category= "Effect Settings")
	UNiagaraSystem* ShamanEffect = nullptr; // Effect type
	UPROPERTY(EditDefaultsOnly, Category= "Effect Settings")
	FVector EffectTargetLocation; // how much location want to spawn
	FRotator EffectTargetRotation = FRotator(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditDefaultsOnly, Category= "Effect Settings")
	int EffectCount = 9; // Effect number
	UPROPERTY(EditAnywhere, Category= "Effect Settings")
	ATriggerVolume* DamageArea; // Damage trigger area
};

class AContraCharacter;

UCLASS()
class RETURNOFTHEGORM_API ABallBoss : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallBoss();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	USceneComponent* Root;
	// Sphere
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	;
	UStaticMeshComponent* BigSphere;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	;
	UStaticMeshComponent* MiddleSphere;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	;
	UStaticMeshComponent* SmallSphere;
	//Timer
	FTimerHandle EffectTimer, DamageTimer;
	// Movement
	float AngleAxis = 10;
	float CircularMovementLast;
	float MiddleMovementLast;
	// Switch Logic
	bool CanSwitch = true;
	bool CanCircularMove = true;
	bool CanCreateShamanEffect = true;
	bool CanCreateShamanEffect2 = true;
	bool CanGiveDamage = false;
	bool CanGiveDamage2 = false;
	// Location knowledge
	FVector CircularLocation();
	UPROPERTY(EditAnywhere, Category = "Boss Settings")
	FVector TargetLocation = FVector(-10890.000000, -3925.000000, 1665.000000);
	UPROPERTY(EditAnywhere, Category = "Boss Settings")
	FVector BigInitialLocation = FVector(-10900.000000, -3295.000000, 1215.000000);
	UPROPERTY(EditAnywhere, Category = "Boss Settings")
	FVector MiddleInitialLocation = FVector(-10900.000000, -3295.000000, 1335.000000);
	UPROPERTY(EditAnywhere, Category = "Boss Settings")
	FVector SmallInitialLocation = FVector(-10900.000000, -3295.000000, 1490.000000);
	// Functions
	void Calculate();
	void ChangeLocation();
	void CircularMovement(FVector Location) const;
	void MiddleMovement();
	void BackToInitial();
	void CreateShamanEffect();
	void CreateShamanEffect2();
	void DecreaseDamage();
	void StopDamage();

	// Cast main player
	AContraCharacter* ContraCharacter = nullptr;
	// Select Effect
	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Shaman Effect"))
	TArray<struct FShamanEffect> ShamanEffects;
	UPROPERTY(EditDefaultsOnly)
	float EffectDelay = 9.0f;
	// Start Effect Area
	UPROPERTY(EditAnywhere)
	ATriggerVolume* ShamanStart = nullptr;
	// Health
	bool IsDead() const;
	bool ReturnGameModeDead = true;
	UPROPERTY(EditDefaultsOnly, Category= "Boss Settings")
	float BigMaxHealth = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category= "Boss Settings")
	float MiddleMaxHealth = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category= "Boss Settings")
	float SmallMaxHealth = 100.0f;
	float Health = 100;
	float BigSphereHealth;
	float MiddleSphereHealth;
	float SmallSphereHealth;
	// Slow Motion
	void BacktoNormal();
	bool SlowLogic = false;
	FTimerHandle BacktoNormalTimer;
	float TimerValue = 0.01;

	// Dead Effect Choice
	UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DeathParticle = nullptr;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

};
