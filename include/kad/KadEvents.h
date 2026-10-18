// Events published by Kad, and the snapshots that can be asked for by ID.
//
// Contract:
//  - Kad owns every object it creates and is the only one that destroys it.
//    Nothing in this header points into Kad: every payload is a plain value.
//  - Events are delivered synchronously, on the thread that runs Kad, while
//    Kad waits for the call to return. A payload received by reference is
//    valid only for the duration of the call; a consumer that wants to keep
//    it makes its own copy, and that copy is the consumer's.
//  - An event handler must not block and must not call back into Kad to
//    change it (start/stop searches, remove contacts, ...). Reading through
//    the query functions below is allowed.
//  - Small data travels with the event (push). Large data - the lookup
//    history of a search - is asked for by handle (pull), so a consumer
//    that does not need it never pays for it.
//  - Identity: a contact is identified by its KadID, a search by its handle
//    (unique for every search object; the searchID inside KadSearchInfo is
//    the application's own number and is not unique). An event may name a
//    contact or a search the consumer never heard of: ignore it.
#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include <vector>

namespace Kademlia
{
	struct KadID
	{
		uint8_t bytes[16];	// CUInt128::ToByteArray() order
	};

	struct KadState
	{
		bool		running;
		bool		connected;
		bool		firewalled;		// TCP
		bool		firewalledUDP;
		uint32_t	ip;				// host byte order, 0 if unknown
		KadID		id;				// our node ID, zero if not running
		uint32_t	users;			// estimated users in the network
		uint32_t	files;			// estimated files in the network
	};

	struct KadContactInfo
	{
		KadID		id;
		KadID		distance;		// XOR distance from our node ID
		uint32_t	ip;				// host byte order
		uint16_t	udpPort;
		uint16_t	tcpPort;
		uint8_t		type;			// 0 (best) .. 4 (about to expire)
		uint8_t		version;		// Kad protocol version
		time_t		created;
		time_t		expires;
		bool		ipVerified;
		bool		bootstrap;		// listed from nodes.dat, not in the routing table
	};

	struct KadSearchInfo
	{
		uint32_t	handle;
		uint32_t	searchID;		// application search number, UINT32_MAX if none
		uint32_t	type;			// CSearch::NODE .. CSearch::NODEFWCHECKUDP
		std::string	typeName;		// UTF-8
		std::string	name;			// UTF-8, what the search is for
		KadID		target;
		uint32_t	answers;
		uint32_t	packetsSent;
		uint32_t	requestAnswers;
		uint32_t	nodeLoad;
		uint32_t	nodeLoadResponses;
		uint32_t	nodeLoadTotal;
		bool		stopping;
	};

	struct KadLookupEntry
	{
		KadID		contactID;
		KadID		distance;
		std::vector<int> receivedFrom;	// indexes into KadLookupHistory::entries
		uint32_t	askedContactsTick;		// GetTickCount() ms, 0 = never
		uint32_t	respondedContact;
		uint32_t	askedSearchItemTick;	// GetTickCount() ms, 0 = never
		uint32_t	respondedSearchItem;
		uint32_t	ip;
		uint16_t	port;
		uint8_t		contactVersion;
		bool		providedCloser;
		bool		forcedInteresting;
	};

	struct KadLookupHistory
	{
		uint32_t	handle;			// of the search it belongs to
		uint32_t	type;
		std::string	name;			// UTF-8
		bool		stopped;
		std::vector<KadLookupEntry> entries;
	};

	class IKadEvents
	{
	public:
		virtual ~IKadEvents() = default;

		// network state
		virtual void OnStateChanged(const KadState&) {}
		virtual void OnUserCountChanged(const KadState&) {}
		virtual void OnPortMappingRefreshRequested() {}

		// routing table
		virtual void OnContactAdded(const KadContactInfo&) {}
		virtual void OnContactUpdated(const KadContactInfo&) {}
		virtual void OnContactRemoved(const KadID&) {}
		virtual void OnBootstrapListShown() {}	// the contacts that follow come from nodes.dat
		virtual void OnContactsBulkBegin() {}	// many contact events follow
		virtual void OnContactsBulkEnd() {}		// may also come alone: the contacts changed

		// searches
		virtual void OnSearchAdded(const KadSearchInfo&) {}		// sent on creation: type, target and name come with the first update
		virtual void OnSearchUpdated(const KadSearchInfo&) {}
		virtual void OnSearchRemoved(uint32_t handle) {}
		virtual void OnKeywordSearchEnded(uint32_t searchID) {}	// its lifetime expired; no more results
		virtual void OnLookupHistoryChanged(uint32_t handle) {}	// pull it with GetLookupHistory()
	};

	// nullptr (the default) turns events off. Not owned.
	void SetEvents(IKadEvents *pEvents);

	// Queries: run on Kad's thread, return copies.
	KadState GetState();
	bool GetSearchInfo(uint32_t handle, KadSearchInfo &out);
	bool GetLookupHistory(uint32_t handle, KadLookupHistory &out);
}
