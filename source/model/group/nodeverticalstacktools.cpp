module;

#include <QSizeF>

#include "source/model/enums.h"

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
