/*
 * VncKeyMapper.cpp - VNC key mapper
 *
 * Copyright (c) 2006-2017 Tobias Doerffel <tobydox/at/users/dot/sf/dot/net>
 *
 * This file is part of Veyon - http://veyon.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#define XK_KOREAN
#include "rfb/keysym.h"

#include "VncKeyMapper.h"


static const QMap<Qt::Key, VncKeyMapper::XKey> qtKeyMap = {
	{ Qt::Key_Backspace, XK_BackSpace },
	{ Qt::Key_Tab, XK_Tab },
	{ Qt::Key_Clear, XK_Clear },
	{ Qt::Key_Return, XK_Return },
	{ Qt::Key_Pause, XK_Pause },
	{ Qt::Key_ScrollLock, XK_Scroll_Lock },
	{ Qt::Key_Escape, XK_Escape },
	{ Qt::Key_Delete, XK_Delete },

	// Modifiers
	{ Qt::Key_Shift, XK_Shift_L },
	{ Qt::Key_Control, XK_Control_L },
	{ Qt::Key_Alt, XK_Alt_L },
	{ Qt::Key_CapsLock, XK_Caps_Lock },
	{ Qt::Key_Meta, XK_Meta_L },

	// Left & Right Windows keys & Windows Menu Key
	{ Qt::Key_Super_L, XK_Super_L },
	{ Qt::Key_Super_R, XK_Super_R },
	{ Qt::Key_Menu, XK_Menu },

	// Cursor control & motion
	{ Qt::Key_Home, XK_Home },
	{ Qt::Key_Left, XK_Left },
	{ Qt::Key_Up, XK_Up },
	{ Qt::Key_Right, XK_Right },
	{ Qt::Key_Down, XK_Down },
	{ Qt::Key_PageUp, XK_Page_Up },
	{ Qt::Key_PageDown, XK_Page_Down },
	{ Qt::Key_End, XK_End },

	// Misc Functions
	{ Qt::Key_Select, XK_Select },
	{ Qt::Key_Print, XK_Print },
	{ Qt::Key_Execute, XK_Execute },
	{ Qt::Key_Insert, XK_Insert },
	{ Qt::Key_Help, XK_Help },
	{ Qt::Key_Cancel, XK_Break },
	{ Qt::Key_NumLock, XK_Num_Lock },

	// Auxilliary Functions - must come before XK_KP_F1, etc
	{ Qt::Key_F1, XK_F1 },
	{ Qt::Key_F2, XK_F2 },
	{ Qt::Key_F3, XK_F3 },
	{ Qt::Key_F4, XK_F4 },
	{ Qt::Key_F5, XK_F5 },
	{ Qt::Key_F6, XK_F6 },
	{ Qt::Key_F7, XK_F7 },
	{ Qt::Key_F8, XK_F8 },
	{ Qt::Key_F9, XK_F9 },
	{ Qt::Key_F10, XK_F10 },
	{ Qt::Key_F11, XK_F11 },
	{ Qt::Key_F12, XK_F12 },
	{ Qt::Key_F13, XK_F13 },
	{ Qt::Key_F14, XK_F14 },
	{ Qt::Key_F15, XK_F15 },
	{ Qt::Key_F16, XK_F16 },
	{ Qt::Key_F17, XK_F17 },
	{ Qt::Key_F18, XK_F18 },
	{ Qt::Key_F19, XK_F19 },
	{ Qt::Key_F20, XK_F20 },
	{ Qt::Key_F21, XK_F21 },
	{ Qt::Key_F22, XK_F22 },
	{ Qt::Key_F23, XK_F23 },
	{ Qt::Key_F24, XK_F24 },

	{ Qt::Key_Hyper_L, XK_Hyper_L },
	{ Qt::Key_Hyper_R, XK_Hyper_R },
	{ Qt::Key_AltGr, XK_ISO_Level3_Shift },
	{ Qt::Key_Multi_key, XK_Multi_key },
	{ Qt::Key_SingleCandidate, XK_SingleCandidate },
	{ Qt::Key_MultipleCandidate, XK_MultipleCandidate },
	{ Qt::Key_PreviousCandidate, XK_PreviousCandidate },
	{ Qt::Key_Mode_switch, XK_Mode_switch },
	{ Qt::Key_Kanji, XK_Kanji },
	{ Qt::Key_Muhenkan, XK_Muhenkan },
	{ Qt::Key_Henkan, XK_Henkan },
	{ Qt::Key_Romaji, XK_Romaji },
	{ Qt::Key_Hiragana, XK_Hiragana },
	{ Qt::Key_Katakana, XK_Katakana },
	{ Qt::Key_Hiragana_Katakana, XK_Hiragana_Katakana },
	{ Qt::Key_Zenkaku, XK_Zenkaku },
	{ Qt::Key_Hankaku, XK_Hankaku },
	{ Qt::Key_Zenkaku_Hankaku, XK_Zenkaku_Hankaku },
	{ Qt::Key_Touroku, XK_Touroku },
	{ Qt::Key_Massyo, XK_Massyo },
	{ Qt::Key_Kana_Lock, XK_Kana_Lock },
	{ Qt::Key_Kana_Shift, XK_Kana_Shift },
	{ Qt::Key_Eisu_Shift, XK_Eisu_Shift },
	{ Qt::Key_Eisu_toggle, XK_Eisu_toggle },
	{ Qt::Key_Hangul, XK_Hangul },
	{ Qt::Key_Hangul_Start, XK_Hangul_Start },
	{ Qt::Key_Hangul_End, XK_Hangul_End },
	{ Qt::Key_Hangul_Hanja, XK_Hangul_Hanja },
	{ Qt::Key_Hangul_Jamo, XK_Hangul_Jamo },
	{ Qt::Key_Hangul_Romaja, XK_Hangul_Romaja },
	{ Qt::Key_Hangul_Jeonja, XK_Hangul_Jeonja },
	{ Qt::Key_Hangul_Banja, XK_Hangul_Banja },
	{ Qt::Key_Hangul_PreHanja, XK_Hangul_PreHanja },
	{ Qt::Key_Hangul_PostHanja, XK_Hangul_PostHanja },
	{ Qt::Key_Hangul_Special, XK_Hangul_Special },

	{ Qt::Key_Dead_Grave, XK_dead_grave },
	{ Qt::Key_Dead_Acute, XK_dead_acute },
	{ Qt::Key_Dead_Circumflex, XK_dead_circumflex },
	{ Qt::Key_Dead_Tilde, XK_dead_tilde },
	{ Qt::Key_Dead_Macron, XK_dead_macron },
	{ Qt::Key_Dead_Breve, XK_dead_breve },
	{ Qt::Key_Dead_Abovedot, XK_dead_abovedot },
	{ Qt::Key_Dead_Diaeresis, XK_dead_diaeresis },
	{ Qt::Key_Dead_Abovering, XK_dead_abovering },
	{ Qt::Key_Dead_Doubleacute, XK_dead_doubleacute },
	{ Qt::Key_Dead_Caron, XK_dead_caron },
	{ Qt::Key_Dead_Cedilla, XK_dead_cedilla },
	{ Qt::Key_Dead_Ogonek, XK_dead_ogonek },
	{ Qt::Key_Dead_Iota, XK_dead_iota },
	{ Qt::Key_Dead_Voiced_Sound, XK_dead_voiced_sound },
	{ Qt::Key_Dead_Semivoiced_Sound, XK_dead_semivoiced_sound },
	{ Qt::Key_Dead_Belowdot, XK_dead_belowdot },

};


// Define the X11 KeySym and UCS map, excerpted from X11R6.8.1
/* $XFree86: xc/lib/X11/imKStoUCS.c,v 1.4 2003/04/29 11:29:18 pascal Exp $ */
static unsigned short const keysym_to_unicode_1a1_1ff[] = {
			0x0104, 0x02d8, 0x0141, 0x0000, 0x013d, 0x015a, 0x0000, /* 0x01a0-0x01a7 */
	0x0000, 0x0160, 0x015e, 0x0164, 0x0179, 0x0000, 0x017d, 0x017b, /* 0x01a8-0x01af */
	0x0000, 0x0105, 0x02db, 0x0142, 0x0000, 0x013e, 0x015b, 0x02c7, /* 0x01b0-0x01b7 */
	0x0000, 0x0161, 0x015f, 0x0165, 0x017a, 0x02dd, 0x017e, 0x017c, /* 0x01b8-0x01bf */
	0x0154, 0x0000, 0x0000, 0x0102, 0x0000, 0x0139, 0x0106, 0x0000, /* 0x01c0-0x01c7 */
	0x010c, 0x0000, 0x0118, 0x0000, 0x011a, 0x0000, 0x0000, 0x010e, /* 0x01c8-0x01cf */
	0x0110, 0x0143, 0x0147, 0x0000, 0x0000, 0x0150, 0x0000, 0x0000, /* 0x01d0-0x01d7 */
	0x0158, 0x016e, 0x0000, 0x0170, 0x0000, 0x0000, 0x0162, 0x0000, /* 0x01d8-0x01df */
	0x0155, 0x0000, 0x0000, 0x0103, 0x0000, 0x013a, 0x0107, 0x0000, /* 0x01e0-0x01e7 */
	0x010d, 0x0000, 0x0119, 0x0000, 0x011b, 0x0000, 0x0000, 0x010f, /* 0x01e8-0x01ef */
	0x0111, 0x0144, 0x0148, 0x0000, 0x0000, 0x0151, 0x0000, 0x0000, /* 0x01f0-0x01f7 */
	0x0159, 0x016f, 0x0000, 0x0171, 0x0000, 0x0000, 0x0163, 0x02d9  /* 0x01f8-0x01ff */
};

static unsigned short const keysym_to_unicode_2a1_2fe[] = {
			0x0126, 0x0000, 0x0000, 0x0000, 0x0000, 0x0124, 0x0000, /* 0x02a0-0x02a7 */
	0x0000, 0x0130, 0x0000, 0x011e, 0x0134, 0x0000, 0x0000, 0x0000, /* 0x02a8-0x02af */
	0x0000, 0x0127, 0x0000, 0x0000, 0x0000, 0x0000, 0x0125, 0x0000, /* 0x02b0-0x02b7 */
	0x0000, 0x0131, 0x0000, 0x011f, 0x0135, 0x0000, 0x0000, 0x0000, /* 0x02b8-0x02bf */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x010a, 0x0108, 0x0000, /* 0x02c0-0x02c7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x02c8-0x02cf */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0120, 0x0000, 0x0000, /* 0x02d0-0x02d7 */
	0x011c, 0x0000, 0x0000, 0x0000, 0x0000, 0x016c, 0x015c, 0x0000, /* 0x02d8-0x02df */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x010b, 0x0109, 0x0000, /* 0x02e0-0x02e7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x02e8-0x02ef */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0121, 0x0000, 0x0000, /* 0x02f0-0x02f7 */
	0x011d, 0x0000, 0x0000, 0x0000, 0x0000, 0x016d, 0x015d          /* 0x02f8-0x02ff */
};

static unsigned short const keysym_to_unicode_3a2_3fe[] = {
					0x0138, 0x0156, 0x0000, 0x0128, 0x013b, 0x0000, /* 0x03a0-0x03a7 */
	0x0000, 0x0000, 0x0112, 0x0122, 0x0166, 0x0000, 0x0000, 0x0000, /* 0x03a8-0x03af */
	0x0000, 0x0000, 0x0000, 0x0157, 0x0000, 0x0129, 0x013c, 0x0000, /* 0x03b0-0x03b7 */
	0x0000, 0x0000, 0x0113, 0x0123, 0x0167, 0x014a, 0x0000, 0x014b, /* 0x03b8-0x03bf */
	0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x012e, /* 0x03c0-0x03c7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0116, 0x0000, 0x0000, 0x012a, /* 0x03c8-0x03cf */
	0x0000, 0x0145, 0x014c, 0x0136, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x03d0-0x03d7 */
	0x0000, 0x0172, 0x0000, 0x0000, 0x0000, 0x0168, 0x016a, 0x0000, /* 0x03d8-0x03df */
	0x0101, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x012f, /* 0x03e0-0x03e7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0117, 0x0000, 0x0000, 0x012b, /* 0x03e8-0x03ef */
	0x0000, 0x0146, 0x014d, 0x0137, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x03f0-0x03f7 */
	0x0000, 0x0173, 0x0000, 0x0000, 0x0000, 0x0169, 0x016b          /* 0x03f8-0x03ff */
};

static unsigned short const keysym_to_unicode_4a1_4df[] = {
			0x3002, 0x3008, 0x3009, 0x3001, 0x30fb, 0x30f2, 0x30a1, /* 0x04a0-0x04a7 */
	0x30a3, 0x30a5, 0x30a7, 0x30a9, 0x30e3, 0x30e5, 0x30e7, 0x30c3, /* 0x04a8-0x04af */
	0x30fc, 0x30a2, 0x30a4, 0x30a6, 0x30a8, 0x30aa, 0x30ab, 0x30ad, /* 0x04b0-0x04b7 */
	0x30af, 0x30b1, 0x30b3, 0x30b5, 0x30b7, 0x30b9, 0x30bb, 0x30bd, /* 0x04b8-0x04bf */
	0x30bf, 0x30c1, 0x30c4, 0x30c6, 0x30c8, 0x30ca, 0x30cb, 0x30cc, /* 0x04c0-0x04c7 */
	0x30cd, 0x30ce, 0x30cf, 0x30d2, 0x30d5, 0x30d8, 0x30db, 0x30de, /* 0x04c8-0x04cf */
	0x30df, 0x30e0, 0x30e1, 0x30e2, 0x30e4, 0x30e6, 0x30e8, 0x30e9, /* 0x04d0-0x04d7 */
	0x30ea, 0x30eb, 0x30ec, 0x30ed, 0x30ef, 0x30f3, 0x309b, 0x309c  /* 0x04d8-0x04df */
};

static unsigned short const keysym_to_unicode_590_5fe[] = {
	0x06f0, 0x06f1, 0x06f2, 0x06f3, 0x06f4, 0x06f5, 0x06f6, 0x06f7, /* 0x0590-0x0597 */
	0x06f8, 0x06f9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x0598-0x059f */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x066a, 0x0670, 0x0679, /* 0x05a0-0x05a7 */
	0x067e, 0x0686, 0x0688, 0x0691, 0x060c, 0x0000, 0x06d4, 0x0000, /* 0x05a8-0x05af */
	0x0660, 0x0661, 0x0662, 0x0663, 0x0664, 0x0665, 0x0666, 0x0667, /* 0x05b0-0x05b7 */
	0x0668, 0x0669, 0x0000, 0x061b, 0x0000, 0x0000, 0x0000, 0x061f, /* 0x05b8-0x05bf */
	0x0000, 0x0621, 0x0622, 0x0623, 0x0624, 0x0625, 0x0626, 0x0627, /* 0x05c0-0x05c7 */
	0x0628, 0x0629, 0x062a, 0x062b, 0x062c, 0x062d, 0x062e, 0x062f, /* 0x05c8-0x05cf */
	0x0630, 0x0631, 0x0632, 0x0633, 0x0634, 0x0635, 0x0636, 0x0637, /* 0x05d0-0x05d7 */
	0x0638, 0x0639, 0x063a, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x05d8-0x05df */
	0x0640, 0x0641, 0x0642, 0x0643, 0x0644, 0x0645, 0x0646, 0x0647, /* 0x05e0-0x05e7 */
	0x0648, 0x0649, 0x064a, 0x064b, 0x064c, 0x064d, 0x064e, 0x064f, /* 0x05e8-0x05ef */
	0x0650, 0x0651, 0x0652, 0x0653, 0x0654, 0x0655, 0x0698, 0x06a4, /* 0x05f0-0x05f7 */
	0x06a9, 0x06af, 0x06ba, 0x06be, 0x06cc, 0x06d2, 0x06c1          /* 0x05f8-0x05fe */
};

static unsigned short const keysym_to_unicode_680_6ff[] = {
	0x0492, 0x0496, 0x049a, 0x049c, 0x04a2, 0x04ae, 0x04b0, 0x04b2, /* 0x0680-0x0687 */
	0x04b6, 0x04b8, 0x04ba, 0x0000, 0x04d8, 0x04e2, 0x04e8, 0x04ee, /* 0x0688-0x068f */
	0x0493, 0x0497, 0x049b, 0x049d, 0x04a3, 0x04af, 0x04b1, 0x04b3, /* 0x0690-0x0697 */
	0x04b7, 0x04b9, 0x04bb, 0x0000, 0x04d9, 0x04e3, 0x04e9, 0x04ef, /* 0x0698-0x069f */
	0x0000, 0x0452, 0x0453, 0x0451, 0x0454, 0x0455, 0x0456, 0x0457, /* 0x06a0-0x06a7 */
	0x0458, 0x0459, 0x045a, 0x045b, 0x045c, 0x0491, 0x045e, 0x045f, /* 0x06a8-0x06af */
	0x2116, 0x0402, 0x0403, 0x0401, 0x0404, 0x0405, 0x0406, 0x0407, /* 0x06b0-0x06b7 */
	0x0408, 0x0409, 0x040a, 0x040b, 0x040c, 0x0490, 0x040e, 0x040f, /* 0x06b8-0x06bf */
	0x044e, 0x0430, 0x0431, 0x0446, 0x0434, 0x0435, 0x0444, 0x0433, /* 0x06c0-0x06c7 */
	0x0445, 0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e, /* 0x06c8-0x06cf */
	0x043f, 0x044f, 0x0440, 0x0441, 0x0442, 0x0443, 0x0436, 0x0432, /* 0x06d0-0x06d7 */
	0x044c, 0x044b, 0x0437, 0x0448, 0x044d, 0x0449, 0x0447, 0x044a, /* 0x06d8-0x06df */
	0x042e, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413, /* 0x06e0-0x06e7 */
	0x0425, 0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, /* 0x06e8-0x06ef */
	0x041f, 0x042f, 0x0420, 0x0421, 0x0422, 0x0423, 0x0416, 0x0412, /* 0x06f0-0x06f7 */
	0x042c, 0x042b, 0x0417, 0x0428, 0x042d, 0x0429, 0x0427, 0x042a  /* 0x06f8-0x06ff */
};

static unsigned short const keysym_to_unicode_7a1_7f9[] = {
			0x0386, 0x0388, 0x0389, 0x038a, 0x03aa, 0x0000, 0x038c, /* 0x07a0-0x07a7 */
	0x038e, 0x03ab, 0x0000, 0x038f, 0x0000, 0x0000, 0x0385, 0x2015, /* 0x07a8-0x07af */
	0x0000, 0x03ac, 0x03ad, 0x03ae, 0x03af, 0x03ca, 0x0390, 0x03cc, /* 0x07b0-0x07b7 */
	0x03cd, 0x03cb, 0x03b0, 0x03ce, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x07b8-0x07bf */
	0x0000, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, /* 0x07c0-0x07c7 */
	0x0398, 0x0399, 0x039a, 0x039b, 0x039c, 0x039d, 0x039e, 0x039f, /* 0x07c8-0x07cf */
	0x03a0, 0x03a1, 0x03a3, 0x0000, 0x03a4, 0x03a5, 0x03a6, 0x03a7, /* 0x07d0-0x07d7 */
	0x03a8, 0x03a9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x07d8-0x07df */
	0x0000, 0x03b1, 0x03b2, 0x03b3, 0x03b4, 0x03b5, 0x03b6, 0x03b7, /* 0x07e0-0x07e7 */
	0x03b8, 0x03b9, 0x03ba, 0x03bb, 0x03bc, 0x03bd, 0x03be, 0x03bf, /* 0x07e8-0x07ef */
	0x03c0, 0x03c1, 0x03c3, 0x03c2, 0x03c4, 0x03c5, 0x03c6, 0x03c7, /* 0x07f0-0x07f7 */
	0x03c8, 0x03c9                                                  /* 0x07f8-0x07ff */
};

static unsigned short const keysym_to_unicode_8a4_8fe[] = {
									0x2320, 0x2321, 0x0000, 0x231c, /* 0x08a0-0x08a7 */
	0x231d, 0x231e, 0x231f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x08a8-0x08af */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x08b0-0x08b7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x2264, 0x2260, 0x2265, 0x222b, /* 0x08b8-0x08bf */
	0x2234, 0x0000, 0x221e, 0x0000, 0x0000, 0x2207, 0x0000, 0x0000, /* 0x08c0-0x08c7 */
	0x2245, 0x2246, 0x0000, 0x0000, 0x0000, 0x0000, 0x22a2, 0x0000, /* 0x08c8-0x08cf */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x221a, 0x0000, /* 0x08d0-0x08d7 */
	0x0000, 0x0000, 0x2282, 0x2283, 0x2229, 0x222a, 0x2227, 0x2228, /* 0x08d8-0x08df */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x08e0-0x08e7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x08e8-0x08ef */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0192, 0x0000, /* 0x08f0-0x08f7 */
	0x0000, 0x0000, 0x0000, 0x2190, 0x2191, 0x2192, 0x2193          /* 0x08f8-0x08ff */
};

static unsigned short const keysym_to_unicode_9df_9f8[] = {
															0x2422, /* 0x09d8-0x09df */
	0x2666, 0x25a6, 0x2409, 0x240c, 0x240d, 0x240a, 0x0000, 0x0000, /* 0x09e0-0x09e7 */
	0x240a, 0x240b, 0x2518, 0x2510, 0x250c, 0x2514, 0x253c, 0x2500, /* 0x09e8-0x09ef */
	0x0000, 0x0000, 0x0000, 0x0000, 0x251c, 0x2524, 0x2534, 0x252c, /* 0x09f0-0x09f7 */
	0x2502                                                          /* 0x09f8-0x09ff */
};

static unsigned short const keysym_to_unicode_aa1_afe[] = {
			0x2003, 0x2002, 0x2004, 0x2005, 0x2007, 0x2008, 0x2009, /* 0x0aa0-0x0aa7 */
	0x200a, 0x2014, 0x2013, 0x0000, 0x0000, 0x0000, 0x2026, 0x2025, /* 0x0aa8-0x0aaf */
	0x2153, 0x2154, 0x2155, 0x2156, 0x2157, 0x2158, 0x2159, 0x215a, /* 0x0ab0-0x0ab7 */
	0x2105, 0x0000, 0x0000, 0x2012, 0x2039, 0x2024, 0x203a, 0x0000, /* 0x0ab8-0x0abf */
	0x0000, 0x0000, 0x0000, 0x215b, 0x215c, 0x215d, 0x215e, 0x0000, /* 0x0ac0-0x0ac7 */
	0x0000, 0x2122, 0x2120, 0x0000, 0x25c1, 0x25b7, 0x25cb, 0x25ad, /* 0x0ac8-0x0acf */
	0x2018, 0x2019, 0x201c, 0x201d, 0x211e, 0x0000, 0x2032, 0x2033, /* 0x0ad0-0x0ad7 */
	0x0000, 0x271d, 0x0000, 0x220e, 0x25c2, 0x2023, 0x25cf, 0x25ac, /* 0x0ad8-0x0adf */
	0x25e6, 0x25ab, 0x25ae, 0x25b5, 0x25bf, 0x2606, 0x2022, 0x25aa, /* 0x0ae0-0x0ae7 */
	0x25b4, 0x25be, 0x261a, 0x261b, 0x2663, 0x2666, 0x2665, 0x0000, /* 0x0ae8-0x0aef */
	0x2720, 0x2020, 0x2021, 0x2713, 0x2612, 0x266f, 0x266d, 0x2642, /* 0x0af0-0x0af7 */
	0x2640, 0x2121, 0x2315, 0x2117, 0x2038, 0x201a, 0x201e          /* 0x0af8-0x0aff */
};

/* none of the APL keysyms match the Unicode characters */

static unsigned short const keysym_to_unicode_cdf_cfa[] = {
															0x2017, /* 0x0cd8-0x0cdf */
	0x05d0, 0x05d1, 0x05d2, 0x05d3, 0x05d4, 0x05d5, 0x05d6, 0x05d7, /* 0x0ce0-0x0ce7 */
	0x05d8, 0x05d9, 0x05da, 0x05db, 0x05dc, 0x05dd, 0x05de, 0x05df, /* 0x0ce8-0x0cef */
	0x05e0, 0x05e1, 0x05e2, 0x05e3, 0x05e4, 0x05e5, 0x05e6, 0x05e7, /* 0x0cf0-0x0cf7 */
	0x05e8, 0x05e9, 0x05ea                                          /* 0x0cf8-0x0cff */
};

static unsigned short const keysym_to_unicode_da1_df9[] = {
			0x0e01, 0x0e02, 0x0e03, 0x0e04, 0x0e05, 0x0e06, 0x0e07, /* 0x0da0-0x0da7 */
	0x0e08, 0x0e09, 0x0e0a, 0x0e0b, 0x0e0c, 0x0e0d, 0x0e0e, 0x0e0f, /* 0x0da8-0x0daf */
	0x0e10, 0x0e11, 0x0e12, 0x0e13, 0x0e14, 0x0e15, 0x0e16, 0x0e17, /* 0x0db0-0x0db7 */
	0x0e18, 0x0e19, 0x0e1a, 0x0e1b, 0x0e1c, 0x0e1d, 0x0e1e, 0x0e1f, /* 0x0db8-0x0dbf */
	0x0e20, 0x0e21, 0x0e22, 0x0e23, 0x0e24, 0x0e25, 0x0e26, 0x0e27, /* 0x0dc0-0x0dc7 */
	0x0e28, 0x0e29, 0x0e2a, 0x0e2b, 0x0e2c, 0x0e2d, 0x0e2e, 0x0e2f, /* 0x0dc8-0x0dcf */
	0x0e30, 0x0e31, 0x0e32, 0x0e33, 0x0e34, 0x0e35, 0x0e36, 0x0e37, /* 0x0dd0-0x0dd7 */
	0x0e38, 0x0e39, 0x0e3a, 0x0000, 0x0000, 0x0000, 0x0e3e, 0x0e3f, /* 0x0dd8-0x0ddf */
	0x0e40, 0x0e41, 0x0e42, 0x0e43, 0x0e44, 0x0e45, 0x0e46, 0x0e47, /* 0x0de0-0x0de7 */
	0x0e48, 0x0e49, 0x0e4a, 0x0e4b, 0x0e4c, 0x0e4d, 0x0000, 0x0000, /* 0x0de8-0x0def */
	0x0e50, 0x0e51, 0x0e52, 0x0e53, 0x0e54, 0x0e55, 0x0e56, 0x0e57, /* 0x0df0-0x0df7 */
	0x0e58, 0x0e59                                                  /* 0x0df8-0x0dff */
};

static unsigned short const keysym_to_unicode_ea0_eff[] = {
	0x0000, 0x1101, 0x1101, 0x11aa, 0x1102, 0x11ac, 0x11ad, 0x1103, /* 0x0ea0-0x0ea7 */
	0x1104, 0x1105, 0x11b0, 0x11b1, 0x11b2, 0x11b3, 0x11b4, 0x11b5, /* 0x0ea8-0x0eaf */
	0x11b6, 0x1106, 0x1107, 0x1108, 0x11b9, 0x1109, 0x110a, 0x110b, /* 0x0eb0-0x0eb7 */
	0x110c, 0x110d, 0x110e, 0x110f, 0x1110, 0x1111, 0x1112, 0x1161, /* 0x0eb8-0x0ebf */
	0x1162, 0x1163, 0x1164, 0x1165, 0x1166, 0x1167, 0x1168, 0x1169, /* 0x0ec0-0x0ec7 */
	0x116a, 0x116b, 0x116c, 0x116d, 0x116e, 0x116f, 0x1170, 0x1171, /* 0x0ec8-0x0ecf */
	0x1172, 0x1173, 0x1174, 0x1175, 0x11a8, 0x11a9, 0x11aa, 0x11ab, /* 0x0ed0-0x0ed7 */
	0x11ac, 0x11ad, 0x11ae, 0x11af, 0x11b0, 0x11b1, 0x11b2, 0x11b3, /* 0x0ed8-0x0edf */
	0x11b4, 0x11b5, 0x11b6, 0x11b7, 0x11b8, 0x11b9, 0x11ba, 0x11bb, /* 0x0ee0-0x0ee7 */
	0x11bc, 0x11bd, 0x11be, 0x11bf, 0x11c0, 0x11c1, 0x11c2, 0x0000, /* 0x0ee8-0x0eef */
	0x0000, 0x0000, 0x1140, 0x0000, 0x0000, 0x1159, 0x119e, 0x0000, /* 0x0ef0-0x0ef7 */
	0x11eb, 0x0000, 0x11f9, 0x0000, 0x0000, 0x0000, 0x0000, 0x20a9, /* 0x0ef8-0x0eff */
};

static unsigned short const keysym_to_unicode_12a1_12fe[] = {
			0x1e02, 0x1e03, 0x0000, 0x0000, 0x0000, 0x1e0a, 0x0000, /* 0x12a0-0x12a7 */
	0x1e80, 0x0000, 0x1e82, 0x1e0b, 0x1ef2, 0x0000, 0x0000, 0x0000, /* 0x12a8-0x12af */
	0x1e1e, 0x1e1f, 0x0000, 0x0000, 0x1e40, 0x1e41, 0x0000, 0x1e56, /* 0x12b0-0x12b7 */
	0x1e81, 0x1e57, 0x1e83, 0x1e60, 0x1ef3, 0x1e84, 0x1e85, 0x1e61, /* 0x12b8-0x12bf */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x12c0-0x12c7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x12c8-0x12cf */
	0x0174, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1e6a, /* 0x12d0-0x12d7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0176, 0x0000, /* 0x12d8-0x12df */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x12e0-0x12e7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x12e8-0x12ef */
	0x0175, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1e6b, /* 0x12f0-0x12f7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0177          /* 0x12f0-0x12ff */
};

static unsigned short const keysym_to_unicode_13bc_13be[] = {
									0x0152, 0x0153, 0x0178          /* 0x13b8-0x13bf */
};

static unsigned short const keysym_to_unicode_14a1_14ff[] = {
			0x2741, 0x00a7, 0x0589, 0x0029, 0x0028, 0x00bb, 0x00ab, /* 0x14a0-0x14a7 */
	0x2014, 0x002e, 0x055d, 0x002c, 0x2013, 0x058a, 0x2026, 0x055c, /* 0x14a8-0x14af */
	0x055b, 0x055e, 0x0531, 0x0561, 0x0532, 0x0562, 0x0533, 0x0563, /* 0x14b0-0x14b7 */
	0x0534, 0x0564, 0x0535, 0x0565, 0x0536, 0x0566, 0x0537, 0x0567, /* 0x14b8-0x14bf */
	0x0538, 0x0568, 0x0539, 0x0569, 0x053a, 0x056a, 0x053b, 0x056b, /* 0x14c0-0x14c7 */
	0x053c, 0x056c, 0x053d, 0x056d, 0x053e, 0x056e, 0x053f, 0x056f, /* 0x14c8-0x14cf */
	0x0540, 0x0570, 0x0541, 0x0571, 0x0542, 0x0572, 0x0543, 0x0573, /* 0x14d0-0x14d7 */
	0x0544, 0x0574, 0x0545, 0x0575, 0x0546, 0x0576, 0x0547, 0x0577, /* 0x14d8-0x14df */
	0x0548, 0x0578, 0x0549, 0x0579, 0x054a, 0x057a, 0x054b, 0x057b, /* 0x14e0-0x14e7 */
	0x054c, 0x057c, 0x054d, 0x057d, 0x054e, 0x057e, 0x054f, 0x057f, /* 0x14e8-0x14ef */
	0x0550, 0x0580, 0x0551, 0x0581, 0x0552, 0x0582, 0x0553, 0x0583, /* 0x14f0-0x14f7 */
	0x0554, 0x0584, 0x0555, 0x0585, 0x0556, 0x0586, 0x2019, 0x0027, /* 0x14f8-0x14ff */
};

static unsigned short const keysym_to_unicode_15d0_15f6[] = {
	0x10d0, 0x10d1, 0x10d2, 0x10d3, 0x10d4, 0x10d5, 0x10d6, 0x10d7, /* 0x15d0-0x15d7 */
	0x10d8, 0x10d9, 0x10da, 0x10db, 0x10dc, 0x10dd, 0x10de, 0x10df, /* 0x15d8-0x15df */
	0x10e0, 0x10e1, 0x10e2, 0x10e3, 0x10e4, 0x10e5, 0x10e6, 0x10e7, /* 0x15e0-0x15e7 */
	0x10e8, 0x10e9, 0x10ea, 0x10eb, 0x10ec, 0x10ed, 0x10ee, 0x10ef, /* 0x15e8-0x15ef */
	0x10f0, 0x10f1, 0x10f2, 0x10f3, 0x10f4, 0x10f5, 0x10f6          /* 0x15f0-0x15f7 */
};

static unsigned short const keysym_to_unicode_16a0_16f6[] = {
	0x0000, 0x0000, 0xf0a2, 0x1e8a, 0x0000, 0xf0a5, 0x012c, 0xf0a7, /* 0x16a0-0x16a7 */
	0xf0a8, 0x01b5, 0x01e6, 0x0000, 0x0000, 0x0000, 0x0000, 0x019f, /* 0x16a8-0x16af */
	0x0000, 0x0000, 0xf0b2, 0x1e8b, 0x01d1, 0xf0b5, 0x012d, 0xf0b7, /* 0x16b0-0x16b7 */
	0xf0b8, 0x01b6, 0x01e7, 0x0000, 0x0000, 0x01d2, 0x0000, 0x0275, /* 0x16b8-0x16bf */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x018f, 0x0000, /* 0x16c0-0x16c7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x16c8-0x16cf */
	0x0000, 0x1e36, 0xf0d2, 0xf0d3, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x16d0-0x16d7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x16d8-0x16df */
	0x0000, 0x1e37, 0xf0e2, 0xf0e3, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x16e0-0x16e7 */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0x16e8-0x16ef */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0259          /* 0x16f0-0x16f6 */
};

static unsigned short const keysym_to_unicode_1e9f_1eff[] = {
															0x0303,
	0x1ea0, 0x1ea1, 0x1ea2, 0x1ea3, 0x1ea4, 0x1ea5, 0x1ea6, 0x1ea7, /* 0x1ea0-0x1ea7 */
	0x1ea8, 0x1ea9, 0x1eaa, 0x1eab, 0x1eac, 0x1ead, 0x1eae, 0x1eaf, /* 0x1ea8-0x1eaf */
	0x1eb0, 0x1eb1, 0x1eb2, 0x1eb3, 0x1eb4, 0x1eb5, 0x1eb6, 0x1eb7, /* 0x1eb0-0x1eb7 */
	0x1eb8, 0x1eb9, 0x1eba, 0x1ebb, 0x1ebc, 0x1ebd, 0x1ebe, 0x1ebf, /* 0x1eb8-0x1ebf */
	0x1ec0, 0x1ec1, 0x1ec2, 0x1ec3, 0x1ec4, 0x1ec5, 0x1ec6, 0x1ec7, /* 0x1ec0-0x1ec7 */
	0x1ec8, 0x1ec9, 0x1eca, 0x1ecb, 0x1ecc, 0x1ecd, 0x1ece, 0x1ecf, /* 0x1ec8-0x1ecf */
	0x1ed0, 0x1ed1, 0x1ed2, 0x1ed3, 0x1ed4, 0x1ed5, 0x1ed6, 0x1ed7, /* 0x1ed0-0x1ed7 */
	0x1ed8, 0x1ed9, 0x1eda, 0x1edb, 0x1edc, 0x1edd, 0x1ede, 0x1edf, /* 0x1ed8-0x1edf */
	0x1ee0, 0x1ee1, 0x1ee2, 0x1ee3, 0x1ee4, 0x1ee5, 0x1ee6, 0x1ee7, /* 0x1ee0-0x1ee7 */
	0x1ee8, 0x1ee9, 0x1eea, 0x1eeb, 0x1eec, 0x1eed, 0x1eee, 0x1eef, /* 0x1ee8-0x1eef */
	0x1ef0, 0x1ef1, 0x0300, 0x0301, 0x1ef4, 0x1ef5, 0x1ef6, 0x1ef7, /* 0x1ef0-0x1ef7 */
	0x1ef8, 0x1ef9, 0x01a0, 0x01a1, 0x01af, 0x01b0, 0x0309, 0x0323  /* 0x1ef8-0x1eff */
};

static unsigned short const keysym_to_unicode_20a0_20ac[] = {
	0x20a0, 0x20a1, 0x20a2, 0x20a3, 0x20a4, 0x20a5, 0x20a6, 0x20a7, /* 0x20a0-0x20a7 */
	0x20a8, 0x20a9, 0x20aa, 0x20ab, 0x20ac                          /* 0x20a8-0x20af */
};


#define IsUCSPrintableLatin1(c) ( ( ((c) >= 32) && ((c) <= 126) ) || \
								  ( ((c) >= 160) && ((c) <= 255) ) \
								)
#define IsUCSLatinExtAB(c)      ( ((c) >= 0x0100) && ((c) <= 0x024f) )
#define IsUCSGreek_Coptic(c)    ( ((c) >= 0x0370) && ((c) <= 0x03ff) )
#define IsUCSCyrillic(c)        ( ((c) >= 0x0400) && ((c) <= 0x04ff) )
#define IsUCSArmenian(c)        ( ((c) >= 0x0530) && ((c) <= 0x058f) )
#define IsUCSHebrew(c)          ( ((c) >= 0x0590) && ((c) <= 0x05ff) )
#define IsUCSArabic(c)          ( ((c) >= 0x0600) && ((c) <= 0x06ff) )
#define IsUCSThai(c)            ( ((c) >= 0x0e00) && ((c) <= 0x0e7f) )
#define IsUCSGeorgian(c)        ( ((c) >= 0x10a0) && ((c) <= 0x10ff) )
#define IsUCSHangulJamo(c)      ( ((c) >= 0x1100) && ((c) <= 0x11ff) )
#define IsUCSLatinExtAdd(c)     ( ((c) >= 0x1e00) && ((c) <= 0x1eff) )
#define IsUCSGenPunc(c)         ( ((c) >= 0x2000) && ((c) <= 0x206f) )
#define IsUCSCurrSym(c)         ( ((c) >= 0x20a0) && ((c) <= 0x20cf) )
#define IsUCSCJKSym_Punc(c)     ( ((c) >= 0x3000) && ((c) <= 0x303f) )
#define IsUCSKatakana(c)        ( ((c) >= 0x30a0) && ((c) <= 0x30ff) )

#define IsKSLatinExt(c)         ( ( ((c) & 0xff00) == 0x01 ) || \
								  ( ((c) & 0xff00) == 0x02 ) || \
								  ( ((c) & 0xff00) == 0x03 ) || \
								  ( ((c) & 0xff00) == 0x12 ) || \
								  ( ((c) & 0xff00) == 0x13 ) \
								)
#define IsKSKatakana(c)         ( ((c) & 0xff00) == 0x0400 )
#define IsKSArabic(c)           ( ((c) & 0xff00) == 0x0500 )
#define IsKSCyrillic(c)         ( ((c) & 0xff00) == 0x0600 )
#define IsKSGreek(c)            ( ((c) & 0xff00) == 0x0700 )
#define IsKSTechnical(c)        ( ((c) & 0xff00) == 0x0800 )
#define IsKSSpecial(c)          ( ((c) & 0xff00) == 0x0900 )
#define IsKSPublishing(c)       ( ((c) & 0xff00) == 0x0a00 )
#define IsKSAPL(c)              ( ((c) & 0xff00) == 0x0b00 )
#define IsKSHebrew(c)           ( ((c) & 0xff00) == 0x0c00 )
#define IsKSThai(c)             ( ((c) & 0xff00) == 0x0d00 )
#define IsKSKorean(c)           ( ((c) & 0xff00) == 0x0e00 )
#define IsKSArmenian(c)         ( ((c) & 0xff00) == 0x1400 )
#define IsKSGeorgian(c)         ( ((c) & 0xff00) == 0x1500 )
#define IsKSAzeri(c)            ( ((c) & 0xff00) == 0x1600 )
#define IsKSVietnamese(c)       ( ((c) & 0xff00) == 0x1e00 )
#define IsKSCurrency(c)         ( ((c) & 0xff00) == 0x2000 )
#define IsKSUnicode(c)          ( ((c) & 0x01000000) == 0x01000000 )



VncKeyMapper::VncKeyMapper() :
	qtModifierKeys( {
					Qt::Key_Shift,
					Qt::Key_Control,
					Qt::Key_Alt,
					Qt::Key_Meta,
					Qt::Key_Super_L,
					Qt::Key_Super_R
					} )
{
	initMaps();
}



bool VncKeyMapper::isModifier( Qt::Key qtKey ) const
{
	return qtModifierKeys.contains( qtKey );
}



VncKeyMapper::XKey VncKeyMapper::qtToXKey( Qt::Key qtKey ) const
{
	return qtKeyMap.value( qtKey, XK_VoidSymbol );
}



VncKeyMapper::XKey VncKeyMapper::unicodeToXKey( VncKeyMapper::UnicodeKey unicodeKey ) const
{
	XKey xkey = XK_VoidSymbol;
	bool isUnicodeKeysym = false;

	if (IsUCSPrintableLatin1(unicodeKey)) {
		// If the key is printable Latin-1 character
		// Latin-1 in UCS and X KeySym are exactly the same code
		xkey = (XKey) (unicodeKey & 0xffff);
	} else if (unicodeKey < 255) {
		// Do not send non printable Latin-1 character
	   qWarning("VncKeyMapper: 0x%04x (Non printable Latin-1): ", unicodeKey);
	} else if (IsUCSLatinExtAB(unicodeKey)) {
		// Latin Extended-A or B
		if (ucsLatinExtABMap.find(unicodeKey) != ucsLatinExtABMap.end()) {
			xkey = ucsLatinExtABMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSGreek_Coptic(unicodeKey)) {
		// Greek and Coptic
		if (ucsGreek_CopticMap.find(unicodeKey) != ucsGreek_CopticMap.end()) {
			xkey = ucsGreek_CopticMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSCyrillic(unicodeKey)) {
		// Cyrillic
		if (ucsCyrillicMap.find(unicodeKey) != ucsCyrillicMap.end()) {
			xkey = ucsCyrillicMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSArmenian(unicodeKey)) {
		// Armenian
		if (ucsArmenianMap.find(unicodeKey) != ucsArmenianMap.end()) {
			xkey = ucsArmenianMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSHebrew(unicodeKey)) {
		// Hebrew
		if (ucsHebrewMap.find(unicodeKey) != ucsHebrewMap.end()) {
			xkey = ucsHebrewMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSArabic(unicodeKey)) {
		// Arabic
		if (ucsArabicMap.find(unicodeKey) != ucsArabicMap.end()) {
			xkey = ucsArabicMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSThai(unicodeKey)) {
		// Thai
		if (ucsThaiMap.find(unicodeKey) != ucsThaiMap.end()) {
			xkey = ucsThaiMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSGeorgian(unicodeKey)) {
		// Georgian
		if (ucsGeorgianMap.find(unicodeKey) != ucsGeorgianMap.end()) {
			xkey = ucsGeorgianMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSHangulJamo(unicodeKey)) {
		// Hangul Jamo
		if (ucsHangulJamoMap.find(unicodeKey) != ucsHangulJamoMap.end()) {
			xkey = ucsHangulJamoMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSLatinExtAdd(unicodeKey)) {
		// Latin Extended Additional
		if (ucsLatinExtAddMap.find(unicodeKey) != ucsLatinExtAddMap.end()) {
			xkey = ucsLatinExtAddMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSGenPunc(unicodeKey)) {
		// General Punctuation
		if (ucsGenPuncMap.find(unicodeKey) != ucsGenPuncMap.end()) {
			xkey = ucsGenPuncMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSCurrSym(unicodeKey)) {
		// Currency Symbols
		if (ucsCurrSymMap.find(unicodeKey) != ucsCurrSymMap.end()) {
			xkey = ucsCurrSymMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSCJKSym_Punc(unicodeKey)) {
		// CJK Symbols and Punctuation
		if (ucsCJKSym_PuncMap.find(unicodeKey) != ucsCJKSym_PuncMap.end()) {
			xkey = ucsCJKSym_PuncMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (IsUCSKatakana(unicodeKey)) {
		// Katakana
		if (ucsKatakanaMap.find(unicodeKey) != ucsKatakanaMap.end()) {
			xkey = ucsKatakanaMap[unicodeKey];
		} else {
			isUnicodeKeysym = true;
		}
	} else if (ucsOthersMap.find(unicodeKey) != ucsOthersMap.end()) {
		// Other Unicode Characters
		xkey = ucsOthersMap[unicodeKey];
	} else {
		isUnicodeKeysym = true;
	}

	if (isUnicodeKeysym) {
		// Send the undefined X KeySym by using X protocol convention for
		//   defining new KeySym (Unicode KeySym):
		//   KeySym = ((UCS & 0xffffff) | 0x01000000)
		// Since Windows only support UCS-16 so we can have this formula:
		xkey = (((XKey) unicodeKey) & 0xffff) | 0x01000000;
	}

	return xkey;
}



void VncKeyMapper::initMaps()
{
	size_t i;
	XKey j;
	UnicodeKey ucs;

	// Building map for KeySym Latin-2, 3, 4, 8 & 9 and UCS Latin Extended-A & B
	for( i = 0, j = 0x01a1; i < (sizeof(keysym_to_unicode_1a1_1ff) / sizeof(short)); i++, j++ ) {
		ksLatinExtMap[j] = ucs = (UnicodeKey) keysym_to_unicode_1a1_1ff[i];
		if (IsUCSLatinExtAB(ucs))
			ucsLatinExtABMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}
	for( i = 0, j = 0x02a1; i < (sizeof(keysym_to_unicode_2a1_2fe) / sizeof(short)); i++, j++ ) {
		ksLatinExtMap[j] = ucs = (UnicodeKey) keysym_to_unicode_1a1_1ff[i];
		if (IsUCSLatinExtAB(ucs))
			ucsLatinExtABMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}
	for( i = 0, j = 0x03a2; i < (sizeof(keysym_to_unicode_3a2_3fe) / sizeof(short)); i++, j++ ) {
		ksLatinExtMap[j] = ucs = (UnicodeKey) keysym_to_unicode_1a1_1ff[i];
		if (IsUCSLatinExtAB(ucs))
			ucsLatinExtABMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}
	for( i = 0, j = 0x12a1; i < (sizeof(keysym_to_unicode_12a1_12fe) / sizeof(short)); i++, j++ ) {
		ksLatinExtMap[j] = ucs = (UnicodeKey) keysym_to_unicode_1a1_1ff[i];
		if (IsUCSLatinExtAB(ucs))
			ucsLatinExtABMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}
	for( i = 0, j = 0x13bc; i < (sizeof(keysym_to_unicode_13bc_13be) / sizeof(short)); i++, j++ ) {
		ksLatinExtMap[j] = ucs = (UnicodeKey) keysym_to_unicode_1a1_1ff[i];
		if (IsUCSLatinExtAB(ucs))
			ucsLatinExtABMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Katakana and UCS Katakana, CJK Symbols and Punctuation
	for( i = 0, j = 0x04a1; i < (sizeof(keysym_to_unicode_4a1_4df) / sizeof(short)); i++, j++ ) {
		ksKatakanaMap[j] = ucs = (UnicodeKey) keysym_to_unicode_4a1_4df[i];
		if (IsUCSKatakana(ucs))
			ucsKatakanaMap[ucs] = j;
		  else if (IsUCSCJKSym_Punc(ucs))
			ucsCJKSym_PuncMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Arabic and UCS Arabic
	for( i = 0, j = 0x0590; i < (sizeof(keysym_to_unicode_590_5fe) / sizeof(short)); i++, j++ ) {
		ksArabicMap[j] = ucs = (UnicodeKey) keysym_to_unicode_590_5fe[i];
		if (IsUCSArabic(ucs))
			ucsArabicMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Cyrillic and UCS Cyrillic
	for( i = 0, j = 0x0680; i < (sizeof(keysym_to_unicode_680_6ff) / sizeof(short)); i++, j++ ) {
		ksCyrillicMap[j] = ucs = (UnicodeKey) keysym_to_unicode_680_6ff[i];
		if (IsUCSCyrillic(ucs))
			ucsCyrillicMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Greek and UCS Greek and Coptic
	for( i = 0, j = 0x07a1; i < (sizeof(keysym_to_unicode_7a1_7f9) / sizeof(short)); i++, j++ ) {
		ksGreekMap[j] = ucs = (UnicodeKey) keysym_to_unicode_7a1_7f9[i];
		if (IsUCSGreek_Coptic(ucs))
			ucsGreek_CopticMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Technical and UCS Others
	for( i = 0, j = 0x08a4; i < (sizeof(keysym_to_unicode_8a4_8fe) / sizeof(short)); i++, j++ ) {
		ksTechnicalMap[j] = ucs = (UnicodeKey) keysym_to_unicode_8a4_8fe[i];
		if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Special and UCS Others
	for( i = 0, j = 0x09df; i < (sizeof(keysym_to_unicode_9df_9f8) / sizeof(short)); i++, j++ ) {
		ksSpecialMap[j] = ucs = (UnicodeKey) keysym_to_unicode_aa1_afe[i];
		if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Publishing and UCS Others
	for( i = 0, j = 0x0aa1; i < (sizeof(keysym_to_unicode_aa1_afe) / sizeof(short)); i++, j++ ) {
		ksPublishingMap[j] = ucs = (UnicodeKey) keysym_to_unicode_aa1_afe[i];
		if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Hebrew and UCS Hebrew, General Punctuation
	for( i = 0, j = 0x0cdf; i < (sizeof(keysym_to_unicode_cdf_cfa) / sizeof(short)); i++, j++ ) {
		ksHebrewMap[j] = ucs = (UnicodeKey) keysym_to_unicode_cdf_cfa[i];
		if (IsUCSHebrew(ucs))
			ucsHebrewMap[ucs] = j;
		  else if (IsUCSGenPunc(ucs))
			ucsGenPuncMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Thai and UCS Thai
	for( i = 0, j = 0x0da1; i < (sizeof(keysym_to_unicode_da1_df9) / sizeof(short)); i++, j++ ) {
		ksThaiMap[j] = ucs = (UnicodeKey) keysym_to_unicode_da1_df9[i];
		if (IsUCSThai(ucs))
			ucsThaiMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Korean and UCS Hangul Jamo
	for( i = 0, j = 0x0ea0; i < (sizeof(keysym_to_unicode_ea0_eff) / sizeof(short)); i++, j++ ) {
		ksKoreanMap[j] = ucs = (UnicodeKey) keysym_to_unicode_ea0_eff[i];
		if (IsUCSHangulJamo(ucs))
			ucsHangulJamoMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Armenian and UCS Armenian, General Punctuation
	for( i = 0, j = 0x14a1; i < (sizeof(keysym_to_unicode_14a1_14ff) / sizeof(short)); i++, j++ ) {
		ksArmenianMap[j] = ucs = (UnicodeKey) keysym_to_unicode_14a1_14ff[i];
		if (IsUCSArmenian(ucs))
			ucsArmenianMap[ucs] = j;
		  else if (IsUCSGenPunc(ucs))
			ucsGenPuncMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Georgian and UCS Georgian
	for( i = 0, j = 0x15d0; i < (sizeof(keysym_to_unicode_15d0_15f6) / sizeof(short)); i++, j++ ) {
		ksGeorgianMap[j] = ucs = (UnicodeKey) keysym_to_unicode_15d0_15f6[i];
		if (IsUCSGeorgian(ucs))
			ucsGeorgianMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Azeri and UCS Others
	for( i = 0, j = 0x16a0; i < (sizeof(keysym_to_unicode_16a0_16f6) / sizeof(short)); i++, j++ ) {
		ksAzeriMap[j] = ucs = (UnicodeKey) keysym_to_unicode_16a0_16f6[i];
		if (IsUCSLatinExtAdd(ucs))
			ucsLatinExtAddMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Vietnamese and UCS Latin Extended Additional, Latin Extended-A & B
	for( i = 0, j = 0x1e9f; i < (sizeof(keysym_to_unicode_1e9f_1eff) / sizeof(short)); i++, j++ ) {
		ksVietnameseMap[j] = ucs = (UnicodeKey) keysym_to_unicode_1e9f_1eff[i];
		if (IsUCSLatinExtAdd(ucs))
			ucsLatinExtAddMap[ucs] = j;
		  else if (IsUCSLatinExtAB(ucs))
			ucsLatinExtABMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}

	// Building map for KeySym Currency and UCS Currency Symbols
	for( i = 0, j = 0x20a0; i < (sizeof(keysym_to_unicode_20a0_20ac) / sizeof(short)); i++, j++ ) {
		ksCurrencyMap[j] = ucs = (UnicodeKey) keysym_to_unicode_20a0_20ac[i];
		if (IsUCSCurrSym(ucs))
			ucsCurrSymMap[ucs] = j;
		  else if (ucs && (ucsOthersMap.find(ucs) != ucsOthersMap.end()))
			ucsOthersMap[ucs] = j;
	}
}
