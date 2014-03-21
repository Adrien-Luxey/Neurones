#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

/**
 * 
 * @param fileName
 * 
 * Utility singleton class parsing a config file (default 'files/config.cfg')
 * that contains constants used by a project, organized by type (int, float, string),
 * referenced by their name and stored in maps.
 * 
 * I wrote this class a long time ago, to allow me to quickly change any constant int/float/string without recompiling every time.
 * Using maps, it can be quite long to process when called a lot. This is why I save all constants in private const in every class I use them.
 * I won't discuss its inner working, as it is oooooold.
 */
class ConfigParser {
	public :
		static void create(const std::string &fileName = "");
		static void kill();

		static ConfigParser* get();

		int readInt(const std::string &key, const int &defaultValue = 0) const;
		float readFloat(const std::string &key, const float &defaultValue = 0.f) const;
		std::string readString(const std::string &key, const std::string &defaultValue = "") const;

		void setInt(const std::string &key, const int &value);
		void setFloat(const std::string &key, const float &value);
		void setString(const std::string &key, const std::string &value);

		void readFile(const std::string &fileName);
		void saveFile(const std::string &fileName = "");

	private :
		ConfigParser(const std::string &fileName);

		void initMaps();
		
		static ConfigParser *singleton;

		enum typeEnum { INT, FLOAT, STRING, NONE };
		const std::string typeString[3];
		const std::string commentString;
		const std::string extensionString;
		const char assignChar;
		const std::string defaultFile;

		std::string mFileName;

		std::map<std::string, int> mIntMap;
		std::map<std::string, float> mFloatMap;
		std::map<std::string, std::string> mStringMap;
};

#endif // CONFIG_PARSER_H
