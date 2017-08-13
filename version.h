#ifndef VERSION_H
#define VERSION_H

#define MIM_VERSION_MAJOR 0
#define MIM_VERSION_MINOR 0
#define MIM_VERSION_PATCH 0
#define MIM_VERSION_IS_RELEASE 0
#define MIM_VERSION_SUFFIX "dev"

#define MIM_VERSION_HEX          \
    ((MIM_VERSION_MAJOR << 16) | \
    (MIM_VERSION_MINOR <<  8)  | \
    (MIM_VERSION_PATCH))

#define MIM_STRINGIFY(v) MIM_STRINGIFY_HELPER(v)
#define MIM_STRINGIFY_HELPER(v) #v

#define MIM_VERSION_STRING_BASE          \
    MIM_STRINGIFY(MIM_VERSION_MAJOR) "." \
    MIM_STRINGIFY(MIM_VERSION_MINOR) "." \
    MIM_STRINGIFY(MIM_VERSION_PATCH)

#if MIM_VERSION_IS_RELEASE
# define MIM_VERSION_STRING  MIM_VERSION_STRING_BASE
#else
# define MIM_VERSION_STRING  MIM_VERSION_STRING_BASE "-" MIM_VERSION_SUFFIX
#endif

unsigned int mim_version(void) {
    return MIM_VERSION_HEX;
}

const char* mim_version_string(void) {
    return MIM_VERSION_STRING;
}

#endif // VERSION_H
