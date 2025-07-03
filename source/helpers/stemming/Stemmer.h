#pragma once

#include <memory>
#include <QString>

enum class StemmingLanguage { English, Russian };

class Stemmer
{
public:
    virtual ~Stemmer()                              = default;
    virtual QString stem(const QString& word) const = 0;
};

std::unique_ptr<Stemmer> create_stemmer(StemmingLanguage lang);
