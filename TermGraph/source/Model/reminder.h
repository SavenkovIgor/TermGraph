#ifndef REMINDER_H
#define REMINDER_H

#include <QWidget>
#include <QListWidgetItem>
#include "./Termin/graphicitemterm.h"

namespace Ui {
class Reminder;
}

struct RemItem{
    GraphicItemTerm *nd;
    QListWidgetItem* itm;
};

class Reminder : public QWidget
{
    Q_OBJECT
    
public:
    explicit Reminder(GraphicItemTerm::List allNodes, QWidget *parent = nullptr);
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

    GraphicItemTerm::List allNodes;

    Ui::Reminder *ui;

};

#endif // REMINDER_H
