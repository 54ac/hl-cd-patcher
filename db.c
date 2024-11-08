#include "db.h"

const char *patchTypes[] = {"CD check", "timebomb"};

// version name, build number, filesize, checksum, patch type, offset, old byte, new byte
const Database patches[] = {
    {"Beta 9 Net Test 1", 722, 1171456, 103791551, TIMEBOMB, 269647, 0x22, 0x6B},
    {"Beta 12 Net Test 2", 725, 1191936, 105952782, TIMEBOMB, 277199, 0x22, 0x6B},
    {"Beta 12 Net Test 2", 725, 1191936, 105952782, CD_CHECK, 279168, 0x75, 0xEB},
    {"1.0.0.5 (Italian)", 735, 1044480, 94794306, CD_CHECK, 171733, 0x75, 0xEB},
    {"RC4", 738, 1042944, 94129104, CD_CHECK, 114741, 0x75, 0xEB}, // reconstructed
    {"1.0.0.5", 742, 1044480, 94259778, CD_CHECK, 114789, 0x75, 0xEB},
    {"1.0.0.6", 761, 1047040, 94646854, CD_CHECK, 254640, 0x75, 0xEB},
    {"1.0.0.8", 807, 1055232, 95703142, CD_CHECK, 371200, 0x75, 0xEB},
    {"1.0.0.9 (TFC beta)", 874, 1062400, 97111873, CD_CHECK, 372106, 0x85, 0x89},
    {"1.0.0.9 (TFC beta)", 881, 1054208, 95833272, CD_CHECK, 368474, 0x85, 0x89},
    {"1.0.0.9", 893, 1065984, 97205227, CD_CHECK, 376554, 0x85, 0x89},
    {"1.0.1.0", 929, 1067520, 97556496, CD_CHECK, 378858, 0x85, 0x89},
    {"1.0.1.3", 1013, 1141760, 103849247, CD_CHECK, 428858, 0x85, 0x89},
    {"1.0.1.5", 1083, 1142784, 103969561, CD_CHECK, 23738, 0x85, 0x89},
    {"1.0.1.6", 1202, 1144832, 104200096, CD_CHECK, 470746, 0x85, 0x89},
    {"1.1.0.0", 1321, 917504, 84141384, CD_CHECK, 61175, 0x85, 0x89},
    {"1.1.0.1", 1341, 905216, 83376604, CD_CHECK, 61191, 0x85, 0x89}};

const int patchesCount = sizeof(patches) / sizeof(patches[0]);