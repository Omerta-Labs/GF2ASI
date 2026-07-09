// Reimplementation of the game's Displ_SetGamma (0x609090) hardware ramp curve
// as a post-process pixel shader. Used in windowed mode, where the game's
// IDirect3DDevice9::SetGammaRamp call is silently ignored by D3D9.
//
// The curve matches the ramp the game builds per channel:
//   out = saturate(pow(in, gamma) * contrast + (1 - contrast) * 0.5 + brightness)
//
// Regenerate the embedded bytecode with (Windows SDK):
//   fxc /nologo /T ps_2_0 /E main /Fh GammaFixShader.h /Vn g_GammaFixPixelShader GammaCorrect.hlsl

sampler2D FrameSampler : register(s0);

// x = gamma exponent, y = contrast, z = brightness (Displ_SetGamma inputs)
float4 GammaParams : register(c0);

float4 main(float2 UV : TEXCOORD0) : COLOR0
{
	float4 Frame = tex2D(FrameSampler, UV);

	float3 Curved = pow(saturate(Frame.rgb), GammaParams.x);
	Curved = (Curved * GammaParams.y) + ((1.0f - GammaParams.y) * 0.5f) + GammaParams.z;

	return float4(saturate(Curved), Frame.a);
}
