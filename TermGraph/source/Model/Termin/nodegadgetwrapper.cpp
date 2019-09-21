#include "nodegadgetwrapper.h"

NodeGadgetWrapper::NodeGadgetWrapper(const NodeInfoContainer& info)
    : _info(info)
{}

bool NodeGadgetWrapper::isNull()
{
    return _info.isNull();
}

QString NodeGadgetWrapper::getUuid() const
{
    return _info.uuid.toString();
}

void NodeGadgetWrapper::setUuid(const QString& uuid)
{
    _info.uuid.fromString(uuid);
}

QString NodeGadgetWrapper::getTerm() const
{
    return _info.term;
}

void NodeGadgetWrapper::setTerm(const QString& term)
{
    _info.term = term;
}

QString NodeGadgetWrapper::getDefinition() const
{
    return _info.definition;
}

void NodeGadgetWrapper::setDefinition(const QString& definition)
{
    _info.definition = definition;
}

QString NodeGadgetWrapper::getDescription() const
{
    return _info.description;
}

void NodeGadgetWrapper::setDescription(const QString& description)
{
    _info.description = description;
}

QString NodeGadgetWrapper::getExamples() const
{
    return _info.examples;
}

void NodeGadgetWrapper::setExamples(const QString& examples)
{
    _info.examples = examples;
}

QString NodeGadgetWrapper::getWikiUrl() const
{
    return _info.wikiUrl;
}

void NodeGadgetWrapper::setWikiUrl(const QString& wikiUrl)
{
    _info.wikiUrl = wikiUrl;
}

QString NodeGadgetWrapper::getWikiImage() const
{
    return _info.wikiImage;
}

void NodeGadgetWrapper::setWikiImage(const QString& wikiImage)
{
    _info.wikiImage = wikiImage;
}

QString NodeGadgetWrapper::getGroupUuid() const
{
    return _info.groupUuid.toString();
}

void NodeGadgetWrapper::setGroupUuid(const QString& groupUuid)
{
    _info.groupUuid.fromString(groupUuid);
}

QString NodeGadgetWrapper::getLastEdit() const
{
    return _info.lastEdit.toString("dd MMM yyyy hh:mm:ss t");
}

NodeInfoContainer NodeGadgetWrapper::unwrap() const
{
    return _info;
}
