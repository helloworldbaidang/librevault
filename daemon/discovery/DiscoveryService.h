/* Copyright (C) 2016 Alexander Shishenko <alex@shishenko.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#pragma once
#include <boost/signals2/signal.hpp>
#include <util/parse_url.h>
#include <util/network.h>
#include <util/blob.h>

namespace librevault {

class FolderGroup;
class Client;

class StaticDiscovery;
class MulticastDiscovery;
class BTTrackerDiscovery;
class MLDHTDiscovery;

class NodeKey;
class PortMappingService;

class DiscoveryService {
	friend class ControlServer;
public:
	struct ConnectCredentials {
		std::string source;

		librevault::url url;
		tcp_endpoint endpoint;
		blob pubkey;
	};

	DiscoveryService(Client& client, NodeKey& node_key, PortMappingService& port_mapping);
	virtual ~DiscoveryService();

	void register_group(std::shared_ptr<FolderGroup> group_ptr);
	void unregister_group(std::shared_ptr<FolderGroup> group_ptr);

	boost::signals2::signal<void(ConnectCredentials, std::shared_ptr<FolderGroup>)> discovered_node_signal;

protected:
	Client& client_;

	std::unique_ptr<StaticDiscovery> static_discovery_;
	std::unique_ptr<MulticastDiscovery> multicast4_, multicast6_;
	std::unique_ptr<BTTrackerDiscovery> bttracker_;
	std::unique_ptr<MLDHTDiscovery> mldht_;
};

} /* namespace librevault */