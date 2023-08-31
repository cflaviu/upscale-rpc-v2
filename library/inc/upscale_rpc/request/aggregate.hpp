// Upscale RPC library
// Copyright (C) 2023 Flaviu C.
#pragma once
#ifndef PCH
    #include <upscale_rpc/basic_types.hpp>
#endif

namespace upscale_rpc::request::aggregate
{
    void serialize(const context_id_t context_id, const call_data::vector& calls, raw_data_t& buffer);

    std::pair<const call_data::vector*, context_id_t> deserialize(const c_raw_data_t& buffer);
}
