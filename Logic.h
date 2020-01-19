#pragma once
#include <cstdint>
#include <string>

uint32_t countWords(const std::string& file, const std::string& word);
uint32_t getChecksum(const std::string& file);
