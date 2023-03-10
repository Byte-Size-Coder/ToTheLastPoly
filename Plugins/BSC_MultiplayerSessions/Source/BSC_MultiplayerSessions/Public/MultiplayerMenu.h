// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class BSC_MULTIPLAYERSESSIONS_API UMultiplayerMenu : public UUserWidget
{
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintCallable)
		void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/ThirdPerson/Maps/Lobby")));

	protected:
		virtual bool Initialize() override;
		virtual void NativeDestruct() override;

		//
		// Callbacks for the custom delegates on the MultiplayerSessionsSubsystyem
		//
		UFUNCTION()
		void OnCreateSession(bool bWasSuccessful);
		UFUNCTION()
		void OnDestroySession(bool bWasSuccessful);
		UFUNCTION()
		void OnStartSession(bool bWasSuccessful);

		void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
		void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

	private:
		UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

		UPROPERTY(meta = (BindWidget))
		UButton* JoinButton;

		UFUNCTION()
		void HostButtonClicked();
		
		UFUNCTION()
		void JoinButtonClicked();

		void MenuTearDown();

		// The subsytem designed to handle all online session functionality
		class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

		int32 NumPublicConnections {4};
		FString MatchType {TEXT("FreeForAll")};
		FString PathToLobby{TEXT("")};
};