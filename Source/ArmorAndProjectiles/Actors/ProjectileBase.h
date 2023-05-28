// ArmorAndProjectiles / Fedosov Vladislav

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ProjectileBase.generated.h"

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	EPT_AP UMETA(DisplayName = "EPT_AP"),
	EPT_HEAT UMETA(DisplayName = "EPT_HEAT"),
	EPT_HE UMETA(DisplayName = "EPT_HE"),
};

USTRUCT(BlueprintType)
struct FProjectileProperties : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* ProjectileMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ArmorPenetration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RicohetAngle;
};

UCLASS()
class ARMORANDPROJECTILES_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	void DrawNormalVector(const FVector& Point, const FVector& Normal);
	float CalculateAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB);
	float GetArmorThicknessAtAngle(float AngleInDegrees, float ArmorThickness);
	void DrawPenetrationLine(const FVector& Point, const FVector& Direction, float PenetrationDistance);
	void RicochetProjectile(const FVector& Point, const FVector& ProjDirection, const FVector& Normal);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EProjectileType ProjectileType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString ProjectilePropertiesDataTablePath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ArmorPenetration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RicochetAngle;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
