#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

class ConfigParser {
	public :
		static void create(const std::string &fileName = defaultFile);
		static void kill();

		static ConfigParser* get();

		const int readInt(const std::string &key, const int &defaultValue = 0);
		const float readFloat(const std::string &key, const float &defaultValue = 0.f);
		const std::string readString(const std::string &key, const std::string &defaultValue = "");

		void setInt(const std::string &key, const int &value);
		void setFloat(const std::string &key, const float &value);
		void setString(const std::string &key, const std::string &value);

		void readFile(const std::string &fileName);
		void saveFile(const std::string &fileName = "");

	private :
		ConfigParser(const std::string &fileName);

		static ConfigParser *singleton;

		enum typeEnum { INT, FLOAT, STRING, NONE };
		static std::string typeString[];
		static std::string commentString;
		static std::string extensionString;
		static char assignChar;
		static std::string defaultFile;
		void initMaps();

		std::string mFileName;

		std::map<std::string, int> mIntMap;
		std::map<std::string, float> mFloatMap;
		std::map<std::string, std::string> mStringMap;
};

#endif // CONFIG_PARSER_H
