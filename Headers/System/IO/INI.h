#ifndef SYSTEM_TOOLS_INI_H
#define SYSTEM_TOOLS_INI_H

/*
INI ini;
ini.Load("settings.ini");
int width = ini.GetValue<int>("Window", "Width", 800);
std::string title = ini.GetValue<std::string>("Window", "Title", "Untitled");
ini.SetValue("Window", "Fullscreen", "true");
ini.Save("settings.ini");
*/
namespace System::Tools{
	class INI{
		private:
			// map<section, map<key, value>>
			std::map<std::string, std::map<std::string, std::string> > config;
			
			std::string CapitalizeFirst(const std::string& str){
				if (str.empty()) return str;
				std::string result = str;
				result[0] = std::toupper(static_cast<unsigned char>(result[0]));
				return result;
			}
		public:
			INI(){
				
			}
			void Load(const std::string& filename){
				
				
			}
			void Save(const std::string& filename){
				
			}
			void SetValue(const std::string& section, const std::string& key, const std::string& value) {
				config[CapitalizeFirst(section)][CapitalizeFirst(key)] = value;
			}
			template<>
			bool GetValue<bool>(const std::string& section, const std::string& key, const bool& defaultValue) const {
				auto str = GetValue<std::string>(section, key, defaultValue ? "true" : "false");
				return str == "true" || str == "True" || str == "1";
			}
		
			template<typename T>
			T GetValue(const std::string& section, const std::string& key, const T& defaultValue) const {
				auto secIt = config.find(CapitalizeFirst(section));
				if (secIt != config.end()) {
					auto keyIt = secIt->second.find(CapitalizeFirst(key));
					if (keyIt != secIt->second.end()) {
						std::istringstream iss(keyIt->second);
						T value;
						if (iss >> value) {
							return value;
						}
					}
				}
				return defaultValue;
			}
		
	};
}
#endif