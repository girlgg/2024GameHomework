#include "CourseGameInstance.h"

#include "Player/CourseLocalPlayer.h"
#include "UserInterface/Subsystem/GameUIManagerSubsystem.h"

static uint8* ReadFile(IPlatformFile& PlatformFile, FString path, uint32& len)
{
	IFileHandle* FileHandle = PlatformFile.OpenRead(*path);
	if (FileHandle)
	{
		len = (uint32)FileHandle->Size();
		uint8* buf = new uint8[len];

		FileHandle->Read(buf, len);

		// Close the file again
		delete FileHandle;

		return buf;
	}

	return nullptr;
}

UCourseGameInstance::UCourseGameInstance()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		CreateLuaState();
	}
}

void UCourseGameInstance::Shutdown()
{
	CloseLuaState();
	Super::Shutdown();
}

int32 UCourseGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
	if (ReturnVal != INDEX_NONE)
	{
		if (!PrimaryPlayer.IsValid())
		{
			PrimaryPlayer = NewPlayer;
		}
		
		GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerAdded(Cast<UCourseLocalPlayer>(NewPlayer));
	}
	
	return ReturnVal;
}

void UCourseGameInstance::CloseLuaState()
{
	if (state)
	{
		state->close();
		delete state;
		state = nullptr;
	}
}

void UCourseGameInstance::CreateLuaState()
{
	NS_SLUA::LuaState::onInitEvent.AddUObject(this, &UCourseGameInstance::LuaStateInitCallback);

	CloseLuaState();
	state = new NS_SLUA::LuaState("SLuaMainState", this);
	state->setLoadFileDelegate([](const char* fn, FString& filepath)-> TArray<uint8>
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		FString path = FPaths::ProjectContentDir();
#ifdef WITH_EDITOR
		path = FPaths::GameSourceDir();
#endif
		FString filename = UTF8_TO_TCHAR(fn);
		path /= "Lua";
		path /= filename.Replace(TEXT("."), TEXT("/"));

		TArray<uint8> Content;
		TArray<FString> luaExts = {UTF8_TO_TCHAR(".lua"), UTF8_TO_TCHAR(".luac")};
		for (auto& it : luaExts)
		{
			auto fullPath = path + *it;

			FFileHelper::LoadFileToArray(Content, *fullPath);
			if (Content.Num() > 0)
			{
				filepath = fullPath;
				return MoveTemp(Content);
			}
		}

		return MoveTemp(Content);
	});
	state->init();
}

static int32 PrintLog(NS_SLUA::lua_State* L)
{
	FString str;
	size_t len;
	const char* s = luaL_tolstring(L, 1, &len);
	if (s) str += UTF8_TO_TCHAR(s);
	NS_SLUA::Log::Log("PrintLog %s", TCHAR_TO_UTF8(*str));
	return 0;
}

void UCourseGameInstance::LuaStateInitCallback(slua::lua_State* L)
{
	lua_pushcfunction(L, PrintLog);
	lua_setglobal(L, "PrintLog");
}
