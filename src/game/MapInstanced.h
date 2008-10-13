/*
 * Copyright (C) 2005-2008 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef MANGOS_MAP_INSTANCED_H
#define MANGOS_MAP_INSTANCED_H

#include "Map.h"
#include "InstanceSaveMgr.h"

class MANGOS_DLL_DECL MapInstanced : public Map
{
    friend class MapManager;
    public:
        typedef HM_NAMESPACE::hash_map< uint32, Map* > InstancedMaps;

        MapInstanced(uint32 id, time_t, uint32 aInstanceId);
        ~MapInstanced() {}

        // functions overwrite Map versions
        void Update(const uint32&);
        void MoveAllCreaturesInMoveList();
        void RemoveAllObjectsInRemoveList();
        bool RemoveBones(uint64 guid, float x, float y);
        void UnloadAll(bool pForce);

        Map* GetInstance(const WorldObject* obj);
        Map* FindMap(uint32 InstanceId) { return _FindMap(InstanceId); }
        void DestroyInstance(uint32 InstanceId);
        void DestroyInstance(InstancedMaps::iterator &itr);
        void AddGridMapReference(const GridPair &p) { ++GridMapReference[p.x_coord][p.y_coord]; }
        void RemoveGridMapReference(const GridPair &p)
        {
            --GridMapReference[p.x_coord][p.y_coord];
            if (!GridMapReference[p.x_coord][p.y_coord]) { SetUnloadFlag(GridPair(63-p.x_coord,63-p.y_coord), true); }
        }

        InstancedMaps &GetInstancedMaps() { return m_InstancedMaps; }

    private:

        InstanceMap* CreateInstance(uint32 InstanceId, InstanceSave *save, uint8 difficulty);
        BattleGroundMap* CreateBattleGround(uint32 InstanceId);

        InstancedMaps m_InstancedMaps;

        Map* _FindMap(uint32 InstanceId)
        {
            InstancedMaps::iterator i = m_InstancedMaps.find(InstanceId);

            return(i == m_InstancedMaps.end() ? NULL : i->second);
        }

        uint16 GridMapReference[MAX_NUMBER_OF_GRIDS][MAX_NUMBER_OF_GRIDS];
};
#endif