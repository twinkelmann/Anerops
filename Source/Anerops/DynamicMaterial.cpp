// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "DynamicMaterial.h"
/*

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
	if(m_materialInstanceDynamic == NULL || m_texture == NULL || force == true)
	{

		//Convert the static material in our mesh into a dynamic one, and store it (please note that if you have more than one material that you wish to mark dynamic, do so here).
		mDynamicMaterials.Add(StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0));
		//Create a dynamic texture with the default compression (B8G8R8A8)
		mDynamicTexture = UTexture2D::CreateTransient(w, h);
		//Make sure it won't be compressed
		mDynamicTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		//Turn off Gamma-correction
		mDynamicTexture->SRGB = 0;
		//Guarantee no garbage collection by adding it as a root reference
		mDynamicTexture->AddToRoot();
		//Update the texture with new variable values.
		mDynamicTexture->UpdateResource();
		//Grab the colorvalues from our existing texture (the one we created at '''Texture Setup''') and copy it into a uint8* mTextureColors variable.
		int32 w, h;
		w = textureToReadFrom->GetSizeX();
		h = textureToReadFrom->GetSizeY();
		FTexture2DMipMap& readMip = textureToReadFrom->PlatformData->Mips[0];
		mDataSize = w * h * 4; // * 4 because we're working with uint8's - which are 4 bytes large
		mDataSqrtSize = w * 4; // * 4 because we're working with uint8's - which are 4 bytes large
		readMip.BulkData.GetCopy((void**)&mTextureColors);
		// Initalize our dynamic pixel array with data size
		mDynamicColors = new uint8[mDataSize];
		// Copy our current texture's colors into our dynamic colors
		FMemory::Memcpy(mDynamicColors, mTextureColors, mDataSize);
		// Create a new texture region with the width and height of our dynamic texture
		mUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, w, h);
		// Set the Paramater in our material to our texture
		mDynamicMaterials[0]->SetTextureParameterValue("DynamicTextureParam", mDynamicTexture);



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

bool ADynamicMaterial::copyColorImageToBuffer(Image* image, const TArray<uint8> &data, const uint32 width, const uint32 height)
{

	if(image == NULL)
	{
		return false;
	}
	// Extracts the raw data from the PXCImage object.
	ImageData imageData;
	Status result = image->AcquireAccess(Image::ACCESS_READ, Image::PIXEL_FORMAT_RGB24, &imageData);

	if(result != STATUS_NO_ERROR)
	{
		return false;
	}

	uint32 i = 0;
	for(uint32 y = 0; y < height; ++y)
	{
		//color points to one row of color image data.
		const uint8_t* color = imageData.planes[0] + (imageData.pitches[0] * y);
		for(uint32 x = 0; x < width; x++, color += 3)
		{
			data[i++] = color[0];
			data[i++] = color[1];
			data[i++] = color[2];
			data[i++] = 0xff; // alpha = 255
		}
	}

	image->ReleaseAccess(&imageData);

	return true;
}

void ADynamicMaterial::updateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture && Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
			for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
			{
				int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
				if (RegionData->MipIndex >= CurrentFirstMip)
				{
					RHIUpdateTexture2D(
						RegionData->Texture2DResource->GetTexture2DRHI(),
						RegionData->MipIndex - CurrentFirstMip,
						RegionData->Regions[RegionIndex],
						RegionData->SrcPitch,
						RegionData->SrcData
						+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
						+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
				}
			}
			if (bFreeData)
			{
				FMemory::Free(RegionData->Regions);
				FMemory::Free(RegionData->SrcData);
			}
			delete RegionData;
		});
	}
}
*/
