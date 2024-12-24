local LuaGameMode = {}

function LuaGameMode:ReceiveBeginPlay()
    self.Super:ReceiveBeginPlay()
    print("gamemode:ReceiveBeginPlay")
end

return Class(nil, nil, LuaGameMode)