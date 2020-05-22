#ifndef MAGE_HEADER_TONE_MAPPING
#define MAGE_HEADER_TONE_MAPPING

//-----------------------------------------------------------------------------
// Engine Configuration
//-----------------------------------------------------------------------------
// Defines			                        | Default
//-----------------------------------------------------------------------------
// AA_INVERSE_TONE_MAP_FUNCTION             | InverseToneMap_Max3
// AA_TONE_MAP_FUNCTION                     | ToneMap_Max3
// TONE_MAP_FUNCTION                        | ToneMap_Uncharted

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "math.hlsli"

//-----------------------------------------------------------------------------
// Engine Defines
//-----------------------------------------------------------------------------

#ifndef AA_TONE_MAP_FUNCTION
	#define AA_TONE_MAP_FUNCTION ToneMap_Max3
#endif // AA_TONE_MAP_FUNCTION

#ifndef AA_INVERSE_TONE_MAP_FUNCTION
	#define AA_INVERSE_TONE_MAP_FUNCTION InverseToneMap_Max3
#endif // AA_INVERSE_TONE_MAP_FUNCTION

#ifndef TONE_MAP_FUNCTION
	#define TONE_MAP_FUNCTION ToneMap_Uncharted
#endif // TONE_MAP_FUNCTION

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------

/**
 Calculates the luminance of the given RGB color.

 @param[in]		rgb
				The RGB color.
 @return		The luminance of the given RGB color.
 */
float Luminance(float3 rgb) {
	static const float3 rgb_to_y = { 0.212671f, 0.715160f, 0.072169f };
	return dot(rgb, rgb_to_y);
}

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: ACES Filmic
//-----------------------------------------------------------------------------

float3 ToneMap_ACESFilmic(float3 hdr) {
	static const float a = 2.51f;
	static const float b = 0.03f;
	static const float c = 2.43f;
	static const float d = 0.59f;
	static const float e = 0.14f;

	return (hdr * (a * hdr + b))
		 / (hdr * (c * hdr + d) + e);
}

float4 ToneMap_ACESFilmic(float4 hdr) {
	return float4(ToneMap_ACESFilmic(hdr.xyz), hdr.w);
}

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: Max3
//-----------------------------------------------------------------------------

float3 ToneMap_Max3(float3 hdr) {
	return hdr * rcp(1.0f + Max(hdr));
}

float4 ToneMap_Max3(float4 hdr) {
	return float4(ToneMap_Max3(hdr.xyz), hdr.w);
}

float3 ToneMap_Max3(float3 hdr, float weight) {
	return hdr * (weight * rcp(1.0f + Max(hdr)));
}

float4 ToneMap_Max3(float4 hdr, float weight) {
	return float4(ToneMap_Max3(hdr.xyz, weight), hdr.w);
}

float3 InverseToneMap_Max3(float3 ldr) {
	return ldr * rcp(1.0f - Max(ldr));
}

float4 InverseToneMap_Max3(float4 ldr) {
	return float4(InverseToneMap_Max3(ldr.xyz), ldr.w);
}

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: Reinhard
//-----------------------------------------------------------------------------

float3 ToneMap_Reinhard(float3 hdr) {
	return hdr / (1.0f + hdr);
}

float4 ToneMap_Reinhard(float4 hdr) {
	return float4(ToneMap_Reinhard(hdr.xyz), hdr.w);
}

float3 InverseToneMap_Reinhard(float3 ldr) {
	return ldr / (1.0f - ldr);
}

float4 InverseToneMap_Reinhard(float4 ldr) {
	return float4(InverseToneMap_Reinhard(ldr.xyz), ldr.w);
}

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: Uncharted
//-----------------------------------------------------------------------------

float3 ToneMap_Uncharted(float3 hdr) {
	static const float a = 0.22f;
	static const float b = 0.30f;
	static const float c = 0.10f;
	static const float d = 0.20f;
	static const float e = 0.01f;
	static const float f = 0.30f;

	return ((hdr * (a * hdr + b * c) + d * e)
		  / (hdr * (a * hdr + b) + d * f)) - e / f;
}

float4 ToneMap_Uncharted(float4 hdr) {
	return float4(ToneMap_Uncharted(hdr.xyz), hdr.w);
}

#endif // MAGE_HEADER_TONE_MAPPING