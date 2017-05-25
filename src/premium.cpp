#include "Define.h"
#include "GossipDef.h"
#include "Item.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"

enum vendor
{
 NPC_VENDOR = 54, 
 NPC_AUCTION = 9856, //Horde
 NPC_AUCTION2 = 8670, //Alliance
 ALLIANCE_MOUNT = 470,
 HORDE_MOUNT = 6653
};

class premium_account : public ItemScript
{
public:
    premium_account() : ItemScript("premium_account") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override // Any hook here
    {
		if (!sConfigMgr->GetBoolDefault("PremiumAccount", true)) 
            return false;
		
        QueryResult result = CharacterDatabase.PQuery("SELECT AccountId FROM premium WHERE active = 1 and AccountId = %u", player->GetSession()->GetAccountId());

        if (!result)
            return false;
        
        if (player->IsInCombat())
            return false;

        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Morph", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Demorph", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Show Bank", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Mail Box", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Vendor", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT_16, "Mount", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Auction House", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
        return false; // Cast the spell on use normally
    }

    void OnGossipSelect(Player* player, Item* /*item*/, uint32 /*sender*/, uint32 action) override
    {

        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF + 1: /*Morph*/
        {
            player->CLOSE_GOSSIP_MENU();
            uint32 random = (urand(1, 26)); /* change this line when adding more morphs */
            {
                switch (random)
                {
                case 1: player->SetDisplayId(10134); break;     // Troll Female                 'Orb of Deception'
                case 2: player->SetDisplayId(10135); break;     // Troll Male                   'Orb of Deception'
                case 3: player->SetDisplayId(10136); break;     // Tauren Male                  'Orb of Deception'
                case 4: player->SetDisplayId(10137); break;     // Human Male                   'Orb of Deception'
                case 5: player->SetDisplayId(10138); break;     // Human Female                 'Orb of Deception'
                case 6: player->SetDisplayId(10139); break;     // Orc Male                     'Orb of Deception'
                case 7: player->SetDisplayId(10140); break;     // Orc Female                   'Orb of Deception' 
                case 8: player->SetDisplayId(10141); break;     // Dwarf Male                   'Orb of Deception'
                case 9: player->SetDisplayId(10142); break;     // Dwarf Female                 'Orb of Deception' 
                case 10: player->SetDisplayId(10143); break;    // NightElf Male                'Orb of Deception'
                case 11: player->SetDisplayId(10144); break;    // NightElf Female              'Orb of Deception'
                case 12: player->SetDisplayId(10145); break;    // Undead Female                'Orb of Deception'
                case 13: player->SetDisplayId(10146); break;    // Undead Male                  'Orb of Deception'
                case 14: player->SetDisplayId(10147); break;    // Tauren Female                'Orb of Deception'
                case 15: player->SetDisplayId(10148); break;    // Gnome Male                   'Orb of Deception'
                case 16: player->SetDisplayId(10149); break;    // Gnome Female                 'Orb of Deception'
                case 17: player->SetDisplayId(4527); break;     // Thrall                       'Orgrimmar Boss'
                case 18: player->SetDisplayId(11657); break;    // Lady Sylvanas                'Undercity Boss'
                case 19: player->SetDisplayId(4307); break;     // Cairne Bloodhoof             'Thunderbluff Boss'
                case 20: player->SetDisplayId(17122); break;    // Lor´themar Theron            'Silvermoon City Boss'
                case 21: player->SetDisplayId(3597); break;     // King Magni Bronzebeard       'Ironforge Boss'
                case 22: player->SetDisplayId(5566); break;     // Highlord Bolvar Fordragon    'Stormwind Boss'
                case 23: player->SetDisplayId(7006); break;     // High Tinker Mekkatorque      'Gnomer Boss'
                case 24: player->SetDisplayId(7274); break;     // Tyrande Whisperwind          'Darnassus Boss'
                case 25: player->SetDisplayId(21976); break;    // Arthus Small                 'Arthus'
                case 26: player->SetDisplayId(24641); break;    // Arthus Ghost                 'Arthus Ghost'

                default:
                    break;
                }
            }
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 2: /*Demorph*/
            player->DeMorph();
            player->CLOSE_GOSSIP_MENU();
            break;
        case GOSSIP_ACTION_INFO_DEF + 3: /*Show Bank*/
            player->GetSession()->SendShowBank(player->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4: /*Mail Box*/
            player->GetSession()->SendShowMailBox(player->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5: /*Vendor*/
        {
            Creature* vendor = player->SummonCreature(NPC_VENDOR, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            vendor->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            vendor->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, player->GetFollowAngle());
            vendor->setFaction(player->getFaction());
            vendor->MonsterWhisper("At your service", player, false);
            player->CLOSE_GOSSIP_MENU();
        }
        break;
        case GOSSIP_ACTION_INFO_DEF + 6: /*Mount*/
            player->CLOSE_GOSSIP_MENU();
            if (player->GetTeamId() == TEAM_HORDE)
                player->CastSpell(player, HORDE_MOUNT);
            else
                player->CastSpell(player, ALLIANCE_MOUNT);
            break;
        case GOSSIP_ACTION_INFO_DEF + 7: /*Auction House*/
            
            if (player->GetTeamId() == TEAM_HORDE)
            {
                Creature* auction = player->SummonCreature(NPC_AUCTION, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                auction->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                auction->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, player->GetFollowAngle());
                auction->setFaction(player->getFaction());
                auction->MonsterWhisper("I will go shortly, i need to get back to Orgrimmar", player, false);
            }
            else
            {
                Creature* auction = player->SummonCreature(NPC_AUCTION2, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                auction->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                auction->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, player->GetFollowAngle());
                auction->setFaction(player->getFaction());
                auction->MonsterWhisper("I will go shortly, i need to get back to Stormwind City", player, false);
            }
            player->CLOSE_GOSSIP_MENU();
            
            break;
        }
        player->PlayerTalkClass->ClearMenus();
    }
};

void AddSC_premium_account() 
{
    new premium_account();
}