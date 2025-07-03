#include "source/helpers/stemming/Stemmer.h"

#include <english_stem.h>
#include <russian_stem.h>

using namespace stemming;

namespace {

class EnglishStemmer : public Stemmer
{
public:
    QString stem(const QString& word) const override
    {
        std::wstring               tmp = word.toStdWString();
        english_stem<std::wstring> st;
        st(tmp);
        return QString::fromWCharArray(tmp.c_str());
    }
};

class RussianStemmer : public Stemmer
{
public:
    QString stem(const QString& word) const override
    {
        std::wstring               tmp = word.toStdWString();
        russian_stem<std::wstring> st;
        st(tmp);
        return QString::fromWCharArray(tmp.c_str());
    }
};

} // namespace

std::unique_ptr<Stemmer> create_stemmer(StemmingLanguage lang)
{
    switch (lang) {
    case StemmingLanguage::English: return std::make_unique<EnglishStemmer>();
    case StemmingLanguage::Russian: return std::make_unique<RussianStemmer>();
    }
    return nullptr;
}
