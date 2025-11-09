#pragma once

#ifndef HP_SAVE_SAVECRYPTO_H
#define HP_SAVE_SAVECRYPTO_H

#include "types.h"

void savc_encryptBlock(u8* data, u32 length);
bool32 savc_decryptBlock(u8* data, u32 length, u32 signature);

#endif
