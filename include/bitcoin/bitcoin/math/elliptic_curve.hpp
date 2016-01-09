/**
/// Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
/// This file is part of libbitcoin.
 *
/// libbitcoin is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Affero General Public License with
/// additional permissions to the one published by the Free Software
/// Foundation, either version 3 of the License, or (at your option)
/// any later version. For more information see LICENSE.
 *
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU Affero General Public License for more details.
 *
/// You should have received a copy of the GNU Affero General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_ELLIPTIC_CURVE_HPP
#define LIBBITCOIN_ELLIPTIC_CURVE_HPP

#include <cstddef>
#include <secp256k1.h>
#include <bitcoin/bitcoin/compat.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/math/hash.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>

namespace libbitcoin {

/// Private key:
BC_CONSTEXPR size_t ec_secret_size = 32;
typedef byte_array<ec_secret_size> ec_secret;

/// Compressed public key:
BC_CONSTEXPR size_t ec_compressed_size = 33;
typedef byte_array<ec_compressed_size> ec_compressed;

/// Uncompressed public key:
BC_CONSTEXPR size_t ec_uncompressed_size = 65;
typedef byte_array<ec_uncompressed_size> ec_uncompressed;

// Parsed ECDSA signature:
BC_CONSTEXPR size_t ec_signature_size = 64;
typedef byte_array<ec_signature_size> ec_signature;

// DER encoded signature:
BC_CONSTEXPR size_t max_der_signature_size = 72;
typedef data_chunk der_signature;

/// DER encoded signature with sighash byte for input endorsement:
BC_CONSTEXPR size_t max_endorsement_size = 73;
typedef data_chunk endorsement;

/// Recoverable ecdsa signature for message signing:
struct BC_API recoverable_signature
{
    ec_signature signature;
    uint8_t recovery_id;
};

BC_CONSTEXPR ec_compressed null_compressed_point =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

BC_CONSTEXPR ec_uncompressed null_uncompressed_point =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Add and multiply EC values
// ----------------------------------------------------------------------------

/// Compute the sum a += G*b, where G is the curve's generator point.
/// return false on failure (such as infinity or zero).
BC_API bool ec_add(ec_compressed& point, const ec_secret& secret);

/// Compute the sum a += G*b, where G is the curve's generator point.
/// return false on failure (such as infinity or zero).
BC_API bool ec_add(ec_uncompressed& point, const ec_secret& secret);

/// Compute the sum a = (a + b) % n, where n is the curve order.
/// return false on failure (such as a zero result).
BC_API bool ec_add(ec_secret& left, const ec_secret& right);

/// Compute the product point *= secret.
/// return false on failure (such as infinity or zero).
BC_API bool ec_multiply(ec_compressed& point, const ec_secret& secret);

/// Compute the product point *= secret.
/// return false on failure (such as infinity or zero).
BC_API bool ec_multiply(ec_uncompressed& point, const ec_secret& secret);

/// Compute the product a = (a * b) % n, where n is the curve order.
/// return false on failure (such as a zero result).
BC_API bool ec_multiply(ec_secret& left, const ec_secret& right);

// Convert keys
// ----------------------------------------------------------------------------

/// Convert an uncompressed public point to compressed.
BC_API bool compress(ec_compressed& out, const ec_uncompressed& point);

/// Convert a compressed public point to decompressed.
BC_API bool decompress(ec_uncompressed& out, const ec_compressed& point);

/// Convert a secret to a compressed public point.
BC_API bool secret_to_public(ec_compressed& out, const ec_secret& secret);

/// Convert a secret parameter to an uncompressed public point.
BC_API bool secret_to_public(ec_uncompressed& out, const ec_secret& secret);

// Verify keys
// ----------------------------------------------------------------------------

/// Verify a secret.
BC_API bool verify(const ec_secret& secret);

/// Verify a point.
BC_API bool verify(const ec_compressed& point);

/// Verify a point.
BC_API bool verify(const ec_uncompressed& point);

/// Fast verification of point structure.
BC_API bool is_point(data_slice point);

// DER parse/sign/verify
// ----------------------------------------------------------------------------

/// Parse a DER encoded signature with optional strict DER enforcement.
/// Treat an empty DER signature as invalid, in accordance with BIP66.
BC_API bool parse_signature(ec_signature& out, 
    const der_signature& der_signature, bool strict);

/// Create a deterministic and strict DER signature using a private key.
/// This function will always produce a valid signature.
BC_API der_signature der_sign(const ec_secret& secret,
    const hash_digest& hash);

/// This overload is exposed to optimize script processing.
/// Verify a DER signature with a point (compressed or uncompressed).
BC_API bool der_verify(const data_chunk& point, const hash_digest& hash,
    const der_signature& der_signature, bool strict);

////// Endorsement verify
////// ----------------------------------------------------------------------------
////
/////// Verify an endorsement with compressed point.
////BC_API bool verify_signature(const ec_compressed& point,
////    const hash_digest& hash, const endorsement& endorsement, bool strict);
////
/////// Verify an endorsement with uncompressed point.
////BC_API bool verify_signature(const ec_uncompressed& point,
////    const hash_digest& hash, const endorsement& endorsement, bool strict);

// EC signature verify
// ----------------------------------------------------------------------------

/// Verify an EC signature using a compressed point.
BC_API bool verify_signature(const ec_compressed& point,
    const hash_digest& hash, const ec_signature& signature);

/// Verify an EC signature using an uncompressed point.
BC_API bool verify_signature(const ec_uncompressed& point,
    const hash_digest& hash, const ec_signature& signature);

// Recoverable sign/recover
// ----------------------------------------------------------------------------

/// Create a recoverable signature for use in message signing.
BC_API bool sign_recoverable(recoverable_signature& out,
    const ec_secret& secret, const hash_digest& hash);

/// Recover the compressed point from a recoverable message signature.
BC_API bool recover_public(ec_compressed& point,
    const recoverable_signature& recoverable, const hash_digest& hash);

/// Recover the uncompressed point from a recoverable message signature.
BC_API bool recover_public(ec_uncompressed& point,
    const recoverable_signature& recoverable, const hash_digest& hash);

} // namespace libbitcoin

#endif
