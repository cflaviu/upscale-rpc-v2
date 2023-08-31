// Upscale RPC library
// Copyright (C) 2023 Flaviu C.
#pragma once
#ifndef PCH
    #include <cista/serialization.h>
    #include <cstring>
    #include <span>
    #include <stdexcept>
    #include <upscale_rpc/basic_types.hpp>
#endif

namespace upscale_rpc
{
    template <typename _Header>
    void serialize(const auto& input, raw_data_t& buffer, auto header_serializer)
    {
        const auto serialization_buffer = cista::serialize(input);
        const auto serialization_buffer_size = serialization_buffer.size();

        const auto storage_size = sizeof(_Header) + serialization_buffer_size;
        if (storage_size > buffer.size())
            throw std::runtime_error("buffer too small");

        buffer = raw_data_t(buffer.data(), storage_size);
        header_serializer(buffer.data(), serialization_buffer_size);
        std::memcpy(buffer.data() + sizeof(_Header), serialization_buffer.data(), serialization_buffer_size);
    }

    template <typename _Output, typename _Header>
    std::pair<const _Output*, const _Header*> deserialize(const c_raw_data_t buffer, const action_t expected_action,
                                                          const encoding_scheme_t expected_encoding_scheme)
    {
        constexpr auto min_storage_size = sizeof(_Header) + 4u;
        if (buffer.size() < min_storage_size)
            throw std::runtime_error("buffer too small");

        auto header_item = reinterpret_cast<const _Header*>(buffer.data());
        if (header_item->payload_size != (buffer.size() - sizeof(_Header)))
            throw std::runtime_error("wrong buffer size");

        {
            const auto& primary_header = header_item->basic_header;
            if (primary_header.action() != expected_action)
                throw std::runtime_error("wrong action");

            if (primary_header.encoding_scheme() != expected_encoding_scheme)
                throw std::runtime_error("wrong encoding scheme");
        }

        const auto buffer_start = buffer.data() + sizeof(_Header);
        const auto buffer_end = buffer_start + header_item->payload_size;
        return {cista::deserialize<_Output, cista::mode::NONE, std::uint8_t>(buffer_start, buffer_end), header_item};
    }
}
