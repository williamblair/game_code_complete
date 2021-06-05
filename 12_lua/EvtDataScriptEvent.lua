-- test basic event
print('test')

function TestEventHandler( eventData )
    print('In test event handler lua')
    if eventData then
        print('  Setting event data to ' .. (eventData + 1))
        eventData = eventData + 1
    else
        print('Bad event Data')
    end

    -- add an event to the event manager
    QueueEvent( EventType.EvtDataScriptEventFromLua, eventData )
end

if RegisterEventListener then
    RegisterEventListener( EventType.EvtDataScriptEventToLua, TestEventHandler )
else
    print('RegisterEventListener unknown')
end

