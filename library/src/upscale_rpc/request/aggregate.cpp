#include "upscale_rpc/request/aggregate.hpp"
#include "upscale_rpc/utils.hpp"

namespace upscale_rpc::request::aggregate
{
    using header = common_header;

    void serialize(const context_id_t context_id, const call_data::vector& calls, raw_data_t& buffer)
    {
        upscale_rpc::serialize<header>(calls, buffer,
                                       [context_id](std::uint8_t* buffer_data, const std::size_t serialization_buffer_size) noexcept
                                       {
                                           auto header_item = reinterpret_cast<header*>(buffer_data);
                                           header_item->basic_header = primary_header(action_t::request, encoding_scheme_t::aggregate);
                                           header_item->context_id = context_id;
                                           header_item->payload_size = serialization_buffer_size;
                                       });
    }

    std::pair<const call_data::vector*, context_id_t> deserialize(const c_raw_data_t& buffer)
    {
        auto result = upscale_rpc::deserialize<call_data::vector, header>(buffer, action_t::request, encoding_scheme_t::aggregate);
        return {result.first, result.second->context_id};
    }
}
