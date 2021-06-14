// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"
#include "LevelSequence.h"
#include "DefaultLevelSequence.generated.h"

class UMovieScene;
class UAnimSequence;
class USkeletalMesh;
/**
 * 
 */
UCLASS(ClassGroup = "TheGame|Default", DisplayName = "DefaultLevelSequence", BlueprintType, Blueprintable)
class THEGAME_API UDefaultLevelSequence : public ULevelSequence
{

	GENERATED_BODY()
	
public:
	UDefaultLevelSequence(const FObjectInitializer& ObjectInitializer);

	//virtual void Serialize(FArchive& Ar) override {}
	virtual void Serialize(FStructuredArchive::FRecord Record) override;
	virtual bool NeedsLoadForClient() const override { return true; }
	virtual bool NeedsLoadForEditorGame() const override { return true;	}

	virtual void PostInitProperties() override;
	virtual void PostLoad() override;

	virtual void PreSave(const class ITargetPlatform* TargetPlatform);

	// ULevelSequence overrides
	virtual void Initialize() override;

	// UMovieSceneSequence overrides
	virtual void BindPossessableObject(const FGuid& ObjectId, UObject& PossessedObject, UObject* Context) override;
	virtual bool CanPossessObject(UObject& Object, UObject* InPlaybackContext) const override;
	virtual UObject* GetParentObject(UObject* Object) const override;
	virtual bool AllowsSpawnableObjects() const { return true; }
	virtual void UnbindPossessableObjects(const FGuid& ObjectId) override;
	virtual bool CanRebindPossessable(const FMovieScenePossessable& InPossessable) const override { return false; }
	virtual UObject* MakeSpawnableTemplateFromInstance(UObject& InSourceObject, FName ObjectName) override;
	virtual bool CanAnimateObject(UObject& InObject) const override;

#if WITH_EDITOR
//	virtual void GetAssetRegistryTagMetadata(TMap<FName, FAssetRegistryTagMetadata>& OutMetadata) const override { Super::GetAssetRegistryTagMetadata(OutMetadata); }
//	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override { Super::GetAssetRegistryTags(OutTags); }
//
	virtual bool Modify(bool bAlwaysMarkDirty = true) override { return true; }
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override {  }
	virtual void PostEditUndo() override {  }
	virtual void PostEditUndo(TSharedPtr<ITransactionObjectAnnotation> TransactionAnnotation) override {  }
#endif

protected:
	// Called when the game starts
	//virtual void BeginPlay() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
