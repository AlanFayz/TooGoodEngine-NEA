#include "Application.h"
#include "Math/Random.h"
#include "Files/FileReader.h"

#include <vector>
#include <algorithm>

namespace TooGoodEngine {

	void Application::Run()
	{
		FileReader Reader("test.txt");

		StrongRef<MemoryAllocator> memory = Reader.Read(Reader.GetSize());

		char buff[6]{};

		memcpy_s(buff, sizeof(char) * 5, memory->GetRaw(), memory->GetCapacity());

		buff[5] = '\0';
		std::cout << buff;
	}
}