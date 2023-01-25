// SPDX-FileCopyrightText: 2023-present pfSimulator contributors
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PF_ASSERT_H
#define PF_ASSERT_H

#include <SDL2/SDL.h>

#define PF_ASSERT_DEBUG(condition)   SDL_assert(condition)
#define PF_ASSERT(condition)         SDL_assert_release(condition)

#endif
