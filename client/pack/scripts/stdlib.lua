function b2Vec2_add (a,b) return b2Vec2(a.x+b.x, a.y+b.y) end
function b2Vec2_sub (a,b) return b2Vec2(a.x-b.x, a.y-b.y) end
function b2Vec2_scale (a,b) return b2Vec2(a*b.x, a*b.y) end

function contact_velocity(contact)
    bodyA = contact:GetFixtureA():GetBody()
    bodyB = contact:GetFixtureB():GetBody()

    wm = b2WorldManifold:new()
    contact:GetWorldManifold(wm)

    vel1 = bodyA:GetLinearVelocityFromWorldPoint(wm.points[0])
    vel2 = bodyB:GetLinearVelocityFromWorldPoint(wm.points[0])
    return b2Vec2_sub(vel1, vel2):Length()
end

print "Loaded stdlib..."
