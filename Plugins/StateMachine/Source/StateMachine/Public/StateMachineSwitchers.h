
#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "State.h"
#include "StateMachineSwitchers.generated.h"

UCLASS(ClassGroup = "TheGame|StateMachine")
class STATEMACHINE_API UOnBaseStateNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override
	{
		if (!IsValid(MeshComp))
		{
			return;
		}

		AActor* MOwner = MeshComp->GetOwner();
		if (!IsValid(MOwner))
		{
			return;
		}

		//UState *MFSM = Cast<UState>(MOwner->GetComponentByClass(UState::StaticClass()));
		//if (!IsValid(MFSM))
		//{
		//	return;
		//}

		//MFSM->OnStateNotify(StatesFrom, StatesTo);
	}

private:
	//UPROPERTY(Category = "TheGame", EditDefaultsOnly, BlueprintReadWrite, Blueprintable, meta = (AllowPrivateAccess = "true"))
	//TArray<UState> StatesFrom;

	//UPROPERTY(Category = "TheGame", EditDefaultsOnly, BlueprintReadWrite, Blueprintable, meta = (AllowPrivateAccess = "true"))
	//TArray<UState> StatesTo;
};