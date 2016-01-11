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
#ifndef LIBBITCOIN_SUBSCRIBER_IPP
#define LIBBITCOIN_SUBSCRIBER_IPP

#include <functional>
#include <memory>
#include <mutex>
#include <bitcoin/bitcoin/utility/assert.hpp>
#include <bitcoin/bitcoin/utility/dispatcher.hpp>
#include <bitcoin/bitcoin/utility/threadpool.hpp>

namespace libbitcoin {

template <typename... Args>
subscriber<Args...>::subscriber(threadpool& pool,
    const std::string& class_name)
  : stopped_(false),
    dispatch_(pool, class_name)
    /*, track<subscriber<Args...>>(class_name)*/
{
}

template <typename... Args>
subscriber<Args...>::~subscriber()
{
    BITCOIN_ASSERT_MSG(stopped_, "subscriber not stopped");
    BITCOIN_ASSERT_MSG(subscriptions_.empty(), "subscriber not cleared");
}

template <typename... Args>
void subscriber<Args...>::stop()
{
    // Critical Section
    ///////////////////////////////////////////////////////////////////////////
    std::lock_guard<std::mutex> lock(mutex_);

    stopped_ = true;
    ///////////////////////////////////////////////////////////////////////////
}

template <typename... Args>
void subscriber<Args...>::subscribe(handler notifier)
{
    // Critical Section
    ///////////////////////////////////////////////////////////////////////////
    std::lock_guard<std::mutex> lock(mutex_);

    if (!stopped_)
        dispatch_.ordered(&subscriber<Args...>::do_subscribe,
            this->shared_from_this(), notifier);
    ///////////////////////////////////////////////////////////////////////////
}

template <typename... Args>
void subscriber<Args...>::do_subscribe(handler notifier)
{
    subscriptions_.push_back(notifier);
}

template <typename... Args>
void subscriber<Args...>::relay(Args... args)
{
    dispatch_.ordered(&subscriber<Args...>::do_relay,
        this->shared_from_this(), args...);
}

template <typename... Args>
void subscriber<Args...>::do_relay(Args... args)
{
    for (const auto notifier: subscriptions_)
        notifier(args...);

    subscriptions_.clear();
}

} // namespace libbitcoin

#endif
