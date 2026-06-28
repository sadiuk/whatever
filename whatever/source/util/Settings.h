#pragma once
#include "IService.h"
#include <fstream>
#include <cassert>
#include <map>
#include <string>
#include <sstream>
#include "glm/glm.hpp"
namespace wtv
{
	class Settings : public IService
	{
		IMPL_ISERVICE(Settings);
		Settings(const std::string& configFilePath) : m_configFilePath(configFilePath)
		{
			std::ifstream ifs(configFilePath);
			ParseSettings(ifs);
		}

        void SaveSettings()
        {
			std::ofstream ofs(m_configFilePath);
			assert(ofs.is_open());
			WriteSettings(ofs);
        }
        template<typename T>
        T GetValue(const std::string& key, const T& defaultValue)
        {
            auto it = m_settings.find(key);
            if (it == m_settings.end())
                return defaultValue;

            T result;
            std::istringstream iss(it->second);
            if (!(iss >> result))
                return defaultValue;

            return result;
        }

        template<>
        std::string GetValue(const std::string& key, const std::string& defaultValue)
        {
            auto it = m_settings.find(key);
            if (it == m_settings.end())
                return defaultValue;

            return it->second;
        }

        template<>
        glm::vec2 GetValue(const std::string& key, const glm::vec2& defaultValue)
        {
            auto it = m_settings.find(key);
            if (it == m_settings.end())
                return defaultValue;

            glm::vec2 result;
            char ignored;
            std::istringstream iss(it->second);
            if (!(iss >> ignored >> result.x >> result.y >> ignored))
                return defaultValue;

            return result;
        }

        template<>
        glm::vec3 GetValue(const std::string& key, const glm::vec3& defaultValue)
        {
            auto it = m_settings.find(key);
            if (it == m_settings.end())
                return defaultValue;

            glm::vec3 result;
            char ignored;
            std::istringstream iss(it->second);
            if (!(iss >> ignored >> result.x >> result.y >> result.z >> ignored))
                return defaultValue;

            return result;
        }

        template<>
        glm::vec4 GetValue(const std::string& key, const glm::vec4& defaultValue)
        {
            auto it = m_settings.find(key);
            if (it == m_settings.end())
                return defaultValue;

            glm::vec4 result;
            char ignored;
            std::istringstream iss(it->second);
            if (!(iss >> ignored >> result.x >> result.y >> result.z >> result.w >> ignored))
                return defaultValue;

            return result;
        }

        template<typename T>
        void SetValue(const std::string& key, const T& value)
        {
            std::ostringstream oss;
            oss << value;
            m_settings[key] = oss.str();
        }

        template<>
        void SetValue(const std::string& key, const glm::vec2& value)
        {
            m_settings[key] = "{" + std::to_string(value.x) + " " + std::to_string(value.y) + "}";
        }

        template<>
        void SetValue(const std::string& key, const glm::vec3& value)
        {
            m_settings[key] = "{" + std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z) + "}";
        }

        template<>
        void SetValue(const std::string& key, const glm::vec4& value)
        {
            m_settings[key] = "{" + std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z) + " " + std::to_string(value.w) + "}";
        }

        template<>
        void SetValue(const std::string& key, const std::string& value)
        {
            m_settings[key] = value;
        }
	private:
		void ParseSettings(std::ifstream& ifs)
		{
            std::string line;
            while (std::getline(ifs, line))
            {
                // Trim leading whitespace
                size_t start = line.find_first_not_of(" \t\r\n");
                if (start == std::string::npos)
                    continue;
                line = line.substr(start);

                // Skip empty lines and section headers
                if (line.empty() || line.front() == '[')
                    continue;

                size_t delimPos = line.find('=');
                if (delimPos == std::string::npos)
                    continue;

                std::string key = line.substr(0, delimPos);
                std::string value = line.substr(delimPos + 1);

                // Trim trailing whitespace from key and leading from value
                auto trimRight = [](std::string& s) {
                    size_t end = s.find_last_not_of(" \t\r\n");
                    s = (end == std::string::npos) ? "" : s.substr(0, end + 1);
                    };
                auto trimLeft = [](std::string& s) {
                    size_t start = s.find_first_not_of(" \t\r\n");
                    s = (start == std::string::npos) ? "" : s.substr(start);
                    };

                trimRight(key);
                trimLeft(value);
                trimRight(value);

                // Strip surrounding quotes from value if present
                if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
                    value = value.substr(1, value.size() - 2);

                m_settings[key] = value;
            }
		}

        void WriteSettings(std::ofstream& ofs)
        {
            for (const auto& [key, value] : m_settings)
                ofs << key << " = " << value << "\n";
        }
	private:
		std::map<std::string, std::string> m_settings;
		std::string m_configFilePath;
	};
}