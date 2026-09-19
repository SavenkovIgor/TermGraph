module;

#include <QSizeF>

export module NodeLayerTools;

import CommonTools.HandyTypes;
import Enums.CoordType;
import PaintedTerm;

export class NodeLayerTools
{
public:
    static QSizeF getNodeLayerSize(const PaintedTerm::List& nodes)
    {
        SizeList sizeList;

        for (const auto& node : nodes) {
            sizeList.push_back(node->getFrameRect(CoordType::zeroPoint).size());
        }

        return sizeList.totalStackedSize(Qt::Vertical);
    }
};
