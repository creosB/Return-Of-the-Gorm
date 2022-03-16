// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NiagaraComponent.h"
#include "Engine/TriggerVolume.h"
#include "ElecticShock.generated.h"

class AContraCharacter;

UCLASS()
class RETURNOFTHEGORM_API AElecticShock : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElecticShock();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

private:
	// Component
	USceneComponent* SceneComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Start = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* End = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Left = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Right = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* ElectricEffect = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* ElectricEffect2 = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* ElectricEffect3 = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BossMesh = nullptr;

	// Cast
	AContraCharacter* ContraCharacter;
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressPlate = nullptr;
	// Function
	void EffectLogic(float DeltaTime);
	void EffectOn(float DeltaTime);
	void EffectOff(float DeltaTime);
	void EffectVisibility();
	void EffectLocation();
	void ElectricHit();
	bool EffectHitStart();
	bool EffectHitLeft();
	bool EffectHitRight();

	// niagara effect name
	FName EffectName = "User.BeamEnd";
	// Start/Initial Location
	FVector EndLocation;
	FVector LeftLocation;
	FVector RightLocation;
	// Update/Current Location
	FVector EndUpdateLocation;
	FVector LeftUpdateLocation;
	FVector RightUpdateLocation;
	// Target Location
	UPROPERTY(EditAnywhere, Category= "Effect Settigs")
	FVector EndTargetLocation;
	UPROPERTY(EditAnywhere, Category= "Effect Settigs")
	FVector LeftTargetLocation;
	UPROPERTY(EditAnywhere, Category= "Effect Settigs")
	FVector RightTargetLocation;
	// Variables
	bool EffectVisibilityLogic = true;
	bool EffectSwitch = true;
	float EffectOnLast;
	float EffectOffLast;
	UPROPERTY(EditAnywhere, Category= "Effect Settigs")
	float EffectDelay = 10.0f;
	// Slow Motion
	void BacktoNormal();
	bool SlowLogic = false;
	FTimerHandle BacktoNormalTimer;
	float TimerValue = 0.01;
	// Health
	bool ReturnGameModeDead = true;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere)
	float Health;

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
