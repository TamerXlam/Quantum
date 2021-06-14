// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultLevelSequence.h"

#include "UObject/Package.h"
#include "MovieScene.h"

#include "Engine/Engine.h"

UDefaultLevelSequence::UDefaultLevelSequence(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDefaultLevelSequence::PreSave(const class ITargetPlatform* TargetPlatform)
{

	UMovieSceneSignedObject::PreSave(TargetPlatform);

	//Super::PreSave(TargetPlatform);

}

void UDefaultLevelSequence::PostInitProperties()
{

	UObject::PostInitProperties();

}

void UDefaultLevelSequence::PostLoad()
{

	ULevelSequence::PostLoad();

}

void UDefaultLevelSequence::Serialize(FStructuredArchive::FRecord Record)
{

	UMovieSceneSignedObject::Serialize(Record);

	bool bIsCookedForEditor = false;
#if WITH_EDITORONLY_DATA
	bIsCookedForEditor = GetOutermost()->bIsCookedForEditor;
#endif // WITH_EDITORONLY_DATA

}

void UDefaultLevelSequence::Initialize()
{
	Super::Initialize();

	//
}

void UDefaultLevelSequence::BindPossessableObject(const FGuid& ObjectId, UObject& PossessedObject, UObject* Context)
{

	Super::BindPossessableObject(ObjectId, PossessedObject, Context);


}

bool UDefaultLevelSequence::CanPossessObject(UObject& Object, UObject* InPlaybackContext) const
{
	return true;
}

UObject* UDefaultLevelSequence::GetParentObject(UObject* Object) const
{
	return ULevelSequence::GetParentObject(Object);
}

void UDefaultLevelSequence::UnbindPossessableObjects(const FGuid& ObjectId)
{

	ULevelSequence::UnbindPossessableObjects(ObjectId);

}

UObject* UDefaultLevelSequence::MakeSpawnableTemplateFromInstance(UObject& InSourceObject, FName ObjectName)
{
	UObject* NewInstance = NewObject<UObject>(MovieScene, InSourceObject.GetClass(), ObjectName);

	UEngine::FCopyPropertiesForUnrelatedObjectsParams CopyParams;
	CopyParams.bNotifyObjectReplacement = false;
	UEngine::CopyPropertiesForUnrelatedObjects(&InSourceObject, NewInstance, CopyParams);

	return NewInstance;
}

bool UDefaultLevelSequence::CanAnimateObject(UObject& InObject) const
{
	return Super::CanAnimateObject(InObject);
}
