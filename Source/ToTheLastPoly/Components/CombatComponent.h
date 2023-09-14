// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 80000.0f

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOTHELASTPOLY_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	public:	
		UCombatComponent();
		friend class APlayerCharacter;
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		
		void EquipWeapon(AWeapon* WeaponToEquip);
	
	protected:
		virtual void BeginPlay() override;
		void SetAiming(bool bIsAiming);

		UFUNCTION(Server, Reliable)
		void ServerSetAiming(bool bIsAiming);

		UFUNCTION()
		void OnRep_EquippedWeapon();

		void FirePressed(bool bPressed);

		UFUNCTION(Server, Reliable)
		void ServerFire(const FVector_NetQuantize& TraceHitTarget);

		UFUNCTION(NetMulticast, Reliable)
		void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

		void TraceUnderCrosshairs(FHitResult& TraceHitResult);

		void SetHUDCrosshairs(float DeltaTime);
	private:
		class APlayerCharacter* PlayerCharacter;
		class APolyPlayerController* PlayerController;
		class APlayerHUD* HUD;

		UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
		AWeapon* EquippedWeapon;

		UPROPERTY(Replicated)
		bool bAiming;

		UPROPERTY(EditAnywhere)
		float BaseWalkSpeed;

		UPROPERTY(EditAnywhere)
		float AimWalkSpeed;

		bool bFirePressed;

		float CrosshairVelocityFactor;

		float CrosshairInAirFactor;

		FVector HitTarget;

		float DefaultFOV;

		UPROPERTY(EditAnywhere, Category = Combat)
		float ZoomedFOV = 30.f;

		float CurrentFOV;

		UPROPERTY(EditAnywhere, Category = Combat)
		float ZoomInterpSpeed = 20.f;

		void InterpFOV(float DeltaTime);
};
