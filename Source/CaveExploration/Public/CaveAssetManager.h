// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CaveAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UCaveAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};
