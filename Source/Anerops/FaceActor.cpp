// "Anerops" is licenced under the GNU GPL 3 licence. Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "FaceActor.h"


// Sets default values
AFaceActor::AFaceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFaceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFaceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

