// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "KillEmAllGameMode.h"
#include "GameFramework/Character.h"
#include "ContraCharacter.generated.h"

class AGun;
class AShotgun;

UCLASS(config=Game)
class RETURNOFTHEGORM_API AContraCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:
	virtual void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Value);
	void MoveForward(float AxisValue);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	AContraCharacter();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	           FVector NormalImpulse, const FHitResult& Hit);

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;
	// Gun function
	void Shoot();
	void StopShooting();
	void SwapWeapon();

	UPROPERTY()
	AGun* Gun;

	FTimerHandle ShootTimer;
	FTimerHandle SwapWeaponTimer;

	UFUNCTION(BlueprintPure)
	bool IsDead();
	// Weapon swap logic
	bool IsCanChange = false;
	// Prone Mode
	UFUNCTION(BlueprintPure)
	bool IsInProne();
	bool SetProne = false;
	UFUNCTION(BlueprintPure)
	bool IsProneFire();
	bool SetProneFire = false;
	// Health
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere)
	float Health;
	// Open Door
	bool CanOpen;

	AKillEmAllGameMode* KillEmAllGameMode;

private:
	// Function
	void IncreaseSpeed();
	void DecreaseSpeed();
	void Reload();
	void SwapLogic();
	void AddWeapon();
	void Prone();
	void Open();
	void BacktoNormal();
	void Pause();
	void CheckPlayerDead();

	bool bIsShiftPressed = false;
	// Weapon System
	bool LeftMouseHeld = true;
	bool ShotWithShotgun = false;
	bool CanSpawn = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Weapon", meta = (AllowPrivateAccess = true))
	TSubclassOf<AGun> NormalGunClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Weapon", meta = (AllowPrivateAccess = true))
	TSubclassOf<AShotgun> ShotgunClass;
	UPROPERTY()
	AShotgun* Agun;
	UPROPERTY(EditAnywhere)
	float RotationRate = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Weapon", meta = (AllowPrivateAccess = true))
	float WeaponSwapTime = 5.0f;

	// Slow Motion
	float TimerValue = 0.01;
	FTimerHandle BacktoNormalTimer;
	bool SlowLogic = false;

};
