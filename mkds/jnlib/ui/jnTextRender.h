#pragma once

#ifndef HP_JNLIB_UI_JNTEXTRENDER_H
#define HP_JNLIB_UI_JNTEXTRENDER_H

#include "types.h"
#include <nnsys/g2d/g2d_TextCanvas.h>

void jn_ui_drawFormattedTextRect(const NNSG2dTextCanvas* canvas, int x, int y, int width, int height, int color,
								 int alignFlags, int a8, const u16* text);
void jn_ui_drawFormattedTextRectMonospace(const NNSG2dTextCanvas* canvas, int x, int y, int width, int height,
										  int color, int alignFlags, int a8, const u16* text, bool32 monospaceNumbers,
										  int charWidth);
void jn_ui_drawFormattedTextRectSomething(const NNSG2dTextCanvas* canvas, int x, int y, int width, int height,
										  int color, int alignFlags, int a8, const u16* text);

#endif
