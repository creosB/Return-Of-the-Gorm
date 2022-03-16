// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContraCharacter.h"
#include "HoverBoss.h"
#include "GameFramework/Pawn.h"
#include "HoverMini.generated.h"

UCLASS()
class RETURNOFTHEGORM_API AHoverMini : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHoverMini();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
						AActor* DamageCauser) override;
	// Cast to HoverBoss
	AHoverBoss* HoverBoss;
	bool DirectionLogic = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(EditAnywhere,Category= "Components");
	UStaticMeshComponent* HoverMesh;


	void FirstMovement(float DeltaTime);
	void SecondMovement(float DeltaTime);
	void ThirdMovement(float DeltaTime);
	void SetMovementValue();
	void BacktoNormal();

	////////// Movement //////////
	// First
	FVector FirstInitial, FirstCurrent, FirstTarget;
	// Second
	FVector SecondInitial, SecondCurrent,SecondTarget;
	// Third
	FVector ThirdInitial, ThirdCurrent,ThirdTarget;
	// for the movement
	FVector Forward;

	// Target Value
	UPROPERTY(EditAnywhere, Category= "Target Settings");
	float FirstTargetValue = 50.0f;
	UPROPERTY(EditAnywhere, Category= "Target Settings");
	float SecondTargetValue = 300.0f;
	UPROPERTY(EditAnywhere, Category= "Target Settings");
	float ThirdTargetValue = 1000.0f;

	// Speed
	UPROPERTY(EditAnywhere, Category= "Target Settings");
	float FirstMovementSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category= "Target Settings");
	float SecondMovementSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category= "Target Settings");
	float ThirdMovementSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category= "Target Settings");
	float FirstMovementHeight;

	// Movement Logic
	bool FirstLogic = true;
	bool SecondLogic = false;
	bool ThirdLogic = false;

	//Health
	bool IsDead() const;
	float Health;
	UPROPERTY(EditAnywhere, Category= "Components")
	float MaxHealth = 100.0f;

	// Damage
	float DamageValue = 0.1f;
	// SLOW MOTION
	bool SlowLogic = false;
	FTimerHandle BacktoNormalTimer;
	float TimerValue = 0.01;
	bool ReturnGameModeDead = true;

	// Effect
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle = nullptr;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound;

	// Cast to player
	AContraCharacter* Player;

};
