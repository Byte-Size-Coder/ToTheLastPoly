// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"
#include "ToTheLastPoly/Weapon/Weapon.h"
#include "ToTheLastPoly/Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMeshSocket.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	BaseWalkSpeed = 600.0f;
	AimWalkSpeed = 450.0f;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
		if (PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
}

void UCombatComponent::EquipWeapon(AWeapon *WeaponToEquip)
{
	if (PlayerCharacter == nullptr || WeaponToEquip == nullptr) return;

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* HandSocket = PlayerCharacter->GetCharacterMesh()->GetSocketByName(FName("RightHandSocket"));

	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, PlayerCharacter->GetCharacterMesh());
	}

	EquippedWeapon->SetOwner(PlayerCharacter);
	EquippedWeapon->ShowPickupWidget(false);
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	PlayerCharacter->bUseControllerRotationYaw = true;
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		PlayerCharacter->bUseControllerRotationYaw = true;
	}
}

