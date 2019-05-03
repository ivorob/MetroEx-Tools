#pragma once

#include <dxgi.h>
#include <d3d11.h>

#include "mymath.h"
#include "camera.h"

class MetroModel;
class MetroSkeleton;
class MetroMotion;
class MetroTexture;

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MetroEX {
    struct RenderTexture {
        ID3D11Texture2D*            tex;
        ID3D11ShaderResourceView*   srv;
    };

    struct RenderGeometry {
        ID3D11Buffer*   vb;
        ID3D11Buffer*   ib;
        size_t          numFaces;
        RenderTexture*  texture;
        bool            isCollision;
    };

    struct ViewingParams {
        vec2  lastLMPos;
        vec2  lastRMPos;
        vec2  offset;
        float zoom;
    };

    struct ConstantBufferData {
        vec4 modelBSphere;
        mat4 matModel;
        mat4 matView;
        mat4 matProjection;
        mat4 matModelView;
        mat4 matModelViewProj;
        vec4 camParams;
        mat4 bones[256];
    };

    struct AnimBone {
        size_t  idx;
        size_t  parentIdx;
    };

    struct Animation {
        float       time;
        AnimBone    bones[256];
        mat4        bindPoseInv[256];
    };


    public ref class RenderPanel : public System::Windows::Forms::Panel {
    public:
        RenderPanel();

    protected:
        ~RenderPanel() {
            if (components) {
                delete components;
            }
        }

    public:
        bool        InitGraphics();
        void        SetModel(MetroModel* model);
        MetroModel* GetModel();
        void        SetCubemap(MetroTexture* cubemap);
        void        SetShowWireframe(const bool wireframe);
        void        SetShowCollision(const bool collision);

        void        SwitchMotion(const size_t idx);
        bool        IsPlayingAnim();
        void        PlayAnim(const bool play);

        void        ResetCamera(const bool forceRender);

    private:
        bool        CreateRenderTargets();
        void        UpdateCamera();
        void        CreateModelGeometries();
        void        CreateTextures();
        void        CreateRenderTexture(const MetroTexture* srcTexture, RenderTexture* rt);
        void        ResetAnimation();
        void        UpdateAnimation(const float dt);
        void        Render();

    protected:
        virtual void OnResize(System::EventArgs^ e) override;
        virtual void OnPaint(System::Windows::Forms::PaintEventArgs^ e) override;
        virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs^ e) override;
        virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) override;
        virtual void OnMouseWheel(System::Windows::Forms::MouseEventArgs^ e) override;

        void AnimationTimer_Tick(System::Object^ sender, System::EventArgs^ e);


    private: System::ComponentModel::IContainer^  components;

    private:
        IDXGISwapChain*             mSwapChain;
        ID3D11Device*               mDevice;
        ID3D11DeviceContext*        mDeviceContext;
        ID3D11RenderTargetView*     mRenderTargetView;
        ID3D11Texture2D*            mDepthStencilBuffer;
        ID3D11DepthStencilState*    mDepthStencilState;
        ID3D11DepthStencilView*     mDepthStencilView;
        ID3D11RasterizerState*      mRasterState;
        ID3D11RasterizerState*      mRasterStateWireframe;
        // model viewer
        ID3D11VertexShader*         mModelViewerVS;
        ID3D11PixelShader*          mModelViewerPS;
        ID3D11PixelShader*          mModelViewerWireframePS;
        ID3D11InputLayout*          mModelInputLayout;
        ID3D11Buffer*               mModelConstantBuffer;
        ID3D11SamplerState*         mModelTextureSampler;
        array<RenderGeometry*>^     mModelGeometries;
        System::Collections::Generic::Dictionary<String^,IntPtr>^ mModelTextures;

        // model viewer stuff
        MetroModel*                 mModel;
        // animations
        const MetroMotion*          mCurrentMotion;
        Animation*                  mAnimation;
        System::Windows::Forms::Timer^  mAnimTimer;

        // cubemap viewer stuff
        Camera*                     mCamera;
        MetroTexture*               mCubemap;
        RenderTexture*              mCubemapTexture;
        ID3D11VertexShader*         mCubemapViewerVS;
        ID3D11PixelShader*          mCubemapViewerPS;

        PointF                      mLastLMPos;
        PointF                      mLastRMPos;
        float                       mZoom;
        bool                        mShowWireframe;
        bool                        mShowCollision;
        ConstantBufferData*         mConstantBufferData;
    };

}

