


#include "WidgetActors/GrapplePointHighlight.h"
#include "Components/WidgetComponent.h"

// Sets default values
AGrapplePointHighlight::AGrapplePointHighlight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create the WidgetComponent and attach it to the root component
	HighlightWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	RootComponent = HighlightWidget;
}


// Called when the game starts or when spawned
void AGrapplePointHighlight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrapplePointHighlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

