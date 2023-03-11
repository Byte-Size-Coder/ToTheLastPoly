 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class USkeletalMeshComponet;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOTHELASTPOLY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	public:
		APlayerCharacter();
		virtual void Tick(float DeltaTime) override;
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		virtual void PostInitializeComponents() override;

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		// Input
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputMappingContext* PlayerCharacterMappingContext;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* MoveAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* LookAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* JumpAction;
		
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* CrouchAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* EquipAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* AimAction;

		void Move(const FInputActionValue& Value);
		void Look(const FInputActionValue& Value);
		void Equip();
		void ToggleCrouch();
		void AimPressed();
		void AimReleased();
		void AimOffset(float DeltaTime);

	private:
		// Mesh
		UPROPERTY(VisibleAnywhere, Category = Mesh)
		USkeletalMeshComponent* CharacterMesh;

		// Camera
		UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* CameraBoom;

		UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* FollowCamera;

		UPROPERTY(VisibleAnywhere, Category = Camera)
		float CameraBoomLength = 600.0f;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* OverheadWidget;

		UPROPERTY(VisibleAnywhere)
		class UCombatComponent* Combat;

		UFUNCTION(Server, Reliable)
		void ServerEquip();

		UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
		class AWeapon* OverlappingWeapon;

		UFUNCTION()
		void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

		float AO_Yaw;
		float AO_Pitch;
		FRotator StartingAimRotation;
		
	public:	
		void SetOverlappingWeapon(AWeapon* Weapon);
		bool IsWeaponEquipped();
		bool IsAiming();
		AWeapon* GetEquippedWeapon();

		FORCEINLINE USkeletalMeshComponent* GetCharacterMesh() { return CharacterMesh; }
		FORCEINLINE float GetAOYaw() const { return AO_Yaw; }
		FORCEINLINE float GetAOPitch() const { return AO_Pitch; }
};
