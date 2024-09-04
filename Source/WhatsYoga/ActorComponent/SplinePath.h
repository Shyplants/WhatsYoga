#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SplinePath.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHATSYOGA_API USplinePath : public USceneComponent
{
	GENERATED_BODY()

public:	
	USplinePath();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void MoveAlongSpline(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SetSplineComponent(class USplineComponent* SplineComp);

	class USplineComponent* GetSplineComponent() const;

private:
	UPROPERTY(EditAnywhere, Category = "SplinePath", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere)
	float Speed;

	float CurrentDistance;
		
};
