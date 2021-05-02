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

// TODO: Try to add goodcheck to conan

#pragma once

#include "source/Model/Base/node.hpp"

template<typename Data>
class Edge
{
public:
    explicit Edge(Node<Data>* from, Node<Data>* to)
        : mFrom(from)
        , mTo(to)
    {
        assert(from != nullptr);
        assert(to != nullptr);
    }

private:
    Node<Data>* mFrom = nullptr;
    Node<Data>* mTo   = nullptr;
};
