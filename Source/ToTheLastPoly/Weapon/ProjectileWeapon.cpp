// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"

void AProjectileWeapon::Fire(const FVector &HitTarget)
{
    Super::Fire(HitTarget);

    if (!HasAuthority()) return;

    APawn* InstigatorPawn = Cast<APawn>(GetOwner());

    const USkeletalMeshSocket* GunShotSocket = GetWeaponMesh()->GetSocketByName(FName("GunShot"));

    if (GunShotSocket)
    {
        FTransform SocketTransform = GunShotSocket->GetSocketTransform(GetWeaponMesh());

        // From Gunshot socket to hit location from TraceUnderCrosshairs
        FVector ToTarget = HitTarget - SocketTransform.GetLocation();
        FRotator TargetRotation = ToTarget.Rotation();

        if (ProjectileClass && InstigatorPawn)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = GetOwner();
            SpawnParams.Instigator = InstigatorPawn;

            UWorld* World = GetWorld();
            if (World)
            {
                World->SpawnActor<AProjectile>(
                    ProjectileClass,
                    SocketTransform.GetLocation(),
                    TargetRotation,
                    SpawnParams
                );
            }
        }
    }
}
