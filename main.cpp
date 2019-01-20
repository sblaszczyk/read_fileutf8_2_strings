#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>
#include <vector>
#include <string>

/**
 * g++ -o read_utf8_file -std=c++11 main.cpp
 * Use: ./read_utf8_file utf8_file.txt 100
 */

typedef std::wstring_convert<std::codecvt_utf8<wchar_t>> ConvWString2String;

void readFile(std::wifstream& wif, ConvWString2String& convWs2S, int par_size, std::vector<std::string>& dbusPayloads) {
    std::wstring wsBuffer(L"\0", par_size);

    while (wif) {
        wif.read(const_cast<wchar_t*>(wsBuffer.data()), par_size);
        if (wif.gcount() < par_size) {
            wsBuffer.resize(wif.gcount());
            wsBuffer.shrink_to_fit();
        }
        if (wsBuffer.size() > 0) {
            std::string payload = convWs2S.to_bytes(wsBuffer);
            dbusPayloads.push_back(payload);
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Use: ./read_utf8_file UTF8_FILE COUNT_BYTES_TO_SPLIT" << std::endl;
        return 0;
    }
    std::string filename = argv[1];
    const int par_size = std::atoi(argv[2]);
    if (par_size == 0) {
        std::cerr << "Error: Get COUNT_BYTES_TO_SPLIT great then 0!" << std::endl;
        return 1;
    }

    ConvWString2String convWs2S;
    std::locale l1(std::locale("pl_PL.UTF-8"), new std::codecvt_utf8<wchar_t>);
    std::wifstream wif(filename);
    wif.imbue(l1);

    std::vector<std::string> dbusPayloads;
    readFile(wif, convWs2S, par_size, dbusPayloads);

    for (auto& element : dbusPayloads) {
        std::cout << "element = " << element << "; size = " << element.size() << std::endl;
    }

    return 0;
}
