#ifndef PORT_ENHANCEMENTS_INTERNAL_BUTTONS_H
#define PORT_ENHANCEMENTS_INTERNAL_BUTTONS_H

// N64 button bitmasks. Duplicated here so the C++ enhancement layer doesn't
// have to pull in PR/os.h. Kept narrow (only what these enhancements need); if
// more enhancements grow C++-side button decoding, extend this header.

namespace ssb64 {
namespace enhancements {
namespace internal {

constexpr unsigned short A_BUTTON  = 0x8000;
constexpr unsigned short U_CBUTTON = 0x0008;
constexpr unsigned short D_CBUTTON = 0x0004;
constexpr unsigned short L_CBUTTON = 0x0002;
constexpr unsigned short R_CBUTTON = 0x0001;
constexpr unsigned short U_JPAD    = 0x0800;
constexpr unsigned short D_JPAD    = 0x0400;
constexpr unsigned short L_JPAD    = 0x0200;
constexpr unsigned short R_JPAD    = 0x0100;

}
}
}

#endif
