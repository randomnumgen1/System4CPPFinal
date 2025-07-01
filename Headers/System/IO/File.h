#ifndef _SYSTEM_IO_FILE_H
#define _SYSTEM_IO_FILE_H


#include <string>
#include <fileapi.h>
#include <handleapi.h>
#include <stdexcept>
#include <vector>


#include <ostream>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)

#include <errhandlingapi.h>
#include <winerror.h>
#include <WinBase.h>

#else
	
#include <sys/stat.h>

#endif
namespace System::IO{
	class File{
		public:
			static bool Create(std::string path) {
#if defined(_WIN32) || defined(_WIN64)
				HANDLE hFile;
				hFile = CreateFileA(path.c_str(),                // name of the write
								   GENERIC_WRITE,          // open for writing
								   0,                      // do not share
								   NULL,                   // default security
								   CREATE_NEW,             // create new file only
								   FILE_ATTRIBUTE_NORMAL,  // normal file
								   NULL);                  // no attr. template
				if (hFile == INVALID_HANDLE_VALUE) {
					throw std::runtime_error("File::Create failed");
				}
#else

#endif
			}
		static bool Exists(std::string path){
#if defined(_WIN32) || defined(_WIN64)
			DWORD       fileAttr;
			fileAttr = GetFileAttributesA(path.c_str());
			if (0xFFFFFFFF == fileAttr && GetLastError() == ERROR_FILE_NOT_FOUND)
				return false;
			return true;
#else
			struct stat st;
			if (stat("myfile", &st) == 0) {
				std::cout << "myfile exists\n";
			}
			else {
				std::cout << "Can't stat myfile: errno=" << errno << ": "
					<< strerror(errno) << '\n';
			}
#endif
		}
		static void Delete(std::string path){
			if (DeleteFileA(path.c_str()) == 0){
				DWORD error = GetLastError();
				std::cerr << "Error deleting file: " << error << std::endl;
			}else{
				std::cout << "File deleted successfully." << std::endl;
			}
		}
		static void Move(std::string sourceFileName, std::string destFileName){
			if (MoveFileA(sourceFileName.c_str(), destFileName.c_str()) == 0) {
				DWORD error = GetLastError();
				std::cerr << "Error moving file: " << error << std::endl;
			}
		}
		static void WriteAllBytes(std::string path, std::vector<uint8_t> bytes){
			std::ofstream file(path, std::ios::binary);
			if (!file) {
				std::cerr << "Error opening file: " << path << std::endl;
				return;
			}
			file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
			if (!file) {
				std::cerr << "Error writing to file: " << path << std::endl;
			}
			else {
				std::cout << "Successfully wrote to file: " << path << std::endl;
			}
			file.close();
		}
		static void WriteAllText(std::string path, std::string text){
			std::ofstream file(path);
			if (!file) {
				std::cerr << "Error opening file: " << path << std::endl;
				return;
			}
			file << text;
			if (!file) {
				std::cerr << "Error writing to file: " << path << std::endl;
			}
			else {
				std::cout << "Successfully wrote to file: " << path << std::endl;
			}
			file.close();
		}
		static void CreateSymboliclink(std::string path, std::string pathToTarget){
			if (CreateSymbolicLinkA(path.c_str(), pathToTarget.c_str(), 0x00000000) == 0) {
				// Handle error
				DWORD error = GetLastError();
				std::cerr << "Error creating symbolic link: " << error << std::endl;
			}
			else {
				std::cout << "Symbolic link created successfully." << std::endl;
			}
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	};
}
#endif