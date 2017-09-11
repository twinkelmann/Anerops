// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "Background.h"

#include "Utilities.h"

/**
 * @brief ABackground::~ABackground
 * Destructor the actor, freeing what it created
 */
ABackground::~ABackground()
{
	UE_LOG(GeneralLog, Warning, TEXT("--Background actor destruction--"));

	if(m_colors != NULL)
	{
		delete[] m_colors;
	}

	if(m_updateTextureRegion != NULL)
	{
		delete m_updateTextureRegion;
	}

	UE_LOG(GeneralLog, Warning, TEXT("--Done destroying Background actor--"));
}

/**
 * @brief ABackground::PostInitializeComponents
 * Called after the initialization of the components.
 * Creates the dynamic texture and material
 */
void ABackground::PostInitializeComponents()
{
	UE_LOG(GeneralLog, Warning, TEXT("--Background actor initialization--"));

	Super::PostInitializeComponents();

	//Texture initialization

	m_dynamicMaterial = GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamic(0);

	if(m_dynamicMaterial == NULL)
	{
		UE_LOG(GeneralLog, Warning, TEXT("--Could not create material instance--"));
		return;
	}
	m_dynamicTexture = UTexture2D::CreateTransient(Constantes::STREAM_WIDTH, Constantes::STREAM_HEIGHT);
	m_dynamicTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	m_dynamicTexture->SRGB = 0;
	m_dynamicTexture->AddToRoot();
	m_dynamicTexture->UpdateResource();

	m_updateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, Constantes::STREAM_WIDTH, Constantes::STREAM_HEIGHT);

	m_dynamicMaterial->SetTextureParameterValue("DynamicTextureParam", m_dynamicTexture);

	m_dataSize = Constantes::STREAM_WIDTH * Constantes::STREAM_HEIGHT * Constantes::BYTES_PER_PIXEL;
	m_dataSizeSqrt = Constantes::STREAM_WIDTH * Constantes::BYTES_PER_PIXEL;
	m_colors = new uint8[m_dataSize];

	updateDynamicTexture();

	UE_LOG(GeneralLog, Warning, TEXT("--Done initializing Background actor--"));
}

/**
 * @brief ABackground::updateDynamicTexture
 * Updates the UTexture2D with the new buffer data
 * Reloads the texture in the Material instance
 */
void ABackground::updateDynamicTexture()
{
	//UE_LOG(GeneralLog, Warning, TEXT("--Backgorund update--"));
	m_dynamicTexture->UpdateTextureRegions(0, 1, m_updateTextureRegion, m_dataSizeSqrt, Constantes::BYTES_PER_PIXEL, m_colors);
	m_dynamicMaterial->SetTextureParameterValue("DynamicTextureParam", m_dynamicTexture);
}

/**
 * @brief ABackground::updateImage
 * Update the buffer with a new frame from the camera
 * @param image the RealSense color image
 * @return true if it succeded, false otherwise
 */
bool ABackground::updateImage(Image *image)
{
	if(image == NULL)
	{
		return false;
	}
	// Extracts the raw data from the Image object.
	ImageData imageData;
	Status result = image->AcquireAccess(Image::ACCESS_READ, Image::PIXEL_FORMAT_RGB24, &imageData);

	if(result != STATUS_NO_ERROR)
	{
		return false;
	}

	uint32 i = 0;
	for(uint32 y = 0; y < Constantes::STREAM_HEIGHT; y++)
	{
		//color points to one row of color image data.
		const uint8_t* color = imageData.planes[0] + (imageData.pitches[0] * y);
		for(uint32 x = 0; x < Constantes::STREAM_WIDTH; x++, color += 3)
		{
			m_colors[i++] = color[0];
			m_colors[i++] = color[1];
			m_colors[i++] = color[2];
			m_colors[i++] = 0xff; // alpha = 255
		}
	}

	image->ReleaseAccess(&imageData);

	updateDynamicTexture();

	return true;
}
