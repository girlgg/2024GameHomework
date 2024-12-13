#include "ScoreBoxManager.h"

#include "Course01/Items/ScoreCube.h"


UScoreBoxManager::UScoreBoxManager()
{
}

void UScoreBoxManager::AddScoreCube(AScoreCube* ScoreCube)
{
	ScoreCubes.AddUnique(ScoreCube);
}

void UScoreBoxManager::AddCubeSpawnLocation(FVector InLocation)
{
	SpawnLocation.AddUnique(InLocation);
}

void UScoreBoxManager::SpawnRandomCube(bool bIsImport)
{
	if (ScoreCubeClass && !SpawnLocation.IsEmpty())
	{
		FVector Location = SpawnLocation[FMath::RandRange(0, SpawnLocation.Num() - 1)];
		AScoreCube* NewCube = GetWorld()->SpawnActor<AScoreCube>(ScoreCubeClass, Location, FRotator::ZeroRotator);
		if (NewCube)
		{
			AddScoreCube(NewCube);
			NewCube->CubeBaseScore = CubeBaseScore;
			NewCube->CubeScale = CubeScale;
			NewCube->HitTimes = HitTimes;
			NewCube->bIsImport = bIsImport;
			if (bIsImport)
			{
				UMaterialInstanceDynamic* DynamicMaterial = NewCube->Cube->CreateAndSetMaterialInstanceDynamic(0);
				if (DynamicMaterial)
				{
					DynamicMaterial->SetVectorParameterValue(FName("Base Color"), FLinearColor::Red);
				}
			}
		}
	}
}

void UScoreBoxManager::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < BoxNum; ++i)
	{
		SpawnRandomCube(i < ImportNum);
	}
}
