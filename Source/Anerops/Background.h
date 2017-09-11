// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#pragma once

#include "Engine/StaticMeshActor.h"
#include "StaticMeshResources.h"

#include "RealSense/SampleReader.h"

#include "Background.generated.h"

//base RealSense namespace
using namespace Intel::RealSense;

/**
 * @brief The ABackground class
 * This Actor is the background stream. It can be updated
 * with a new image by using the `updateImage` method
 */
UCLASS()
class ANEROPS_API ABackground : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	~ABackground();
	virtual void PostInitializeComponents() override;

	bool updateImage(Image* image);

private:
	void updateDynamicTexture();

	UMaterialInstanceDynamic* m_dynamicMaterial;

	UPROPERTY()
	UTexture2D* m_dynamicTexture;
	//the region tu update (the whole image)
	FUpdateTextureRegion2D* m_updateTextureRegion;

	uint8* m_colors;
	uint32 m_dataSize;
	uint32 m_dataSizeSqrt;
};
