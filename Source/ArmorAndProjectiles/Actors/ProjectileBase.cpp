// ArmorAndProjectiles / Fedosov Vladislav

#include "Actors/ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysMaterials/ArmorPhysicalMaterial.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->InitialSpeed = 200.f;
	ProjectileMovementComponent->MaxSpeed = 300.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;


	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	SetRootComponent(ProjectileMesh);
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	ProjectilePropertiesDataTablePath = FString("DataTable'/Game/Misc/DT_ProjectileProperties.DT_ProjectileProperties'");

	
}

void AProjectileBase::OnConstruction(const FTransform& Transform)
{
	const UDataTable* ProjectilePropertiesTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ProjectilePropertiesDataTablePath));
	if (ProjectilePropertiesTableObject)
	{
		FProjectileProperties* ProjectileProperties = nullptr;
		switch (ProjectileType)
		{
		case EProjectileType::EPT_AP:
			ProjectileProperties = ProjectilePropertiesTableObject->FindRow<FProjectileProperties>("PT_AP", TEXT(""));
			break;
		case EProjectileType::EPT_HE:
			ProjectileProperties = ProjectilePropertiesTableObject->FindRow<FProjectileProperties>("PT_HE", TEXT(""));
			break;
		case EProjectileType::EPT_HEAT:
			ProjectileProperties = ProjectilePropertiesTableObject->FindRow<FProjectileProperties>("PT_HEAT", TEXT(""));
			break;
		default:
			break;
		}
		if (ProjectileProperties)
		{
			ProjectileMesh->SetStaticMesh(ProjectileProperties->ProjectileMesh);
			ArmorPenetration = ProjectileProperties->ArmorPenetration;
			RicochetAngle = ProjectileProperties->RicohetAngle;
		}
	}
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DrawNormalVector(Hit.ImpactPoint, Hit.ImpactNormal);
	FVector Direction = (Hit.TraceEnd - Hit.TraceStart).GetSafeNormal();
	float AngleInRadians = CalculateAngleBetweenVectors(Hit.ImpactNormal * -1.f, Direction);
	float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Ricochet Angle: %f"), RicochetAngle));
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Hit Angle: %f"), AngleInDegrees));

	if (AngleInDegrees <= RicochetAngle) //Check ricochet condition, if true -> penetration test
	{
		UMaterialInterface* Material = OtherComp->GetMaterial(0);
		if (Material)
		{
			UPhysicalMaterial* PhysMaterial = Material->GetPhysicalMaterial();
			if (PhysMaterial)
			{
				UArmorPhysicalMaterial* ArmorMaterial = Cast<UArmorPhysicalMaterial>(PhysMaterial);
				if (ArmorMaterial)
				{
					float ArmorThicknessAtAngle = GetArmorThicknessAtAngle(AngleInDegrees, ArmorMaterial->GetArmorThickness());
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Armor Thickness: %f"), ArmorMaterial->GetArmorThickness()));
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Armor Thickness At Angle: %f"), ArmorThicknessAtAngle));
					if (ArmorThicknessAtAngle <= ArmorPenetration) //Comparison of projectile penetration and armor thickness
					{
						DrawPenetrationLine(Hit.ImpactPoint, Direction, ArmorPenetration);
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Armor Penetration!")));
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Not Armor Penetration!")));
					}
				}
			}
		}
		Destroy();
	}
	else
	{
		RicochetProjectile(Hit.ImpactPoint, Direction, Hit.ImpactNormal);
	}
}

void AProjectileBase::DrawNormalVector(const FVector& Point, const FVector& Normal)
{
	FVector LineEnd = Point + Normal * 20.f;
	DrawDebugLine(GetWorld(), Point, LineEnd, FColor::Blue, false, 2.f, 2.f);
}

float AProjectileBase::CalculateAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB)
{
	float DotProduct = FVector::DotProduct(VectorA, VectorB);
	float LengthA = VectorA.Size();
	float LengthB = VectorB.Size();

	if (LengthA < KINDA_SMALL_NUMBER || LengthB < KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	float CosineValue = DotProduct / (LengthA * LengthB);
	float Radians = FMath::Acos(FMath::Clamp(CosineValue, -1.0f, 1.0f));

	return Radians;
}

float AProjectileBase::GetArmorThicknessAtAngle(float AngleInDegrees, float ArmorThickness)
{
	float AngleInRadians = FMath::DegreesToRadians(AngleInDegrees);
	return ArmorThickness / FMath::Cos(AngleInRadians);
}

void AProjectileBase::DrawPenetrationLine(const FVector& Point, const FVector& Direction, float PenetrationDistance)
{
	FVector PenetrationThickness = Direction * PenetrationDistance;
	FVector PenetratinEnd = Point + PenetrationThickness;
	DrawDebugLine(GetWorld(), Point, PenetratinEnd, FColor::Red, false, 2.f, 2.f);
}

void AProjectileBase::RicochetProjectile(const FVector& Point, const FVector& ProjDirection, const FVector& Normal)
{
	FVector RicochetEnd = Point + ProjDirection.MirrorByVector(Normal);
	FRotator Direction = UKismetMathLibrary::MakeRotFromX(FVector(RicochetEnd - Point));
	SetActorRotation(Direction);
	SetLifeSpan(3.f);
}