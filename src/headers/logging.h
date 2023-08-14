// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#pragma once

void logging_set_onloop(bool value);
bool logging_get_onloop();

void info(char *msg, ...);
