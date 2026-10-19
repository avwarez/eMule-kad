//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#pragma once
#include "kad/KadPrefs.h"

namespace Kademlia
{
	// The application's settings, as last set with SetPrefs(); all zero/empty
	// until then. Not to be confused with CPrefs, which is Kad's own state.
	const KadPrefs& AppPrefs();

	// configDir and commentFilter, converted once for the code that wants a CString
	const CString& AppConfigDir();
	const CString& AppCommentFilter();
}
