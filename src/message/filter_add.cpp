/*
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
#include <bitcoin/bitcoin/message/filter_add.hpp>

#include <boost/iostreams/stream.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/utility/assert.hpp>
#include <bitcoin/bitcoin/utility/container_sink.hpp>
#include <bitcoin/bitcoin/utility/container_source.hpp>
#include <bitcoin/bitcoin/utility/istream_reader.hpp>
#include <bitcoin/bitcoin/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

const std::string message::filter_add::command = "filteradd";

filter_add filter_add::factory_from_data(const data_chunk& data)
{
    filter_add instance;
    instance.from_data(data);
    return instance;
}

filter_add filter_add::factory_from_data(std::istream& stream)
{
    filter_add instance;
    instance.from_data(stream);
    return instance;
}

filter_add filter_add::factory_from_data(reader& source)
{
    filter_add instance;
    instance.from_data(source);
    return instance;
}

bool filter_add::is_valid() const
{
    return !data.empty();
}

void filter_add::reset()
{
    data.clear();
}

bool filter_add::from_data(const data_chunk& data)
{
    boost::iostreams::stream<byte_source<data_chunk>> istream(data);
    return from_data(istream);
}

bool filter_add::from_data(std::istream& stream)
{
    istream_reader source(stream);
    return from_data(source);
}

bool filter_add::from_data(reader& source)
{
    reset();

    auto size = source.read_variable_uint_little_endian();
    BITCOIN_ASSERT(size <= bc::max_size_t);
    const auto data_size = static_cast<size_t>(size);
    bool result = static_cast<bool>(source);

    if (result)
    {
        data = source.read_data(data_size);
        result = source && (data.size() == data_size);
    }

    if (!result)
        reset();

    return result;
}

data_chunk filter_add::to_data() const
{
    data_chunk data;
    boost::iostreams::stream<byte_sink<data_chunk>> ostream(data);
    to_data(ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == serialized_size());
    return data;
}

void filter_add::to_data(std::ostream& stream) const
{
    ostream_writer sink(stream);
    to_data(sink);
}

void filter_add::to_data(writer& sink) const
{
    sink.write_variable_uint_little_endian(data.size());
    sink.write_data(data);
}

uint64_t filter_add::serialized_size() const
{
    return variable_uint_size(data.size()) + data.size();
}

bool operator==(const filter_add& left,
    const filter_add& right)
{
    bool result = (left.data.size() == right.data.size());

    for (data_chunk::size_type i = 0; i < left.data.size() && result; i++)
        result = (left.data[i] == right.data[i]);

    return result;
}

bool operator!=(const filter_add& left,
    const filter_add& right)
{
    return !(left == right);
}

} // end message
} // end libbitcoin
