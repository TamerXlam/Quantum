
#pragma once

#include "CoreMinimal.h"
#include "Character/ALSBaseCharacter.h"
#include "ALSCharacter.generated.h"

/**
 * Specialized character class, with additional features like held object etc.
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AALSCharacter : public AALSBaseCharacter
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	int32 SkinIndex = int32(0);

public:
	AALSCharacter(const FObjectInitializer& ObjectInitializer);

	/** Implemented on BP to update held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void ClearHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh,
	                  class UClass* NewAnimClass, bool bLeftHand, FVector Offset);

	virtual void RagdollStart() override;
	virtual void RagdollEnd() override;

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	virtual FVector GetFirstPersonCameraTarget() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|HeldObject")
	AActor* GetActiveWeapon() const;

	UFUNCTION()
	bool HasAimingCameraTarget(FTransform& NewTransform);

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	const int32 GetSkinIndex() const { return SkinIndex; }

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void AddToBlackTeam();


	virtual void UsingAnotherPawn_BP(AActor* Vehicle) override;


	UFUNCTION(NetMulticast, Reliable, Category = "ALS|Character States")
	void Multicast_UsingAnotherPawn_BP(AActor* Vehicle);

	UFUNCTION(Server, Reliable, Category = "ALS|Character States")
	void Server_UsingAnotherPawn_BP(AActor* Vehicle);

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(EALSOverlayState PreviousState) override;

	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObjectAnimations();


private:
	bool bNeedsColorReset = false;
};
