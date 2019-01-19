// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellCaster.h"
#include "GMGestures.h"
#include "GMMisc.h"

#include "Kismet/GameplayStatics.h"

const float UGMSpellCaster::Cast_Time_Scale_Coefficient = 0.00001;

#if !UE_BUILD_SHIPPING

const float UGMSpellCaster::Debug_Info_Tick_Interval = 0.15f;

#endif

void UGMSpellCaster::StartDamageGesture()
{
#if !UE_BUILD_SHIPPING
	Debug_GestureType = FGMDamageGesture::Inst().GetDominantType();
	Debug_GestureVector = FGMDamageGesture::Inst().GetBase();
#endif
	StartGestureGeneric();
}

void UGMSpellCaster::StopDamageGesture()
{
	StopGestureGeneric(FGMDamageGesture::Inst().GetBase(), FGMDamageGesture::Inst().GetDominantType());
}

void UGMSpellCaster::StartControlGesture()
{
#if !UE_BUILD_SHIPPING
	Debug_GestureType = FGMControlGesture::Inst().GetDominantType();
	Debug_GestureVector = FGMControlGesture::Inst().GetBase();
#endif
	StartGestureGeneric();
}

void UGMSpellCaster::StopControlGesture()
{
	StopGestureGeneric(FGMControlGesture::Inst().GetBase(), FGMControlGesture::Inst().GetDominantType());
}

void UGMSpellCaster::StartChangeGesture()
{
#if !UE_BUILD_SHIPPING
	Debug_GestureType = FGMChangeGesture::Inst().GetDominantType();
	Debug_GestureVector = FGMChangeGesture::Inst().GetBase();
#endif
	StartGestureGeneric();
}

void UGMSpellCaster::StopChangeGesture()
{
	StopGestureGeneric(FGMChangeGesture::Inst().GetBase(), FGMChangeGesture::Inst().GetDominantType());
}

void UGMSpellCaster::StartGestureGeneric()
{
	CastStartTime = FTimespan::FromSeconds(UGameplayStatics::GetRealTimeSeconds(GenHandler.GerOwner()->GetWorld()));

#if !UE_BUILD_SHIPPING
	GenHandler.GerOwner()->GetWorldTimerManager().SetTimer(Debug_TimerHandle_Info, this, &UGMSpellCaster::Debug_OnTickInfo, Debug_Info_Tick_Interval, true);
#endif
}

void UGMSpellCaster::StopGestureGeneric(const FVector& GestureVector, FGMBaseGesture::EType Type)
{
	const FVector ScaledGestureVector = GestureVector * GetCastDurationMilliseconds() * Cast_Time_Scale_Coefficient;
	State.AddEffect(ScaledGestureVector, Type);

	GenHandler.ExecuteReleaseCallBack();

#if !UE_BUILD_SHIPPING
	GenHandler.GerOwner()->GetWorldTimerManager().ClearTimer(Debug_TimerHandle_Info);
#endif
}

double UGMSpellCaster::GetCastDurationMilliseconds() const
{
	return (FTimespan::FromSeconds(UGameplayStatics::GetRealTimeSeconds(GenHandler.GerOwner()->GetWorld())) - CastStartTime).GetTotalMicroseconds();
}

void UGMSpellCaster::Debug_OnTickInfo() const
{
#if !UE_BUILD_SHIPPING
	const double CastTime = GetCastDurationMilliseconds();

	FString CastVectorTypeInfo("Cast Gesture Type: ");
	switch (Debug_GestureType)
	{
	case FGMBaseGesture::EType::Damage:
		CastVectorTypeInfo.Append("Damage");
		break;
	case FGMBaseGesture::EType::Control:
		CastVectorTypeInfo.Append("Control");
		break;
	case FGMBaseGesture::EType::Change:
		CastVectorTypeInfo.Append("Change");
		break;
	default:
		check(false);
		break;
	}


	const FString CastTimeInfo = FString::Printf(TEXT("Cast Timer: %s"),
		TCHAR_TO_WCHAR(*FString::SanitizeFloat(CastTime)));
	

	const FVector CastVector = Debug_GestureVector * CastTime;

	const FString CastVectorValueInfo = FString::Printf(TEXT("Cast Gesture Value: %s, %s, %s"),
		TCHAR_TO_WCHAR(*FString::SanitizeFloat(FGA::E1(CastVector))),
		TCHAR_TO_WCHAR(*FString::SanitizeFloat(FGA::E2(CastVector))),
		TCHAR_TO_WCHAR(*FString::SanitizeFloat(FGA::E3(CastVector)))
		);
	
	GEngine->AddOnScreenDebugMessage(GM_DEBUG_GESTURE_CAST_ID, 2.0f, FColor::Green, 
		FString::Printf(TEXT("--- CURRENT GESCTURE --- \n %s \n %s \n %s \n"),
		TCHAR_TO_WCHAR(*CastVectorTypeInfo),
		TCHAR_TO_WCHAR(*CastTimeInfo),
		TCHAR_TO_WCHAR(*CastVectorValueInfo)
	));

#endif
}

