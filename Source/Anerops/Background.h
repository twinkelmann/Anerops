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
 * Background actor, showing the camera color stream
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

	FUpdateTextureRegion2D* m_updateTextureRegion;

	uint8* m_colors;
	uint32 m_dataSize;
	uint32 m_dataSizeSqrt;

	static const uint32 m_width = 1920;
	static const uint32 m_height = 1080;
};
