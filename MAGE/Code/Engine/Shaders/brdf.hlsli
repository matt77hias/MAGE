#ifndef MAGE_HEADER_BRDF
#define MAGE_HEADER_BRDF

// Shading can be performed in an arbitrary shading space. The input arguments
// (n, l, v) for evaluating BRDFs and BRDF components must be expressed in the
// same coordinate system which can be chosen arbitrarily.

//-----------------------------------------------------------------------------
// Engine Configuration
//-----------------------------------------------------------------------------
// Defines			                        | Default
//-----------------------------------------------------------------------------
// BRDF_DOT_EPSILON                         | 1e-5f
// BRDF_D_FUNCTION                          | D_GGX
// BRDF_F_FUNCTION                          | F_Schlick
// BRDF_FUNCTION                            | not defined
// BRDF_MINIMUM_ALPHA                       | 1e-1f
// BRDF_V_FUNCTION                          | G_GXX
// DISABLE_BRDF_DIFFUSE                     | not defined
// DISABLE_BRDF_SPECULAR                    | not defined

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "material.hlsli"
#include "math.hlsli"

//-----------------------------------------------------------------------------
// Engine Defines
//-----------------------------------------------------------------------------

#ifdef BRDF_BLINN_PHONG
	#define BRDF_FUNCTION   CookTorranceBRDF
	#define BRDF_D_FUNCTION D_BlinnPhong
	#define BRDF_F_FUNCTION F_None
	#define BRDF_V_FUNCTION V_Implicit
#endif // BRDF_BLINN_PHONG

#ifdef BRDF_COOK_TORRANCE
	#define BRDF_FUNCTION   CookTorranceBRDF
	#define BRDF_D_FUNCTION D_GGX
	#define BRDF_F_FUNCTION F_Schlick
	#define BRDF_V_FUNCTION V_GGX
#endif // BRDF_COOK_TORRANCE

#ifdef BRDF_FROSTBITE
	#define BRDF_FUNCTION   FrostbiteBRDF
	#define BRDF_D_FUNCTION D_GGX
	#define BRDF_F_FUNCTION F_Schlick
	#define BRDF_V_FUNCTION V_GGX
#endif // BRDF_FROSTBITE

#ifdef BRDF_LAMBERTIAN
	#define BRDF_FUNCTION	LambertianBRDF
	#define DISABLE_BRDF_SPECULAR
#endif // BRDF_LAMBERTIAN

#ifdef BRDF_WARD_DUER
	#define BRDF_FUNCTION   CookTorranceBRDF
	#define BRDF_D_FUNCTION D_WardDuer
	#define BRDF_F_FUNCTION F_None
	#define BRDF_V_FUNCTION V_Ward
#endif // BRDF_WARD_DUER

#ifndef BRDF_D_FUNCTION
	#define BRDF_D_FUNCTION D_GGX
#endif // BRDF_D_FUNCTION

#ifndef BRDF_F_FUNCTION
	#define BRDF_F_FUNCTION F_Schlick
#endif // BRDF_F_FUNCTION

#ifndef BRDF_V_FUNCTION
	#define BRDF_V_FUNCTION V_GGX
#endif // BRDF_V_FUNCTION

#ifndef BRDF_MINIMUM_ALPHA
	#define BRDF_MINIMUM_ALPHA 1e-3f
#endif // BRDF_MINIMUM_ALPHA

#ifndef BRDF_DOT_EPSILON
	#define BRDF_DOT_EPSILON 1e-5f
#endif // BRDF_DOT_EPSILON

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: Constants
//-----------------------------------------------------------------------------

/**
 The reflectance at normal incidence for dielectric (i.e. non-metal) materials.
 */
static const float g_dielectric_F0 = 0.04f;

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: Normal Distribution Function
//-----------------------------------------------------------------------------

/**
 Calculates the Beckmann Normal Distribution Function component.

 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Beckmann Normal Distribution Function component.
 */
float D_Beckmann(float n_dot_h, float alpha) {

	//               1            [  n_dot_h^2 - 1  ]             1             [tan(theta_h)]^2
	// D:= -------------------- e^[-----------------] = -------------------- e^-[------------]
	//     pi alpha^2 n_dot_h^4   [n_dot_h^2 alpha^2]   pi alpha^2 n_dot_h^4    [   alpha    ]

	const float inv_alpha2   = 1.0f / sqr(alpha);
	const float n_dot_h2     = sqr(n_dot_h);
	const float inv_n_dot_h4 = 1.0f / sqr(n_dot_h2);
	const float t2           = SqrCosToSqrTan(n_dot_h2);

	return g_inv_pi * inv_alpha2 * inv_n_dot_h4 * exp(-t2 * inv_alpha2);
}

/**
 Calculates the Ward-Duer Normal Distribution Function component.

 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Ward-Duer Normal Distribution Function component.
 */
float D_WardDuer(float n_dot_h, float alpha) {

	//         1        [  n_dot_h^2 - 1  ]        1        [tan(theta_h)]^2
	// D:= ---------- e^[-----------------] = ---------- e^-[------------]
	//     pi alpha^2   [n_dot_h^2 alpha^2]   pi alpha^2    [   alpha    ]

	const float inv_alpha2   = 1.0f / sqr(alpha);
	const float n_dot_h2     = sqr(n_dot_h);
	const float t2           = SqrCosToSqrTan(n_dot_h2);

	return g_inv_pi * inv_alpha2 * exp(-t2 * inv_alpha2);
}

/**
 Calculates the Blinn-Phong Normal Distribution Function component.

 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Blinn-Phong Normal Distribution Function component.
 */
float D_BlinnPhong(float n_dot_h, float alpha) {

	//         1              [   2       ]   Ns + 2
	// D:= ---------- n_dot_h^[------- - 2] = ------ n_dot_h^Ns
	//     pi alpha^2         [alpha^2    ]    pi 2

	const float inv_alpha2 = 1.0f / sqr(alpha);
	const float Ns         = 2.0f * inv_alpha2 - 2.0f;

	return g_inv_pi * inv_alpha2 * pow(n_dot_h, Ns);
}

/**
 Calculates the Trowbridge-Reitz Normal Distribution Function component.

 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Trowbridge-Reitz Normal Distribution Function component.
 */
float D_TrowbridgeReitz(float n_dot_h, float alpha) {

	//                  alpha^2                                      c
	// D:= ---------------------------------- = ---------------------------------------------
	//     pi (n_dot_h^2 (alpha^2 - 1) + 1)^2   (alpha^2 * cos(theta_h)^2 + sin(theta_h)^2)^2

	const float alpha2   = sqr(alpha);
	const float n_dot_h2 = sqr(n_dot_h);
	const float temp1    = n_dot_h2 * (alpha2 - 1.0f) + 1.0f;

	return g_inv_pi * alpha2 / sqr(temp1);
}

/**
 Calculates the GGX Normal Distribution Function component.

 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The GGX Normal Distribution Function component.
 */
float D_GGX(float n_dot_h, float alpha) {
	return D_TrowbridgeReitz(n_dot_h, alpha);
}

/**
 Calculates the GTR2 Normal Distribution Function component.

 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The GTR2 Normal Distribution Function component.
 */
float D_GTR2(float n_dot_h, float alpha) {
	return D_TrowbridgeReitz(n_dot_h, alpha);
}

/**
 Calculates the Berry Normal Distribution Function component.

 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Berry Normal Distribution Function component.
 */
float D_Berry(float n_dot_h, float alpha) {

	//                      alpha^2 - 1                                          c
	// D:= --------------------------------------------- = -------------------------------------------
	//     log(alpha^2) pi (n_dot_h^2 (alpha^2 - 1) + 1)   (alpha^2 * cos(theta_h)^2 + sin(theta_h)^2)

	const float alpha2   = sqr(alpha);
	const float n_dot_h2 = sqr(n_dot_h);
	const float temp1    = n_dot_h2 * (alpha2 - 1.0f) + 1.0f;

	return (1.0f <= alpha) ? g_inv_pi
	                       : g_inv_pi * (alpha2 - 1.0f) / (log(alpha2) * temp1);
}

/**
 Calculates the GTR1 Normal Distribution Function component.

 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The GTR1 Normal Distribution Function component.
 */
float D_GTR1(float n_dot_h, float alpha) {
	return D_Berry(n_dot_h, alpha);
}

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: Partial Geometric Schadowing
//-----------------------------------------------------------------------------

/**
 Calculates the GGX partial Geometric Schadowing component.

 @param[in]		n_dot_vl
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction. Or the clamped cosine of the light angle. The light
				angle is the angle between the surface normal and the light
				(hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The GGX partial Geometric Schadowing component.
 */
float G1_GGX(float n_dot_vl, float alpha) {

	//                          2 n_dot_vl                                              2
	// G1 := --------------------------------------------------- = --------------------------------------------
	//       n_dot_vl + sqrt(alpha^2 + (1 - alpha^2) n_dot_vl^2)   1 + sqrt((alpha/n_dot_vl)^2 + (1 - alpha^2))

	const float alpha2    = sqr(alpha);
	const float n_dot_vl2 = sqr(n_dot_vl);

	return 2.0f * n_dot_vl / (n_dot_vl * sqrt(alpha2 + (1.0f - alpha2) * n_dot_vl2));
}

/**
 Calculates the Schlick partial Geometric Schadowing component.

 @param[in]		n_dot_vl
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction. Or the clamped cosine of the light angle. The light
				angle is the angle between the surface normal and the light
				(hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Schlick-GGX partial Geometric Schadowing component.
 */
float G1_SchlickGGX(float n_dot_vl, float alpha) {

	//             n_dot_vl                      n_dot_vl
	// G1 := --------------------- = --------------------------------
	//       n_dot_vl (1 - k) + k    n_dot_vl (1 - alpha/2) + alpha/2

	const float k = 0.5f * alpha;

	return n_dot_vl / (n_dot_vl * (1.0f - k) + k);
}

/**
 Calculates the Beckmann partial Geometric Schadowing component.

 @param[in]		n_dot_vl
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction. Or the clamped cosine of the light angle. The light
				angle is the angle between the surface normal and the light
				(hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Beckmann partial Geometric Schadowing component.
 */
float G1_Beckmann(float n_dot_vl, float alpha) {

	//                n_dot_vl
	// c  := --------------------------
	//       alpha sqrt(1 - n_dot_vl^2)
	//
	//         3.535 c + 2.181 c^2
	// G1 := ----------------------- (if c < 1.6) | 1 (otherwise)
	//       1 + 2.276 c + 2.577 c^2

	const float n_dot_vl2 = sqr(n_dot_vl);
	const float c         = n_dot_vl2 / (alpha * sqrt(1.0f - n_dot_vl2));
	const float c2        = sqr(c);

	return (1.6f > c) ? (3.535f * c + 2.8181f * c2)
		              / (1.0f + 2.276f * c + 2.577f * c2)
		              : 1.0f;
}

/**
 Calculates the Schlick-Beckmann partial Geometric Schadowing component.

 @param[in]		n_dot_vl
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction. Or the clamped cosine of the light angle. The light
				angle is the angle between the surface normal and the light
				(hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Schlick-Beckmann partial Geometric Schadowing component.
 */
float G1_SchlickBeckmann(float n_dot_vl, float alpha) {

	//             n_dot_vl                               n_dot_vl
	// G1 := --------------------- = --------------------------------------------------
	//       n_dot_vl (1 - k) + k    n_dot_vl (1 - alpha sqrt(2/pi)) + alpha sqrt(2/pi)

	const float k = alpha * g_sqrt_2_inv_pi;

	return n_dot_vl / (n_dot_vl * (1.0f - k) + k);
}

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: Geometric Schadowing
//-----------------------------------------------------------------------------

/**
 Calculates the Implicit Geometric Schadowing component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Implicit Geometric Schadowing component.
 */
float G_Implicit(float n_dot_v, float n_dot_l,
	             float n_dot_h, float v_dot_h, float alpha) {

	// G := n_dot_v n_dot_l

	return n_dot_v * n_dot_l;
}

/**
 Calculates the Ward Geometric Schadowing component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Ward Geometric Schadowing component.
 */
float G_Ward(float n_dot_v, float n_dot_l,
	         float n_dot_h, float v_dot_h, float alpha) {

	// G := 1

	return 1.0f;
}

/**
 Calculates the Neumann Geometric Schadowing component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Neumann Geometric Schadowing component.
 */
float G_Neumann(float n_dot_v, float n_dot_l,
	            float n_dot_h, float v_dot_h, float alpha) {

	//         n_dot_v n_dot_l
	// G := ---------------------
	//      max(n_dot_v, n_dot_l)

	return (n_dot_v * n_dot_l) / max(n_dot_v, n_dot_l);
}

/**
 Calculates the Ashikhmin-Premoze Geometric Schadowing component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Ashikhmin-Premoze Geometric Schadowing component.
 */
float G_AshikhminPremoze(float n_dot_v, float n_dot_l,
	                     float n_dot_h, float v_dot_h, float alpha) {

	//                n_dot_v n_dot_l
	// G := -----------------------------------
	//      n_dot_v + n_dot_l - n_dot_v n_dot_l

	return (n_dot_v * n_dot_l) / (n_dot_v + n_dot_l - n_dot_v * n_dot_l);
}

/**
 Calculates the Kelemann Geometric Schadowing component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Kelemann Geometric Schadowing component.
 */
float G_Kelemann(float n_dot_v, float n_dot_l,
	             float n_dot_h, float v_dot_h, float alpha) {

	//      n_dot_v n_dot_l
	// G := ---------------
	//          v_dot_h

	return (n_dot_v * n_dot_l) / v_dot_h;
}

/**
 Calculates the Cook-Torrance Geometric Schadowing component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Cook-Torrance Geometric Schadowing component.
 */
float G_CookTorrance(float n_dot_v, float n_dot_l,
	                 float n_dot_h, float v_dot_h, float alpha) {

	//         [   2 n_dot_h n_dot_v  2 n_dot_h n_dot_l]
	// G := min[1, -----------------, -----------------]
	//         [        v_dot_h           v_dot_h      ]

	return min(1.0f, 2.0f * (n_dot_h / v_dot_h) * min(n_dot_v, n_dot_l));
}

/**
 Calculates the (correlated) GGX Geometric Schadowing component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The (correlated) GGX Geometric Schadowing component.
 */
float G_GGX(float n_dot_v, float n_dot_l,
	        float n_dot_h, float v_dot_h, float alpha) {

	//                                           2 (n_dot_l) (n_dot_v)
	// G := -------------------------------------------------------------------------------------------------
	//      n_dot_v sqrt(alpha^2 + (1 - alpha^2) n_dot_l^2) + n_dot_l sqrt(alpha^2 + (1 - alpha^2) n_dot_v^2)
	//
	//                1
	//    = -----------------------
	//      1 + Lambda_v + lambda_l
	//
	//            sqrt(alpha^2 + (1 - alpha^2) (n_dot_v)^2)   1
	// Lambda_v = ----------------------------------------- - -
	//                           2 n_dot_v                    2

	const float alpha2   = sqr(alpha);
	const float lambda_v = sqrt(alpha2 + (1.0f - alpha2) * sqr(n_dot_v));
	const float lambda_l = sqrt(alpha2 + (1.0f - alpha2) * sqr(n_dot_l));

	return (2.0f * n_dot_l * n_dot_v) / (n_dot_v * lambda_l + n_dot_l * lambda_v);
}

/**
 Calculates the Smith GGX Geometric Schadowing component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Smith GGX Geometric Schadowing component.
 */
float G_Smith_GGX(float n_dot_v, float n_dot_l,
	              float n_dot_h, float v_dot_h, float alpha) {

	return G1_GGX(n_dot_v, alpha) * G1_GGX(n_dot_l, alpha);
}

/**
 Calculates the Smith Schlick-GGX Geometric Schadowing component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Smith Schlick-GGX Geometric Schadowing component.
 */
float G_Smith_SchlickGGX(float n_dot_v, float n_dot_l,
	                     float n_dot_h, float v_dot_h, float alpha) {

	return G1_SchlickGGX(n_dot_v, alpha) * G1_SchlickGGX(n_dot_l, alpha);
}

/**
 Calculates the Smith Beckmann Geometric Schadowing component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Smith Beckmann Geometric Schadowing component.
 */
float G_Smith_Beckmann(float n_dot_v, float n_dot_l,
	                   float n_dot_h, float v_dot_h, float alpha) {

	return G1_Beckmann(n_dot_v, alpha) * G1_Beckmann(n_dot_l, alpha);
}

/**
 Calculates the Smith Schlick-Beckmann Geometric Schadowing component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Smith Schlick-Beckmann Geometric Schadowing component.
 */
float G_Smith_SchlickBeckmann(float n_dot_v, float n_dot_l,
	                          float n_dot_h, float v_dot_h, float alpha) {

	return G1_SchlickBeckmann(n_dot_v, alpha) * G1_SchlickBeckmann(n_dot_l, alpha);
}

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: Partial Visibility
//-----------------------------------------------------------------------------

//          G1
// V1 := --------
//       n_dot_vl

/**
 Calculates the GGX partial Visibility component.

 @param[in]		n_dot_vl
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction. Or the clamped cosine of the light angle. The light
				angle is the angle between the surface normal and the light
				(hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The GGX partial Visibility component.
 */
float V1_GGX(float n_dot_vl, float alpha) {

	//                               2
	// V1 := ---------------------------------------------------
	//       n_dot_vl + sqrt(alpha^2 + (1 - alpha^2) n_dot_vl^2)

	const float alpha2    = sqr(alpha);
	const float n_dot_vl2 = sqr(n_dot_vl);

	return 2.0f / (n_dot_vl * sqrt(alpha2 + (1.0f - alpha2) * n_dot_vl2));
}

/**
 Calculates the Schlick partial Visibility component.

 @param[in]		n_dot_vl
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction. Or the clamped cosine of the light angle. The light
				angle is the angle between the surface normal and the light
				(hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Schlick-GGX partial Visibility component.
 */
float V1_SchlickGGX(float n_dot_vl, float alpha) {

	//                 1                            1
	// V1 := --------------------- = --------------------------------
	//       n_dot_vl (1 - k) + k    n_dot_vl (1 - alpha/2) + alpha/2

	const float k = 0.5f * alpha;

	return 1.0f / (n_dot_vl * (1.0f - k) + k);
}

/**
 Calculates the Schlick-Beckmann partial Visibility component.

 @param[in]		n_dot_vl
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction. Or the clamped cosine of the light angle. The light
				angle is the angle between the surface normal and the light
				(hit-to-light) direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Schlick-Beckmann partial Visibility component.
 */
float V1_SchlickBeckmann(float n_dot_vl, float alpha) {

	//                1                                     1
	// V1 := --------------------- = --------------------------------------------------
	//       n_dot_vl (1 - k) + k    n_dot_vl (1 - alpha sqrt(2/pi)) + alpha sqrt(2/pi)

	const float k = alpha * g_sqrt_2_inv_pi;

	return 1.0f / (n_dot_vl * (1.0f - k) + k);
}

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: Visibility
//-----------------------------------------------------------------------------

//            G
// V := ---------------
//      n_dot_v n_dot_l

/**
 Calculates the Implicit Visibility component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Implicit Visibility component.
 */
float V_Implicit(float n_dot_v, float n_dot_l,
	             float n_dot_h, float v_dot_h, float alpha) {

	// V := 1

	return 1.0f;
}

/**
 Calculates the Ward Visibility component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Ward Visibility component.
 */
float V_Ward(float n_dot_v, float n_dot_l,
	         float n_dot_h, float v_dot_h, float alpha) {

	//             1
	// V := ---------------
	//      n_dot_v n_dot_l

	return 1.0f / (n_dot_v * n_dot_l);
}

/**
 Calculates the Neumann Visibility component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Neumann Visibility component.
 */
float V_Neumann(float n_dot_v, float n_dot_l,
	            float n_dot_h, float v_dot_h, float alpha) {

	//               1
	// V := ---------------------
	//      max(n_dot_v, n_dot_l)

	return 1.0f / max(n_dot_v, n_dot_l);
}

/**
 Calculates the Ashikhmin-Premoze Visibility component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Ashikhmin-Premoze Visibility component.
 */
float V_AshikhminPremoze(float n_dot_v, float n_dot_l,
	                     float n_dot_h, float v_dot_h, float alpha) {

	//                      1
	// V := -----------------------------------
	//      n_dot_v + n_dot_l - n_dot_v n_dot_l

	return 1.0f / (n_dot_v + n_dot_l - n_dot_v * n_dot_l);
}

/**
 Calculates the Kelemann Visibility component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Kelemann Visibility component.
 */
float V_Kelemann(float n_dot_v, float n_dot_l,
	             float n_dot_h, float v_dot_h, float alpha) {

	//         1
	// V := -------
	//      v_dot_h

	return 1.0f / v_dot_h;
}

/**
 Calculates the Cook-Torrance Visibility component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Cook-Torrance Visibility component.
 */
float V_CookTorrance(float n_dot_v, float n_dot_l,
	                 float n_dot_h, float v_dot_h, float alpha) {

	//         [       1            2 n_dot_h        2 n_dot_h   ]
	// V := min[---------------, ---------------, ---------------]
	//         [n_dot_v n_dot_l  v_dot_h n_dot_l  v_dot_h n_dot_v]

	const float v_ward    = 1.0f / (n_dot_v * n_dot_l);
	const float v_neumann = 1.0f / max(n_dot_v, n_dot_l);

	return min(v_ward, 2.0f * (n_dot_h / v_dot_h) * v_neumann);
}

/**
 Calculates the (correlated) GGX Visibility component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The (correlated) GGX Visibility component.
 */
float V_GGX(float n_dot_v, float n_dot_l,
	        float n_dot_h, float v_dot_h, float alpha) {

	//                                                      2
	// V := -------------------------------------------------------------------------------------------------
	//      n_dot_v sqrt(alpha^2 + (1 - alpha^2) n_dot_l^2) + n_dot_l sqrt(alpha^2 + (1 - alpha^2) n_dot_v^2)

	const float alpha2   = sqr(alpha);
	const float lambda_v = sqrt(alpha2 + (1.0f - alpha2) * sqr(n_dot_v));
	const float lambda_l = sqrt(alpha2 + (1.0f - alpha2) * sqr(n_dot_l));

	return 2.0f / (n_dot_v * lambda_l + n_dot_l * lambda_v);
}

/**
 Calculates the Smith GGX Visibility component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Smith GGX Visibility component.
 */
float V_Smith_GGX(float n_dot_v, float n_dot_l,
	              float n_dot_h, float v_dot_h, float alpha) {

	return V1_GGX(n_dot_v, alpha) * V1_GGX(n_dot_l, alpha);
}

/**
 Calculates the Smith Schlick-GGX Visibility component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Smith Schlick-GGX Visibility component.
 */
float V_Smith_SchlickGGX(float n_dot_v, float n_dot_l,
	                     float n_dot_h, float v_dot_h, float alpha) {

	return V1_SchlickGGX(n_dot_v, alpha) * V1_SchlickGGX(n_dot_l, alpha);
}

/**
 Calculates the Smith Beckmann Visibility component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Smith Beckmann Visibility component.
 */
float V_Smith_Beckmann(float n_dot_v, float n_dot_l,
	                   float n_dot_h, float v_dot_h, float alpha) {

	return G_Smith_Beckmann(n_dot_v, n_dot_l, n_dot_h, v_dot_h, alpha)
			/ (n_dot_v * n_dot_l);
}

/**
 Calculates the Smith Schlick-Beckmann Visibility component.

 @param[in]		n_dot_v
				The clamped cosine of the view angle. The view angle is the
				angle between the surface normal and the view (hit-to-eye)
				direction.
 @param[in]		n_dot_l
				The clamped cosine of the light angle. The light angle is the
				angle between the surface normal and the light (hit-to-light)
				direction.
 @param[in]		n_dot_h
				The clamped cosine of the half angle. The half angle is the
				angle between the surface normal and the half direction between
				the view (hit-to-eye) and light (hit-to-light) direction.
 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		alpha
				The alpha value which is equal to the square of the surface
				roughness.
 @return		The Smith Schlick-Beckmann Visibility component.
 */
float V_Smith_SchlickBeckmann(float n_dot_v, float n_dot_l,
	                          float n_dot_h, float v_dot_h, float alpha) {

	return V1_SchlickBeckmann(n_dot_v, alpha) * V1_SchlickBeckmann(n_dot_l, alpha);
}

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: Fresnel
//-----------------------------------------------------------------------------

/**
 Calculates the None Fresnel component.

 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		F0
				The reflectance at normal incidence.
 @return		The None Fresnel component.
 */
float F_None(float v_dot_h, float F0) {
	// F := F0

	return F0;
}

/**
 Calculates the None Fresnel component.

 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		F0
				The reflectance at normal incidence.
 @return		The None Fresnel component.
 */
float3 F_None(float v_dot_h, float3 F0) {
	// F := F0

	return F0;
}

/**
 Calculates the Schlick Fresnel component.

 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		F0
				The reflectance at normal incidence.
 @param[in]		F90
				The reflectance at tangent incidence.
 @return		The Schlick Fresnel component.
 */
float F_Schlick(float v_dot_h, float F0, float F90) {
	// F := F0 + (F90 - F0) (1 - v_dot_h)^5

	const float m  = (1.0f - v_dot_h);
	const float m2 = sqr(m);
	const float m5 = sqr(m2) * m;

	return lerp(F0, F90, m5);
}

/**
 Calculates the Schlick Fresnel component.

 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		F0
				The reflectance at normal incidence.
 @param[in]		F90
				The reflectance at tangent incidence.
 @return		The Schlick Fresnel component.
 */
float3 F_Schlick(float v_dot_h, float3 F0, float F90) {
	// F := F0 + (F90 - F0) (1 - v_dot_h)^5

	const float m = (1.0f - v_dot_h);
	const float m2 = sqr(m);
	const float m5 = sqr(m2) * m;

	return lerp(F0, F90, m5);
}

/**
 Calculates the Schlick Fresnel component.

 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		F0
				The reflectance at normal incidence.
 @return		The Schlick Fresnel component.
 */
float F_Schlick(float v_dot_h, float F0) {
	// F := F0 + (1 - F0) (1 - v_dot_h)^5

	return F_Schlick(v_dot_h, F0, 1.0f);
}

/**
 Calculates the Schlick Fresnel component.

 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		F0
				The reflectance at normal incidence.
 @return		The Schlick Fresnel component.
 */
float3 F_Schlick(float v_dot_h, float3 F0) {
	// F := F0 + (1 - F0) (1 - v_dot_h)^5

	return F_Schlick(v_dot_h, F0, 1.0f);
}

/**
 Calculates the Cook-Torrance Fresnel component.

 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		F0
				The reflectance at normal incidence.
 @return		The Cook-Torrance Fresnel component.
 */
float F_CookTorrance(float v_dot_h, float F0) {

	// c   := v_dot_h
	//
	//        1 + sqrt(F0)
	// eta := ------------
	//        1 - sqrt(F0)
	//
	// g   := sqrt(eta^2 + c^2 - 1)
    //
	//        1 [g - c]^2 [    [(g + c) c - 1]^2]
	// F   := - [-----]   [1 + [-------------]  ]
	//        2 [g + c]   [    [(g - c) c + 1]  ]

	const float sqrt_F0  = sqrt(F0);
	const float eta      = (1.0f + sqrt_F0) / (1.0f - sqrt_F0);
	const float eta2     = sqr(eta);
	const float v_dot_h2 = sqr(v_dot_h);
	const float g        = sqrt(eta2 + v_dot_h2 - 1.0f);
	const float g1       = g + v_dot_h;
	const float g2       = g - v_dot_h;

	return 0.5f * sqr(g2 / g1) * (1.0f + sqr((g1 * v_dot_h - 1.0f) / (g2 * v_dot_h + 1.0f)));
}

/**
 Calculates the Cook-Torrance Fresnel component.

 @param[in]		v_dot_h
				The clamped cosine of the difference angle. The difference
				angle is the angle between the view (hit-to-eye) direction and
				half direction and is the angle between the light
				(hit-to-light) direction and half direction.
 @param[in]		F0
				The reflectance at normal incidence.
 @return		The Cook-Torrance Fresnel component.
 */
float3 F_CookTorrance(float v_dot_h, float3 F0) {

	// c   := v_dot_h
	//
	//        1 + sqrt(F0)
	// eta := ------------
	//        1 - sqrt(F0)
	//
	// g   := sqrt(eta^2 + c^2 - 1)
    //
	//        1 [g - c]^2 [    [(g + c) (c - 1)]^2]
	// F   := - [-----]   [1 + [---------------]  ]
	//        2 [g + c]   [    [(g - c) (c + 1)]  ]

	const float3 sqrt_F0  = sqrt(F0);
	const float3 eta      = (1.0f + sqrt_F0) / (1.0f - sqrt_F0);
	const float3 eta2     = sqr(eta);
	const float3 v_dot_h2 = sqr(v_dot_h);
	const float3 g        = sqrt(eta2 + v_dot_h2 - 1.0f);
	const float3 g1       = g + v_dot_h;
	const float3 g2       = g - v_dot_h;

	return 0.5f * sqr(g2 / g1) * (1.0f + sqr((g1 * (v_dot_h - 1.0f))
										   / (g2 * (v_dot_h + 1.0f))));
}

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: Material
//-----------------------------------------------------------------------------

float3 F0_Specular(Material material) {
	return lerp(g_dielectric_F0, material.m_base_color, material.m_metalness);
}

float RoughnessToAlpha(float roughness) {
	return max(BRDF_MINIMUM_ALPHA, sqr(roughness));
} 

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions: BRDFs
//-----------------------------------------------------------------------------

#ifdef BRDF
	#error Illegal symbol definition.
#endif

/**
 A struct of BRDFs.
 */
struct BRDF {

	//-------------------------------------------------------------------------
	// Member Variables
	//-------------------------------------------------------------------------

	/**
	 The diffuse component of the BRDF.
	 */
	float3 m_diffuse;

	/**
	 The specular component of the BRDF.
	 */
	float3 m_specular;
};

BRDF ConstructBRDF(float3 brdf_diffuse, float3 brdf_specular) {
	#ifdef DISABLE_BRDF_DIFFUSE
	brdf_diffuse = 0.0f;
	#endif // DISABLE_BRDF_DIFFUSE

	#ifdef DISABLE_BRDF_SPECULAR
	brdf_specular = 0.0f;
	#endif // DISABLE_BRDF_SPECULAR

	const BRDF brdf = { brdf_diffuse, brdf_specular };
	return brdf;
}

/**
 Calculates the Lambertian BRDF.

 @param[in]		material
				The material.
 @return		The Lambertian BRDF.
 */
BRDF LambertianBRDF(Material material) {
	const float3 brdf_diffuse  = (1.0f - material.m_metalness)
		                         * material.m_base_color * g_inv_pi;
	const float3 brdf_specular = 0.0f;
	return ConstructBRDF(brdf_diffuse, brdf_specular);
}

/**
 Calculates the Lambertian BRDF.

 @pre			@a n is normalized.
 @pre			@a l is normalized.
 @pre			@a v is normalized.
 @param[in]		n
				The surface normal expressed in shading space.
 @param[in]		l
				The light (hit-to-light) direction expressed in shading space.
 @param[in]		v
				The view (hit-to-eye) direction expressed in shading space.
 @param[in]		material
				The material.
 @return		The Lambertian BRDF.
 */
BRDF LambertianBRDF(float3 n, float3 l, float3 v, Material material) {
	const float3 brdf_diffuse  = (1.0f - material.m_metalness)
		                         * material.m_base_color * g_inv_pi;
	const float3 brdf_specular = 0.0f;
	return ConstructBRDF(brdf_diffuse, brdf_specular);
}

float FrostbiteDiffuseBRDF(float n_dot_v, float n_dot_l,
						   float v_dot_h, float roughness) {

	const float F0  = 1.0f;
	const float F90 = (0.5f + 2.0f * sqr(v_dot_h)) * roughness;
	const float F_v = F_Schlick(n_dot_v, F0, F90);
	const float F_l = F_Schlick(n_dot_l, F0, F90);

	const float energy_factor = lerp(1.0f, 1.0f / 1.51f, roughness);

	return energy_factor * F_v * F_l;
}

/**
 Calculates the Frostbite BRDF.

 @pre			@a n is normalized.
 @pre			@a l is normalized.
 @pre			@a v is normalized.
 @param[in]		n
				The surface normal expressed in shading space.
 @param[in]		l
				The light (hit-to-light) direction expressed in shading space.
 @param[in]		v
				The view (hit-to-eye) direction expressed in shading space.
 @param[in]		material
				The material.
 @return		The Cook-Torrance BRDF.
 */
BRDF FrostbiteBRDF(float3 n, float3 l, float3 v, Material material) {
	const float  alpha         = RoughnessToAlpha(material.m_roughness);
	const float  n_dot_l       = sat_dot(n, l) + BRDF_DOT_EPSILON;
	const float  n_dot_v       = sat_dot(n, v) + BRDF_DOT_EPSILON;
	const float3 h             = HalfDirection(l, v);
	const float  n_dot_h       = sat_dot(n, h) + BRDF_DOT_EPSILON;
	const float  v_dot_h       = sat_dot(v, h) + BRDF_DOT_EPSILON;

	const float  D             = BRDF_D_FUNCTION(n_dot_h, alpha);
	const float  V             = BRDF_V_FUNCTION(n_dot_v, n_dot_l, n_dot_h, v_dot_h, alpha);
	const float3 F_specular    = BRDF_F_FUNCTION(v_dot_h, F0_Specular(material));
	const float  F_diffuse     = FrostbiteDiffuseBRDF(n_dot_v, n_dot_l, v_dot_h, material.m_roughness)
		                         * (1.0f - material.m_metalness);

	const float3 brdf_diffuse  = F_diffuse * material.m_base_color * g_inv_pi;
	const float3 brdf_specular = F_specular * 0.25f * D * V;
	return ConstructBRDF(brdf_diffuse, brdf_specular);
}

/**
 Calculates the Cook-Torrance BRDF.

 @pre			@a n is normalized.
 @pre			@a l is normalized.
 @pre			@a v is normalized.
 @param[in]		n
				The surface normal expressed in shading space.
 @param[in]		l
				The light (hit-to-light) direction expressed in shading space.
 @param[in]		v
				The view (hit-to-eye) direction expressed in shading space.
 @param[in]		material
				The material.
 @return		The Cook-Torrance BRDF.
 */
BRDF CookTorranceBRDF(float3 n, float3 l, float3 v, Material material) {
	const float  alpha         = RoughnessToAlpha(material.m_roughness);
	const float  n_dot_l       = sat_dot(n, l) + BRDF_DOT_EPSILON;
	const float  n_dot_v       = sat_dot(n, v) + BRDF_DOT_EPSILON;
	const float3 h             = HalfDirection(l, v);
	const float  n_dot_h       = sat_dot(n, h) + BRDF_DOT_EPSILON;
	const float  v_dot_h       = sat_dot(v, h) + BRDF_DOT_EPSILON;

	const float  D             = BRDF_D_FUNCTION(n_dot_h, alpha);
	const float  V             = BRDF_V_FUNCTION(n_dot_v, n_dot_l, n_dot_h, v_dot_h, alpha);
	const float3 F_specular    = BRDF_F_FUNCTION(v_dot_h, F0_Specular(material));
	const float3 F_diffuse     = (1.0f - F_specular) * (1.0f - material.m_metalness);

	const float3 brdf_diffuse  = F_diffuse * material.m_base_color * g_inv_pi;
	const float3 brdf_specular = F_specular * 0.25f * D * V;
	return ConstructBRDF(brdf_diffuse, brdf_specular);
}

#endif // MAGE_HEADER_BRDF
