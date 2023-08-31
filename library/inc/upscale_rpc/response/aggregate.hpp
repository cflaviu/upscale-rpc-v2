// Upscale RPC library
// Copyright (C) 2023 Flaviu C.
#pragma once
#ifndef PCH
    #include <cista/containers/pair.h>
    #include <upscale_rpc/basic_types.hpp>
#endif

namespace upscale_rpc::response::aggregate
{
    using call_data_items_context_pair = cista::offset::pair<call_data::vector, context_id_t>;

    // vector of {context + call data vector}
    using call_data_items_context_pair_vector = cista::offset::vector<call_data_items_context_pair>;

    void serialize(const call_data_items_context_pair_vector& items, raw_data_t& buffer);
    const call_data_items_context_pair_vector* deserialize(const c_raw_data_t& buffer);
}
