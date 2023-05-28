// ArmorAndProjectiles

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ArmorPhysicalMaterial.generated.h"

UCLASS()
class ARMORANDPROJECTILES_API UArmorPhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:
	float GetArmorThickness() { return ArmorThickness; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ArmorThickness;
};
