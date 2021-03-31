/*
 * KDiff3 - Text Diff And Merge Tool
 *
 * SPDX-FileCopyrightText: 2019-2020 Michael Reeves reeves.87@gmail.com
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "Logging.h"

Q_LOGGING_CATEGORY(kdiffMain, "org.kde.kdiff3")
Q_LOGGING_CATEGORY(kdiffFileAccess, "org.kde.kdiff3.fileAccess")

/*
    This can make a lot of noise at info level. Depending exactly what the user is doing.
    This is typically needed only for debugging.
*/
Q_LOGGING_CATEGORY(kdiffDiffTextWindow, "org.kde.kdiff3.kdifftextwindow", QtWarningMsg);
//The following is very noisey if debug is turned on and not really useful unless your making changes in the core data processing.
Q_LOGGING_CATEGORY(kdiffCore, "org.kde.kdiff3.core", QtWarningMsg)
Q_LOGGING_CATEGORY(kdiffGitIgnoreList, "org.kde.kdiff3.gitIgnoreList")
