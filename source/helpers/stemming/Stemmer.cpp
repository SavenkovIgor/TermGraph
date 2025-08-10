// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <memory>
#include <QString>

#include <english_stem.h>
#include <russian_stem.h>

export module Stemmer;

export enum class StemmingLanguage { English, Russian };

class Stemmer
{
public:
    virtual ~Stemmer()                              = default;
    virtual QString stem(const QString& word) const = 0;
};

class EnglishStemmer : public Stemmer
{
public:
    QString stem(const QString& word) const override
    {
        std::wstring                         tmp = word.toStdWString();
        stemming::english_stem<std::wstring> st;
        st(tmp);
        return QString::fromWCharArray(tmp.c_str());
    }
};

class RussianStemmer : public Stemmer
{
public:
    QString stem(const QString& word) const override
    {
        std::wstring                         tmp = word.toStdWString();
        stemming::russian_stem<std::wstring> st;
        st(tmp);
        return QString::fromWCharArray(tmp.c_str());
    }
};

export std::unique_ptr<Stemmer> create_stemmer(StemmingLanguage lang)
{
    switch (lang) {
    case StemmingLanguage::English: return std::make_unique<EnglishStemmer>();
    case StemmingLanguage::Russian: return std::make_unique<RussianStemmer>();
    }
    return nullptr;
}
