#include "Camel.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Animation/AnimBlueprint.h"
#include "ActorComponent/SplinePath.h"


ACamel::ACamel()
{
	PrimaryActorTick.bCanEverTick = true;

	CamelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CamelMesh"));
	RootComponent = CamelMesh;

	SplinePath = CreateDefaultSubobject<USplinePath>(TEXT("SplinePath"));
	SplinePath->SetupAttachment(RootComponent);
}

void ACamel::BeginPlay()
{
	Super::BeginPlay();
	
	if (CamelAnimBlueprint)
	{
		CamelMesh->SetAnimInstanceClass(CamelAnimBlueprint->GeneratedClass);
	}

	CamelMesh->PlayAnimation(CamelWalkAnim, true);
}

void ACamel::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (SplinePath && SplinePath->GetSplineComponent())
	{
		FVector CamelLocation = GetActorLocation();
		SplinePath->GetSplineComponent()->SetLocationAtSplinePoint(0, CamelLocation, ESplineCoordinateSpace::World);
	}
}

void ACamel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SplinePath->MoveAlongSpline(DeltaTime);
}

