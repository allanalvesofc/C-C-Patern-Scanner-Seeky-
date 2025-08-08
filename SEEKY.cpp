#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <shellapi.h>  // For ShellExecuteA

// Function to find all occurrences of pattern in data buffer
std::vector<size_t> FindAllOccurrences(const unsigned char* data, size_t dataSize, const unsigned char* pattern, size_t patternSize)
{
    std::vector<size_t> results;

    if (patternSize == 0 || dataSize < patternSize)
        return results;

    for (size_t i = 0; i <= dataSize - patternSize; ++i)
    {
        bool match = true;
        for (size_t j = 0; j < patternSize; ++j)
        {
            if (data[i + j] != pattern[j])
            {
                match = false;
                break;
            }
        }
        if (match)
            results.push_back(i);
    }

    return results;
}

// Function to get the full line string at a given position in the mapped file data
std::string GetLineAtPosition(const unsigned char* data, size_t dataSize, size_t position)
{
    size_t start = position;
    while (start > 0 && data[start - 1] != '\n')
    {
        --start;
    }

    size_t end = position;
    while (end < dataSize && data[end] != '\n')
    {
        ++end;
    }

    size_t lineLength = end - start;
    if (lineLength > 0 && data[end - 1] == '\r')
    {
        --lineLength;
    }

    return std::string(reinterpret_cast<const char*>(data + start), lineLength);
}

#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <shellapi.h>

// Assuma que essas funções já estão implementadas corretamente
std::vector<size_t> FindAllOccurrences(const unsigned char* data, size_t dataSize, const unsigned char* term, size_t termSize);
std::string GetLineAtPosition(const unsigned char* data, size_t dataSize, size_t position);

int main(int argc, char* argv[])
{
	const size_t MAX_MAP_SIZE = 5ULL * 1024 * 1024 * 1024; // 5GB por mapeamento

	std::string fileName;
	std::string searchTerm;

	// Se argumentos foram passados, usa-os
	if (argc == 3) {
		fileName = argv[1];
		searchTerm = argv[2];
	}
	// Se nenhum argumento, pede entrada ao usuário
	else if (argc == 1) {
		std::cout << "Type file name: ";
		std::getline(std::cin, fileName);

		std::cout << "Search term: ";
		std::getline(std::cin, searchTerm);
	}
	// Número inválido de argumentos
	else {
		std::cerr << "Uso: " << argv[0] << " <fileName> <searchTerm>\n";
		std::cerr << "Ou execute sem argumentos para usar entrada manual.\n";
		return 1;
	}

	std::cout << "\n\nWaiting operation...\n\n";

	std::string fullPath = ".\\db\\" + fileName;

	// Conversão para wchar_t para APIs do Windows
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, fullPath.c_str(), (int)fullPath.size(), NULL, 0);
	std::wstring wFullPath(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, fullPath.c_str(), (int)fullPath.size(), &wFullPath[0], size_needed);

	HANDLE hFile = CreateFileW(wFullPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Error opening file." << std::endl;
		system("pause");
		return 1;
	}

	LARGE_INTEGER fileSize;
	if (!GetFileSizeEx(hFile, &fileSize))
	{
		std::cerr << "Error getting file size." << std::endl;
		CloseHandle(hFile);
		system("pause");
		return 1;
	}

	if (fileSize.QuadPart == 0)
	{
		std::cerr << "Empty file." << std::endl;
		CloseHandle(hFile);
		system("pause");
		return 1;
	}

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	size_t pageSize = sysInfo.dwAllocationGranularity;

	// Geração do nome do arquivo de saída com timestamp
	time_t now = time(NULL);
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	std::stringstream ss;
	ss << searchTerm << ".txt";
	std::string outputFileName = ss.str();

	std::ofstream outFile(outputFileName.c_str());
	if (!outFile.is_open())
	{
		std::cerr << "Error creating output file." << std::endl;
		CloseHandle(hFile);
		system("pause");
		return 1;
	}

	ULONGLONG offset = 0;
	size_t totalResults = 0;
	clock_t startClock = clock();

	while (offset < (ULONGLONG)fileSize.QuadPart)
	{
		ULONGLONG bytesLeft = fileSize.QuadPart - offset;
		size_t mapSize = (size_t)(bytesLeft > MAX_MAP_SIZE ? MAX_MAP_SIZE : bytesLeft);

		ULONGLONG alignedOffset = offset - (offset % pageSize);
		size_t offsetDiff = (size_t)(offset - alignedOffset);
		size_t adjustedMapSize = mapSize + offsetDiff;

		HANDLE hMap = CreateFileMappingW(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (hMap == NULL)
		{
			std::cerr << "Error creating file mapping." << std::endl;
			break;
		}

		const unsigned char* mapView = (const unsigned char*)MapViewOfFile(hMap, FILE_MAP_READ,
			(DWORD)(alignedOffset >> 32), (DWORD)(alignedOffset & 0xFFFFFFFF), adjustedMapSize);

		if (mapView == NULL)
		{
			std::cerr << "Error mapping view of file." << std::endl;
			CloseHandle(hMap);
			break;
		}

		const unsigned char* data = mapView + offsetDiff;

		std::vector<size_t> positions = FindAllOccurrences(data, mapSize,
			(const unsigned char*)searchTerm.c_str(), searchTerm.size());

		totalResults += positions.size();

		for (size_t i = 0; i < positions.size(); ++i)
		{
			std::string line = GetLineAtPosition(data, mapSize, positions[i]);
			outFile << line << std::endl;
		}

		UnmapViewOfFile(mapView);
		CloseHandle(hMap);

		offset += mapSize;
	}

	clock_t endClock = clock();
	double elapsedSecs = double(endClock - startClock) / CLOCKS_PER_SEC;

	outFile << "\nTotal results found: " << totalResults << std::endl;
	outFile.close();

	std::cout << "\nTotal results found: " << totalResults << std::endl;
	std::cout << "Elapsed time: " << elapsedSecs << " seconds\n";
	
	CloseHandle(hFile);
	ShellExecuteA(NULL, "open", outputFileName.c_str(), NULL, NULL, SW_SHOW);
	system("pause");
	return 0;
}
