#include "common.h"
#include "patcher.h"

#include "IniFile.h"

WRAPPER void CIniFile::LoadIniFile(void) { EAXJMP(0x4A1770); }