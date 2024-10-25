#include "pch.h"

//********************************************************************************************************************
// FILE/MEMORY STREAM
//********************************************************************************************************************

Stream::Stream()
{
    f_file = nullptr;
    m_open = false;
    m_size = 0;
}

Stream::~Stream()
{
    Close();
}

void Stream::Close()
{
    if (f_file != nullptr)
    {
        fclose(f_file);
        f_file = nullptr;
    }
}

u64 Stream::Read(void *buffer, u64 size)
{

    return fread(buffer, 1, size, f_file);
}

u64 Stream::Write(const void *buffer, u64 size)
{
    return fwrite(buffer, 1, size, f_file);
}

u64 Stream::Cursor() const
{
    return ftell(f_file);
}

std::string Stream::ReadString()
{
    std::string value;
    value = "";
    char c;
    while ((c = ReadChar()) != '\0')
    {
        value += c;
    }
    return value;
}

size_t Stream::WriteString(const std::string &value)
{
    size_t size = 0;
    for (u32 i = 0; i < value.length(); i++)
    {
        size += WriteChar(value[i]);
    }
    size += WriteChar('\0');
    return size;
}
size_t Stream::WriteStringSize(const char *value, size_t size)
{

    for (u32 i = 0; i < size; i++)
    {
        WriteChar(value[i]);
    }
    return size;
}
size_t Stream::WriteUTFString(const std::string &value)
{
    uint32_t lengthInBytes = value.size();  // Tamanho em bytes da string UTF-8
    Write(&lengthInBytes, sizeof(lengthInBytes));  // Grava o comprimento em bytes
    return fwrite(value.c_str(), sizeof(char), lengthInBytes, f_file) + sizeof(lengthInBytes);
}

std::string Stream::ReadUTFString()
{
    uint32_t lengthInBytes;
    Read(&lengthInBytes, sizeof(lengthInBytes));  // Lê o comprimento em bytes
    std::string value(lengthInBytes, '\0');  
    Read(&value[0], lengthInBytes);  
    
    return value; 
}

bool Stream::IsEOF() const
{
    return Cursor() >= m_size;
}

void Stream::ignore()
{
    ReadChar();
}

char Stream::ReadChar()
{
    char value;
    Read(&value, sizeof(value));
    return value;
}

size_t Stream::WriteChar(char value)
{
    Write(&value, sizeof(value));
    return 1;
}

unsigned char Stream::ReadByte()
{
    unsigned char value;
    Read(&value, sizeof(unsigned char));
    return value;
}

short Stream::ReadShort()
{
    short value = 0;
    Read(&value, sizeof(short));
    return value;
}

int Stream::ReadInt()
{
    int value;
    Read(&value, sizeof(value));
    return value;
}

long Stream::ReadLong()
{
    long value;
    Read(&value, sizeof(long));
    return value;
}

float Stream::ReadFloat()
{
    float value;
    Read(&value, sizeof(float));
    return value;
}

double Stream::ReadDouble()
{
    double value;
    Read(&value, sizeof(double));
    return value;
}

std::string Stream::ReadLine(bool breakLine)
{
    std::string value = "";
    const c8 nlChar1 = 13, nlChar2 = 10;
    c8 CurChar = 0;

    while (!IsEOF())
    {
        CurChar = ReadChar();

        if (IsEOF())
            break;
        if (CurChar == nlChar1)
        {
            if (breakLine)
                break;
            else
                continue;
        }
        if (CurChar == nlChar2)
            break;

        value += CurChar;
    }

    return value;
}

size_t Stream::WriteByte(unsigned char value)
{

    Write(&value, sizeof(unsigned char));
    return 1;
}

size_t Stream::WriteShort(short value)
{
    Write(&value, sizeof(value));
    return 2;
}

size_t Stream::WriteInt(int32_t value)
{
    Write(&value, sizeof(value));
    return 4;
}

size_t Stream::WriteLong(long value)
{
    Write(&value, sizeof(value));
    return 4;
}

size_t Stream::WriteFloat(float value)
{
    Write(&value, sizeof(value));
    return 4;
}

size_t Stream::WriteDouble(double value)
{
    Write(&value, sizeof(value));
    return 8;
}

u64 Stream::Seek(u64 offset, bool relative)
{
    return fseek(f_file, offset, relative ? SEEK_CUR : SEEK_SET);
}

u64 Stream::Seek(u64 offset, int relative)
{
    return fseek(f_file, offset, relative);
}

FileStream::FileStream() : Stream()
{
}

FileStream::FileStream(const std::string &filePath, const std::string &mode) : Stream()
{
    Open(filePath, mode);
}

bool FileStream::Open(const std::string &filePath, const std::string &mode)
{
    f_file = fopen(filePath.c_str(), mode.c_str());
    if (f_file == nullptr)
    {
        printf("[FILESTREAM] Can't open: %s\n", filePath.c_str());
        m_open = false;
        return false;
    }
    fseek(f_file, 0, SEEK_END);
    m_size = ftell(f_file);
    fseek(f_file, 0, SEEK_SET);
    m_open = true;
    printf("FILE: file read: %s\n", filePath.c_str());
    return true;
}

bool FileStream::Create(const std::string &filePath, bool overwrite)
{
    const char *mode = overwrite ? "wb" : "ab";
    f_file = fopen(filePath.c_str(), mode);
    if (f_file == nullptr)
    {
        printf("[FILESTREAM] Can't create: %s\n", filePath.c_str());
        m_open = false;
        return false;
    }
    m_open = true;
    printf("FILE: file created: %s\n", filePath.c_str());
    return true;
}
