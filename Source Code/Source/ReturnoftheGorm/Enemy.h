// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ContraCharacter.h"
#include "Spawner.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UBoxComponent;

UCLASS()
class RETURNOFTHEGORM_API AEnemy : public AContraCharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// BTService_TargetLocation value

	// JUMP AI
	UPROPERTY(EditAnywhere, Category= "AI Settings")
	FVector TargetLocation;
	// Level 6 Boss AI function
	void BossAIGrenade();
	//Boss jump camera shake
	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<UMatineeCameraShake> BossShake;
	UPROPERTY(EditAnywhere, Category = "Boss")
	UAnimationAsset* BossJumpAnim = nullptr;
	UPROPERTY(EditAnywhere, Category = "Boss")
	FVector BossDroneSpawnLocation;
	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<AActor> Drone;
	bool AnimIsPlaying = false;

private:
	///////////////////////////////
	void CalculateRotationAI();
	void SetRotationAI();
	bool EnemySight();

	//AI SETTINGS
	AActor* Player = nullptr;
	//AI Rotation function and values
	UPROPERTY(EditAnywhere, Category= "AI Settings", meta = (AllowPrivateAccess = "true"))
	float AIRotationValue = 200.0f;
	FVector AILocation;
	FRotator AIRotation;
	FRotator RotationCalculate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI Settings", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Eyes = nullptr;
};
