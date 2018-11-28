#pragma once
#include "Engine.h"

/**
*	类型转换工具类
*/

#define STDSTRING_TO_FSTRING(StringValue) (StringValue.c_str())

#define BOOL_TO_FSTRING(BoolValue) (BoolValue ? TEXT("true") : TEXT("false"))

#define TCHAR_TO_FSTRING(TCharValue) (TCHAR_TO_UTF8(TCharValue))

#define FNAME_TO_FSTRING(FNameValue) (FNameValue.ToString())

#define FTEXT_TO_FSTRING(FTextValue) (FTextValue.ToString())

#define INT_TO_FSTRING(IntValue) (FString::FromInt(IntValue))

#define FLOAT_TO_FSTRING(FloatValue) (FString::SanitizeFloat(FloatValue))

#define FSTRING_TO_FNAME(FStringValue) (FName(*FStringValue))

#define FSTRING_TO_FTEXT(FStringValue) (FText::FromString(FStringValue))

#define FSTRING_TO_TCHAR(FStringValue) (*FStringValue)

#define FNAME_TO_TCHAR(FNameValue) (FSTRING_TO_TCHAR(FNAME_TO_FSTRING(FNameValue)))

#define FNAME_TO_FTEXT(FNameValue) (FText::FromName(FNameValue))

#define INT64_TO_FSTRING(Int64Value) (FString::Printf(TEXT("%I64d"),Int64Value))