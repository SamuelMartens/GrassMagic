// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellState.h"
#include "GMMisc.h"

// For debug purposes
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

void FGMSpellState::AddEffect(const FVector& EffectValue, FGMBaseGesture::EType Type) noexcept
{

	const int ActiveGrade = GetActiveGrade();

	// Not sure about order of multiply here. For now will
	// follow the rule OldValue ^ NewValue. But it might
	// produce negative values so may be changed in future
	switch (ActiveGrade)
	{
	case 0:
		Base.Vector = EffectValue;
		break;
	case 1:
		Base.Bivector = FGA::WedgeProduct(Base.Vector, EffectValue);
		break;
	case 2:
		Base.Trivector = FGA::WedgeProduct(Base.Bivector, EffectValue);
		break;
	case 3:
		// We need to make sure that this may happen only if we reach our dimensions amount.
		// Because that's how Grassmann Algebra works
		check(ActiveGrade == FGA::Dimensions);
		Base.Scalar = 0;
		break;
	default:
		check(false);
		break;
	}

	// General routine for mixing gestures
	if (ActiveGrade < FGA::Dimensions)
	{
		ActiveTypes = static_cast<uint8_t>(Type) |
			static_cast<uint8_t>(ActiveTypes);
	}
	else
	{
		ActiveTypes = static_cast<uint8_t>(FGMBaseGesture::EType::None);
	}

	Debug_PrintState();
}

void FGMSpellState::Debug_PrintState() const
{
#if !UE_BUILD_SHIPPING

	// Get byte string
	FString ActiveTypesBinaryRepresent("ActiveBasis: 0b");
	for (int i = sizeof(decltype(ActiveTypes)) * 8 - 1; i >=  0; --i)
	{
		if ((ActiveTypes >> i) & 0b1)
			ActiveTypesBinaryRepresent.AppendChar('1');
		else
			ActiveTypesBinaryRepresent.AppendChar('0');
	}

	FString ActiveGradeInfo = FString::Printf(TEXT(" ActiveGrade: %d "),  GetActiveGrade());
	
	FString BasisInfo;
	switch (GetActiveGrade())
	{
	case 0:
		BasisInfo = FString::Printf(TEXT(" Scalar: %s "), TCHAR_TO_WCHAR(*FString::SanitizeFloat(Base.Scalar)));
		break;
	case 1:
		BasisInfo = FString::Printf(TEXT(" Vector: E1 %s , E2 %s , E3 %s "),
			TCHAR_TO_WCHAR(*FString::SanitizeFloat(FGA::E1(Base.Vector))),
			TCHAR_TO_WCHAR(*FString::SanitizeFloat(FGA::E2(Base.Vector))),
			TCHAR_TO_WCHAR(*FString::SanitizeFloat(FGA::E3(Base.Vector))));
		break;
	case 2:
		BasisInfo = FString::Printf(TEXT(" Bivector: E23 %s , E31 %s , E12 %s "),
			TCHAR_TO_WCHAR(*FString::SanitizeFloat(FGA::E23(Base.Bivector))),
			TCHAR_TO_WCHAR(*FString::SanitizeFloat(FGA::E31(Base.Bivector))),
			TCHAR_TO_WCHAR(*FString::SanitizeFloat(FGA::E12(Base.Bivector))));
		break;
	case 3:
		BasisInfo = FString::Printf(TEXT(" Trivector: E123 %s "),
			TCHAR_TO_WCHAR(*FString::SanitizeFloat(FGA::E123(Base.Trivector))));
		break;
	default:
		BasisInfo = FString("Unsupportable active grade");
		break;
	}

	GEngine->AddOnScreenDebugMessage(GM_DEBUG_SPELL_STATE_ID, 5.0f, FColor::Red,
		FString::Printf(TEXT("--- SPELL STATE INFO --- \n %s \n %s \n %s \n -------------"),
		TCHAR_TO_WCHAR(*ActiveTypesBinaryRepresent),
		TCHAR_TO_WCHAR(*ActiveGradeInfo),
		TCHAR_TO_WCHAR(*BasisInfo)
	));
#endif
}
