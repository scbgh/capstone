button_down = 0

function init(s)
    print("Hello from inside lua script")
end

function key_down(s, key)
end

function key_up(s, key)
end

function begin_contact(s, contact)
    doorbody = s:body("door1body")
    button = s:fixture("button1")
    doorstop = s:body("door1stop")
    dooredge = s:body("door1edge")

    isbutton, btn, other = either_is(button, contact:GetFixtureA(),
            contact:GetFixtureB())
    if isbutton then
        if button_down == 0 then
            doorbody:SetLinearVelocity(b2Vec2(0, 2))
            print("Button down")
        end
        button_down = button_down + 1
    end

    isdoorstop, edge, stop = both_are(doorstop, dooredge,
            contact:GetFixtureA():GetBody(),
            contact:GetFixtureB():GetBody())
    if isdoorstop then
        doorbody:SetLinearVelocity(b2Vec2(0, 0))
    end
end

function end_contact(s, contact)
    door = s:body("door1body")
    doorfixture = s:fixture("door1fixture")
    button = s:fixture("button1")

    isbutton, btn, other = either_is(button, contact:GetFixtureA(),
            contact:GetFixtureB())
    if isbutton then
        button_down = button_down - 1
        if button_down == 0 then
            doorbody:SetLinearVelocity(b2Vec2(0, -2))
            print("Button up")
        end
    end
end

function presolve_contact(s, contact, old_manifold)
end

function postsolve_contact(s, contact, impulse)
end
