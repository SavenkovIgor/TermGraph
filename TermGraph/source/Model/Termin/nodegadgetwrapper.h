#pragma once

#include <QObject>

#include "source/Model/Termin/nodeinfocontainer.h"

class NodeGadgetWrapper
{
public:
    Q_GADGET

    Q_PROPERTY(QString uuid READ uuid WRITE setUuid)
    Q_PROPERTY(QString term READ term WRITE setTerm)
    Q_PROPERTY(QString termForms READ termForms WRITE setTermForms)
    Q_PROPERTY(QString definition READ definition WRITE setDefinition)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString examples READ examples WRITE setExamples)
    Q_PROPERTY(QString wikiUrl READ wikiUrl WRITE setWikiUrl)
    Q_PROPERTY(QString wikiImage READ wikiImage WRITE setWikiImage)
    Q_PROPERTY(QString groupUuid READ groupUuid WRITE setGroupUuid)

    NodeGadgetWrapper(const NodeInfoContainer& info);

    // Uuid
    QString uuid() const;
    void setUuid(const QString& uuid);

    // Term
    QString term() const;
    void setTerm(const QString& term);

    // TermForms
    QString termForms() const;
    void setTermForms(const QString& termForms);

    // Definition
    QString definition() const;
    void setDefinition(const QString& definition);

    // Description
    QString description() const;
    void setDescription(const QString& description);

    // Examples
    QString examples() const;
    void setExamples(const QString& examples);

    // WikiUrl
    QString wikiUrl() const;
    void setWikiUrl(const QString& wikiUrl);

    // WikiImage
    QString wikiImage() const;
    void setWikiImage(const QString& wikiImage);

    // GroupUuid
    QString groupUuid() const;
    void setGroupUuid(const QString& groupUuid);

    // LastEdit
    // Not editable...

    NodeInfoContainer unwrap() const;
private:
    NodeInfoContainer _info;
};
