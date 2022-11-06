
vec3 computeReflect(vec3 nzLightVec, vec3 nzNormalVec){
	vec3 x = 2*dot(nzLightVec,nzNormalVec)* nzNormalVec;
	return x-nzLightVec;
}

vec3 blinnRefl(vec3 nzViewVec,vec3 nzLightVec){
	return normalize((nzViewVec + nzLightVec));
}

vec2 calcSphereTexCoord(vec3 point,vec3 pMin,vec3 pMax){
	vec3 center = (pMin + pMax)/2.;
	vec3 alignedPoint = point - center;
	float theta = atan(alignedPoint.z,alignedPoint.x );
	float alignedMiny = pMin.y - center.y;
	float alignedMaxy = pMax.y - center.y;
	float y = (alignedPoint.y - alignedMiny)/(alignedMaxy - alignedMiny);
	return vec2(theta/6.14f, y);
}
vec2 calcCylindricalTexCoord(vec3 point,vec3 pMin,vec3 pMax){
	vec3 center = (pMin + pMax)/2.;
	vec3 alignedPoint = point - center;
	float theta = atan(alignedPoint.z,alignedPoint.x );
	float r = sqrt(alignedPoint.x*alignedPoint.x + alignedPoint.y*alignedPoint.y + alignedPoint.z*alignedPoint.z);
	float latitude = acos(alignedPoint.y/r);

	return vec2(theta/6.14f, 3.14f - latitude/3.14f);
}
vec2 calcPlanarTexCoord(vec3 point,vec3 pMin,vec3 pMax){
	vec3 center = (pMin + pMax)/2.;
	vec3 alignedPoint = point - center;
	vec3 alignedPointMin = pMin - center;
	vec3 alignedPointMax = pMax - center;

	vec3 uv = (alignedPoint - alignedPointMin)/(alignedPointMax - alignedPointMin);
return vec2(uv.xy);

}
vec2 calcCubeMapTexCoord(vec3 point,vec3 pMin,vec3 pMax){
	vec3 center = (pMin + pMax)/2.;
	vec3 alignedPoint = point - center;
	vec3 absAlignedP = abs(alignedPoint);
	bool isXPositive = alignedPoint.x >=0;
	bool isYPositive = alignedPoint.y >=0;
	bool isZPositive = alignedPoint.z >=0;

	vec2 uv = vec2(0);
	if(absAlignedP.x >=absAlignedP.y && absAlignedP.x >=absAlignedP.z){
		if(isXPositive == true){
			uv.x = -absAlignedP.z/absAlignedP.x;
			uv.y = absAlignedP.y/absAlignedP.x;
		}else{
			uv.x = absAlignedP.z/absAlignedP.x;
			uv.y = absAlignedP.y/absAlignedP.x;
		}
	}
	else if(absAlignedP.y >=absAlignedP.x && absAlignedP.y >=absAlignedP.z){
		if(isYPositive == true){
			uv.x = absAlignedP.x/absAlignedP.y;
			uv.y = -absAlignedP.z/absAlignedP.y;
		}else{
			uv.x = absAlignedP.x/absAlignedP.y;
			uv.y = absAlignedP.z/absAlignedP.y;
		}
	}
	else if(absAlignedP.z >=absAlignedP.y && absAlignedP.z >=absAlignedP.x){
		if(isZPositive == true){
			uv.x = absAlignedP.x/absAlignedP.z;
			uv.y = absAlignedP.y/absAlignedP.z;
		}else{
			uv.x = -absAlignedP.x/absAlignedP.z;
			uv.y = absAlignedP.y/absAlignedP.z;
		}
	}

	return (uv - vec2(1.))*0.5f;
}