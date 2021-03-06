#pragma once

#include <fc/io/enum_type.hpp>
#include <fc/io/raw.hpp>
#include <fc/reflect/reflect.hpp>
#include <fc/reflect/variant.hpp>
#include <bts/blockchain/address.hpp>
#include <bts/blockchain/types.hpp>
#include <bts/blockchain/withdraw_types.hpp>
#include <bts/game/rule_record.hpp>
#include <bts/blockchain/transaction_evaluation_state.hpp>

#include <bts/blockchain/chain_database.hpp>
#include <bts/wallet/wallet.hpp>
#include <bts/wallet/wallet_records.hpp>

/**
 *  The C keyword 'not' is NOT friendly on VC++ but we still want to use
 *  it for readability, so we will have the pre-processor convert it to the
 *  more traditional form.  The goal here is to make the understanding of
 *  the validation logic as english-like as possible.
 */
#define NOT !

#define BTS_BLOCKCHAIN_NUM_DICE                             (BTS_BLOCKCHAIN_NUM_DELEGATES / 10)
#define BTS_BLOCKCHAIN_DICE_RANGE                           (100000000)
#define BTS_BLOCKCHAIN_DICE_HOUSE_EDGE                      (0) // 0% house edge

namespace bts { namespace game {
    using namespace bts::blockchain;
    using namespace bts::wallet;
    
    typedef transaction_id_type                dice_id_type;
    
    struct dice_data
    {
        dice_data(){}
        
        dice_id_type        id = dice_id_type();
        address             owner;
        share_type          amount;
        uint32_t            odds;
        uint32_t            guess;
    };
    
    typedef rule_record< dice_data,                dice_rule_type >  rule_dice_record;
    
    struct dice_rule
    {
        static const uint8_t    type;
        
        dice_rule():amount(0), odds(1){}
        
        dice_rule( const bts::blockchain::address& owner, bts::blockchain::share_type amnt, uint32_t odds = 2, uint32_t g = 1 );
        
        bts::blockchain::address owner()const;
        
        /** owner is just the hash of the condition */
        bts::blockchain::balance_id_type                balance_id()const;
        
        bts::blockchain::share_type          amount;
        uint32_t            odds;
        uint32_t            guess;
        
        /** the condition that the funds may be withdrawn,
         *  this is only necessary if the address is new.
         */
        bts::blockchain::withdraw_condition  condition;
        
        void evaluate( transaction_evaluation_state& eval_state );
        
        bool scan( wallet_transaction_record& trx_rec, bts::wallet::wallet_ptr w );
        
        static bool scan_result( const rule_result_transaction& rtrx,
                                       uint32_t block_num,
                                       const time_point_sec& block_time,
                         const uint32_t trx_index, bts::wallet::wallet_ptr w);
        
        static wallet_transaction_record play( chain_database_ptr blockchain, bts::wallet::wallet_ptr w, const variant& params, bool sign);
        
        static void execute( chain_database_ptr blockchain, uint32_t block_num, const pending_chain_state_ptr& pending_state );
    };
    
    struct dice_input
    {
        dice_input() {}
        dice_input( std::string name, double a, uint32_t o, uint32_t g )
        :from_account_name(name), amount(a), odds(o), guess(g) {}
        
        std::string     from_account_name;
        double          amount;
        uint32_t        odds;
        uint32_t        guess;
    };
    
    struct dice_transaction
    {
        static const uint8_t    type;
        
        dice_transaction(){}
        
        address                                   play_owner;
        address                                   jackpot_owner;
        share_type                                play_amount;
        share_type                                jackpot_received;
        uint32_t                                  odds;
        uint32_t                                  lucky_number;
    };
    
} } // bts::game
FC_REFLECT( bts::game::dice_data, (id)(owner)(amount)(odds)(guess) )

FC_REFLECT( bts::game::dice_rule, (amount)(odds)(guess)(condition) )
FC_REFLECT( bts::game::dice_input, (from_account_name)(amount)(odds)(guess) )
FC_REFLECT( bts::game::dice_transaction, (play_owner)(jackpot_owner)(play_amount)(jackpot_received)(odds)(lucky_number) )
