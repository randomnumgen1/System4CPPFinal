#ifndef _SYSTEM_VERSION_H
#define _SYSTEM_VERSION_H
/*
Version.h
Total Size: 8 Bytes

Major - 2 Bytes
Minor - 2 Bytes
Revision - 2 Bytes
Build - 2 Bytes
*/
#include <cinttypes>
#include <sstream>
namespace System{
struct Version{
public:
	uint16_t Major, Minor,Revision, Build;
	
	Version(std::string versionStr)
	{
        sscanf_s(versionStr.c_str(), "%"   SCNu16   ".%"   SCNu16   ".%"   SCNu16   ".%"   SCNu16, &this->Major, &this->Minor, &this->Revision, &this->Build);
	}
    Version(uint16_t major, uint16_t minor, uint16_t revision, uint16_t build)
        : Major(major), Minor(minor), Revision(revision), Build(build) {}

    std::string ToString() const {
        std::ostringstream oss{};
        oss << Major << '.' << Minor << '.' << Revision << '.' << Build;
        return oss.str();
    }
    bool operator<(const Version& otherVersion) const
    {
        if (Major < otherVersion.Major)
            return true;
        if (Minor < otherVersion.Minor)
            return true;
        if (Revision < otherVersion.Revision)
            return true;
        if (Build < otherVersion.Build)
            return true;
        return false;
    }
    bool operator==(const Version& other) const {
        return Major == other.Major && Minor == other.Minor && Revision == other.Revision && Build == other.Build;
    }

    bool operator!=(const Version& other) const {
        return !(*this == other);
    }
    bool operator<=(const Version& other) const {
        return *this < other || *this == other;
    }

    bool operator>(const Version& other) const {
        return !(*this <= other);
    }

    bool operator>=(const Version& other) const {
        return !(*this < other);
    }
    friend std::ostream& operator << (std::ostream& stream, const Version& ver) {
        stream << '[';
        stream << ver.Major;
        stream << '.';
        stream << ver.Minor;
        stream << '.';
        stream << ver.Revision;
        stream << '.';
        stream << ver.Build;
        stream << ']';
        return stream;
    };
};

}
#endif