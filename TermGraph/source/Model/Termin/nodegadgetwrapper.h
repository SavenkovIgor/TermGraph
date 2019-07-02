#pragma once

#include <QObject>

#include "source/Model/Termin/nodeinfocontainer.h"

class NodeGadgetWrapper
{
    Q_GADGET

public:
    Q_PROPERTY(QString uuid READ getUuid WRITE setUuid)
    Q_PROPERTY(QString term READ getTerm WRITE setTerm)
    Q_PROPERTY(QString termForms READ getTermForms WRITE setTermForms)
    Q_PROPERTY(QString definition READ getDefinition WRITE setDefinition)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription)
    Q_PROPERTY(QString examples READ getExamples WRITE setExamples)
    Q_PROPERTY(QString wikiUrl READ getWikiUrl WRITE setWikiUrl)
    Q_PROPERTY(QString wikiImage READ getWikiImage WRITE setWikiImage)
    Q_PROPERTY(QString groupUuid READ getGroupUuid WRITE setGroupUuid)

    NodeGadgetWrapper() = default;
    NodeGadgetWrapper(const NodeInfoContainer& info);

    // Uuid
    QString getUuid() const;
    void setUuid(const QString& uuid);

    // Term
    QString getTerm() const;
    void setTerm(const QString& term);

    // TermForms
    QString getTermForms() const;
    void setTermForms(const QString& termForms);

    // Definition
    QString getDefinition() const;
    void setDefinition(const QString& definition);

    // Description
    QString getDescription() const;
    void setDescription(const QString& description);

    // Examples
    QString getExamples() const;
    void setExamples(const QString& examples);

    // WikiUrl
    QString getWikiUrl() const;
    void setWikiUrl(const QString& wikiUrl);

    // WikiImage
    QString getWikiImage() const;
    void setWikiImage(const QString& wikiImage);

    // GroupUuid
    QString getGroupUuid() const;
    void setGroupUuid(const QString& groupUuid);

    // LastEdit
    // Not editable...

    NodeInfoContainer unwrap() const;

private:
    NodeInfoContainer _info;
};

Q_DECLARE_METATYPE(NodeGadgetWrapper)
