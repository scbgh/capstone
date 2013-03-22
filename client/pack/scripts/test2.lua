function init(s)
    print("Hello from inside lua script")
end

function key_down(s, key)
end

function key_up(s, key)
end

function begin_contact(s, contact)
    print(contact_velocity(contact))
end

function end_contact(s, contact)
end

