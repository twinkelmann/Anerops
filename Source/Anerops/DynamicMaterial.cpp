// "Anerops" is licenced under the GNU GPL 3 licence. Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "DynamicMaterial.h"

/*
ADynamicMaterial::ADynamicMaterial()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	//---------------- setup the actor components ----------------//

	// give this actor a transformable root
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	// add a mesh to preview the dynamic texture on
	testMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Mesh"));
	testMesh->SetupAttachment(root);

	// get a test mesh from the content browser to use in the staticmeshcomponent
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshBox(TEXT("StaticMesh'/Game/Core/Systems/dTex/Shape_Cube.Shape_Cube'"));
	if (StaticMeshBox.Object)	testMesh->SetStaticMesh(StaticMeshBox.Object);

	// and finally register the static mesh component
	testMesh->RegisterComponent();

}

////////////////////////
//                    //
//  OnConstruction()  //
//                    //
////////////////////////

// regenerate instance (in editor) when necessary
void ADynamicMaterial::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	CreateTexture(false); // don't force new texture (use existing one if found)
	UpdateTexture();
}

///////////////////
//               //
//  BeginPlay()  //
//               //
///////////////////

// Called when the game starts or when spawned
void ADynamicMaterial::BeginPlay()
{
	Super::BeginPlay();

}

//////////////////////////////////
//                              //
//  PostInitializeComponents()  //
//                              //
//////////////////////////////////

// Called when the components have been initialized
void ADynamicMaterial::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CreateTexture(true); // do force new texture
	UpdateTexture();
}

//////////////
//          //
//  Tick()  //
//          //
//////////////

// Called every frame
void ADynamicMaterial::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	UpdateTexture();
}

///////////////////////
//                   //
//  CreateTexture()  //
//                   //
///////////////////////

void ADynamicMaterial::CreateTexture(bool argForceMake)
{
	UE_LOG(LogTemp, Warning, TEXT("---------- Creating Texture! ----------"));

	// check to see if we actually need to make all this from scratch
	if (dtMaterialInstanceDynamic == nullptr || dtTexture == nullptr || argForceMake == true) {

		// dynamic texture properties (hard wired here for now)
		dtBytesPerPixel = 4;
		dtWidth = 2048;
		dtHeight = 2048;

		// create buffers to collate pixel data into
		dtBufferSize = dtWidth * dtHeight * dtBytesPerPixel;
		dtBufferSizeSqrt = dtWidth * dtBytesPerPixel;
		dtBuffer = new uint8[dtBufferSize]; // this is the data that we Memcpy into the dynamic texture

		// Create dynamic material
		dtMaterialInstanceDynamic = testMesh->CreateAndSetMaterialInstanceDynamic(0);

		// create dynamic texture
		dtTexture = UTexture2D::CreateTransient(dtWidth, dtHeight);
		dtTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		dtTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		dtTexture->SRGB = 0;
		dtTexture->AddToRoot();		// Guarantee no garbage collection by adding it as a root reference
		dtTexture->UpdateResource();	// Update the texture with new variable values.

		// plug the dynamic texture into the dynamic material
		dtMaterialInstanceDynamic->SetTextureParameterValue(FName("DynamicTextureParam"), dtTexture);

		// Create a new texture region with the width and height of our dynamic texture
		dtUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, dtWidth, dtHeight);
	}

}

///////////////////////
//                   //
//  UpdateTexture()  //
//                   //
///////////////////////

void ADynamicMaterial::UpdateTexture()
{
	UE_LOG(LogTemp, Warning, TEXT("---------- Updating Texture! ----------"));

	// Set a squares width and height
	float squareW = dtWidth / 10;
	float squareH = dtHeight / 10;

	// Choose a random position for it
	float randomX = rnd() * (dtWidth  - (dtWidth  / 10));
	float randomY = rnd() * (dtHeight - (dtHeight / 10));

	// Redraw the texture with a black background and a randomly positioned red square
	int pixelCount = dtBufferSize / dtBytesPerPixel;
	for (int i = 0; i < pixelCount; i++) { // > delete this comment, it's workaround for a STUPID wordpress bug

		// figure out index for current pixels color components
		int iBlue = i * 4 + 0;
		int iGreen = i * 4 + 1;
		int iRed = i * 4 + 2;
		int iAlpha = i * 4 + 3;

		// figure out the X,Y position (relative to dtWidth & dtHeight) of the current pixel
		int iX = i % dtWidth;
		int iY = i / dtWidth;

		// if the current pixel is inside the randomly chosen position for the square, make it red
		if(iX > randomX && iX < randomX + squareW && iY > randomY && iY < randomY + squareH){ // > another fixer
			dtBuffer[iBlue] = 0;
			dtBuffer[iGreen] = 0;
			dtBuffer[iRed] = 255;
			dtBuffer[iAlpha] = 255;
		}
		// otherwise, just make it black
		else {
			dtBuffer[iBlue] = 0;
			dtBuffer[iGreen] = 0;
			dtBuffer[iRed] = 0;
			dtBuffer[iAlpha] = 255;
		}
	}

	UpdateTextureRegions(dtTexture, 0, 1, dtUpdateTextureRegion, dtBufferSizeSqrt, (uint32)4, dtBuffer, false);
	dtMaterialInstanceDynamic->SetTextureParameterValue("DynamicTextureParam", dtTexture);
}

/////////////
//         //
//  rnd()  //
//         //
/////////////

// Quick random function for testing
double ADynamicMaterial::rnd() { return double(rand()) / RAND_MAX; }

//////////////////////////////
//                          //
//  UpdateTextureRegions()  //
//                          //
//////////////////////////////

void ADynamicMaterial::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
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
