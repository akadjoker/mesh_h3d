#pragma once

#include "Config.hpp"





class     Stream 
{
    public:
        Stream();
        virtual ~Stream();


 
    u64 Read(void* buffer, u64 size) ;
    u64 Write(const void* buffer, u64 size) ;
    u64 Cursor() const ;
    u64 Seek(u64 offset, bool relative = false) ;
    u64 Seek(u64 offset, int whence) ;
    

    virtual void Close() ;

    u64  Size() const { return m_size; }
    bool IsOpen() const { return m_open; }

    bool IsEOF() const;
    
    u8 peek() const;
    void ignore();

    char ReadChar();
    unsigned char ReadByte();
    short         ReadShort();
    int           ReadInt();
    long          ReadLong();
    float         ReadFloat();
    double        ReadDouble();

    std::string ReadLine(bool breakLine = false);

    std::string ReadString() ;
    size_t  WriteString(const std::string& value);
    size_t WriteStringSize(const char* value, size_t size);

    size_t  WriteUTFString(const std::string& value);
    std::string  ReadUTFString()  ;

    size_t  WriteChar(char value);
    size_t  WriteByte(unsigned char value);
    size_t  WriteShort(short value);
    size_t  WriteInt(int32_t value);
    size_t  WriteLong(long value);
    size_t  WriteFloat(float value);
    size_t  WriteDouble(double value);
protected:
     FILE* f_file;
    bool m_open;
    u64 m_size;



};

class     FileStream : public Stream
{
public:
    FileStream();
    FileStream(const std::string& filePath, const std::string& mode);
    bool Open(const std::string& filePath, const std::string& mode);
    bool Create(const std::string& filePath, bool overwrite = false);
    std::string& GetPath() { return m_path; }
    std::string& GetFileName()  { return m_fileName; }   
private:     
    std::string m_path;
    std::string m_fileName;
};
