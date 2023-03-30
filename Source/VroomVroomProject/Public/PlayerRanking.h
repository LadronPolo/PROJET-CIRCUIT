#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Pawn.h"
#include "PlayerRanking.generated.h"

USTRUCT(BlueprintType)
struct FPlayerRanking
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int laps;

    UPROPERTY(BlueprintReadWrite)
    int checkpoint;

    UPROPERTY(BlueprintReadWrite)
    float distance;

    UPROPERTY(BlueprintReadWrite)
    APawn* playerController;

    UPROPERTY(BlueprintReadWrite)
    int idx;

    FPlayerRanking()
        : FPlayerRanking(0, 0, 0, 0, nullptr)
    {}

    FPlayerRanking(int pLaps, int pCheckPoint, float pDist, int pIdx, APawn* pPlayerController)
        : laps(pLaps), checkpoint(pCheckPoint), distance(pDist), idx(pIdx), playerController(pPlayerController)
    {}

    FPlayerRanking(const FPlayerRanking& Other)
        : FPlayerRanking(Other.laps, Other.checkpoint, Other.distance, Other.idx, Other.playerController)
    {}

    bool operator==(const FPlayerRanking& Other) const
    {
        return Equals(Other);
    }

    bool Equals(const FPlayerRanking& Other) const
    {
        return laps == Other.laps && checkpoint == Other.checkpoint && distance == Other.distance && playerController == Other.playerController && idx == Other.idx;
    }

};