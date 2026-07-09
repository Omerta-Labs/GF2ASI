#include "GammaFix.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/Settings.h"
#include "Addons/tConsole.h"

// Pl2
#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

#include <cmath>
#include <d3d9.h>

#include "GammaFixShader.h"

uint64_t Displ_SetGamma_Old;
void __cdecl HOOK_Displ_SetGamma(float* xGamContBrite);

namespace
{
	// Game globals (Steam exe)
	hook::Type<IDirect3DDevice9*> Dx9Device = hook::Type<IDirect3DDevice9*>(0x1205750);
	hook::Type<D3DPRESENT_PARAMETERS> Dx9PresentParams = hook::Type<D3DPRESENT_PARAMETERS>(0x11D9188);

	// Last curve requested through Displ_SetGamma: gamma, contrast, brightness.
	// The game only ever varies gamma (clamped to [0.8, 1.2] by GameData); it
	// always passes contrast = 1 and brightness = 0.
	float LastGammaParams[3] = { 1.0f, 1.0f, 0.0f };

	// Desktop gamma ramp captured before the game touches the hardware ramp,
	// so ICC/night-light calibration can be preserved and restored.
	D3DGAMMARAMP DesktopGammaRamp;
	bool bHasDesktopRamp = false;

	// Whether we let a (non-identity) game ramp through to the hardware
	bool bGameRampActive = false;

	// Windowed post-process objects, created lazily and released on device reset
	IDirect3DPixelShader9* GammaPixelShader = nullptr;
	IDirect3DTexture9* FrameCopyTexture = nullptr;
	D3DSURFACE_DESC FrameCopyDesc = {};
	bool bShaderCreateFailed = false;

	struct GammaQuadVertex
	{
		float x, y, z, rhw;
		float u, v;
	};

	bool IsIdentityCurve()
	{
		return std::fabs(LastGammaParams[0] - 1.0f) < 0.0001f
			&& std::fabs(LastGammaParams[1] - 1.0f) < 0.0001f
			&& std::fabs(LastGammaParams[2]) < 0.0001f;
	}

	bool IsWindowed()
	{
		return Dx9PresentParams->Windowed != FALSE;
	}

	// Snapshot the desktop ramp via GDI before the game enters exclusive mode.
	// The GDI ramp is WORD[3][256], identical in layout to D3DGAMMARAMP.
	void CaptureDesktopGammaRamp()
	{
		if (HDC ScreenDC = GetDC(nullptr))
		{
			bHasDesktopRamp = GetDeviceGammaRamp(ScreenDC, &DesktopGammaRamp) != FALSE;
			ReleaseDC(nullptr, ScreenDC);
		}

		if (!bHasDesktopRamp)
		{
			tConsole::fWriteLine("GammaFix: could not capture desktop gamma ramp, identity ramps will pass through");
		}
	}

	void ReleaseFrameCopy()
	{
		if (FrameCopyTexture)
		{
			FrameCopyTexture->Release();
			FrameCopyTexture = nullptr;
		}
	}

	// (Re)create the frame copy target to match the current backbuffer
	bool EnsureFrameCopy(IDirect3DDevice9* Device, const D3DSURFACE_DESC& BackBufferDesc)
	{
		if (FrameCopyTexture
			&& FrameCopyDesc.Width == BackBufferDesc.Width
			&& FrameCopyDesc.Height == BackBufferDesc.Height
			&& FrameCopyDesc.Format == BackBufferDesc.Format)
		{
			return true;
		}

		ReleaseFrameCopy();

		if (FAILED(Device->CreateTexture(BackBufferDesc.Width, BackBufferDesc.Height, 1,
			D3DUSAGE_RENDERTARGET, BackBufferDesc.Format, D3DPOOL_DEFAULT, &FrameCopyTexture, nullptr)))
		{
			return false;
		}

		FrameCopyDesc = BackBufferDesc;
		return true;
	}

	void DrawGammaQuad(IDirect3DDevice9* Device, const D3DSURFACE_DESC& BackBufferDesc)
	{
		Device->SetPixelShader(GammaPixelShader);
		Device->SetVertexShader(nullptr);
		Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
		Device->SetTexture(0, FrameCopyTexture);

		const float ShaderParams[4] = { LastGammaParams[0], LastGammaParams[1], LastGammaParams[2], 0.0f };
		Device->SetPixelShaderConstantF(0, ShaderParams, 1);

		Device->SetRenderState(D3DRS_ZENABLE, FALSE);
		Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		Device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		Device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		Device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
		Device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);

		Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		Device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, FALSE);

		D3DVIEWPORT9 FullViewport = { 0, 0, BackBufferDesc.Width, BackBufferDesc.Height, 0.0f, 1.0f };
		Device->SetViewport(&FullViewport);

		// half-pixel offset maps texels 1:1 onto pixels
		const float W = static_cast<float>(BackBufferDesc.Width);
		const float H = static_cast<float>(BackBufferDesc.Height);
		const GammaQuadVertex Verts[4] =
		{
			{ -0.5f,    -0.5f,    0.0f, 1.0f, 0.0f, 0.0f },
			{ W - 0.5f, -0.5f,    0.0f, 1.0f, 1.0f, 0.0f },
			{ -0.5f,    H - 0.5f, 0.0f, 1.0f, 0.0f, 1.0f },
			{ W - 0.5f, H - 0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
		};

		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Verts, sizeof(GammaQuadVertex));
	}

	// Fullscreen-only: decide what actually reaches the hardware gamma ramp
	void ApplyHardwareRamp()
	{
		if (IsIdentityCurve())
		{
			// Option 3: an identity ramp would only clobber the desktop
			// calibration (ICC profile, night light). Restore the desktop ramp
			// if we previously let a game ramp through, otherwise leave the
			// hardware untouched.
			if (bGameRampActive)
			{
				if (bHasDesktopRamp && Dx9Device != nullptr)
				{
					Dx9Device->SetGammaRamp(0, D3DSGR_NO_CALIBRATION, &DesktopGammaRamp);
				}
				else
				{
					// no snapshot to restore; at least reset to the game's identity ramp
					PLH::FnCast(Displ_SetGamma_Old, &HOOK_Displ_SetGamma)(LastGammaParams);
				}

				bGameRampActive = false;
			}

			return;
		}

		bGameRampActive = true;
		PLH::FnCast(Displ_SetGamma_Old, &HOOK_Displ_SetGamma)(LastGammaParams);
	}
}

void __cdecl HOOK_Displ_SetGamma(float* xGamContBrite)
{
	if (!Settings::GetCheckedRef().ApplyBrightnessFix())
	{
		PLH::FnCast(Displ_SetGamma_Old, &HOOK_Displ_SetGamma)(xGamContBrite);
		return;
	}

	LastGammaParams[0] = xGamContBrite[0];
	LastGammaParams[1] = xGamContBrite[1];
	LastGammaParams[2] = xGamContBrite[2];

	// Option 1: in windowed mode SetGammaRamp is a silent no-op, so skip it and
	// let OnEndScene apply the same curve as a post-process instead.
	if (IsWindowed())
	{
		return;
	}

	ApplyHardwareRamp();
}

void Mod::GammaFix::StaticApplyHooks()
{
	CaptureDesktopGammaRamp();

	PLH::ZydisDisassembler dis(PLH::Mode::x86);

	PLH::x86Detour detour190((char*)0x609090, (char*)&HOOK_Displ_SetGamma, &Displ_SetGamma_Old, dis);
	detour190.hook();
}

void Mod::GammaFix::OnEndScene()
{
	if (!Settings::GetCheckedRef().ApplyBrightnessFix())
	{
		return;
	}

	if (!IsWindowed() || IsIdentityCurve() || bShaderCreateFailed)
	{
		return;
	}

	IDirect3DDevice9* Device = Dx9Device;
	if (!Device)
	{
		return;
	}

	if (!GammaPixelShader)
	{
		if (FAILED(Device->CreatePixelShader(reinterpret_cast<const DWORD*>(g_GammaFixPixelShader), &GammaPixelShader)))
		{
			tConsole::fWriteLine("GammaFix: CreatePixelShader failed, windowed gamma disabled");
			bShaderCreateFailed = true;
			return;
		}
	}

	IDirect3DSurface9* BackBuffer = nullptr;
	if (FAILED(Device->GetRenderTarget(0, &BackBuffer)) || !BackBuffer)
	{
		return;
	}

	D3DSURFACE_DESC BackBufferDesc = {};
	BackBuffer->GetDesc(&BackBufferDesc);

	if (EnsureFrameCopy(Device, BackBufferDesc))
	{
		IDirect3DSurface9* CopySurface = nullptr;
		if (SUCCEEDED(FrameCopyTexture->GetSurfaceLevel(0, &CopySurface)) && CopySurface)
		{
			if (SUCCEEDED(Device->StretchRect(BackBuffer, nullptr, CopySurface, nullptr, D3DTEXF_NONE)))
			{
				// snapshot the whole pipeline, same approach as the ImGui backend
				IDirect3DStateBlock9* StateBlock = nullptr;
				if (SUCCEEDED(Device->CreateStateBlock(D3DSBT_ALL, &StateBlock)) && StateBlock)
				{
					DrawGammaQuad(Device, BackBufferDesc);

					StateBlock->Apply();
					StateBlock->Release();
				}
			}

			CopySurface->Release();
		}
	}

	BackBuffer->Release();
}

void Mod::GammaFix::OnDeviceReset()
{
	ReleaseFrameCopy();

	if (GammaPixelShader)
	{
		GammaPixelShader->Release();
		GammaPixelShader = nullptr;
	}
}
