#pragma once
#include "DXCore.h"

class TextureDraw {
public:
    TextureDraw( int width, int height): width(width), height(height), texRTV(nullptr), texSRV(nullptr){
    }

    ~TextureDraw() {
        cleanup();
    }

    // Bind reflection as the current render target
    void drawToTexture(DXCOre* core) {
        core->devicecontext->OMSetRenderTargets(1, &texRTV, depthStencilView);
        core->devicecontext->RSSetViewports(1, &viewport);
        clearTarget(core, texRTV);
    }


    // Get the reflection shader resource view
    ID3D11ShaderResourceView* gettexSRV() const {
        return texSRV;
    }

    // Unbind all SRVs from the pixel shader
    void unbindSRVs(DXCOre* core) {
        core->devicecontext->PSSetShaderResources(0, 1, nullptr);
    }

    void init(DXCOre* core, int _width, int _height) {
        // Create textures and views
        width = _width;
        height = _height;
        createRenderTargetAndSRV(core);
    }

private:
    int width, height;

    ID3D11RenderTargetView* texRTV;
    ID3D11ShaderResourceView* texSRV;
    ID3D11Texture2D* tex;

    ID3D11Texture2D* depthStencilBuffer;
    ID3D11DepthStencilView* depthStencilView;
    D3D11_VIEWPORT viewport;

    void createRenderTargetAndSRV(DXCOre* core) {
        // Texture description
        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        core->device->CreateTexture2D(&textureDesc, NULL, &tex);


        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
        ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;
        core->device->CreateRenderTargetView(tex, &renderTargetViewDesc, &texRTV);



        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
        ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
        shaderResourceViewDesc.Format = textureDesc.Format;
        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
        shaderResourceViewDesc.Texture2D.MipLevels = 1;
        core->device->CreateShaderResourceView(tex, &shaderResourceViewDesc, &texSRV);


        D3D11_TEXTURE2D_DESC depthBufferDesc;
        ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

        // Set up the description of the depth buffer.
        depthBufferDesc.Width = width;
        depthBufferDesc.Height = height;
        depthBufferDesc.MipLevels = 1;
        depthBufferDesc.ArraySize = 1;
        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.SampleDesc.Count = 1;
        depthBufferDesc.SampleDesc.Quality = 0;
        depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthBufferDesc.CPUAccessFlags = 0;
        depthBufferDesc.MiscFlags = 0;

        core->device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);



        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

        // Set up the depth stencil view description.
        depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;
        core->device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);

        viewport.Width = (float)width;
        viewport.Height = (float)height;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        // Release the texture (RTV and SRV hold references)
        tex->Release();
    }

    void clearTarget(DXCOre* core, ID3D11RenderTargetView* rtv) {
        float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        core->devicecontext->ClearRenderTargetView(rtv, clearColor);
        core->devicecontext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    void cleanup() {
        if (texRTV) texRTV->Release();
        if (texSRV) texSRV->Release();
    }
};
