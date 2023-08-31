#include "upscale_rpc/response/aggregate.hpp"
#include "upscale_rpc/utils.hpp"

namespace upscale_rpc::response::aggregate
{
    struct header
    {
        primary_header basic_header {};
        std::uint8_t padding[3u];
        payload_size_t payload_size {};
    };

    void serialize(const call_data_items_context_pair_vector& items, raw_data_t& buffer)
    {
        upscale_rpc::serialize<header>(items, buffer,
                                       [](std::uint8_t* buffer_data, const std::size_t serialization_buffer_size) noexcept
                                       {
                                           auto header_item = reinterpret_cast<header*>(buffer_data);
                                           header_item->basic_header = primary_header(action_t::response, encoding_scheme_t::aggregate);
                                           header_item->payload_size = serialization_buffer_size;
                                       });
    }

    const call_data_items_context_pair_vector* deserialize(const c_raw_data_t& buffer)
    {
        return upscale_rpc::deserialize<call_data_items_context_pair_vector, header>(buffer, action_t::response,
                                                                                     encoding_scheme_t::aggregate)
            .first;
    }
}
