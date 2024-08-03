#include "Program.h"

#include "Core/Files/FileReader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace TooGoodEngine {
	namespace OpenGL {

		Program::Program(const ShaderMap& map)
		{
			m_ProgramHandle = glCreateProgram();

			for (const auto& [type, fileLocation] : map)
			{
				std::string shaderData = LoadShaderFromFile(fileLocation);
				const char* cShaderData = shaderData.c_str();

				uint32_t shader = glCreateShader(GetShaderType(type));
				glShaderSource(shader, 1, &cShaderData, nullptr);
				glCompileShader(shader);
				glAttachShader(m_ProgramHandle, shader);

				GLint success;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success) 
				{
					char infoLog[512];
					glGetShaderInfoLog(shader, 512, nullptr, infoLog);
					TGE_LOG_ERROR(infoLog);
				}

				glDeleteShader(shader);
			}

			glLinkProgram(m_ProgramHandle);

			GLint success;
			glGetProgramiv(m_ProgramHandle, GL_LINK_STATUS, &success);

			if (!success) 
			{
				char infoLog[512];
				glGetProgramInfoLog(m_ProgramHandle, 512, nullptr, infoLog);
				TGE_LOG_ERROR(infoLog);
			}
		}

		Program::~Program()
		{
			if (m_ProgramHandle)
				glDeleteProgram(m_ProgramHandle);

			m_ProgramHandle = 0;
		}
		Program::Program(const Program& other)
			: m_ProgramHandle(other.m_ProgramHandle)
		{
		}
		Program::Program(Program&& other) noexcept
			: m_ProgramHandle(other.m_ProgramHandle)
		{
			other.m_ProgramHandle = 0;
		}
		Program& Program::operator=(const Program& other)
		{
			if (this != &other)
				m_ProgramHandle = other.m_ProgramHandle;
			
			return *this;
		}
		Program& Program::operator=(Program&& other) noexcept
		{
			if (this != &other)
			{
				m_ProgramHandle = other.m_ProgramHandle;
				other.m_ProgramHandle = 0;
			}

			return *this;
		}

		template<>
		void Program::SetUniform(const std::string& name, const int& data)
		{
			GLuint location = glGetUniformLocation(m_ProgramHandle, name.c_str());

			if (location != -1)
				glUniform1i(location, (GLuint)data);
			else
				TGE_LOG_ERROR("not a valid uniform name");
		}

		template<>
		void Program::SetUniform(const std::string& name, const glm::vec3& data)
		{
			GLuint location = glGetUniformLocation(m_ProgramHandle, name.c_str());

			if (location != -1)
				glUniform3f(location, (GLfloat)data[0], (GLfloat)data[1], (GLfloat)data[2]);
			else
				TGE_LOG_ERROR("not a valid uniform name");
		}

		template<>
		void Program::SetUniform(const std::string& name, const glm::vec4& data)
		{
			GLuint location = glGetUniformLocation(m_ProgramHandle, name.c_str());

			if (location != -1)
				glUniform4f(location, (GLfloat)data[0], (GLfloat)data[1], (GLfloat)data[2], (GLfloat)data[3]);
			else
				TGE_LOG_ERROR("not a valid uniform name");
		}

		template<>
		void Program::SetUniform(const std::string& name, const glm::mat4& data)
		{
			GLuint location = glGetUniformLocation(m_ProgramHandle, name.c_str());

			if (location != -1)
				glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
			else
				TGE_LOG_ERROR("not a valid uniform name");
		}


		std::string Program::LoadShaderFromFile(const std::filesystem::path& path)
		{
			std::string contents;
			
			FileReader reader(path, false);
			contents.resize(reader.GetSize());

			Ref<MemoryAllocator> memory = reader.Read(reader.GetSize());

			memcpy(contents.data(), memory->GetRaw(), reader.GetSize());

			return contents;
		}
		constexpr GLenum Program::GetShaderType(ShaderType type)
		{
			switch (type)
			{
				case ShaderType::VertexShader:	 return GL_VERTEX_SHADER;
				case ShaderType::FragmentShader: return GL_FRAGMENT_SHADER;
				case ShaderType::ComputeShader:	 return GL_COMPUTE_SHADER;
				default:						 return GL_NONE;
			}
		}
	}
}