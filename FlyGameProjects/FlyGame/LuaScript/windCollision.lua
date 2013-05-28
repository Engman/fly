
-- Collision of wind and player
-- in WindSphere*, vector3, WindSphere[]*
-- out vector3
function WindCollision(playerSphere, localWind, spheres)
	
	local lPosition = {x = VectorX(WindPosition(playerSphere)), y = VectorY(WindPosition(playerSphere)), z = VectorZ(WindPosition(playerSphere))}
	local lRadius = WindRadius(playerSphere);
	local lWind = {x = VectorX(localWind), y = VectorY(localWind), z = VectorZ(localWind)}
	
	if lWind.x < 0.002 and lWind.x > -0.002 then
		lWind.x = 0.0
	end
	if lWind.x < 0.0 then
		lWind.x = lWind.x + 0.002
	end
	if lWind.x > 0.0 then
		lWind.x = lWind.x - 0.002
	end
	if lWind.y < 0.002 and lWind.y > -0.002 then
		lWind.y = 0.0
	end
	if lWind.y < 0.0 then
		lWind.y = lWind.y + 0.002
	end
	if lWind.y > 0.0 then
		lWind.y = lWind.y - 0.002
	end
	if lWind.z < 0.002 and lWind.z > -0.002 then
		lWind.z = 0.0
	end
	if lWind.z < 0.0 then
		lWind.z = lWind.z + 0.002
	end
	if lWind.z > 0.0 then
		lWind.z = lWind.z - 0.002
	end
	
	for i = 0, ArraySize(spheres)-1, 1 do
		local lSpherePos = {x = VectorX(WindPosition(ArrayAt(spheres,i))), y = VectorY(WindPosition(ArrayAt(spheres,i))), z = VectorZ(WindPosition(ArrayAt(spheres,i)))}
		local lSphereDir = {x = VectorX(WindDirection(ArrayAt(spheres,i))), y = VectorY(WindDirection(ArrayAt(spheres,i))), z = VectorZ(WindDirection(ArrayAt(spheres,i)))}
		local lSphereRad = WindRadius(ArrayAt(spheres,i))
		
		if SphereVSSphere(lSpherePos, lSphereRad, lPosition, lRadius) then
			if lWind.x < lSphereDir.x and lSphereDir.x > 0 then
				lWind.x = lWind.x + lSphereDir.x*0.01
			end
			if lWind.y < lSphereDir.y and lSphereDir.y > 0 then
				lWind.y = lWind.y + lSphereDir.y*0.01
			end
			if lWind.z < lSphereDir.z and lSphereDir.z > 0 then
				lWind.z = lWind.z + lSphereDir.z*0.01
			end
			
			if lWind.x > lSphereDir.x and lSphereDir.x < 0 then
				lWind.x = lWind.x + lSphereDir.x*0.01
			end
			if lWind.y > lSphereDir.y and lSphereDir.y < 0 then
				lWind.y = lWind.y + lSphereDir.y*0.01
			end
			if lWind.z > lSphereDir.z and lSphereDir.z < 0 then
				lWind.z = lWind.z + lSphereDir.z*0.01
			end
		end
	end		
	
	return lWind.x, lWind.y, lWind.z;
end


--/////////// C++ TO LUA HELP FUNCTIONS ///////////
-- These functions are for easier use of light user data which
-- gets sent from C++ to Lua a lot

-- Gets the pointer of a wind sphere
-- in WindSphere[]*, int
-- out WindSphere*
function ArrayAt(pArray, index)
	return windArray.get(pArray, index)
end

-- Gets the size of the array
-- in WindSphere[]*
-- out int
function ArraySize(pArray)
	return windArray.size(pArray)
end

-- Gets the x value of the vector
-- in Vector3*
-- out float
function VectorX(pVector3)
	return vector3.x(pVector3)
end

-- Gets the y value of the vector
-- in Vector3*
-- out float
function VectorY(pVector3)
	return vector3.y(pVector3)
end

-- Gets the z value of the vector
-- in Vector3*
-- out float
function VectorZ(pVector3)
	return vector3.z(pVector3)
end

-- Gets the position of a wind sphere
-- in WindSphere*
-- out vector3*
function WindPosition(pWindSphere)
	return windSphere.position(pWindSphere)
end

-- Gets the direction of a wind sphere
-- in WindSphere*
-- out vector3*
function WindDirection(pWindSphere)
	return windSphere.direction(pWindSphere)
end

-- Gets the radius of a wind sphere
-- in WindSphere*
-- out float
function WindRadius(pWindSphere)
	return windSphere.radius(pWindSphere)
end
--///////// C++ TO LUA HELP FUNCTIONS END /////////



-- Calculates length of a vector
-- in vector3
-- out float
function Length(vector)
	return math.sqrt(vector.x*vector.x+vector.y*vector.y+vector.z*vector.z)
end

-- Calculates length of a vector
-- in vector3
-- out float
function LengthSq(vector)
	return vector.x*vector.x+vector.y*vector.y+vector.z*vector.z
end

-- Calculates vector perependicular to sent vectors
-- in vector3, vector3
-- out vector3
function Cross(vector, vector2)
	local t = {x = 0.0, y = 0.0, z = 0.0}
	
	t.x = vector.y*vector2.z - vector.z*vector2.y
	t.y = vector.z*vector2.x - vector.x*vector2.z
	t.z = vector.x*vector2.y - vector.y*vector2.x
	
	return t
end

-- Calculates the dot product between two vectors
-- in vector3, vector3
-- out float
function Dot(vector, vector2)
	return vector.x*vector2.x+vector.y*vector2.y+vector.z*vector2.z
end

-- Normalizes a vector
-- in vector3
-- out vector3
function Normalize(vector)
	local t = {x = 0.0, y = 0.0, z = 0.0}
	
	local len = Length(vector)
	
	t.x = vector.x/len
	t.y = vector.y/len
	t.z = vector.z/len
	
	return t
end

function SphereVSSphere(spherePos, sphereRad, spherePos2, sphereRad2)
	--vector3
	local lenVec = {x = spherePos.x - spherePos2.x, y = spherePos.y - spherePos2.y, z = spherePos.z - spherePos2.z}
	if LengthSq(lenVec) <= (sphereRad+sphereRad2)*(sphereRad+sphereRad2) then
		return true
	end

	return false
end

-- Intersection between plane and ray
-- in vector3, vector3, vector3, vector3
-- out float
function PlaneVsRay(pOrigin, pNormal, rOrigin, rVector)
	--float
	local d = -Dot(pNormal.x, pNormal.y, pNormal.z, pOrigin.x, pOrigin.y, pOrigin.z)
	local numer = Dot(pNormal.x, pNormal.y, pNormal.z, rOrigin.x, rOrigin.y, rOrigin.z) + d
	local denom = Dot(pNormal.x, pNormal.y, pNormal.z, rVector.x, rVector.y, rVector.z)

	if denom == 0.0 then
		return -1.0
	end

	return -(numer / denom)
end

-- Intersection between sphere and ray
-- in vector3, vector3, vector3, float
-- out float
function RayVsSphere(rO, rV, sO, sR)
	--vector3		
	local Q = {x = sO.x - rO.x, y = sO.y - rO.y, z = sO.z - rO.z}
	--float
	local c = Length(Q.x, Q.y, Q.z)
	local v = Dot(Q.x, Q.y, Q.z, rV.x, rV.y, rV.z)
	local d = sR*sR - (c*c - v*v)
	
	--If there was no intersection, return -1
		
	if d < 0.0 then
		return -1.0
	end
		
	--Return the distance to the [first] intersecting point	
	return v - math.sqrt(d)
end

-- Intersection between point and triangle
-- in vector3, vector3, vector3, vector3
-- out bool
function CheckPointInTriangle(point, a, b, c) 
	--float
	local total_angles = 0.0
       
    --vector3
	local v1 = {x = point.x-a.x, y = point.y-a.y, z = point.z-a.z}
	local v2 = {x = point.x-b.x, y = point.y-b.y, z = point.z-b.z}
	local v3 = {x = point.x-c.x, y = point.y-c.y, z = point.z-c.z}
  
  	
  
	v1 = Normalize(v1.x, v1.y, v1.z)
	v2 = Normalize(v2.x, v2.y, v2.z)
	v3 = Normalize(v3.x, v3.y, v3.z)


	total_angles = math.acos(Dot(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z)) + math.acos(Dot(v2.x, v2.y, v2.z, v3.x, v3.y, v3.z)) + math.acos(Dot(v3.x, v3.y, v3.z, v1.x, v1.y, v1.z))
    	
    	
	if math.abs(total_angles - 2*3.14159265358979323846) <= 0.005 then	
		return true
	end
     
	return false
end

-- Used to find closest point on a triangle from sent position
-- in vector3, vector3, vector3, vector3
-- out vector3
function ClosestPointOnTriangle(a, b, c, p)
	--vector3
	local Rab = ClosestPointOnLine(a, b, p)
	local Rbc = ClosestPointOnLine(b, c, p)
	local Rca = ClosestPointOnLine(c, a, p)

	--float
	local abL = Length(p.x - Rab.x, p.y - Rab.y, p.z - Rab.z)
	local bcL = Length(p.x - Rbc.x, p.y - Rbc.y, p.z - Rbc.z)
	local caL = Length(p.x - Rca.x, p.y - Rca.y, p.z - Rca.z)
	
	--float
	local min = abL
	--vector3
	local closest = Rab

	if bcL < min then
		min = bcL
		closest = Rbc
	end

	if caL < min then
		closest = Rca
	end

	return closest
end

-- in vector3, vector3, vector3
-- out vector3
function ClosestPointOnLine(a, b, p)
	--Determine t (the length of the vector from ‘a’ to ‘p’)

	--vector3
	local c = {x = p.x - a.x, y = p.y - a.y, z = p.z - a.z}
	local V = {x = b.x - a.x, y = b.y - a.y, z = b.z - a.z}
	--float
	local d = Length(V.x, V.y, V.z)
	V = Normalize(V.x, V.y, V.z)
	--float
	local t = Dot(V.x, V.y, V.z, c.x, c.y, c.z)

	--Check to see if ‘t’ is beyond the extents of the line segment
	if t < 0.0 then
		return a
	end
	if t > d then
		return b
	end
 
	--Return the point between ‘a’ and ‘b’
	V.x = V.x*t
	V.y = V.y*t
	V.z = V.z*t
	
	return {x = a.x + V.x, y = a.y + V.y, z = a.z + V.z}
end