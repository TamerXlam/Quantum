
#pragma once
#include "CoreMinimal.h"

#include "BaseGraph.h"
#include "Animation/AnimInstance.h"


#include "BaseSwitchers.generated.h"


UCLASS(ClassGroup = "TheGame|StateMachine")
class STATEMACHINE_API UOnBaseStateIsFinishedNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	/* public: FUNCTION LIST */

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:

	virtual TArray<uint8> GetStates() const { return TArray<uint8>(); }
	virtual TArray<uint8> GetItemStates() const { return TArray<uint8>(); }
};