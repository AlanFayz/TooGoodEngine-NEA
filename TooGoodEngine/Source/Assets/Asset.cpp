#include "Asset.h"

namespace TooGoodEngine {
	//each asset will implement its own specilization
	template<>
	static Ref<Asset> Asset::LoadAssetFromFile(const std::filesystem::path& path, bool isBinary)
	{
		return nullptr;
	}

	//
	// ---- example asset use ----
	//

	/*
	class TestAsset : public Asset
	{
	public:
		TestAsset() = default;
		TestAsset(int data) : m_Data(data) {}
		~TestAsset() = default;

		inline int GetData() const { return m_Data; }

		virtual const AssetType GetAssetType() { return AssetType::None; }

	private:
		int m_Data = 5;
	};

	template<>
	static Ref<TestAsset> Asset::LoadAssetFromFile(const std::filesystem::path& path, bool isBinary)
	{
		FileReader reader(path, isBinary);

		Ref<MemoryAllocator> allocator = reader.Read(reader.GetSize());
		char buf[3]{};
		memcpy(buf, allocator->GetRaw(), allocator->GetCapacity());
		buf[2] = '\0';

		Ref<TestAsset> asset = CreateRef<TestAsset>(std::stoi(buf));

		return asset;
	}*/
}