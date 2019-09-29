#include "groupnamecache.h"

GroupNameCache::GroupNameCache(const PaintedTerm::List& nodes)
{
    for (const auto& node : nodes) {
        auto name = node->getTerm().simplified();
        nodeMap.insert(name, node);
    }
}
