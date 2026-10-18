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
#include "kad/KadEvents.h"
#include "kademlia/utils/UInt128.h"

namespace Kademlia
{
	// The consumer set with SetEvents(), or one that ignores everything:
	// callers never have to check for "nobody listening".
	IKadEvents& Events();

	std::string ToUtf8(const CStringW &str);

	inline KadID ToKadID(const CUInt128 &uID)
	{
		KadID id;
		uID.ToByteArray(id.bytes);
		return id;
	}
}
