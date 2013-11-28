#include "configParser.h"

using namespace std;

#define DEBUG	0

string ConfigParser::typeString[] = { "[int]", "[float]", "[string]" };
string ConfigParser::commentString = "#";
string ConfigParser::extensionString = ".cfg";
char ConfigParser::assignChar = '=';
string ConfigParser::defaultFile = "files/config.cfg";

ConfigParser* ConfigParser::singleton = NULL;
void ConfigParser::create(const string &fileName) {
	if (singleton != NULL)
		kill();

	singleton = new ConfigParser(fileName);
}
void ConfigParser::kill() { delete singleton; }
ConfigParser* ConfigParser::get() {
	if (singleton == NULL)
		create();

	return singleton;
}

ConfigParser::ConfigParser(const std::string &fileName) {
	if (fileName.compare(fileName.size()-extensionString.size(), extensionString.size(), extensionString) == 0) {
		mFileName = fileName;
	} else {
		mFileName = fileName + extensionString;
	}

	initMaps();
}

const int ConfigParser::readInt(const string &key, const int &defaultValue) {
	if (!mIntMap.count(key))
		return defaultValue;
	else
		return mIntMap.find(key)->second;
}

const float ConfigParser::readFloat(const string &key, const float &defaultValue) {
	if (!mFloatMap.count(key))
		return defaultValue;
	else
		return mFloatMap.find(key)->second;
}

const string ConfigParser::readString(const string &key, const string &defaultValue) {
	if (!mStringMap.count(key)) {
		return defaultValue;
	} else {
		return mStringMap.find(key)->second;
	}
}

void ConfigParser::setInt(const string &key, const int &value) {
	if (mIntMap.count(key)) {
		mIntMap.find(key)->second = value;
		return;
	}

	mIntMap.insert(pair<string, int>(key, value));
}

void ConfigParser::setFloat(const string &key, const float &value) {
	if (mFloatMap.count(key)) {
		mFloatMap.find(key)->second = value;
		return;
	}

	mFloatMap.insert(pair<string, float>(key, value));
}

void ConfigParser::setString(const string &key, const string &value) {
	if (mStringMap.count(key)) {
		mStringMap.find(key)->second = value;
		return;
	}

	mStringMap.insert(pair<string, string>(key, value));
}

void ConfigParser::readFile(const std::string &fileName) {
	mFileName = fileName;
	initMaps();
}

void ConfigParser::initMaps() {
	ifstream file(mFileName.c_str());

	if (!file.is_open()) {
		cerr << "Error opening " << mFileName << endl;
		mFileName = defaultFile;
		file.open(mFileName.c_str());
		if (file.is_open())
			cerr << "Opening " << mFileName << " instead" << endl;
		else {
			cerr << "Could not even open " << mFileName << " instead : aborting initMaps()" << endl;
			file.close();
			return;
		}
	}
	mIntMap.clear();
	mFloatMap.clear();
	mStringMap.clear();

	string line;
	typeEnum type = NONE;
	size_t assignPos;
	string key;
	while (getline(file, line)) {
		if (line.empty() || line.compare(0, commentString.size(), commentString) == 0) {
			continue;
		}

		if (line.compare(0, typeString[INT].size(), typeString[INT]) == 0) {
			type = INT;
		}
		else if (line.compare(0, typeString[FLOAT].size(), typeString[FLOAT]) == 0) {
			type = FLOAT;
		}
		else if (line.compare(0, typeString[STRING].size(), typeString[STRING]) == 0) {
			type = STRING;
		}
		else {
			assignPos = line.find(assignChar);
			key = line.substr(0, assignPos);
			istringstream valueIss(line.substr(assignPos+1));

			switch (type) {
				case INT : {
					int value;
					if ((valueIss >> value).fail())
						value = 0;
					if (mIntMap.count(key)) {
						cerr << key << " already exists in mIntMap : using " << mIntMap.find(key)->second << " instead of " << value << endl;
						mIntMap.find(key)->second = value;
					} else {
						mIntMap.insert(pair<string, int>(key, value));
					}
					if (DEBUG)
						cout << mIntMap.find(key)->first << " : " << mIntMap.find(key)->second << endl;
					break;
				}
				case FLOAT : {
					float value;
					if ((valueIss >> value).fail()) {
						value = 0.f;
					if (mFloatMap.count(key))
						cerr << key << " already exists in mFloatMap : using " << mFloatMap.find(key)->second << " instead of " << value << endl;
						mFloatMap.find(key)->second = value;
					} else {
						mFloatMap.insert(pair<string, float>(key, value));
					}
					if (DEBUG)
						cout << mFloatMap.find(key)->first << " : " << mFloatMap.find(key)->second << endl;
					break;
				}
				case STRING : {
					string value = valueIss.str();
					if (mStringMap.count(key)) {
						cerr << key << " already exists in mStringMap : using " << mStringMap.find(key)->second << " instead of " << value << endl;
						mStringMap.find(key)->second = value;
					} else {
						mStringMap.insert(pair<string, string>(key, value));
					}
					if (DEBUG)
						cout << mStringMap.find(key)->first << " : " << mStringMap.find(key)->second << endl;
					break;
				}
				default :
					break;
			}
		}
	}

	file.close();
}

void ConfigParser::saveFile(const std::string &fileName) {
	if (fileName != "") {
		if (fileName.compare(fileName.size()-extensionString.size(), extensionString.size(), extensionString) == 0) {
			mFileName = fileName;
		} else {
			mFileName = fileName + extensionString;
		}
	}

	ofstream file(mFileName.c_str(), ios::out|ios::trunc);

	file << '\n' << typeString[INT] << '\n';
	for (map<string, int>::iterator it = mIntMap.begin(); it != mIntMap.end(); ++it) {
		file << it->first << assignChar << it->second << '\n';
	}
	file << '\n' << typeString[FLOAT] << '\n';
	for (map<string, float>::iterator it = mFloatMap.begin(); it != mFloatMap.end(); ++it) {
		file << it->first << assignChar << it->second << '\n';
	}
	file << '\n' << typeString[STRING] << '\n';
	for (map<string, string>::iterator it = mStringMap.begin(); it != mStringMap.end(); ++it) {
		file << it->first << assignChar << it->second << '\n';
	}

	file.close();
}
