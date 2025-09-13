// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllers/PlayPlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API ALobbyPlayerController : public APlayPlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController();


// Functions
#pragma region Overrides
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	virtual void SetupInputComponent() override;

// UserFunction Overrides

public:
	virtual void InitAllIMC() override;
	virtual void DeleteAllIMC() override;

#pragma endregion

#pragma region InputMappingFunctions
public:
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill1BeginClicked();
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill1Triggered(const FInputActionInstance& Instance);
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill1Holding(const struct FInputActionInstance& Instance);
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill1Cancled();
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill1Completed();

	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill2BeginClicked();
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill2Triggered(const FInputActionInstance& Instance);
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill2Holding(const struct FInputActionInstance& Instance);
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill2Cancled();
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill2Completed();

	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill3BeginClicked();
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill3Triggered(const FInputActionInstance& Instance);
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill3Holding(const struct FInputActionInstance& Instance);
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill3Cancled();
	UFUNCTION(Category = "InputMapping")
	virtual void OnSkill3Completed();

	UFUNCTION(Category = "InputMapping")
	virtual void OnIdentitySkillBeginClicked();
	UFUNCTION(Category = "InputMapping")
	virtual void OnIdentitySkillTriggered(const FInputActionInstance& Instance);
	UFUNCTION(Category = "InputMapping")
	virtual void OnIdentitySkillHolding(const struct FInputActionInstance& Instance);
	UFUNCTION(Category = "InputMapping")
	virtual void OnIdentitySkillCancled();
	UFUNCTION(Category = "InputMapping")
	virtual void OnIdentitySkillCompleted();

	UFUNCTION(Category = "InputMapping")
	virtual void OnEvadeBeginClicked();
	UFUNCTION(Category = "InputMapping")
	virtual void OnEvadeEndClicked();

	UFUNCTION(Category = "InputMapping")
	virtual void OnInteractBeginClicked();
	
	UFUNCTION(Category = "InputMapping")
	virtual void OnInteractEndClicked();

	//0317 박진삼 UI 키 추가
	UFUNCTION(Category = "InputMapping")
	virtual void OnShowUIBeginClicked();

	//0320 김선우 UI 키 추가
	UFUNCTION(Category = "InputMapping")
	virtual void ToggleMenu();

	//0410 김선우 장비창 키 추가
	UFUNCTION(Category = "InputMapping")
	virtual void ToggleEquipmentMenu();

#pragma endregion

// Properties
#pragma region InputMappingProperties
public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* CombatMappingContext;
	// 0317 박진삼 UI 키 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* ShowUIMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Skill1;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill2;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IdentitySkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Evade;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Interact;

	/* ShowUI Input Action 0317 박진삼 UI 키 추가 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* StatusUI;

	/* 0320 김선우 메뉴토글 테스트 키 추가 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MenuToggle;

	/* 0410 김선우 장비창토글 테스트 키 추가 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipmentToggle;

#pragma endregion

public:

	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Player Stats")
	FGuid PlayerGuid;

	UFUNCTION()
	void SetGuid(FGuid currentguid);

	UFUNCTION(Client,Reliable)
	void OwnerSetInfo();

	UFUNCTION(Client, Reliable)
	void OwnerSaveInfo(const EClass playerclass);

	UFUNCTION(Client, Reliable)
	void OwnerUpdatePlayerStat(FPlayerCharacterStat OwnPlayerStat);

	UFUNCTION(Client, Reliable)
	void OwnerUpdateOtherPlayerStat(FPlayerCharacterStat OtherPlayerStat, FGuid OtherPlayerGuid);

	UFUNCTION(Client, Reliable)
	void OwnerSetStatus(const FString& playername, const EClass& playerclass);

	UFUNCTION(Server, Reliable)
	void ServerUpdateNickname(const FString& playernickname);
	
	UFUNCTION(Server, Reliable)
	void ServerUpdateClass(const EClass& playerclass);

	UFUNCTION(Server, Reliable)
	void ServerUpdateStat(const FPlayerCharacterStat& playerstat);

	UFUNCTION(Server, Reliable)
	void ServerUpdateInventory(const TArray<FItemData>& playeritems, const int32& playermoney);

	UFUNCTION(Server, Reliable)
	void ServerShowNickname(const FString& playernickname);

	UFUNCTION(Server, Reliable)
	void SetVote(int32 result);

	UFUNCTION(Server, Reliable)
	void ClearVote();

private:

	bool IsStatusUIVisble;
	bool IsInventoryVisble;
	bool IsEquipmentMenuVisible;
};
