#pragma once

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>

class DataParser {
public:
	DataParser(DataParser& other) = delete;
	void operator=(const DataParser&) = delete;
	static DataParser* Instance();

	nlohmann::json DataFromFile(std::string pathToFile);
	void WriteToFile(std::string path, nlohmann::json data);
	~DataParser();

private:
	static DataParser* mParser;

protected:
	DataParser() {};
	nlohmann::json mData;
};