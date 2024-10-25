
#include "pch.h"


#if defined(PLATFORM_DESKTOP) && defined(_WIN32) && (defined(_MSC_VER) || defined(__TINYC__))

    #include "wdirent.h"    // Required for: DIR, opendir(), closedir() [Used in LoadDirectoryFiles()]
#else
    #include <dirent.h>             // Required for: DIR, opendir(), closedir() [Used in LoadDirectoryFiles()]
#endif

#if defined(_WIN32)
    #include <direct.h>             // Required for: _getch(), _chdir()
    #define GETCWD _getcwd          // NOTE: MSDN recommends not to use getcwd(), chdir()
    #define CHDIR _chdir
    #include <io.h>                 // Required for: _access() [Used in FileExists()]
#else
    #include <unistd.h>             // Required for: getch(), chdir() (POSIX), access()
    #define GETCWD getcwd
    #define CHDIR chdir
#endif
#include <stdarg.h>

#ifdef PLATFORM_WIN

#define CONSOLE_COLOR_RESET ""
#define CONSOLE_COLOR_GREEN ""
#define CONSOLE_COLOR_RED ""
#define CONSOLE_COLOR_PURPLE ""



#else

#define CONSOLE_COLOR_RESET "\033[0m"
#define CONSOLE_COLOR_GREEN "\033[1;32m"
#define CONSOLE_COLOR_RED "\033[1;31m"
#define CONSOLE_COLOR_PURPLE "\033[1;35m"




#endif 

static void LogMessage( int level, const char *msg, va_list args )
{


        char buffer[1024];
        vsnprintf( buffer, sizeof(buffer), msg, args );

    switch (level)
    {
        case 0:
            printf( "%sINFO:%s%s\n", CONSOLE_COLOR_GREEN, buffer,CONSOLE_COLOR_RESET);
            break;
        case 1:
            printf(  "%sWARNING:%s%s\n", CONSOLE_COLOR_RED, buffer,CONSOLE_COLOR_RESET);
            break;
        case 2:
            printf(  "%sERROR:%s%s\n", CONSOLE_COLOR_PURPLE, buffer,CONSOLE_COLOR_RESET);
            break;
 
    }

}


void LogError( const char *msg, ... )
{
	

	va_list args;
	va_start( args, msg );
	LogMessage( 1, msg, args );
	va_end( args );
}


void LogWarning( const char *msg, ... )
{
	
	va_list args;
	va_start( args, msg );
	LogMessage( 2, msg, args );
	va_end( args );
}


void LogInfo( const char *msg, ... )
{
	va_list args;
	va_start( args, msg );
	LogMessage( 0, msg, args );
	va_end( args );
}

//************************************************************************************************
// Logger Implementation
//************************************************************************************************


Logger& Logger::Instance()
{
   static  Logger m_singleton;

   return m_singleton;
}



void Logger::Error( const char *msg, ... )
{
	va_list args;
	va_start( args, msg );
	LogMessage( 1, msg, args );
	va_end( args );
}


void Logger::Warning( const char *msg, ... )
{
  
	va_list args;
	va_start( args, msg );
	LogMessage( 2, msg, args );
	va_end( args );
}


void Logger::Info( const char *msg, ... )
{
   va_list args;
	va_start( args, msg );
	LogMessage( 0, msg, args );
	va_end( args );
}









System& System::Instance()
{
   static  System m_singleton;

   return m_singleton;
}



void System::Exit(int code)
{
   
    exit(code);
}

bool System::FileExists(const char *fileName)
{
   bool result = false;

#if defined(_WIN32)
    if (_access(fileName, 0) != -1) result = true;
#else
    if (access(fileName, F_OK) != -1) result = true;
#endif
    return result;
}

bool System::DirectoryExists(const char *dirPath)
{
    bool result = false;

    DIR *dir = opendir(dirPath);
    if (dir != NULL)
    {
        result = true;
        closedir(dir);
    }

    return result;
}

bool System::IsFileExtension(const char *fileName, const char *ext)
{
    bool result = false;

    const char *fileExt = strrchr(fileName, '.');

    if (fileExt != NULL)
    {
        if (strcmp(fileExt, ext) == 0) result = true;
    }

    return result;
}

int System::GetFileLength(const char *fileName)
{
    int length = 0;

    FILE *file = fopen(fileName, "rb");
    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fclose(file);
    }


    return length;
}

const char *System::GetFileExtension(const char *fileName)
{
    const char *fileExt = strrchr(fileName, '.');

    if (fileExt != NULL) return fileExt;

    return NULL;
}

const char *System::GetFileName(const char *filePath)
{
    const char *fileName = strrchr(filePath, '/');

    if (fileName != NULL) return fileName + 1;

    return filePath;
}

const char *System::GetFileNameWithoutExt(const char *filePath)
{
    static char baseName[256];
    strcpy(baseName, GetFileName(filePath));

    char *dot = strrchr(baseName, '.');
    if (dot) *dot = '\0';

    return baseName;
}

const char *System::GetDirectoryPath(const char *filePath)
{
    static char dirPath[256];
    strcpy(dirPath, filePath);

    char *lastSlash = strrchr(dirPath, '/');

    if (lastSlash != NULL) lastSlash[1] = '\0';
    else dirPath[0] = '\0';

    return dirPath;
}

const char *System::GetPrevDirectoryPath(const char *dirPath)
{
    static char prevDirPath[256];
    strcpy(prevDirPath, dirPath);

    char *lastSlash = strrchr(prevDirPath, '/');

    if (lastSlash != NULL)
    {
        lastSlash[0] = '\0';
        lastSlash = strrchr(prevDirPath, '/');
        if (lastSlash != NULL) lastSlash[1] = '\0';
        else prevDirPath[0] = '\0';
    }
    else prevDirPath[0] = '\0';

    return prevDirPath;
}

const char *System::GetWorkingDirectory()
{
    static char currentDir[256];
    GETCWD(currentDir, 256);
    return currentDir;
}

const char *System::GetApplicationDirectory()
{
    static char appDir[256];
    GETCWD(appDir, 256);
    return appDir;
}

bool System::ChangeDirectory(const char *dir)
{
    return CHDIR(dir) == 0;
}

bool System::IsPathFile(const char *path)
{
    bool result = false;

    DIR *dir = opendir(path);
    if (dir == NULL) result = true;

    return result;
}




 const char *System::TextFormat(const char *text, ...)
{

    #define MAX_TEXTFORMAT_BUFFERS 4        // Maximum number of static buffers for text formatting
    static char buffers[MAX_TEXTFORMAT_BUFFERS][MAX_TEXT_BUFFER_LENGTH] = { 0 };
    static int  index = 0;
    char *currentBuffer = buffers[index];
    memset(currentBuffer, 0, MAX_TEXT_BUFFER_LENGTH);   // Clear buffer before using
    va_list args;
    va_start(args, text);
    vsprintf(currentBuffer, text, args);
    va_end(args);
    index += 1;     // Move to next buffer for next function call
    if (index >= MAX_TEXTFORMAT_BUFFERS) index = 0;

    return currentBuffer;
}

u64 System::GetFileModTime(const char *fileName)
{
    struct stat fileStat;
    if (stat(fileName, &fileStat) == 0) return fileStat.st_mtime;
    return 0;
}


 const char *strprbrk(const char *s, const char *charset)
{
    const char *latestMatch = NULL;
    for (; s = strpbrk(s, charset), s != NULL; latestMatch = s++) { }
    return latestMatch;
}

unsigned char* LoadDataFile(const char* fileName, unsigned int* bytesRead)
{
    unsigned char* data = NULL;
    *bytesRead = 0;

 
         FILE *file = fopen(fileName, "rb");
         if (file != NULL)
         {
             fseek(file, 0, SEEK_END);
             unsigned int size = ftell(file);
             if (size > 0)
             {
                 data = (unsigned char*)malloc(size * sizeof(unsigned char));
                 fseek(file, 0, SEEK_SET);
                 unsigned int count = fread(data, 1, size, file);
                 *bytesRead = count;
                  LogInfo( "FILEIO: [%s] File loaded successfully", fileName);
             }
             else LogError( "FILEIO: [%s] Failed to read file", fileName);
             fclose(file);
         }
         else LogError( "FILEIO: [%s] Failed to open file", fileName);


      

    return data;
}

bool SaveDataFile(const char* fileName, void* data, unsigned int bytesToWrite)
{
    bool success = false;

    FILE *file = fopen(fileName, "wb");
    if (file != NULL)
    {
        unsigned int count = fwrite(data, 1, bytesToWrite, file);
        if (count < bytesToWrite) LogError( "FILEIO: [%s] Failed to write file", fileName);
        else LogInfo( "FILEIO: [%s] File saved successfully", fileName);
        fclose(file);
    }
    else LogError( "FILEIO: [%s] Failed to open file", fileName);
       

 
    return success;
}

char* LoadTextFile(const char* fileName)
{
    char* text = NULL;

    FILE *file = fopen(fileName, "rt");
    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        unsigned int size = ftell(file);
        if (size > 0)
        {
            text = (char*)malloc((size + 1) * sizeof(char));
            unsigned int count = fread(text, sizeof(char), size, file);
            if (count < size) text = (char*)realloc(text, count + 1);
            text[count] = '\0';
             LogInfo( "FILEIO: [%s] Text file loaded successfully", fileName);
        }
        else LogError( "FILEIO: [%s] Failed to read text file", fileName);
        fclose(file);
    }
    else LogError( "FILEIO: [%s] Failed to open text file", fileName);

  
    

    return text;
}

bool SaveTextFile(const char* fileName, char* text)
{
    bool success = false;

    FILE *file = fopen(fileName, "wt");
    if (file != NULL)
    {
        size_t strLen = strlen(text);
        int count = fwrite(text, 1, strLen, file);
        if (count < 0) LogError( "FILEIO: [%s] Failed to write text file", fileName);
        else  LogInfo( "FILEIO: [%s] Text file saved successfully", fileName);
        fclose(file);
    }
    else LogError( "FILEIO: [%s] Failed to open text file", fileName);


    return success;
}
