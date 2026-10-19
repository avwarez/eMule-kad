// Settings Kad takes from the application.
//
// The application owns them: it fills a KadPrefs and hands it to Kad with
// SetPrefs(), on Kad's thread, before starting Kad and again whenever one of
// them changes. Kad keeps its own copy; the one passed in can be discarded
// as soon as the call returns.
#pragma once

#include <cstdint>
#include <string>

namespace Kademlia
{
	struct KadPrefs
	{
		uint16_t	tcpPort;				// where other clients reach us over TCP
		uint16_t	udpPort;				// the Kad UDP port
		uint8_t		userHash[16];			// our client hash
		bool		cryptLayerEnabled;		// protocol obfuscation supported
		uint32_t	kadUDPKey;				// secret behind the UDP verify keys
		std::string	configDir;				// UTF-8, ends with a path separator: nodes.dat, preferencesKad.dat, *_index.dat
		bool		filterLANIPs;			// ignore contacts with LAN addresses
		std::string	commentFilter;			// UTF-8, lower case words separated by '|'

		// logging
		bool		logFilteredIPs;
		bool		logBannedClients;
		int			debugClientKadUDPLevel;
		int			debugClientTCPLevel;
		int			debugServerUDPLevel;
		int			debugServerSearchesLevel;
	};

	void SetPrefs(const KadPrefs &prefs);
}
