local LuaActor = {}

function LuaActor:ReceiveBeginPlay()
    print("actor:ReceiveBeginPlay")
end

return Class(nil, nil, LuaActor)