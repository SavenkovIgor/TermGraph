/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

#include "reminder.h"
#include "ui_reminder.h"

Reminder::Reminder(PaintedTerm::List allNodes, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reminder)
{
    ui->setupUi(this);
    this->allNodes = allNodes;

    formLearnList();

//    connect(ui->learnWgt,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),SLOT(setCurrItem(QListWidgetItem*,QListWidgetItem*)));
//    connect(ui->showAns,SIGNAL(clicked(bool)),SLOT(showResult()));

//    connect(ui->dontRemember,SIGNAL(clicked(bool)),SLOT(dontRemember()));
//    connect(ui->remember,SIGNAL(clicked(bool)),SLOT(remember()));
//    connect(ui->know,SIGNAL(clicked(bool)),SLOT(wellKnow()));
}

void Reminder::formLearnList()
{
//    ui->learnWgt->clear();
//    remItems.clear();

//    for (auto* n : allNodes) {
//        if (n->atLearning()) {
//            if (n->needRemindToday()) {
//                //                qDebug()<<n->infoContainer().term<<n->getRepNum();
//                RemItem* tmp = new RemItem();
//                tmp->nd = n;
//                tmp->itm = new QListWidgetItem();
//                //                tmp->itm->setText(n->getTerm() + " (" + QString::number(n->getRepNum()) + ")" );
//                remItems << tmp;
//            }
//        }
//    }

//    ui->learnWgt->clear();

//    for (RemItem* i : remItems) {
//        ui->learnWgt->addItem(i->itm);
//    }
}

Reminder::~Reminder()
{
    delete ui;
}

void Reminder::updateList()
{
    formLearnList();
}

void Reminder::setCurrItem(QListWidgetItem* it1, [[maybe_unused]] QListWidgetItem* it2)
{
//    currItem = nullptr;
//    for( RemItem* i : remItems ) {
//        if( i->itm == it1 ) {
//            currItem = i;
//            break;
//        }
//    }

//    if( currItem == nullptr )
//        return;

//    ui->term->setText( currItem->nd->getTerm() );
//    ui->ansStack->setCurrentWidget( ui->showAns_2 );
}

void Reminder::showResult()
{
//    if( currItem == nullptr )
//        return;

//    QString def  = currItem->nd->getDefinition();
//    QString desc = currItem->nd->getDescription();
//    if( def. isEmpty() && desc.isEmpty() ) {
//        def = "Нет данных";
//    }

//    ui->answer->setText( def );
//    ui->descr->setText( desc );

//    ui->ansStack->setCurrentWidget(ui->result);
}

void Reminder::dontRemember()
{
    if( currItem == nullptr )
        return;
//    currItem->nd->setRemind(TermNode::KnowLevel::dontKnowLvl );
    formLearnList();
}

void Reminder::remember()
{
    if( currItem == nullptr )
        return;
//    currItem->nd->setRemind(TermNode::KnowLevel::remindLvl );
    formLearnList();
}

void Reminder::wellKnow()
{
    if( currItem == nullptr )
        return;
//    currItem->nd->setRemind(TermNode::KnowLevel::wellRemindLvl );
    formLearnList();
}
