#include "nodegadgetwrapper.h"

NodeGadgetWrapper::NodeGadgetWrapper(const NodeInfoContainer& info)
    : _info(info)
{}

QString NodeGadgetWrapper::uuid() const
{
    return _info.uuid.toString();
}

void NodeGadgetWrapper::setUuid(const QString& uuid)
{
    _info.uuid.fromString(uuid);
}

QString NodeGadgetWrapper::term() const
{
    return _info.term;
}

void NodeGadgetWrapper::setTerm(const QString& term)
{
    _info.term = term;
}

QString NodeGadgetWrapper::termForms() const
{
    return _info.termForms;
}

void NodeGadgetWrapper::setTermForms(const QString& termForms)
{
    _info.termForms = termForms;
}

QString NodeGadgetWrapper::definition() const
{
    return _info.definition;
}

void NodeGadgetWrapper::setDefinition(const QString& definition)
{
    _info.definition = definition;
}

QString NodeGadgetWrapper::description() const
{
    return _info.description;
}

void NodeGadgetWrapper::setDescription(const QString& description)
{
    _info.description = description;
}

QString NodeGadgetWrapper::examples() const
{
    return _info.examples;
}

void NodeGadgetWrapper::setExamples(const QString& examples)
{
    _info.examples = examples;
}

QString NodeGadgetWrapper::wikiUrl() const
{
    return _info.wikiUrl;
}

void NodeGadgetWrapper::setWikiUrl(const QString& wikiUrl)
{
    _info.wikiUrl = wikiUrl;
}

QString NodeGadgetWrapper::wikiImage() const
{
    return _info.wikiImage;
}

void NodeGadgetWrapper::setWikiImage(const QString& wikiImage)
{
    _info.wikiImage = wikiImage;
}

QString NodeGadgetWrapper::groupUuid() const
{
    return _info.groupUuid.toString();
}

void NodeGadgetWrapper::setGroupUuid(const QString& groupUuid)
{
    _info.groupUuid.fromString(groupUuid);
}

NodeInfoContainer NodeGadgetWrapper::unwrap() const
{
    return _info;
}
