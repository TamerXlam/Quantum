
#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Library/ALSCharacterEnumLibrary.h"
#include "Library/ALSCharacterStructLibrary.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"


#include "Kismet/KismetSystemLibrary.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

#include "ALSBaseCharacter.generated.h"

// forward declarations
class UALSDebugComponent;
class UTimelineComponent;
class UAnimInstance;
class UAnimMontage;
class UALSCharacterAnimInstance;
class UALSPlayerCameraBehavior;
enum class EVisibilityBasedAnimTickOption : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpPressedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRagdollStateChangedSignature, bool, bRagdollState);

/*
 * Base character class
 */
UCLASS(BlueprintType)
class ALSV4_CPP_API AALSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "ALS|Basics")
	EALSTeam Team;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Custom", ReplicatedUsing = OnRep_IsAimed)
	bool IsAimed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Custom")
	float HealthMax;

private:
	UPROPERTY(ReplicatedUsing = OnRep_HealthValue)
	float HealthValue;

public:
	AALSBaseCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement")
	FORCEINLINE class UALSCharacterMovementComponent* GetMyMovementComponent() const
	{
		return MyCharacterMovementComponent;
	}

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void PreInitializeComponents() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "ALS|Custom")
	const bool IsAlive() const { return HealthValue > 0.f; }

	/** Ragdoll System */

	/** Implement on BP to get required get up animation according to character's state */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Ragdoll System")
	UAnimMontage* GetGetUpAnimation(bool bRagdollFaceUpState);

	UFUNCTION(BlueprintCallable, Category = "ALS|Ragdoll System")
	virtual void RagdollStart();

	UFUNCTION(BlueprintCallable, Category = "ALS|Ragdoll System")
	virtual void RagdollEnd();

	UFUNCTION(BlueprintCallable, Server, Unreliable, Category = "ALS|Ragdoll System")
	void Server_SetMeshLocationDuringRagdoll(FVector MeshLocation);

	/** Character States */

	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void SetMovementState(EALSMovementState NewState);

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EALSMovementState GetMovementState() const { return MovementState; }

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EALSMovementState GetPrevMovementState() const { return PrevMovementState; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void SetMovementAction(EALSMovementAction NewAction);

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EALSMovementAction GetMovementAction() const { return MovementAction; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void SetStance(EALSStance NewStance);

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EALSStance GetStance() const { return Stance; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void SetGait(EALSGait NewGait);

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EALSGait GetGait() const { return Gait; }

	UFUNCTION(BlueprintGetter, Category = "ALS|CharacterStates")
	EALSGait GetDesiredGait() const { return DesiredGait; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void SetRotationMode(EALSRotationMode NewRotationMode);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Character States")
	void Server_SetRotationMode(EALSRotationMode NewRotationMode);

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EALSRotationMode GetRotationMode() const { return RotationMode; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void SetViewMode(EALSViewMode NewViewMode);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Character States")
	void Server_SetViewMode(EALSViewMode NewViewMode);

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EALSViewMode GetViewMode() const { return ViewMode; }

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Character States")
	void Server_SetIsAimed(bool NewAimed);

	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void SetOverlayState(EALSOverlayState NewState);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Character States")
	void Server_SetOverlayState(EALSOverlayState NewState);

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EALSOverlayState GetOverlayState() const { return OverlayState; }

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EALSOverlayState SwitchRight() const { return OverlayState; }


	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EALSViewMode GetCameraMode() const { return CameraMode; }
	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void SetCameraMode(EALSViewMode Value) { CameraMode = Value; }
	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void SetTargetCameraMode(EALSViewMode Value) { TargetCameraMode = Value; }
	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	EALSViewMode GetTargetCameraMode() const { return TargetCameraMode; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	const EALSTeam GetTeam() const { return Team; };

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	UCameraComponent* GetCamera();

	/** Landed, Jumped, Rolling, Mantling and Ragdoll*/
	/** On Landed*/
	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void EventOnLanded();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ALS|Character States")
	void Multicast_OnLanded();

	/** On Jumped*/
	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void EventOnJumped();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ALS|Character States")
	void Multicast_OnJumped();

	/** Rolling Montage Play Replication*/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Character States")
	void Server_PlayMontage(UAnimMontage* Montage, float PlayRate);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ALS|Character States")
	void Multicast_PlayMontage(UAnimMontage* Montage, float PlayRate);

	/** Ragdolling*/
	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void ReplicatedRagdollStart();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Character States")
	void Server_RagdollStart();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ALS|Character States")
	void Multicast_RagdollStart();

	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void ReplicatedRagdollEnd();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Character States")
	void Server_RagdollEnd(FVector CharacterLocation);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ALS|Character States")
	void Multicast_RagdollEnd(FVector CharacterLocation);

	/** Input */

	UPROPERTY(BlueprintAssignable, Category = "ALS|Input")
	FJumpPressedSignature JumpPressedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "ALS|Input")
	FRagdollStateChangedSignature RagdollStateChangedDelegate;

	UFUNCTION(BlueprintGetter, Category = "ALS|Input")
	EALSStance GetDesiredStance() const { return DesiredStance; }

	UFUNCTION(BlueprintSetter, Category = "ALS|Input")
	void SetDesiredStance(EALSStance NewStance);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Input")
	void Server_SetDesiredStance(EALSStance NewStance);

	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void SetDesiredGait(EALSGait NewGait);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Character States")
	void Server_SetDesiredGait(EALSGait NewGait);

	UFUNCTION(BlueprintGetter, Category = "ALS|Input")
	EALSRotationMode GetDesiredRotationMode() const { return DesiredRotationMode; }

	UFUNCTION(BlueprintSetter, Category = "ALS|Input")
	void SetDesiredRotationMode(EALSRotationMode NewRotMode);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Character States")
	void Server_SetDesiredRotationMode(EALSRotationMode NewRotMode);

	UFUNCTION(BlueprintCallable, Category = "ALS|Input")
	FVector GetPlayerMovementInput() const;

	/** Rotation System */

	UFUNCTION(BlueprintCallable, Category = "ALS|Rotation System")
	void SetActorLocationAndTargetRotation(FVector NewLocation, FRotator NewRotation);

	/** Movement System */

	UFUNCTION(BlueprintGetter, Category = "ALS|Movement System")
	bool HasMovementInput() const { return bHasMovementInput; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void SetHasMovementInput(bool bNewHasMovementInput);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	FALSMovementSettings GetTargetMovementSettings() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	EALSGait GetAllowedGait() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	EALSGait GetActualGait(EALSGait AllowedGait) const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	bool CanSprint() const;

	/** BP implementable function that called when Breakfall starts */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void OnBreakfall();
	virtual void OnBreakfall_Implementation();

	/** BP implementable function that called when A Montage starts, e.g. during rolling */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void Replicated_PlayMontage(UAnimMontage* Montage, float PlayRate);
	virtual void Replicated_PlayMontage_Implementation(UAnimMontage* Montage, float PlayRate);

	/** Implement on BP to get required roll animation according to character's state */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	UAnimMontage* GetRollAnimation();

	/** Utility */

	UFUNCTION(BlueprintCallable, Category = "ALS|Utility")
	UALSCharacterAnimInstance* GetMainAnimInstance() { return MainAnimInstance; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Utility")
	float GetAnimCurveValue(FName CurveName) const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Utility")
	void SetVisibleMesh(USkeletalMesh* NewSkeletalMesh);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Utility")
	void Server_SetVisibleMesh(USkeletalMesh* NewSkeletalMesh);

	/** Camera System */

	UFUNCTION(BlueprintGetter, Category = "ALS|Camera System")
	bool IsRightShoulder() const { return bRightShoulder; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera System")
	void SetRightShoulder(bool bNewRightShoulder);

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera System")
	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius);

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera System")
	virtual FTransform GetThirdPersonPivotTarget();

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera System")
	virtual FVector GetFirstPersonCameraTarget();

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera System")
	void GetCameraParameters(float& TPFOVOut, float& FPFOVOut, bool& bRightShoulderOut) const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera System")
	void SetCameraBehavior(UALSPlayerCameraBehavior* CamBeh) { CameraBehavior = CamBeh; }

	/** Essential Information Getters/Setters */

	UFUNCTION(BlueprintGetter, Category = "ALS|Essential Information")
	FVector GetAcceleration() const { return Acceleration; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	void SetAcceleration(const FVector& NewAcceleration);

	UFUNCTION(BlueprintGetter, Category = "ALS|Essential Information")
	bool IsMoving() const { return bIsMoving; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	void SetIsMoving(bool bNewIsMoving);

	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	FVector GetMovementInput() const;

	UFUNCTION(BlueprintGetter, Category = "ALS|Essential Information")
	float GetMovementInputAmount() const { return MovementInputAmount; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	void SetMovementInputAmount(float NewMovementInputAmount);

	UFUNCTION(BlueprintGetter, Category = "ALS|Essential Information")
	float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	void SetSpeed(float NewSpeed);

	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	FRotator GetAimingRotation() const { return AimingRotation; }

	UFUNCTION(BlueprintGetter, Category = "ALS|Essential Information")
	float GetAimYawRate() const { return AimYawRate; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	void SetAimYawRate(float NewAimYawRate);

	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	void GetControlForwardRightVector(FVector& Forward, FVector& Right) const;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void BeginAimCamera_BP();
	virtual void BeginAimCamera_BP_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void EndAimCamera_BP();
	virtual void EndAimCamera_BP_Implementation() {}


protected:
	/** Ragdoll System */

	void RagdollUpdate(float DeltaTime);

	void SetActorLocationDuringRagdoll(float DeltaTime);

	/** State Changes */

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	virtual void OnMovementStateChanged(EALSMovementState PreviousState);

	virtual void OnMovementActionChanged(EALSMovementAction PreviousAction);

	virtual void OnStanceChanged(EALSStance PreviousStance);

	virtual void OnRotationModeChanged(EALSRotationMode PreviousRotationMode);
	
	virtual void OnGaitChanged(EALSGait PreviousGait);

	virtual void OnIsAimedChanged(bool PreviousIsAimed);
	virtual void OnHealthChanged(float PrevHealth);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Custom")
	void HealthChanged_BP();
	virtual void HealthChanged_BP_Implementation();

	UFUNCTION(BlueprintCallable, Server, Unreliable, Category = "ALS|Custom")
	void Server_DOHealthChanged(float DamagePower);
	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable, Category = "ALS|Custom")
	void Multicast_Die();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Custom")
	void Die_BP();
	virtual void Die_BP_Implementation() {}


	virtual void OnViewModeChanged(EALSViewMode PreviousViewMode);
	virtual void OnFixedAimViewModeChanged(const EALSViewMode PreviousViewMode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void OnViewModeChanged_Native(EALSViewMode NewViewMode);
	virtual void OnViewModeChanged_Native_Implementation(EALSViewMode NewViewMode);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ALS|Character States")
	void Multicast_OnViewModeChanged(EALSViewMode NewViewMode);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void OnViewModeChanged_BP();
	virtual void OnViewModeChanged_BP_Implementation() {}


	virtual void OnOverlayStateChanged(const EALSOverlayState PreviousState);
	virtual void OnCameraModeChanged(const EALSViewMode PreviousState);
	virtual void OnTargetCameraModeChanged(const EALSViewMode PreviousState);

	virtual void OnVisibleMeshChanged(const USkeletalMesh* PreviousSkeletalMesh);

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnJumped_Implementation() override;

	virtual void Landed(const FHitResult& Hit) override;

	void OnLandFrictionReset();

	void SetEssentialValues(float DeltaTime);

	void UpdateCharacterMovement();

	void UpdateGroundedRotation(float DeltaTime);

	void UpdateInAirRotation(float DeltaTime);

	/** Utils */

	void SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed, float DeltaTime);

	float CalculateGroundedRotationRate() const;

	void LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed, float DeltaTime);

	void SetMovementModel();

	/** Input */

	void PlayerForwardMovementInput(float Value);
	void PlayerRightMovementInput(float Value);

	void PlayerCameraUpInput(float Value);
	void PlayerCameraRightInput(float Value);

	void JumpPressedAction();
	void JumpReleasedAction();

	void SprintPressedAction();
	void SprintReleasedAction();

	void AimPressed() {}
	void AimReleased() {}

	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	void AimPressedAction();

	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	void AimReleasedAction();

	UFUNCTION(BlueprintCallable, Server, Unreliable, Category = "ALS|Ragdoll System")
	void Server_AimPressedAction();


	void CameraPressedAction();
	void CameraReleasedAction();

	void JetPressedAction();
	void JetReleasedAction();

	UFUNCTION()
	void InterractPressedAction();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ALS|Character States")
	void Multicast_InterractPressed();

	void InterractReleasedAction();

	void OnSwitchCameraMode();


	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	void OnFPCameraMode();
	UFUNCTION(BlueprintCallable, Category = "ALS|Essential Information")
	void OnTPCameraMode();


	void OnJetPack();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void OnJetPackActivated();
	virtual void OnJetPackActivated_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Movement System")
	void OnJetPackDeactivated();
	virtual void OnJetPackDeactivated_Implementation();


	void StancePressedAction();
	void StanceReleasedAction();

	void WalkPressedAction();
	void WalkReleasedAction();

	void RagdollPressedAction();

	void VelocityDirectionPressedAction();
	void LookingDirectionPressedAction();

	/** Replication */
	UFUNCTION()
	void OnRep_IsAimed(bool PrevAimed);

	UFUNCTION()
	void OnRep_HealthValue(float PrevHealth);

	UFUNCTION()
	void OnRep_MovementAction(EALSMovementAction PrevRotMode);

	UFUNCTION(Category = "ALS|Replication")
	void OnRep_RotationMode(EALSRotationMode PrevRotMode);

	UFUNCTION(Category = "ALS|Replication")
	void OnRep_MovementState(EALSMovementState PrevMovementState);

	UFUNCTION(Category = "ALS|Replication")
	void OnRep_ViewMode(EALSViewMode PrevViewMode);

	UFUNCTION(Category = "ALS|Replication")
	void OnRep_OverlayState(EALSOverlayState PrevOverlayState);

	UFUNCTION(Category = "ALS|Replication")
	void OnRep_VisibleMesh(USkeletalMesh* NewVisibleMesh);

	UFUNCTION()
	void OnRep_CameraMode(EALSViewMode PrevViewMode);

	UFUNCTION()
	void OnRep_TargetCameraMode(EALSViewMode PrevViewMode);

	/** Replication */

	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	virtual void UsingAnotherPawn_BP(AActor* Vehicle) {}

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|Character States")
	const FName GetWeaponCameraSocket() const;

protected:
	/* Custom movement component*/
	UPROPERTY()
	UALSCharacterMovementComponent* MyCharacterMovementComponent;

	/** Input */

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "ALS|Input")
	EALSRotationMode DesiredRotationMode = EALSRotationMode::LookingDirection;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "ALS|Input")
	EALSGait DesiredGait = EALSGait::Running;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "ALS|Input")
	EALSStance DesiredStance = EALSStance::Standing;

	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float LookUpDownRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float LookLeftRightRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float RollDoubleTapTimeout = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float ViewModeSwitchHoldTime = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float JetHoldTime = 0.2f;

	UPROPERTY(Category = "ALS|Input", BlueprintReadOnly)
	int32 TimesPressedStance = 0;

	UPROPERTY(Category = "ALS|Input", BlueprintReadOnly)
	bool bBreakFall = false;

	UPROPERTY(Category = "ALS|Input", BlueprintReadOnly)
	bool bSprintHeld = false;

	/** Camera System */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Camera System")
	float ThirdPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Camera System")
	float FirstPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Camera System")
	bool bRightShoulder = false;

	/** State Values */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|State Values", ReplicatedUsing = OnRep_OverlayState)
	EALSOverlayState OverlayState = EALSOverlayState::Default;

	/** Movement System */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Movement System")
	FDataTableRowHandle MovementModel;

	/** Essential Information */

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	FVector Acceleration = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	bool bIsMoving = false;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	bool bHasMovementInput = false;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	FRotator LastVelocityRotation;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	FRotator LastMovementInputRotation;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	float MovementInputAmount = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	float AimYawRate = 0.0f;

	/** Replicated Essential Information*/

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	float EasedMaxAcceleration = 0.0f;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "ALS|Essential Information")
	FVector ReplicatedCurrentAcceleration = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "ALS|Essential Information")
	FRotator ReplicatedControlRotation = FRotator::ZeroRotator;

	/** Replicated Skeletal Mesh Information*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Skeletal Mesh", ReplicatedUsing = OnRep_VisibleMesh)
	USkeletalMesh* VisibleMesh = nullptr;

	/** State Values */

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values", ReplicatedUsing = OnRep_MovementState)
	EALSMovementState MovementState = EALSMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EALSMovementState PrevMovementState = EALSMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EALSMovementAction MovementAction = EALSMovementAction::None;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values", ReplicatedUsing = OnRep_RotationMode)
	EALSRotationMode RotationMode = EALSRotationMode::LookingDirection;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EALSGait Gait = EALSGait::Walking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|State Values")
	EALSStance Stance = EALSStance::Standing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|State Values", ReplicatedUsing = OnRep_ViewMode)
	EALSViewMode ViewMode = EALSViewMode::ThirdPerson;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|State Values", ReplicatedUsing = OnRep_ViewMode)
	EALSViewMode FixedAimViewMode = EALSViewMode::ThirdPerson;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|State Values", ReplicatedUsing = OnRep_CameraMode)
	EALSViewMode CameraMode = EALSViewMode::ThirdPerson;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|State Values", ReplicatedUsing = OnRep_TargetCameraMode)
	EALSViewMode TargetCameraMode = EALSViewMode::ThirdPerson;

	/** Movement System */

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Movement System")
	FALSMovementStateSettings MovementData;

	/** Rotation System */

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Rotation System")
	FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Rotation System")
	FRotator InAirRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Rotation System")
	float YawOffset = 0.0f;

	/** Breakfall System */

	/** If player hits to the ground with a specified amount of velocity, switch to breakfall state */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "ALS|Breakfall System")
	bool bBreakfallOnLand = true;

	/** If player hits to the ground with an amount of velocity greater than specified value, switch to breakfall state */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "ALS|Breakfall System", meta = (EditCondition =
	        "bBreakfallOnLand"))
	float BreakfallOnLandVelocity = 600.0f;

	/** Ragdoll System */

	/** If the skeleton uses a reversed pelvis bone, flip the calculation operator */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "ALS|Ragdoll System")
	bool bReversedPelvis = false;

	/** If player hits to the ground with a specified amount of velocity, switch to ragdoll state */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "ALS|Ragdoll System")
	bool bRagdollOnLand = false;

	/** If player hits to the ground with an amount of velocity greater than specified value, switch to ragdoll state */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "ALS|Ragdoll System", meta = (EditCondition =
	        "bRagdollOnLand"))
	float RagdollOnLandVelocity = 1000.0f;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Ragdoll System")
	bool bRagdollOnGround = false;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Ragdoll System")
	bool bRagdollFaceUp = false;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Ragdoll System")
	FVector LastRagdollVelocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "ALS|Ragdoll System")
	FVector TargetRagdollLocation = FVector::ZeroVector;

	/* Server ragdoll pull force storage*/
	float ServerRagdollPull = 0.0f;

	/* Dedicated server mesh default visibility based anim tick option*/
	EVisibilityBasedAnimTickOption DefVisBasedTickOp;

	/** Cached Variables */

	FVector PreviousVelocity = FVector::ZeroVector;

	float PreviousAimYaw = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Utility")
	UALSCharacterAnimInstance* MainAnimInstance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Camera")
	UALSPlayerCameraBehavior* CameraBehavior;

	/** Last time the 'first' crouch/roll button is pressed */
	float LastStanceInputTime = 0.0f;

	/** Last time the camera action button is pressed */
	float CameraActionPressedTime = 0.0f;
	float JetPressedTime = 0.0f;

	/* Timer to manage camera mode swap action */
	FTimerHandle OnCameraModeSwapTimer;
	FTimerHandle OnJetTimer;

	/* Timer to manage reset of braking friction factor after on landed event */
	FTimerHandle OnLandedFrictionResetTimer;

	/* Smooth out aiming by interping control rotation*/
	FRotator AimingRotation = FRotator::ZeroRotator;

	/** We won't use curve based movement and a few other features on networked games */
	bool bEnableNetworkOptimizations = false;

private:
	UALSDebugComponent* DebugComponent = nullptr;
};
