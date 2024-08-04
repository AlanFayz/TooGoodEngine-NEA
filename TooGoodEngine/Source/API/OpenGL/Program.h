#pragma once

#include "Core/Base.h"
#include "External/glad/glad.h"

#include <map>
#include <filesystem>

namespace TooGoodEngine {
	namespace OpenGL {
		
		enum class ShaderType 
		{
			VertexShader, FragmentShader, ComputeShader
		};

		using ShaderMap = std::map<ShaderType, std::filesystem::path>;

		class Program
		{
		public:
			Program(const ShaderMap& map);
			Program() = default;
			~Program();

			Program(const Program& other) = delete;
			Program& operator=(const Program& other) = delete;

			Program(Program&& other) noexcept;
			Program& operator=(Program&& other) noexcept;

			inline const uint32_t GetHandle() const { return m_ProgramHandle; }

			template<typename T>
			void SetUniform(const std::string& name, const T& data)
			{
				TGE_LOG_ERROR(typeid(T).name(), " is not supported");
			}

		private:
			std::string LoadShaderFromFile(const std::filesystem::path& path);

			constexpr GLenum GetShaderType(ShaderType type);

		private:
			uint32_t m_ProgramHandle = 0;
		};

	}
}