﻿#include "onemap.h"
#include <algorithm>
OneMap::OneMap():
    max_id(0)
{

}

OneMap::~OneMap()
{
    for(auto f:all_element)delete f;
}

void OneMap::clear()
{
    for(auto f:all_element)delete f;
    all_element.clear();
    max_id = 0;
}

void OneMap::sort()
{
    all_element.sort(PtrComp<MapSpirit>);
}

void OneMap::addSpirit(MapSpirit *spirit)
{
    if(spirit!=nullptr)
        all_element.push_back(spirit);
}

void OneMap::removeSpirit(MapSpirit *spirit)
{
    all_element.remove(spirit);
}

void OneMap::removeSpiritById(int id)
{
    for(auto itr = all_element.begin();itr!=all_element.end();){
        if((*itr)->getId() == id){
            itr = all_element.erase(itr);
        }else{
            itr++;
        }
    }
}

int OneMap::getNextId()
{
    return ++max_id;
}

//复制整个地图
OneMap *OneMap::clone()
{
    OneMap *onemap = new OneMap;
    onemap->setMaxId(max_id);
    for (auto e : all_element) {
        onemap->addSpirit(e->clone());
    }
    return onemap;
}

MapSpirit *OneMap::getSpiritById(int id)
{
    for (auto p : all_element) {
        if(p->getId() == id)return p;
    }
    return nullptr;
}

MapPoint *OneMap::getPointById(int id)
{
    if(all_element.size()<=0)return nullptr;
    for (auto p : all_element) {
        if (p->getId() == id && p->getSpiritType() == MapSpirit::Map_Sprite_Type_Point) {
            return static_cast<MapPoint *>(p);
        }
    }
    return nullptr;
}
MapPath *OneMap::getPathById(int id)
{
    for (auto p : all_element) {
        if (p->getId() == id && p->getSpiritType() == MapSpirit::Map_Sprite_Type_Path) {
            return static_cast<MapPath *>(p);
        }
    }
    return nullptr;
}
MapPath *OneMap::getPathByStartEnd(int start,int end)
{
    for (auto p : all_element) {
        if (p->getSpiritType() == MapSpirit::Map_Sprite_Type_Path) {
            auto pp = static_cast<MapPath *>(p);
            if(pp->getStart() == start && pp->getEnd() == end){
                return pp;
            }
        }
    }
    return nullptr;
}

MapFloor *OneMap::getFloorById(int id)
{
    for (auto p : all_element) {
        if (p->getId() == id && p->getSpiritType() == MapSpirit::Map_Sprite_Type_Floor) {
            return static_cast<MapFloor *>(p);
        }
    }
    return nullptr;
}
MapBackground *OneMap::getBackgroundById(int id)
{
    for (auto p : all_element) {
        if (p->getId() == id && p->getSpiritType() == MapSpirit::Map_Sprite_Type_Background) {
            return static_cast<MapBackground *>(p);
        }
    }
    return nullptr;
}
MapBlock *OneMap::getBlockById(int id)
{
    for (auto p : all_element) {
        if (p->getId() == id && p->getSpiritType() == MapSpirit::Map_Sprite_Type_Block) {
            return static_cast<MapBlock *>(p);
        }
    }
    return nullptr;
}

MapGroup *OneMap::getGroupById(int id)
{
    for (auto p : all_element) {
        if (p->getId() == id && p->getSpiritType() == MapSpirit::Map_Sprite_Type_Group) {
            return static_cast<MapGroup *>(p);
        }
    }
    return nullptr;
}

MapConflictPair *OneMap::getConflictPairById(int id)
{
    for (auto p : all_element) {
        if (p->getId() == id && p->getSpiritType() == MapSpirit::Map_Sprite_Type_Conflict) {
            return static_cast<MapConflictPair *>(p);
        }
    }
    return nullptr;
}



std::vector<int> OneMap::getStations()
{
    std::vector<int> points;
    for(auto s:all_element)
    {
        if(s->getSpiritType() == MapSpirit::Map_Sprite_Type_Point)points.push_back(s->getId());
    }
    return points;
}

std::list<MapFloor *> OneMap::getFloors()
{
    std::list<MapFloor *> floors;
    for(auto s:all_element)
    {
        if(s->getSpiritType() == MapSpirit::Map_Sprite_Type_Floor)floors.push_back(static_cast<MapFloor *>(s));
    }
    return floors;
}

std::list<MapSpirit *> OneMap::getPointsAndPaths()
{
    std::list<MapSpirit *> es;
    for(auto s:all_element)
    {
        if(s->getSpiritType() == MapSpirit::Map_Sprite_Type_Point ||s->getSpiritType() == MapSpirit::Map_Sprite_Type_Path)es.push_back(s);
    }
    return es;
}

std::list<MapPath *> OneMap::getPaths()
{
    std::list<MapPath *> paths;
    for(auto s:all_element)
    {
        if(s->getSpiritType() == MapSpirit::Map_Sprite_Type_Path)paths.push_back(static_cast<MapPath *>(s));
    }
    return paths;
}

std::list<MapPath *> OneMap::getRootPaths()
{
    std::list<MapPath *> all_paths = getPaths();
    std::list<MapFloor *> floors = getFloors();//楼层
    for(auto itr=all_paths.begin();itr!=all_paths.end();){
        //是否楼层上的路径
        bool isFloorPath = false;
        for(auto f:floors){
            if(isFloorPath)break;
            std::list<int> fps = f->getPaths();
            for(auto fp:fps){
                if((*itr)->getId() == fp){
                    isFloorPath = true;
                    break;
                }
            }
        }
        if(isFloorPath){
            itr = all_paths.erase(itr);
        }else{
            itr++;
        }
    }
    return all_paths;
}

std::list<MapBlock *> OneMap::getBlocks()
{
    std::list<MapBlock *> blocks;
    for(auto s:all_element)
    {
        if(s->getSpiritType() == MapSpirit::Map_Sprite_Type_Block)
        {
            blocks.push_back(static_cast<MapBlock *>(s));
        }
    }
    return blocks;
}

std::list<MapGroup *> OneMap::getGroups(int groupType)
{
    std::list<MapGroup *> groups;
    for(auto s:all_element)
    {
        if(s->getSpiritType() == MapSpirit::Map_Sprite_Type_Group)
        {
            MapGroup* group = static_cast<MapGroup *>(s);
            if(group->getGroupType() == groupType)
                groups.push_back(group);
        }
    }
    return groups;
}

std::list<MapGroup *> OneMap::getGroups()
{
    std::list<MapGroup *> groups;
    for(auto s:all_element)
    {
        if(s->getSpiritType() == MapSpirit::Map_Sprite_Type_Group)
        {
            MapGroup* group = static_cast<MapGroup *>(s);
            groups.push_back(group);
        }
    }
    return groups;
}

std::list<MapConflictPair *>  OneMap::getConflictPairs()
{
    std::list<MapConflictPair *> cs;
    for(auto s:all_element)
    {
        if(s->getSpiritType() == MapSpirit::Map_Sprite_Type_Conflict)
        {
            MapConflictPair* group = static_cast<MapConflictPair *>(s);
            cs.push_back(group);
        }
    }
    return cs;
}

