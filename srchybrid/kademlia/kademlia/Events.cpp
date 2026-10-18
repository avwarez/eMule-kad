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
#include "stdafx.h"
#include "kademlia/kademlia/Events.h"
#include "kademlia/kademlia/Kademlia.h"
#include "kademlia/kademlia/Prefs.h"
#include "kademlia/kademlia/Search.h"
#include "kademlia/kademlia/SearchManager.h"
#include "kademlia/kademlia/UDPFirewallTester.h"
#include "kademlia/utils/LookupHistory.h"
#include "StringConversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Kademlia;

static IKadEvents s_noEvents;
static IKadEvents *s_pEvents = NULL;

IKadEvents& Kademlia::Events()
{
	return s_pEvents ? *s_pEvents : s_noEvents;
}

std::string Kademlia::ToUtf8(const CStringW &str)
{
	const CStringA sUtf8(wc2utf8(str));
	return std::string((LPCSTR)sUtf8, (size_t)sUtf8.GetLength());
}

void Kademlia::SetEvents(IKadEvents *pEvents)
{
	s_pEvents = pEvents;
}

KadState Kademlia::GetState()
{
	KadState state = {};
	state.running = CKademlia::IsRunning();
	if (state.running) {
		state.connected = CKademlia::IsConnected();
		state.firewalled = CKademlia::IsFirewalled();
		state.firewalledUDP = CUDPFirewallTester::IsFirewalledUDP(true);
		state.ip = CKademlia::GetIPAddress();
		const CPrefs *pPrefs = CKademlia::GetPrefs();
		if (pPrefs)
			state.id = ToKadID(pPrefs->GetKadID());
		state.users = CKademlia::GetKademliaUsers();
		state.files = CKademlia::GetKademliaFiles();
	}
	return state;
}

bool Kademlia::GetSearchInfo(uint32_t handle, KadSearchInfo &out)
{
	const CSearch *pSearch = CSearchManager::GetSearchByHandle(handle);
	if (pSearch == NULL)
		return false;
	out = pSearch->GetInfo();
	return true;
}

bool Kademlia::GetLookupHistory(uint32_t handle, KadLookupHistory &out)
{
	const CSearch *pSearch = CSearchManager::GetSearchByHandle(handle);
	if (pSearch == NULL || pSearch->GetLookupHistory() == NULL)
		return false;
	pSearch->GetLookupHistory()->GetSnapshot(out);
	out.handle = handle;
	return true;
}
