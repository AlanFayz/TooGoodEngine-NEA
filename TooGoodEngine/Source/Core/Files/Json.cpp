#include "Json.h"

namespace TooGoodEngine {

	// ------- Reader ---------
	JsonReader::JsonReader(const std::filesystem::path& path)
		: m_Stream(path), m_Data()
	{
		if (!m_Stream.is_open())
		{
			TGE_LOG_ERROR("failed to open file path ", path);
			return;
		}

		try
		{
			m_Data = json::parse(m_Stream);
		}
		catch (const std::exception& e)
		{
			TGE_LOG_ERROR("failed to parse json file ", e.what());
			return;
		}

		m_Stream.close();
	}
	JsonReader::~JsonReader()
	{

	}
	void JsonReader::PrintData(bool pretty)
	{
		if (pretty)
			std::cout << std::setw(4) << m_Data;
		else
			std::cout << m_Data;
	}

	
	// ------- Writer ---------
	JsonWriter::JsonWriter(const std::filesystem::path& path, bool optimize)
		: m_Stream(path), m_Data(), m_Optimize(optimize)
	{
	}

	
	JsonWriter::JsonWriter(const std::filesystem::path& path, const JsonReader& reader, bool optimize)
		: m_Stream(path), m_Data(reader.GetData()), m_Optimize(optimize)
	{
		if (!m_Stream.is_open())
		{
			TGE_LOG_ERROR("not a valid path ", path);
			return;
		}
	}
	JsonWriter::~JsonWriter()
	{
		//write the data to disk
		if (m_Optimize)
			m_Stream << m_Data;
		else
			m_Stream << std::setw(4) << m_Data;

		//close the stream
		if (m_Stream.is_open())
			m_Stream.close();
	}

	template<>
	static void Impl_Write::Call(const JsonPath& path, const int& data)
	{

	}
}