// ArmorAndProjectiles / Fedosov Vladislav

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShowCase.generated.h"

class AProjectileBase;

UCLASS()
class ARMORANDPROJECTILES_API AShowCase : public AActor
{
	GENERATED_BODY()
	
public:	
	AShowCase();

protected:
	virtual void BeginPlay() override;
	void SpawnProjectile();
	void Demonstration();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ArmorMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* Material;
	FTimerHandle DemonstrationTimerHandle;
};
