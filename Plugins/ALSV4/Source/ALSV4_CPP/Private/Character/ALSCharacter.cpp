
#include "Character/ALSCharacter.h"

#include "Engine/StaticMesh.h"
#include "AI/ALSAIController.h"
#include "Kismet/GameplayStatics.h"

AALSCharacter::AALSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{


	AIControllerClass = AALSAIController::StaticClass();

	if (GetWorld() && GetWorld()->IsServer())
	{
		SkinIndex = FMath::RandHelper(3);
	}

}


void AALSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AALSCharacter, SkinIndex);

}

void AALSCharacter::ClearHeldObject()
{

}

void AALSCharacter::AddToBlackTeam()
{
	Team = EALSTeam::Black;

	CameraMode = EALSViewMode::ThirdPerson;

	SkinIndex = FMath::RandHelper(3);
}


void AALSCharacter::AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, UClass* NewAnimClass,
                                 bool bLeftHand, FVector Offset)
{
	ClearHeldObject();



	FName AttachBone;
	if (bLeftHand)
	{
		AttachBone = TEXT("VB LHS_ik_hand_gun");
	}
	else
	{
		AttachBone = TEXT("VB RHS_ik_hand_gun");
	}

}

void AALSCharacter::RagdollStart()
{
	ClearHeldObject();
	Super::RagdollStart();
}

void AALSCharacter::RagdollEnd()
{
	Super::RagdollEnd();
	UpdateHeldObject();
}

ECollisionChannel AALSCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	const FName CameraSocketName = bRightShoulder ? TEXT("TP_CameraTrace_R") : TEXT("TP_CameraTrace_L");
	TraceOrigin = GetMesh()->GetSocketLocation(CameraSocketName);
	TraceRadius = 15.0f;
	return ECC_Camera;
}

FTransform AALSCharacter::GetThirdPersonPivotTarget()
{
	return FTransform(GetActorRotation(),
	                  (GetMesh()->GetSocketLocation(TEXT("Head")) + GetMesh()->GetSocketLocation(TEXT("root"))) / 2.0f,
	                  FVector::OneVector);
}

FVector AALSCharacter::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(TEXT("FP_Camera"));
}

void AALSCharacter::OnOverlayStateChanged(EALSOverlayState PreviousState)
{
	Super::OnOverlayStateChanged(PreviousState);
	UpdateHeldObject();
}

void AALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHeldObjectAnimations();
}

void AALSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHeldObject();
}


bool AALSCharacter::HasAimingCameraTarget(FTransform& NewTransform)
{

	AActor* Weapon = GetActiveWeapon();
	if (!IsValid(Weapon))
	{
		return false;
	}

	TInlineComponentArray<USkeletalMeshComponent*> Meshes;
	Weapon->GetComponents(Meshes);

	for (int i = 0; i != Meshes.Num(); i++)
	{

		if (Meshes[i]->DoesSocketExist("AimPoint"))
		{
			NewTransform = Meshes[i]->GetSocketTransform("AimPoint");
			return true;
		}



	}

	return false;

}


void AALSCharacter::UsingAnotherPawn_BP(AActor* Vehicle)
{



	///


	if (HasAuthority())
	{

		Multicast_UsingAnotherPawn_BP(Vehicle);

	}
	else
	{

		Server_UsingAnotherPawn_BP(Vehicle);

	}


	//
}


void AALSCharacter::Server_UsingAnotherPawn_BP_Implementation(AActor* Vehicle)
{


	Multicast_UsingAnotherPawn_BP(Vehicle);


}

void AALSCharacter::Multicast_UsingAnotherPawn_BP_Implementation(AActor* Vehicle)
{




	//AttachToActor(Vehicle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	if (IsValid(GetController()) && IsValid(Vehicle))
	{


		GetController()->Possess((Cast<ACharacter>(Vehicle)));
		AttachToComponent(Vehicle->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	}

	UCapsuleComponent* MyCapsule = GetCapsuleComponent();
	if (IsValid(MyCapsule))
	{

		MyCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MyCapsule->SetMassScale(FName("root"), 0.f);
	}

	USkeletalMeshComponent* SMesh = GetMesh();
	if (IsValid(SMesh))
	{

		SMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (IsValid(GetCamera()))
	{
		GetCamera()->Deactivate();
	}


	UALSCharacterMovementComponent* Movement = Cast<UALSCharacterMovementComponent>(GetMovementComponent());
	if (IsValid(Movement))
	{

		Movement->StopMovementImmediately();
		Movement->GravityScale = 0.f;
		Movement->SetMovementMode(EMovementMode::MOVE_Custom);

	}

	ABaseLevelScriptActor* MyLBP = Cast<ABaseLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (IsValid(MyLBP))
	{
		//MyLBP->UsingAnotherPawn_BP(this, Vehicle);
	}





}