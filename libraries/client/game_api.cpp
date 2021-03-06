#include <bts/client/client_impl.hpp>

using namespace bts::client;
using namespace bts::client::detail;

wallet_transaction_record client_impl::game_buy_chips(
        const std::string& from_account_name, const std::string& quantity, const std::string& quantity_symbol )
{
    auto record = _wallet->buy_chips(from_account_name, _chain_db->to_ugly_asset(quantity, quantity_symbol), true);
    _wallet->cache_transaction(record);
    network_broadcast_transaction(record.trx);
    return record;
}

wallet_transaction_record client_impl::game_create(
                                                                  const std::string& game_name,
                                                                  const std::string& owner_name,
                                                                  uint32_t rule_id,
                                                                  const std::string& description,
                                                                  const fc::variant& public_data /* = fc::json::from_string("null").as<fc::variant>() */ )
{
   auto record = _wallet->create_game( game_name, description, public_data, owner_name, rule_id, true );
   _wallet->cache_transaction( record );
   network_broadcast_transaction( record.trx );
   return record;
}

wallet_transaction_record client_impl::game_play(const std::string& asset_symbol, const fc::variant& param )
{
    auto record = _wallet->play_game(asset_symbol, param, true);
    _wallet->cache_transaction(record);
    network_broadcast_transaction(record.trx);
    return record;
}
