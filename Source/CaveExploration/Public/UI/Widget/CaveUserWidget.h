// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CaveUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API UCaveUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	// 위젯 컨트롤러가 생성되면 실행
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
