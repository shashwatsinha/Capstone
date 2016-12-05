#version 330 core
in vec3 ourPosition;

uniform float iGlobalTime;

out vec4 color;

float cloudyNoise(vec2 uv)
{
    float sx = cos(500. * uv.x);
    float sy = sin(500. * uv.y);
    sx = mix(sx, cos(uv.y * 1000.), .5);
    sy = mix(sy, sin(uv.x * 1000.), .5);
    
    vec2 b = (vec2(sx, sy));
    vec2 bn = normalize(b);

    vec2 _b = b;
	b.x = _b.x * bn.x - _b.y * bn.y;
    b.y = _b.x * bn.y + _b.y * bn.x; 
    vec2 l = uv - vec2(sin(b.x), cos(b.y));
    return length(l - b) - 0.5;
}

float cloudyFbm(vec2 uv)
{
    float f = 0.0;
    vec2 _uv = uv;
    vec2 rotator = (vec2(.91, 1.5));
    
    for (int i = 0; i < 5; ++i)
    {
        vec2 tmp = uv;
        uv.x = tmp.x * rotator.x - tmp.y * rotator.y; 
        uv.y = tmp.x * rotator.y + tmp.y * rotator.x; 
        f += .5 * cloudyNoise(uv) * pow(0.5, float(i + 1));
    }
    return f;
}

float clouds (vec2 uv)
{
    float T = iGlobalTime * .001;

	float x = 0.0;
    x += cloudyFbm( 0.5 * uv + vec2(.1,  -.01) * T) * 0.5;
    x += cloudyFbm( 1.0 * uv + vec2(.12,  .03) * T) * 0.5 * 0.5;
 	x += cloudyFbm( 2.0 * uv + vec2(.15, -.02) * T) * 0.5 * 0.5 * 0.5;
 	//x += cloudyFbm( 4.0 * uv + vec2(.2,   .01) * T) * 0.5 * 0.5 * 0.5 * 0.5;
 	//x += cloudyFbm( 8.0 * uv + vec2(.15, -.01) * T) * 0.5 * 0.5 * 0.5 * 0.5 * 0.5;
	
    x = smoothstep(0.0, .6, x);
    //float f = 0.6;
	//x = (x - f) / (1.0 - f);
    //float _x = x;    
    //x = smoothstep(0.4, 0.55, x);
	//return x * _x;
	return x;   
}


void main()
{
	//color = vec4(ourColor, 1.0f);
	/*
	if(ourPosition.x > 0.5f)
		color = vec4(0.0f,(sin(iGlobalTime) / 2) + 0.5, 0.0f, 1.0f);
	else
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	*/
	
	vec2 uv = vec2(ourPosition);
    vec2 ouv = uv;
    uv -= vec2(0.5);
    //uv.y /= iResolution.x / iResolution.y;
	uv.y /= 800 / 600;
    vec2 _uv = uv * 0.00007;
    
    // clouds 
	float x = clouds(_uv);
    // sky colors
    //vec4 top = vec4(0.1, 0.45, 0.9, 0.0) * .6;
	//vec4 bottom = vec4(0.1, 0.45, 0.9, 0.0) * .6;
	vec4 top = vec4(0.4, 0.0, 0.2, 0.0) * .6;
	vec4 bottom = vec4(0.4, 0.0, 0.2, 0.0) * .6;
    //vec4 bottom = vec4(0., 0.45, .7, 0.0) * 1.2;
    color = mix(bottom, top, smoothstep(0., .7, ouv.y));
    
    // clouds color
    color += x;
    color = mix(vec4(x), color, 1. - x);
	
    // some fake lighting
    vec2 ld = .005 * normalize(vec2(1.0, 1.)) * fwidth(uv);
    float f = .0;
    const int steps = 4;
    for (int i = 1; i <= steps; ++i)
    {
    	float c = clouds(_uv - float(i * i) * ld) * pow(0.55, float(i));
        f += max(c, 0.0);
    }
    f = clamp(f, 0.0, 1.0);
    f = 1.0 - f;
    f = pow(f, 1.2);
    color += vec4(f) * x * .5;
	
}