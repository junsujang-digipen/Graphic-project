
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
return vec2(0.5);

}
vec2 calcCubeMapTexCoord(vec3 point,vec3 pMin,vec3 pMax){

return vec2(0.5);
}