#pragma once

namespace Angazi::Graphics 
{

	class VertexShader
	{
	public:
		void Initialize(const std::filesystem::path& filePath, uint32_t vertextFormat);
		void Terminate();

		void Bind();
	
	private:
		ID3D11VertexShader *mVertexShader = nullptr;
		ID3D11InputLayout *mInputLayout = nullptr;
	};


} //namespace Angazi::Graphics