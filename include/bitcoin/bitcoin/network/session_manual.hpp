/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_NETWORK_SESSION_MANUAL_HPP
#define LIBBITCOIN_NETWORK_SESSION_MANUAL_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/error.hpp>
#include <bitcoin/bitcoin/message/network_address.hpp>
#include <bitcoin/bitcoin/network/channel.hpp>
#include <bitcoin/bitcoin/network/connector.hpp>
#include <bitcoin/bitcoin/network/network_settings.hpp>
#include <bitcoin/bitcoin/network/session.hpp>
#include <bitcoin/bitcoin/utility/threadpool.hpp>

namespace libbitcoin {
namespace network {
    
class p2p;

class BC_API session_manual
  : public session, track<session_manual>
{
public:
    typedef std::shared_ptr<session_manual> ptr;
    typedef std::function<void(const code&, channel::ptr)> channel_handler;

    session_manual(threadpool& pool, p2p& network, const settings& settings);

    void start() override;

    void connect(const std::string& hostname, uint16_t port);
    void connect(const std::string& hostname, uint16_t port,
        channel_handler handler);

private:
    void start_connect(const std::string& hostname, uint16_t port,
        channel_handler handler, uint16_t retries);
    void handle_connect(const code& ec, channel::ptr channel,
        const std::string& hostname, uint16_t port,
        channel_handler handler, uint16_t retries);

    void handle_channel_start(const code& ec, const std::string& hostname,
        uint16_t port, channel::ptr channel, channel_handler handler);
    void handle_channel_stop(const code& ec, const std::string& hostname,
        uint16_t port);

    connector::ptr connect_;
};

} // namespace network
} // namespace libbitcoin

#endif
