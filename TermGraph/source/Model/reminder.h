#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include "source/Model/Termin/paintedterm.h"

namespace Ui {
class Reminder;
}

struct RemItem{
    PaintedTerm *nd;
    QListWidgetItem* itm;
};

class Reminder : public QWidget
{
    Q_OBJECT
    
public:
    explicit Reminder(PaintedTerm::List allNodes, QWidget *parent = nullptr);
    ~Reminder();

public slots:
    void updateList();

private slots:
    void setCurrItem(QListWidgetItem *it1, QListWidgetItem *it2);
    void showResult();

    void dontRemember();
    void remember();
    void wellKnow();

private:
    RemItem* currItem;
    QList<RemItem*> remItems;

    void formLearnList();

    PaintedTerm::List allNodes;

    Ui::Reminder *ui;

};
