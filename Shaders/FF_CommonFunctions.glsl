
vec3 computeReflect(vec3 nzLightVec, vec3 nzNormalVec){
	vec3 x = 2*dot(nzLightVec,nzNormalVec)* nzNormalVec;
	return x-nzLightVec;
}

vec3 blinnRefl(vec3 nzViewVec,vec3 nzLightVec){
	return normalize((nzViewVec + nzLightVec));
}