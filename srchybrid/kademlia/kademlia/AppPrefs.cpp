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
#include "kademlia/kademlia/AppPrefs.h"
#include "StringConversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Kademlia;

static KadPrefs s_prefs = {};
static CString s_sConfigDir;
static CString s_sCommentFilter;

const KadPrefs& Kademlia::AppPrefs()
{
	return s_prefs;
}

const CString& Kademlia::AppConfigDir()
{
	return s_sConfigDir;
}

const CString& Kademlia::AppCommentFilter()
{
	return s_sCommentFilter;
}

void Kademlia::SetPrefs(const KadPrefs &prefs)
{
	s_prefs = prefs;
	s_sConfigDir = OptUtf8ToStr(CStringA(prefs.configDir.c_str()));
	s_sCommentFilter = OptUtf8ToStr(CStringA(prefs.commentFilter.c_str()));
}
