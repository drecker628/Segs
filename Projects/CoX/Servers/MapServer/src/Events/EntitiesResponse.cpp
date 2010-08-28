#include <ace/Assert.h>
#include "Entity.h"
#include "Events/EntitiesResponse.h"
#include "MapEvents.h"
#include "MapClient.h"
#include "MapHandler.h"
#include "MapInstance.h"
//! EntitiesResponse is sent to a client to inform it about the current world state.
EntitiesResponse::EntitiesResponse(MapClient *cl, bool inc) :
    MapLinkEvent(MapEventTypes::evEntitites),
    m_incremental(inc)
{
    m_client = cl;
    abs_time=db_time=0;
}
void EntitiesResponse::serializeto( BitStream &tgt ) const
{
    EntityManager &ent_manager=m_client->current_map()->m_entities;
    tgt.StorePackedBits(1,m_incremental ? 2 : 3); // opcode
    tgt.StoreBits(1,entReceiveUpdate);
    sendCommands(tgt);

    tgt.StoreBits(32,abs_time);
    //tgt.StoreBits(32,db_time);

    tgt.StoreBits(1,unkn2);
    if(unkn2==0)
    {
        tgt.StoreBits(1,debug_info);
        tgt.StoreBits(1,selector1);
        if(selector1==1)
        {
            tgt.StoreBits(2,dword_A655C0);
            tgt.StoreBits(2,BinTrees_PPP);
        }
    }

    //else debug_info = false;
    ent_manager.sendEntities(tgt);
    if(debug_info&&!unkn2)
    {
        ent_manager.sendDebuggedEntities(tgt);
        ent_manager.sendGlobalEntDebugInfo(tgt);
    }
    sendServerPhysicsPositions(tgt); // These are not client specific ?
    sendControlState(tgt);// These are not client specific ?
    ent_manager.sendDeletes(tgt);
    // Client specific part
    sendClientData(tgt);
}
void EntitiesResponse::sendClientData(BitStream &tgt) const
{

    if(!m_incremental)
    {
        // initial character update = level/name/class/origin/map_name
        //m_client->char_entity()->m_char.m_ent=m_client->char_entity();
        m_client->char_entity()->m_char.serializeto(tgt);
    }
    else
    {
        m_client->char_entity()->m_char.sendFullStats(tgt);
    }
    storePowerInfoUpdate(tgt);
    //storePowerModeUpdate(tgt);
    //storeBadgeUpdate(tgt);
    //storeGenericinventoryUpdate(tgt);
    //storeInventionUpdate(tgt);
    storeTeamList(tgt);
    storeSuperStats(tgt);
    storeGroupDyn(tgt);
    bool additional=false;
    tgt.StoreBits(1,additional);
    if(additional)
    {
        tgt.StoreFloat(0.0f);
        tgt.StoreFloat(0.0f); // camera_yaw
        tgt.StoreFloat(0.0f);
    }
}
void EntitiesResponse::sendControlState(BitStream &bs) const
{
    sendServerControlState(bs);
}
void EntitiesResponse::sendServerPhysicsPositions(BitStream &bs) const
{
    bs.StoreBits(1,0);
    bs.StoreBits(1,0);
}
void EntitiesResponse::sendServerControlState(BitStream &bs) const
{
    bs.StoreBits(1,0);
    bs.StoreBits(1,0);
}
void EntitiesResponse::storePowerInfoUpdate(BitStream &bs) const
{
    bs.StoreBits(1,0);
    bs.StoreBits(1,0);
}
void EntitiesResponse::storePowerModeUpdate(BitStream &bs) const
{
    bs.StoreBits(1,0);
    if(false)
    {
        bs.StorePackedBits(3,1);
        for(int i=0; i<1; i++)
        {
            bs.StorePackedBits(3,0);
        }
    }
}
void EntitiesResponse::storeBadgeUpdate(BitStream &bs) const
{
    bs.StoreBits(1,0);
    bs.StoreBits(1,0);
}
void EntitiesResponse::storeGenericinventoryUpdate(BitStream &bs)const
{
    bs.StorePackedBits(1,0);
}
void EntitiesResponse::storeInventionUpdate(BitStream &bs)const
{
    bs.StorePackedBits(1,0);
}
void EntitiesResponse::storeTeamList(BitStream &bs) const
{
    //storePackedBitsConditional(bs,20,0);
    bs.StoreBits(1,0);
    bs.StoreBits(1,0);
    bs.StoreBits(1,0);
}
void EntitiesResponse::storeSuperStats(BitStream &bs) const
{
    bs.StorePackedBits(1,0);
}
void EntitiesResponse::storeGroupDyn(BitStream &bs) const
{
    bs.StorePackedBits(1,0);
}

