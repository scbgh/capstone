function init(s)
    print("Hello from inside lua script")
end

function key_down(s, key)
 joint = tolua.cast(s:joint("wheeljoint"), "b2RevoluteJoint")
    if key == "z" then
        joint:SetMotorSpeed(-150)
        joint:EnableMotor(true)
    elseif key == "x" then
        joint:SetMotorSpeed(150)
        joint:EnableMotor(true)
    end
end

function key_up(s, key)
    joint = s:joint("wheeljoint")
    joint:EnableMotor(false)
end

function begin_contact(s, contact)
end

function end_contact(s, contact)
end

