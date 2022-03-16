// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContraCharacter.h"
#include "PawnBase.h"
#include "PawnTurret.generated.h"

UENUM()
enum EGunType
{
	Normal UMETA(Display = "Normal Weapon"),
	Slow UMETA(Display = "Slow Weapon"),
	Speed UMETA(Display = "Speed Weapon")
};


UCLASS()
class RETURNOFTHEGORM_API APawnTurret : public APawnBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnTurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void HandleDestruction() override;

private:
	void CheckFireCondition();
	float ReturnDistanceToPlayer();

	FTimerHandle FireRateTimerHandle;
	AContraCharacter* PlayerPawn = nullptr;

	// Direction choice
	UPROPERTY(EditAnywhere, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EGunType> GunType;

	// Eđer oyuncu yakýnsa 2 saniyede bir ateş et
	UPROPERTY(EditAnywhere, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.0f;
	// Görüş mesafesinde ise ateş eder
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRange = 500.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
