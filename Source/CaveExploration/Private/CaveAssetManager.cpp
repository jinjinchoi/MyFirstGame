// Copyright Jonwoo-Choi


#include "CaveAssetManager.h"

#include "CaveGameplayTags.h"

UCaveAssetManager& UCaveAssetManager::Get()
{
	check(GEngine);

	return *Cast<UCaveAssetManager>(GEngine->AssetManager);
}

void UCaveAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCaveGameplayTags::InitializeNativeGameplayTags();
}
