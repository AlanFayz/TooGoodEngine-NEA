#pragma once

#include "Core/Base.h"
#include "External/glad/glad.h"

#include <glm/gtc/matrix_transform.hpp>

#include <map>
#include <filesystem>

namespace TooGoodEngine {
	namespace OpenGL {
		
		enum class ShaderType 
		{
			VertexShader, FragmentShader, ComputeShader
		};

		using ShaderMap       = std::map<ShaderType, std::filesystem::path>;
		using ShaderSourceMap = std::map<ShaderType, const char*>;

		class Program
		{
		public:
			Program(const ShaderSourceMap& map);
			Program(const ShaderMap& map);
			Program() = default;
			~Program();

			Program(const Program& other) = delete;
			Program& operator=(const Program& other) = delete;

			Program(Program&& other) noexcept;
			Program& operator=(Program&& other) noexcept;

			inline const void Use() const { glUseProgram(m_ProgramHandle); }
			inline const uint32_t GetHandle() const { return m_ProgramHandle; }

			template<typename T>
			void SetUniform(const std::string& name, const T& data)
			{
				TGE_LOG_ERROR(typeid(T).name(), " is not supported");
			}

			template<>
			void SetUniform(const std::string& name, const int& data);
			template<>
			void SetUniform(const std::string& name, const float& data);
			template<>
			void SetUniform(const std::string& name, const glm::vec3& data);
			template<>
			void SetUniform(const std::string& name, const glm::vec4& data);
			template<>
			void SetUniform(const std::string& name, const glm::mat4& data);


		private:
			std::string LoadShaderFromFile(const std::filesystem::path& path);

			constexpr GLenum GetShaderType(ShaderType type);

		private:
			uint32_t m_ProgramHandle = 0;
		};

	}
}