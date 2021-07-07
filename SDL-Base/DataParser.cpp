#include "DataParser.h"

DataParser* DataParser::mParser = nullptr;

DataParser* DataParser::Instance() {
	if (!mParser) {
		mParser = new DataParser();
	}
	return mParser;
};

DataParser::~DataParser() {
	mParser = nullptr;
}

nlohmann::json DataParser::DataFromFile(std::string pathToFile) {
	std::ifstream i(pathToFile);
	std::cout << pathToFile << std::endl;
	nlohmann::json data;
	try {
		data = nlohmann::json::parse(i);
		return data;
	} catch (nlohmann::json::parse_error) {
		std::cout << "Error parsing JSON file!\n";
	}
	return data;
};

void DataParser::WriteToFile(std::string path, nlohmann::json data) {

};