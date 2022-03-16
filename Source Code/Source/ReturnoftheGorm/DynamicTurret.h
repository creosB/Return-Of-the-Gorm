// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContraCharacter.h"
#include "PawnBase.h"
#include "DynamicTurret.generated.h"

UCLASS()
class RETURNOFTHEGORM_API ADynamicTurret : public APawnBase
{
	GENERATED_BODY()

public:

	// Sets default values for this pawn's properties
	ADynamicTurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void HandleDestruction() override;

private:
	// Dynamic Box
	void CollisionLogic();
	void OnCollision();
	void OffCollision();
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BoxMesh;
	FTimerHandle CollisionTimer;
	bool CanShoot = false;
	bool CollisionResult = true;
	float OnTimer;
	float OffTimer;
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CollisionOffDelay  = 8.0f;
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CollisionOnDelay  = 8.0f;
	// Fire Settings
	void CheckFireCondition();
	float ReturnDistanceToPlayer() const;
	FTimerHandle FireRateTimerHandle;
	// Cast to main player
	AContraCharacter* PlayerPawn = nullptr;

	// every 2 per sec. fire!
	UPROPERTY(EditAnywhere, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.0f;
	// turret sight area
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRange = 500.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
