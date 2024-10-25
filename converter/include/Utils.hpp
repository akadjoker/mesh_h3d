#pragma once

#include "Config.hpp"
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define MAX_TEXT_BUFFER_LENGTH              1024      




const char *strprbrk(const char *s, const char *charset);



void LogError( const char *msg, ... );
void LogInfo( const char *msg, ... );
void LogWarning( const char *msg, ... );


class   Logger 
{
    public:
  

        void Error( const char *msg, ... );
        void Warning( const char *msg, ... );
        void Info( const char *msg, ... );
     
         static Logger& Instance();


    
};

class    System 
{
    public:


        static System& Instance();



     void Sleep(unsigned int ms);

     void Abort();
     void Exit(int code);

 
    bool FileExists(const char *fileName);
    bool DirectoryExists(const char *dirPath);              
    bool IsFileExtension(const char *fileName, const char *ext); // Check file extension (including point: .png, .wav)
    int GetFileLength(const char *fileName);                    
     const char *TextFormat(const char *text, ...);
    const char *GetFileExtension(const char *fileName);         // Get pointer to extension for a filename string (includes dot: '.png')
    const char *GetFileName(const char *filePath);              // Get pointer to filename for a path string
    const char *GetFileNameWithoutExt(const char *filePath);    // Get filename string without extension (uses static string)
    const char *GetDirectoryPath(const char *filePath);         // Get full path for a given fileName with path (uses static string)
    const char *GetPrevDirectoryPath(const char *dirPath);      // Get previous directory path for a given path (uses static string)
    const char *GetWorkingDirectory(void);                      // Get current working directory (uses static string)
    const char *GetApplicationDirectory(void);                  // Get the directory if the running application (uses static string)


    bool ChangeDirectory(const char *dir);                      // Change working directory, return true on success
    bool IsPathFile(const char *path);                          // Check if a given path is a file or a directory


    u64 GetFileModTime(const char *fileName);       

 

private:

    static System* m_singleton;
 


};
