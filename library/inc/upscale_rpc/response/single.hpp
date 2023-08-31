// Upscale RPC library
// Copyright (C) 2023 Flaviu C.
#pragma once
#ifndef PCH
    #include <upscale_rpc/request/single.hpp>
#endif

namespace upscale_rpc::response::single
{
    void serialize(const context_id_t context_id, const call_data& call, raw_data_t& buffer);

    std::pair<const call_data*, context_id_t> deserialize(const c_raw_data_t& buffer);
}
