#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API UPlayerInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	void LoadPlayerInfo();

	void LoadYogaMap();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Transient)
	TObjectPtr<class UEditableTextBox> PlayerNameTextBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Transient)
	TObjectPtr<class UEditableTextBox> PlayerGenderTextBox;
};
