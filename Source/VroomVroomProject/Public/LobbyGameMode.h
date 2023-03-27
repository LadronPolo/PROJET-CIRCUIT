// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <PlayerRanking.h>
#include "LobbyGameMode.generated.h"


/**
 * 
 */
UCLASS()
class VROOMVROOMPROJECT_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exitting) override;

	UFUNCTION(BlueprintCallable)
	static void sort(UPARAM(ref) TArray<FPlayerRanking>& pPlayers, int totalLaps)
	{
		//Algo::SortBy(pPlayers, &FPlayerRanking::distance, TGreater<>());

		pPlayers.Sort([totalLaps](const FPlayerRanking& ip1, const FPlayerRanking& ip2) {
			
			if (ip1.laps == totalLaps)
				return true;

			if (ip1.laps == ip2.laps && ip1.checkpoint == ip2.checkpoint)
				return ip1.distance < ip2.distance;
			else if (ip1.laps == ip2.laps)
				return ip1.checkpoint > ip2.checkpoint;
			else
				return ip1.laps > ip2.laps;
		});
	}
};
