#pragma once
#include "DXCore.h"
#include <string>
#include <fstream>
#include <sstream>

#include <D3Dcompiler.h>
#include <d3d11shader.h>

#pragma comment(lib, "dxguid.lib")


struct alignas(16) ConstantBuffer
{
	float time;
};

struct alignas(16) ConstantBuffer2
{
	float time;
	float padding[3];
	Vec4 lights[4];
};


ID3D11Buffer* constantBuffer;


class Shader {
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* layout;

	ConstantBuffer2 constBufferCPU2;


	std::string readFile(std::string filename) {
		std::ifstream infile(filename);
		std::stringstream output;
		output << infile.rdbuf();
		infile.close();
		return output.str();
	}

	void init(std::string VS_filename , std::string PS_filename, DXCOre* core) {
		std::string vs = readFile(VS_filename);
		std::string ps = readFile(PS_filename);


		//here?
		constBufferCPU2.time = 0;

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		bd.ByteWidth = sizeof(ConstantBuffer2);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		core->device->CreateBuffer(&bd, NULL, &constantBuffer);



		compileVS(vs, core);
		compilePS(ps, core);
	}

	void compileVS(std::string vertexShaderHLSL, DXCOre* core) {
		ID3DBlob* compiledVertexShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(vertexShaderHLSL.c_str(), strlen(vertexShaderHLSL.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);
		if (FAILED(hr))
		{
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
			exit(0);
		}
		core->device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);


		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		core->device->CreateInputLayout(layoutDesc, 2, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

	}

	void compilePS(std::string pixelShaderHLSL, DXCOre* core) {
		ID3DBlob* compiledPixelShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(pixelShaderHLSL.c_str(), strlen(pixelShaderHLSL.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &compiledPixelShader, &status);
		if (FAILED(hr))
		{
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Pixel Shader Error", 0);
			exit(0);
		}
		core->device->CreatePixelShader(compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), NULL, &pixelShader);
	}

	void apply(DXCOre* core) {

		//here?!
		D3D11_MAPPED_SUBRESOURCE mapped;
		core->devicecontext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		memcpy(mapped.pData, &constBufferCPU2, sizeof(ConstantBuffer2));
		core->devicecontext->Unmap(constantBuffer, 0);


		core->devicecontext->IASetInputLayout(layout);
		core->devicecontext->VSSetShader(vertexShader, NULL, 0);
		core->devicecontext->PSSetShader(pixelShader, NULL, 0);

		core->devicecontext->PSSetConstantBuffers(0, 1, &constantBuffer);

	}

};