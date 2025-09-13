// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UEnums/EClass.h"
#include "UStructs/FPlayerCharacterStat.h"
#include "ClassSelectionWidget.generated.h"

/**
 * 
 */

UCLASS()
class ENDLESSSPIRE_API UClassSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UClassSelectionWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EClass SelectClass;

protected:

	virtual void NativeOnInitialized();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Warrior;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Mage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Archer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Cleric;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_OK;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_CANCLE;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Remove;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* VerticalBox_CheckTheSelectedClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USpacer* Spacer;

	UDataTable* ClassStatDataTable;
	FPlayerCharacterStat ClassStat;

private:

	UFUNCTION(BlueprintCallable)
	void WarriorButtonCallback();

	UFUNCTION(BlueprintCallable)
	void MageButtonCallback();

	UFUNCTION(BlueprintCallable)
	void ArcherButtonCallback();

	UFUNCTION(BlueprintCallable)
	void ClericButtonCallback();

	UFUNCTION(BlueprintCallable)
	void OkButtonCallback();

	UFUNCTION(BlueprintCallable)
	void CancleButtonCallback();

	UFUNCTION(BlueprintCallable)
	void RemoveButtonCallback();
	
};
