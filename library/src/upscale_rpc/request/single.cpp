#include "upscale_rpc/request/single.hpp"
#include "upscale_rpc/utils.hpp"

namespace upscale_rpc::request::single
{
    using header = common_header;

    void serialize(const context_id_t context_id, const call_data& call, raw_data_t& buffer)
    {
        upscale_rpc::serialize<header>(call, buffer,
                                       [context_id](std::uint8_t* buffer_data, const std::size_t serialization_buffer_size) noexcept
                                       {
                                           auto header_item = reinterpret_cast<header*>(buffer_data);
                                           header_item->basic_header = primary_header(action_t::request, encoding_scheme_t::single);
                                           header_item->context_id = context_id;
                                           header_item->payload_size = serialization_buffer_size;
                                       });
    }

    std::pair<const call_data*, context_id_t> deserialize(const c_raw_data_t& buffer)
    {
        auto result = upscale_rpc::deserialize<call_data, header>(buffer, action_t::request, encoding_scheme_t::single);
        return {result.first, result.second->context_id};
    }
}
