#include "stream.hpp"

namespace mmd {

    void Stream::read(size_t len, void *ptr) {
        int ret = fread(ptr, len, 1, file);
        CHECK_EQ(ret, 1) << "read failed!";
    }

    template <typename T>
    T Stream::read() {
        T ret;
        read(sizeof(T), &ret);
        return ret;
    }

    template<>
    vec2 Stream::read() {
        vec2 ret;
        read(2 * sizeof(float), &ret.x);
        return ret;
    }

    template<>
    vec3 Stream::read() {
        vec3 ret;
        read(3 * sizeof(float), &ret.x);
        return ret;
    }

    template<>
    vec4 Stream::read() {
        vec4 ret;
        read(4 * sizeof(float), &ret.x);
        return ret;
    }

    void Stream::skip(size_t len) {
        fseek(file, len, SEEK_CUR);
    }

    int Stream::readInt()               { return read<int>();            }
    unsigned Stream::readUInt()         { return read<unsigned>();       }
    short Stream::readShort()           { return read<short>();          }
    unsigned short Stream::readUShort() { return read<unsigned short>(); }
    unsigned char Stream::readByte()    { return read<unsigned char>();  }
    char Stream::readSByte()            { return read<char>();           }
    float Stream::readFloat()           { return read<float>();          }
    vec2 Stream::readVec2()             { return read<vec2>();           }
    vec3 Stream::readVec3()             { return read<vec3>();           }
    vec4 Stream::readVec4()             { return read<vec4>();           }
    void Stream::skipStr() {
        int len = readInt();
        skip(len);
    }

    void Stream::readUTF16(std::string &str) {
        int len = readInt() / 2;
        str.resize(len);
        for (int i = 0; i < len; ++i) {
            str[i] = readByte();
            skip(1);
        }
    }

    void Stream::readUTF8(std::string &str) {
        int len = readInt();
        str.resize(len);
        for (int i = 0; i < len; ++i) str[i] = readByte();
    }

    void Stream::readStr(std::string &str, int text) {
        if (!text) {
            readUTF16(str);
        } else {
            readUTF8(str);
        }
    }

    int Stream::readIndex(int size) {
        switch (size) {
            case 1:
            return readSByte();
            break;
            case 2:
            return readShort();
            break;
            case 4:
            return readInt();
            break;
            default:
            FAIL << "Invalid index size!";
        }
        return -1;
    }

    unsigned Stream::readUIndex(int size) {
        switch (size) {
            case 1:
            return readByte();
            break;
            case 2:
            return readUShort();
            break;
            case 4:
            return readUInt();
            break;
            default:
            FAIL << "Invalid index size!";
        }
        return -1;
    }

    Stream::Stream(const char *path) {
        file = fopen(path, "r");
        CHECK_NQ(file, NULL) << "Open file " << path << " failed!";
    }

    void Stream::close() {
        fclose(file);
        file = NULL;
    }

    Stream::~Stream() {
        if (file) close();
    }

    Fs::~Fs() {}

    Fs *Fs::open(const char *path) {
        return new Stream(path);
    }

} /* mmd */