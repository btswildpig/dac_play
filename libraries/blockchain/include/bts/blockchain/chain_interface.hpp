#pragma once

#include <bts/blockchain/account_record.hpp>
#include <bts/blockchain/asset_record.hpp>
#include <bts/blockchain/balance_record.hpp>
#include <bts/blockchain/block_record.hpp>
#include <bts/blockchain/burn_record.hpp>
#include <bts/blockchain/ad_record.hpp>
#include <bts/blockchain/note_record.hpp>
#include <bts/blockchain/operation_reward_record.hpp>
#include <bts/blockchain/condition.hpp>
#include <bts/blockchain/feed_record.hpp>
#include <bts/blockchain/market_records.hpp>
#include <bts/blockchain/game_record.hpp>
#include <bts/blockchain/property_record.hpp>
#include <bts/blockchain/slate_record.hpp>
#include <bts/blockchain/slot_record.hpp>
#include <bts/blockchain/transaction_record.hpp>
#include <bts/blockchain/types.hpp>
#include <bts/blockchain/withdraw_types.hpp>

namespace bts { namespace blockchain {

   class chain_interface
   : public property_db_interface,
     public account_db_interface,
     public asset_db_interface,
     public slate_db_interface,
     public balance_db_interface,
     public transaction_db_interface,
     public burn_db_interface,
     public ad_db_interface,
     public note_db_interface,
     public operation_reward_db_interface,
     public feed_db_interface,
     public slot_db_interface,
     public game_db_interface
   {
      public:
         virtual ~chain_interface(){};

         virtual fc::time_point_sec now()const = 0;

         optional<string>                   get_parent_account_name( const string& account_name )const;
         static bool                        is_valid_account_name( const string& name );
         bool                               is_valid_symbol_name( const string& symbol )const;

         time_point_sec                     get_genesis_timestamp()const;

         share_type                         get_max_delegate_pay_issued_per_block()const;
         share_type                         get_delegate_registration_fee( uint8_t pay_rate )const;
         share_type                         get_asset_registration_fee( uint8_t symbol_length )const;
         share_type                         get_account_registration_fee( uint8_t name_length )const;
         share_type                         get_game_registration_fee( uint8_t name_length )const;

         vector<account_id_type>            get_active_delegates()const;
         void                               set_active_delegates( const std::vector<account_id_type>& active_delegates );
         bool                               is_active_delegate( const account_id_type id )const;

         /** converts an asset + asset_id to a more friendly representation using the symbol name */
         string                             to_pretty_asset( const asset& a )const;
         string                             to_pretty_price( const price& a, const bool include_units = true )const;
         /** converts a numeric string + asset symbol to an asset */
         asset                              to_ugly_asset( const string& amount, const string& symbol )const;
         /** converts a numeric string and two asset symbols to a price */
         price                              to_ugly_price( const string& price_string,
                                                           const string& base_symbol,
                                                           const string& quote_symbol,
                                                           bool do_precision_dance = true )const;

         void                               set_chain_id( const digest_type& id );
         digest_type                        get_chain_id()const;

         void                               set_statistics_enabled( const bool enabled );
         bool                               get_statistics_enabled()const;

         virtual oprice                     get_active_feed_price( const asset_id_type quote_id )const = 0;

         virtual void                       set_market_dirty( const asset_id_type quote_id,
                                                              const asset_id_type base_id )                = 0;

         fc::ripemd160                      get_current_random_seed()const;

         virtual void                       store_rule_data_record( const rule_id_type& rule_id, const data_id_type& data_id, const rule_data_record& r )               = 0;

         virtual omarket_status             get_market_status( const asset_id_type quote_id,
                                                               const asset_id_type base_id )const          = 0;
         virtual void                       store_market_status( const market_status& s )                   = 0;

         virtual orule_data_record          get_rule_data_record( const rule_id_type& rule_id, const data_id_type& data_id )const    = 0;

         virtual omarket_order              get_lowest_ask_record( const asset_id_type quote_id,
                                                                   const asset_id_type base_id )           = 0;
         virtual oorder_record              get_bid_record( const market_index_key& )const                  = 0;
         virtual oorder_record              get_ask_record( const market_index_key& )const                  = 0;

         virtual void                       store_bid_record( const market_index_key& key,
                                                              const order_record& )                         = 0;

         virtual void                       store_ask_record( const market_index_key& key,
                                                              const order_record& )                         = 0;


         virtual bool                       is_known_transaction( const transaction& trx )const             = 0;

         virtual otransaction_record        get_transaction( const transaction_id_type& trx_id,
                                                             bool exact = true )const                       = 0;

         virtual void                       store_transaction( const transaction_id_type&,
                                                                const transaction_record&  )                = 0;

         game_id_type                       last_game_id()const;
         game_id_type                       new_game_id();

         asset_id_type                      last_asset_id()const;
         asset_id_type                      new_asset_id();

         account_id_type                    last_account_id()const;
         account_id_type                    new_account_id();

         virtual uint32_t                   get_head_block_num()const                                       = 0;

         virtual void                       store_market_history_record( const market_history_key& key,
                                                                         const market_history_record& record ) = 0;
         virtual omarket_history_record     get_market_history_record( const market_history_key& key )const = 0;

         void set_dirty_markets( const std::set<std::pair<asset_id_type, asset_id_type>>& );
         std::set<std::pair<asset_id_type, asset_id_type>> get_dirty_markets()const;

         virtual void                       set_rule_result_transactions( vector<rule_result_transaction> trxs ) = 0;
         virtual void                       set_market_transactions( vector<market_transaction> trxs )      = 0;
         virtual void                       set_operation_reward_transactions( vector<operation_reward_transaction> trxs )      = 0;

         oproperty_record                   get_property_record( const property_id_type id )const;
         void                               store_property_record( const property_id_type id, const variant& value);

         oaccount_record                    get_account_record( const account_id_type id )const;
         oaccount_record                    get_account_record( const string& name )const;
         oaccount_record                    get_account_record( const address& addr )const;
         void                               store_account_record( const account_record& record );

         oasset_record                      get_asset_record( const asset_id_type id )const;
         oasset_record                      get_asset_record( const string& symbol )const;
         void                               store_asset_record( const asset_record& record );
       
         ogame_record                       get_game_record( const game_id_type id )const;
         ogame_record                       get_game_record( const std::string& name )const;
         void                               store_game_record( const game_record& record );

         oslate_record                      get_slate_record( const slate_id_type id )const;
         void                               store_slate_record( const slate_record& record );

         obalance_record                    get_balance_record( const balance_id_type& id )const;
         void                               store_balance_record( const balance_record& record );

         oburn_record                       get_burn_record( const burn_index& index )const;
         void                               store_burn_record( const burn_record& record );
       
       oad_record                           get_ad_record( const ad_index& index )const;
       void                                 store_ad_record( const ad_record& record );
       
       onote_record                       get_note_record( const note_index& index )const;
       void                               store_note_record( const note_record& record );
       
       ooperation_reward_record           get_operation_reward_record( const operation_id_type& type )const;
       void                               store_operation_reward_record( const operation_reward_record& record );

         ofeed_record                       get_feed_record( const feed_index index )const;
         void                               store_feed_record( const feed_record& record );

         oslot_record                       get_slot_record( const slot_index index )const;
         oslot_record                       get_slot_record( const time_point_sec timestamp )const;
         void                               store_slot_record( const slot_record& record );

         template<typename T, typename U>
         optional<T> lookup( const U& key )const
         { try {
             return T::lookup( *this, key );
         } FC_CAPTURE_AND_RETHROW( (key) ) }

         template<typename T, typename U>
         void store( const U& key, const T& record )
         { try {
#ifdef BTS_TEST_NETWORK
             record.sanity_check( *this );
#endif
             T::store( *this, key, record );
         } FC_CAPTURE_AND_RETHROW( (key)(record) ) }

         template<typename T, typename U>
         void remove( const U& key )
         { try {
             T::remove( *this, key );
         } FC_CAPTURE_AND_RETHROW( (key) ) }
   };
   typedef std::shared_ptr<chain_interface> chain_interface_ptr;

} } // bts::blockchain
