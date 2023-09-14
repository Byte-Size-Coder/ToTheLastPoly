// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToTheLastPoly/Weapon/Weapon.h"

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (PlayerCharacter == nullptr)
    {
        PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
    }

    if (PlayerCharacter == nullptr) return;

    FVector Velocity = PlayerCharacter->GetVelocity();
    Velocity.Z = 0.0f;
    Speed = Velocity.Size();

    bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();
    bIsCrouched = PlayerCharacter->bIsCrouched;
    bIsAccelerating = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f ? true : false;
    bWeaponEquipped = PlayerCharacter->IsWeaponEquipped();
    EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
    bAiming = PlayerCharacter->IsAiming();
    TurningInPlace = PlayerCharacter->GetTurningInPlace();

    // Offset Yaw for Straffing
    FRotator AimRotation = PlayerCharacter->GetBaseAimRotation();
    FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetVelocity());
    FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
    DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.0f);
    YawOffset = DeltaRotation.Yaw;

    CharacterRotationLastFrame = CharacterRotation;
    CharacterRotation = PlayerCharacter->GetActorRotation();
    const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
    const float Target = Delta.Yaw / DeltaTime;
    const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.0f);
    Lean = FMath::Clamp(Interp, -90.0f, 90.0f);

    AO_Yaw = PlayerCharacter->GetAOYaw();

    AO_Pitch = PlayerCharacter->GetAOPitch();

    if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && PlayerCharacter->GetMesh())
    {
        LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
        FVector OutPosition;
        FRotator OutRotation;
        PlayerCharacter->GetMesh()->TransformToBoneSpace(FName("Hand_R"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);

        LeftHandTransform.SetLocation(OutPosition);
        LeftHandTransform.SetRotation(FQuat(OutRotation));
    }
}
