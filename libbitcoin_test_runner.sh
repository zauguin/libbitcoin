#!/bin/sh
###############################################################################
#  Copyright (c) 2014-2015 libbitcoin developers (see COPYING).
#
#         GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
#
###############################################################################

# Define tests and options.
#==============================================================================
BOOST_UNIT_TEST_OPTIONS=\
"--run_test=address_tests,alert_payload_tests,alert_tests,authority_tests,base10_tests,base16_tests,base58_tests,base64_tests,base85_tests,binary_tests,bitcoin_uri_tests,block_tests,btc256_tests,checkpoint_tests,checksum_tests,data_tests,ec_private_tests,ec_public_tests,elliptic_curve_tests,encrypted_tests,endian_tests,endpoint_tests,filter_add_tests,filter_clear_tests,filter_load_tests,get_address_tests,get_blocks_tests,get_data_tests,get_headers_tests,hash_number_tests,hash_tests,hd_private_tests,hd_public_tests,header_tests,heading_tests,input_tests,inventory_tests,inventory_type_id_tests,inventory_vector_tests,memory_pool_tests,merkle_block_tests,message_tests,mnemonic_tests,network_address_tests,not_found_tests,output_tests,parameter_tests,payment_address_tests,ping_tests,point_tests,pong_tests,printer_tests,random_tests,reject_tests,script_number_tests,script_tests,serializer_tests,stealth_address_tests,stealth_tests,stream_tests,thread_tests,transaction_tests,unicode_istream_tests,unicode_ostream_tests,unicode_tests,uri_reader_tests,uri_tests,verack_tests,version_tests "\
"--show_progress=no "\
"--detect_memory_leak=0 "\
"--report_level=no "\
"--build_info=yes"


# Run tests.
#==============================================================================
./test/libbitcoin_test ${BOOST_UNIT_TEST_OPTIONS}
