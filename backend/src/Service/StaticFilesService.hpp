#pragma once

#include <oatpp/core/concurrency/SpinLock.hpp>
#include <oatpp/core/Types.hpp>

#include <fstream>
#include <cstring>

#include <unordered_map>
#include <string>

class StaticFilesService {
private:
  oatpp::String m_basePath;
  oatpp::concurrency::SpinLock m_lock;
  std::unordered_map<oatpp::String, oatpp::String> m_cache;

  oatpp::String getExtension(const oatpp::String& filename) {
    v_int32 dotPos = 0;
    for (v_int32 i = filename->getSize() - 1; i > 0; i--) {
      if (filename->getData()[i] == '.') {
        dotPos = i;
        break;
      }
    }
    if (dotPos != 0 && dotPos < filename->getSize() - 1) {
      return oatpp::String((const char*)&filename->getData()[dotPos + 1], filename->getSize() - dotPos - 1);
    }
    return nullptr;
  }

  oatpp::String loadFromFile(const char* fileName) {
    std::ifstream file{fileName, std::ios::in|std::ios::binary|std::ios::ate};
    
    if (file.is_open()) {
      auto result = oatpp::String((v_int32) file.tellg());
      file.seekg(0, std::ios::beg);
      file.read((char*)result->getData(), result->getSize());
      file.close();
      return result;
      
    }
    
    return nullptr;
  }

public:
  
  StaticFilesService(const oatpp::String& basePath)
    : m_basePath(basePath)
  {}

  oatpp::String getFile(const oatpp::String& path) {
    if (!path) {
      return nullptr;
    }
    std::lock_guard<oatpp::concurrency::SpinLock> lock(m_lock);
    auto& file = m_cache[path];
    if (file) {
      return file;
    }
    oatpp::String filename = m_basePath + "/" + path;
    file = loadFromFile(filename->c_str());
    return file;
  }

  oatpp::String guessMimeType(const oatpp::String& filename) {
    auto extension = getExtension(filename);
    if (extension == "png")
      return "image/png";
    else if (extension == "jpg")
      return "image/jpeg";
    else
      return "application/octet-stream";
  }
};