#pragma once

#include "Shader.h"

enum ShaderType
{
	StaticShader,
	AnimatedShader,
	InstanceShader
};

class ShaderManager {
	std::map<std::string, Shader> shaders;
public:
	void init(ShaderType type , std::string shadername ,std::string VS_filename, std::string PS_filename, DXCOre* core) {
		if (type == ShaderType::StaticShader)
			shaders[shadername].init(VS_filename, PS_filename, core);
		else if (type == ShaderType::AnimatedShader)
			shaders[shadername].init_anim(VS_filename, PS_filename, core);
		else if (type == ShaderType::InstanceShader)
			shaders[shadername].init_inst(VS_filename, PS_filename, core);
	}

	void updateConstantVS(std::string shadername, std::string constantBufferName, std::string variableName, void* data)
	{
		shaders[shadername].updateConstantVS(constantBufferName, variableName, data);
	}
	void updateConstantPS(std::string shadername, std::string constantBufferName, std::string variableName, void* data)
	{
		shaders[shadername].updateConstantPS(constantBufferName, variableName, data);
	}
	void updateTexturePS(std::string shadername,DXCOre* core, std::string name, ID3D11ShaderResourceView* tex) {
		shaders[shadername].updateTexturePS(core, name, tex);
	}
	void updateSampler(std::string shadername, DXCOre* core, ID3D11SamplerState* state, int num) {
		shaders[shadername].updateSampler(core, state, num);
	}
	void apply(std::string shadername, DXCOre* core) {
		shaders[shadername].apply(core);
	}
};