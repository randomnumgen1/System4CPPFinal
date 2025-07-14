#pragma once
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif


namespace System::IO{
	class Directory{
		static void CreateDirectory(std::string path){
#ifdef _WIN32
			::CreateDirectory(path.c_str(), NULL);
#else
			mkdir(path.c_str(), 0777);
#endif
		}
		static void CreateSymbolicLink(std::string path, std::string pathToTarget){
#ifdef _WIN32
			::CreateSymbolicLink(path.c_str(), pathToTarget.c_str(), 0);
#else
			symlink(pathToTarget.c_str(), path.c_str());
#endif
		}
		static void Delete(std::string path){
#ifdef _WIN32
			::RemoveDirectory(path.c_str());
#else
			rmdir(path.c_str());
#endif
		}
		static bool Exists(std::string path){
#ifdef _WIN32
			DWORD ftyp = GetFileAttributesA(path.c_str());
			if (ftyp == INVALID_FILE_ATTRIBUTES)
				return false;
			if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
				return true;
			return false;
#else
			struct stat st;
			if (stat(path.c_str(), &st) == 0)
				return S_ISDIR(st.st_mode);
			return false;
#endif
		}
		static void Move(std::string sourceDirName, std::string destDirName){
			rename(sourceDirName.c_str(), destDirName.c_str());
		}
		
		
		
		
		
		
		
	};
}