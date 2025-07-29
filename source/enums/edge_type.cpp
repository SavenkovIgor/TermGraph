// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

export module Enums.EdgeType;

// TODO: Merge back with QmlEdgeType when Qt learn to work with modules
// Until then keep this enums in sync
export enum class EdgeType { Standart, Termin, TerminHardLink, Description, Broken, Redundant };
