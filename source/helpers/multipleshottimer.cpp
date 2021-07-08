/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "source/helpers/multipleshottimer.h"

MultipleShotTimer::MultipleShotTimer(int shots, int interval)
{
    assert(interval > 0);
    assert(shots > 0);

    setSingleShot(false);
    setInterval(interval);
    shotsLimit = shots;
    shotsCount = 0;
    connect(this, &QTimer::timeout, this, &MultipleShotTimer::increaseShot);
}

void MultipleShotTimer::increaseShot()
{
    shotsCount++;
    if (shotsCount >= shotsLimit)
        stop(); // Останавливаем таймер
}
