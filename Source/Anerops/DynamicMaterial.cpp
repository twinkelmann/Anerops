// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "DynamicMaterial.h"


ADynamicMaterial::ADynamicMaterial() :
	m_texture(NULL),
	m_buffer(NULL),
	m_updateTextureRegion(0, 0, 0, 0, m_width, m_height)
{
	m_bufferSize = m_width * m_height * m_bytesPerPixel;
	m_bufferSizeSqrt = m_width * m_bytesPerPixel;

	createTexture(true);
}

ADynamicMaterial::~ADynamicMaterial()
{
	//not sure if I should delete it myself
	if(m_buffer != NULL)
	{
		delete[] m_buffer;
	}
}

void ADynamicMaterial::createTexture(bool force)
{
	UE_LOG(GeneralLog, Warning, TEXT("--Creating Texture--"));

	// check to see if we actually need to make all this from scratch
	if(/*m_materialInstanceDynamic == NULL || */m_texture == NULL || force == true)
	{
		//create buffers to collate pixel data into
		m_buffer = new uint8[m_bufferSize];

		// create dynamic texture
		m_texture = UTexture2D::CreateTransient(m_width, m_height);
		m_texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		m_texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		m_texture->SRGB = 0;
		//guarantee no garbage collection by adding it as a root reference
		m_texture->AddToRoot();
		//update the texture with new variable values.
		m_texture->UpdateResource();

		//plug the dynamic texture into the dynamic material
		//dtMaterialInstanceDynamic->SetTextureParameterValue(FName("DynamicTextureParam"), dtTexture);
	}
}

void ADynamicMaterial::updateTexture(Image* image)
{
	UE_LOG(GeneralLog, Warning, TEXT("--Update Texture--"));

	copyColorImageToBuffer(image, m_buffer, m_width, m_height);

	//updateTextureRegions(m_texture, 0, 1, &m_updateTextureRegion, m_bufferSizeSqrt, (uint32)m_bytesPerPixel, m_buffer, false);
	//TODO: set the new texture on the dymanic material instance
	//dtMaterialInstanceDynamic->SetTextureParameterValue("DynamicTextureParam", m_texture);
}

void ADynamicMaterial::copyColorImageToBuffer(Image* image, TArray<uint8>& data, const uint32 width, const uint32 height)
{

	if(image == NULL)
	{
		return;
	}
	// Extracts the raw data from the PXCImage object.
	ImageData imageData;
	Status result = image->AcquireAccess(Image::ACCESS_READ, Image::PIXEL_FORMAT_RGB24, &imageData);

	if (result != STATUS_NO_ERROR)
	{
		return;
	}

	uint32 i = 0;
	for (uint32 y = 0; y < height; ++y)
	{
		// color points to one row of color image data.
		const uint8_t* color = imageData.planes[0] + (imageData.pitches[0] * y);
		for(uint32 x = 0; x < width; ++x, color += 3)
		{
			data[i++] = color[0];
			data[i++] = color[1];
			data[i++] = color[2];
			data[i++] = 0xff; // alpha = 255
		}
	}

	image->ReleaseAccess(&imageData);
}
