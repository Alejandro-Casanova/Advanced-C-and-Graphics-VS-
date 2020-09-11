#include "pch.h"

#include "IOManager.h"
#include <experimental/filesystem>
#include <fstream>

// Namespace alias
//namespace fs = std::tr2::sys;
namespace fs = std::experimental::filesystem;

namespace Bengine{

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer){
    std::ifstream file(filePath, std::ios::binary);
    if(file.fail()){
        perror(filePath.c_str());
        return false;
    }

    //Seek to the end of the file
    file.seekg(0, std::ios::end);

    //Get the file size
    int fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    //Disregard size of file header, posible data befor file content
    fileSize -= file.tellg();

    buffer.resize(fileSize);
    file.read((char*)buffer.data(), fileSize);
    file.close();

    return true;
}

bool IOManager::readFileToBuffer(std::string filePath, std::string& buffer) {
    std::ifstream file(filePath, std::ios::binary);
    if (file.fail()) {
        perror(filePath.c_str());
        return false;
    }

    //Seek to the end of the file
    file.seekg(0, std::ios::end);

    //Get the file size
    int fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    //Disregard size of file header, posible data befor file content
    fileSize -= file.tellg();

    buffer.resize(fileSize);
    file.read((char*)buffer.data(), fileSize);
    file.close();

    return true;
}

bool IOManager::getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries) {
    //auto dpath = fs::path(path);
    fs::path dpath = path;
    // Must be directory
    if (!fs::is_directory(dpath)) return false;

    for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it) {
        rvEntries.emplace_back();
        rvEntries.back().path = it->path().string();
        if (is_directory(it->path())) {
            rvEntries.back().isDirectory = true;
        }
        else {
            rvEntries.back().isDirectory = false;
        }
    }
    return true;
}

bool IOManager::makeDirectory(const char* path) {
    return fs::create_directory(fs::path(path));
}


}
