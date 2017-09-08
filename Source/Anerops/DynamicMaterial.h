// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#pragma once

#include "Anerops.h"

#include "RealSense/SampleReader.h"

//base RealSense namespace
using namespace Intel::RealSense;

class ANEROPS_API ADynamicMaterial
{
public:

	ADynamicMaterial();
	~ADynamicMaterial();

	void updateTexture(Image* image);
	static void copyColorImageToBuffer(Image* image, TArray<uint8>& data, const uint32 width, const uint32 height);

private:
	void createTexture(bool force);
	static void updateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);

	//texture info
	static const int m_width = 1280;
	static const int m_height = 720;
	static const int m_bytesPerPixel = 4;

	//buffer info
	uint8* m_buffer;
	int m_bufferSize;
	int m_bufferSizeSqrt;

	FUpdateTextureRegion2D m_updateTextureRegion;

	//dynamic material
	//UMaterialInstanceDynamic* m_materialInstanceDynamic;

	//dynamic texture
	UTexture2D* m_texture;

};
