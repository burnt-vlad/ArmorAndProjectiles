// ArmorAndProjectiles

#include "Actors/ShowCase.h"
#include "Actors/ProjectileBase.h"

AShowCase::AShowCase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootPoint = CreateDefaultSubobject<USceneComponent>("RootPoint");
	SetRootComponent(RootPoint);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
	SpawnPoint->SetupAttachment(RootPoint);

	ArmorMesh = CreateDefaultSubobject<UStaticMeshComponent>("ArmorMesh");
	ArmorMesh->SetupAttachment(RootPoint);

	ArmorMesh->SetMaterial(0, Material);
}

void AShowCase::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DemonstrationTimerHandle, this, &AShowCase::Demonstration, 2.f, true);
}

void AShowCase::SpawnProjectile()
{
	auto World = GetWorld();
	if (World)
	{
		FVector Location = SpawnPoint->GetComponentLocation();
		FRotator Rotation = SpawnPoint->GetComponentRotation();
		AProjectileBase* Projectile = World->SpawnActor<AProjectileBase>(ProjectileClass, Location, Rotation);
	}
}

void AShowCase::Demonstration()
{
	SpawnProjectile();
	if (ArmorMesh->GetComponentRotation().Pitch <= -80.f)
	{
		ArmorMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	}
	else
	{
		FRotator CurrnetRotation = ArmorMesh->GetComponentRotation();
		ArmorMesh->AddLocalRotation(FRotator(-7.5f, 0.f, 0.f));
	}
}

