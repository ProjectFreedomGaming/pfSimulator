// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_PFX1_REGISTERS_H
#define PF_PFX1_REGISTERS_H

// -- Register Address Map
#define PF_PFX1_BASE                       0x00D00000
#define PF_PFX1_VSYNC_COUNT                0x00000000
#define PF_PFX1_CONTROL                    0x00000002
#define PF_PFX1_COLOR_RG                   0x00000004
#define PF_PFX1_COLOR_BA                   0x00000006

// -- Register Content
#define PF_PFX1_CONTROL_CLEAR_SCREEN       0x0001
#define PF_PFX1_CONTROL_SWAP_BUFFER        0x0002

#endif
