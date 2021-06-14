
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ALSPlayerController.generated.h"

class AALSBaseCharacter;

/**
 * Player controller class
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AALSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* NewPawn) override;

	virtual void OnRep_Pawn() override;

	virtual void BeginPlayingState() override;

private:
	void SetupCamera();

	void SetupDebugInputs();
	
public:
	/** Main character reference */
	UPROPERTY(BlueprintReadOnly, Category = "ALS Player Controller")
	AALSBaseCharacter* PossessedCharacter = nullptr;
};
