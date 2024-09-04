#include "SplinePath.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"

USplinePath::USplinePath()
{
	PrimaryComponentTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	if (SplineComponent)
	{
		SplineComponent->SetupAttachment(this);
	}

	Speed = 100.0f;
	CurrentDistance = 0.0f;
}


void USplinePath::BeginPlay()
{
	Super::BeginPlay();

	
}


void USplinePath::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveAlongSpline(DeltaTime);
}

void USplinePath::MoveAlongSpline(float DeltaTime)
{
	if (!SplineComponent)
	{
		return;
	}

	CurrentDistance += Speed * DeltaTime;

	FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);

	Rotation.Yaw -= 90.0f;

	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->SetActorLocation(Location);
		Owner->SetActorRotation(Rotation);
	}
}

void USplinePath::SetSplineComponent(USplineComponent* SplineComp)
{
	SplineComponent = SplineComp;
}

USplineComponent* USplinePath::GetSplineComponent() const
{
	return SplineComponent;
}

