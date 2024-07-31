#pragma once

#include "External/nlohmann/json.hpp"

#include "Core/Base.h"

#include <fstream>

namespace TooGoodEngine {

	using json = nlohmann::json;

	// ------- Reader ---------
	class JsonReader
	{
	public:
		JsonReader(const std::filesystem::path& path);
		~JsonReader();

		inline const json& GetData() const { return m_Data; }
		
		void PrintData(bool pretty = true);

	private:
		std::ifstream m_Stream;
		json m_Data;
	};

	using JsonPath = std::vector<std::string>;

	// ------- Writer ---------
	class JsonWriter
	{
	public:
		JsonWriter(const std::filesystem::path& path, bool optimize = false);
		JsonWriter(const std::filesystem::path& path, const JsonReader& reader, bool optimize = false);
		~JsonWriter();

		template<typename T>
		void WriteGeneric(const JsonPath& path, const T& data);

		template<typename T>
		void Write(const JsonPath& path, const T& data);

	private:
		std::ofstream m_Stream;
		json m_Data;
		bool m_Optimize;

	};

	struct Impl_Write
	{
		template<typename T>
		static void Call(const JsonPath& path, const T& data)
		{
			TGE_LOG_ERROR("data type ", typeid(T).name(), " not supported");
		}
	};

	template<>
	static void Impl_Write::Call(const JsonPath& path, const int& data)
	{

	}


	template<typename T>
	inline void JsonWriter::WriteGeneric(const JsonPath& path, const T& data) 
	{
		TGE_VERIFY(path.size() > 0, "path must be bigger than 0");

		json* current = &m_Data;
		size_t last = path.size() - 1;

		for (size_t i = 0; i < last; ++i) 
		{
			auto& ref = *current;
			const std::string& key = path[i];
			if (!current->contains(key)) 
				ref[key] = json::object();
			
			current = &ref[key];
		}

		(*current)[path[last]] = data;
	}

	template<typename T>
	inline void JsonWriter::Write(const JsonPath& path, const T& data)
	{
		TGE_VERIFY(path.size() > 0, "path must be bigger than 0");
		Impl_Write::Call(path, data);
	}
}

