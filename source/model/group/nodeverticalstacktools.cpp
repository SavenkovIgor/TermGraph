module;

#include <QSizeF>

#include "source/enums/coord_type.h"

export module NodeVerticalStackTools;

import CommonTools.HandyTypes;
import PaintedTerm;

export class NodeVerticalStackTools
{
public:
    static QSizeF getNodeVerticalStackedSize(const PaintedTerm::List& nodes)
    {
        SizeList sizeList;

        for (const auto& node : nodes) {
            sizeList.push_back(node->getFrameRect(CoordType::zeroPoint).size());
        }

        return sizeList.totalStackedSize(Qt::Vertical);
    }
};
