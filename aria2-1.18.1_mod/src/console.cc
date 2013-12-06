/* <!-- copyright */
/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2011 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#include "console.h"
#include "NullOutputFile.h"
#ifdef __MINGW32__
# include "WinConsoleFile.h"
#else // !__MINGW32__
# include "BufferedFile.h"
#endif // !__MINGW32__

namespace aria2 {

namespace global {

namespace {
Console consoleCout;
Console consoleCerr;
};

void initConsole(bool suppress)
{
  if(suppress) {
    consoleCerr.reset(new NullOutputFile());
    consoleCout.reset(new NullOutputFile());
  } else {
#ifdef __MINGW32__
    consoleCout.reset(new WinConsoleFile(STD_OUTPUT_HANDLE));
    consoleCerr.reset(new WinConsoleFile(STD_ERROR_HANDLE));
#else // !__MINGW32__
    consoleCout.reset(new BufferedFile(stdout));
    consoleCerr.reset(new BufferedFile(stderr));
#endif // !__MINGW32__
  }
}

const Console& cout()
{
  return consoleCout;
}

const Console& cerr()
{
  return consoleCerr;
}

} // namespace global

} // namespace aria2