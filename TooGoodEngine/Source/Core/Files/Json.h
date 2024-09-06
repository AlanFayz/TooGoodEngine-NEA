#pragma once

#include "External/nlohmann/json.hpp"

#include "Core/Base.h"

#include <fstream>

namespace TooGoodEngine {

	using json = nlohmann::ordered_json;

	using JsonPath = std::vector<std::string>;

	// ------- Reader ---------
	class JsonReader
	{
	public:
		JsonReader(const std::filesystem::path& path);
		~JsonReader();

		inline const json& GetData() const { return m_Data; }

		void PrintData(bool pretty = true);

		template<typename T> 
		T Fetch(const JsonPath& path);

	private:
		std::ifstream m_Stream;
		json m_Data;
	};

	template<typename T>
	inline T JsonReader::Fetch(const JsonPath& path)
	{
		TGE_VERIFY(path.size() > 0, "path must be bigger than 0");

		json* current = &m_Data;
		size_t last = path.size() - 1;

		//essentially treating it like a linked list
		for (size_t i = 0; i < last; ++i)
		{
			auto& ref = *current;
			const std::string& key = path[i];

			if (!current->contains(key))
			{
				TGE_LOG_ERROR("path is invalid");
				return T();
			}

			current = &ref[key];
		}
		
		json& ref = *current;
		return ref[path[last]].get<T>();
	}

	// ------- Writer ---------
	class JsonWriter
	{
	public:
		JsonWriter(const std::filesystem::path& path, bool optimize = false);
		JsonWriter(const std::filesystem::path& path, const JsonReader& reader, bool optimize = false);
		~JsonWriter();

		template<typename T>
		void WriteGeneric(const JsonPath& path, const T& data);

	private:
		std::ofstream m_Stream;
		json m_Data;
		bool m_Optimize;

	};

	template<typename T>
	inline void JsonWriter::WriteGeneric(const JsonPath& path, const T& data) 
	{
		TGE_VERIFY(path.size() > 0, "path must be bigger than 0");

		json* current = &m_Data;
		size_t last = path.size() - 1;

		//essentially treating it like a linked list
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

}

