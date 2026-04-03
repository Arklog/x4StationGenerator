
#include <string>
#include <cstdint>

namespace xsdcpp {

struct ElementContext;
struct Position;
struct ElementInfo;

typedef void* (*get_field_t)(void*);
typedef void (*set_value_t)(void* obj, const Position&, std::string&&);
typedef void (*set_default_t)(void*);
typedef void (*set_any_attribute_t)(void*, std::string&& name, std::string&& value);

struct ChildElementInfo
{
    const char* name;
    size_t trackIndex;
    get_field_t getElementField;
    const ElementInfo* info;
    size_t minOccurs;
    size_t maxOccurs;
};

struct AttributeInfo
{
    const char* name;
    uint64_t trackBit;
    get_field_t getAttribute;
    set_value_t setValue;
    bool isMandatory;
    set_default_t setDefaultValue;
};

struct ElementInfo
{
    enum ElementFlag
    {
        EntryPointFlag = 0x01,
        ReadTextFlag = 0x02,
        SkipProcessingFlag = 0x04,
        AnyAttributeFlag = 0x08,
        CheckChildrenFlag = 0x10,
    };
    
    size_t flags;
    set_value_t addText;
    const ChildElementInfo* children;
    size_t childrenCount;
    const AttributeInfo* attributes;
    uint64_t checkAttributeMask;
    const ElementInfo* base;
    set_any_attribute_t setOtherAttribute;
};

struct ElementContext
{
    const ElementInfo* info;
    void* element;
    size_t processedElements2[64];
    uint64_t processedAttributes2;

    ElementContext(const ElementInfo* info, void* element);
};

void parse(const char* data, const char** namespaces, ElementContext& elementContext);

bool getListItem(const char*& s, std::string& result);

uint32_t toNumeric(const Position& pos, const char* const* values, const std::string& value);

std::string to_string(size_t val, size_t size, const char* const* values, const char* name);

void set_string(std::string* obj,const Position&, std::string&& val);
void set_uint64_t(uint64_t* obj, const Position& pos, std::string&& val);
void set_int64_t(int64_t* obj, const Position& pos, std::string&& val);
void set_uint32_t(uint32_t* obj, const Position& pos, std::string&& val);
void set_int32_t(int32_t* obj, const Position& pos, std::string&& val);
void set_uint16_t(uint16_t* obj, const Position& pos, std::string&& val);
void set_int16_t(int16_t* obj, const Position& pos, std::string&& val);
void set_float(float* obj, const Position& pos, std::string&& val);
void set_double(double* obj, const Position& pos, std::string&& val);
void set_bool(bool* obj, const Position& pos, std::string&& val);

std::string read_file(const std::string& filePath);

}

#include <cstring>
#include <sstream>
#include <fstream>
#include <stdexcept>

namespace xsdcpp {

ElementContext::ElementContext(const ElementInfo* info, void* element)
    : info(info)
    , element(element)
    , processedAttributes2(0)
{
    memset(processedElements2, 0, sizeof(size_t) * info->childrenCount);
}

struct Position
{
    int line;
    const char* pos;
    const char* lineStart;
};

}


namespace {

struct Token
{
    enum Type
    {
        startTagBeginType, // <
        tagEndType, // >
        endTagBeginType, // </
        emptyTagEndType, // />
        equalsSignType, // =
        stringType,
        nameType, // attribute or tag name
    };

    Type type;
    std::string value;
    xsdcpp::Position pos;
};

struct Context
{
    xsdcpp::Position pos;
    Token token;
    const char** namespaces;
};

void skipSpace(xsdcpp::Position& pos)
{
    for (char c;;)
        switch ((c = *pos.pos))
        {
        case '\r':
            if (*(++pos.pos) == '\n')
                ++pos.pos;
            ++pos.line;
            pos.lineStart = pos.pos;
            continue;
        case '\n':
            ++pos.line;
            ++pos.pos;
            pos.lineStart = pos.pos;
            continue;
        case '<':
            if (strncmp(pos.pos + 1, "!--", 3) == 0)
            {
                pos.pos += 4;
                for (;;)
                {
                    const char* end = strpbrk(pos.pos, "-\n\r");
                    if (!end)
                    {
                        pos.pos = pos.pos + strlen(pos.pos);
                        return;
                    }
                    pos.pos = end;
                    switch (*pos.pos)
                    {
                    case '\r':
                        if (*(++pos.pos) == '\n')
                            ++pos.pos;
                        ++pos.line;
                        pos.lineStart = pos.pos;
                        continue;
                    case '\n':
                        ++pos.line;
                        ++pos.pos;
                        pos.lineStart = pos.pos;
                        continue;
                    default:
                        if (strncmp(pos.pos + 1, "->", 2) == 0)
                        {
                            pos.pos = end + 3;
                            break;
                        }
                        ++pos.pos;
                        continue;
                    }
                    break;
                }
                continue;
            }
            return;
        case ' ':
        case '\t':
        case '\v':
        case '\f':
            ++pos.pos;
            continue;
        default:
            return;
        }
}

std::runtime_error SyntaxException(const xsdcpp::Position& pos, const std::string& error)
{
    std::stringstream s;
    s << "Syntax error at line '" << pos.line << "': " << error;
    return std::runtime_error(s.str());
}

std::runtime_error VerificationException(const xsdcpp::Position& pos, const std::string& error)
{
    std::stringstream s;
    s << "Error at line '" << pos.line << "': " << error;
    return std::runtime_error(s.str());
}

void skipText(xsdcpp::Position& pos)
{
    for (;;)
    {
        const char* end = strpbrk(pos.pos, "<\r\n");
        if (!end)
        {
            pos.pos = pos.pos + strlen(pos.pos);
            throw SyntaxException(pos, "Unexpected end of file");
        }
        pos.pos = end;
        switch (*pos.pos)
        {
        case '\r':
            if (*(++pos.pos) == '\n')
                ++pos.pos;
            ++pos.line;
            pos.lineStart = pos.pos;
            continue;
        case '\n':
            ++pos.line;
            ++pos.pos;
            pos.lineStart = pos.pos;
            continue;
        default:
            if (pos.pos[1] == '!')
            {
                skipSpace(pos);
                continue;
            }
            return;
        }
    }
}

const char* _escapeStrings[] = { "apos", "quot", "amp", "lt", "gt" };
const char* _escapeChars = "'\"&<>";

bool appendUnicode(uint32_t ch, std::string& str)
{
    if ((ch & ~(0x80UL - 1)) == 0)
    {
        str.push_back((char)ch);
        return true;
    }
    if ((ch & ~(0x800UL - 1)) == 0)
    {
        str.push_back((ch >> 6) | 0xC0);
        str.push_back((ch & 0x3F) | 0x80);
        return true;
    }
    if ((ch & ~(0x10000UL - 1)) == 0)
    {
        str.push_back((ch >> 12) | 0xE0);
        str.push_back(((ch >> 6) & 0x3F) | 0x80);
        str.push_back((ch & 0x3F) | 0x80);
        return true;
    }
    if (ch < 0x110000UL)
    {
        str.push_back((ch >> 18) | 0xF0);
        str.push_back(((ch >> 12) & 0x3F) | 0x80);
        str.push_back(((ch >> 6) & 0x3F) | 0x80);
        str.push_back((ch & 0x3F) | 0x80);
        return true;
    }
    return false;
}

std::string unescapeString(const char* str, size_t len)
{
    std::string result;
    result.reserve(len);
    for (const char* i = str, * end = str + len;;)
    {
        size_t remainingLen = end - i;
        const char* next = (const char*)memchr(i, '&', remainingLen);
        if (!next)
            return result.append(i, remainingLen);
        else
            result.append(i, next - i);
        i = next + 1;
        remainingLen = end - i;
        const char* sequenceEnd = (const char*)memchr(i, ';', remainingLen);
        if (!sequenceEnd)
        {
            result.push_back('&');
            continue;
        }
        if (*i == '#')
        {
            char* endptr;
            uint32_t unicodeValue = i[1] == 'x' ? strtoul(i + 2, &endptr, 16) : strtoul(i + 1, &endptr, 10);
            if (endptr != sequenceEnd || !appendUnicode(unicodeValue, result))
            {
                result.push_back('&');
                continue;
            }
            i = sequenceEnd + 1;
            continue;
        }
        size_t sequenceLen = sequenceEnd - i;
        for (const char **j = _escapeStrings, **end = _escapeStrings + sizeof(_escapeStrings) / sizeof(*_escapeStrings); j < end; ++j)
            if (strncmp(i, *j, sequenceLen) == 0)
            {
                result.push_back(_escapeChars[j - _escapeStrings]);
                i = sequenceEnd + 1;
                goto sequenceTranslated;
            }
        result.push_back('&');
    sequenceTranslated:;
    }
}

std::string stripComments(const char* str, size_t len)
{
    std::string result;
    result.reserve(len);
    for (const char* i = str, * end = str + len;;)
    {
        size_t remainingLen = end - i;
        const char* next = (const char*)memchr(i, '<', remainingLen);
        if (!next)
            return result.append(i, remainingLen);
        else
            result.append(i, next - i);
        i = next;
        if (strncmp(i + 1, "!--", 3) != 0)
            return result.append(i, end - i);
        i += 4;
        for (;;)
        {
            const char* commentEnd = strpbrk(i, "-");
            if (!commentEnd)
            {
                i = end;
                continue;
            }
            i = commentEnd;
            if (strncmp(i + 1, "->", 2) == 0)
            {
                i += 3;
                break;
            }
            ++i;
        }
    }
}

void readToken(Context& context)
{
    skipSpace(context.pos);
    context.token.pos = context.pos;
    switch (*context.pos.pos)
    {
    case '<':
        if (context.pos.pos[1] == '/')
        {
            context.token.type = Token::endTagBeginType;
            context.pos.pos += 2;
            return;
        }
        context.token.type = Token::startTagBeginType;
        ++context.pos.pos;
        return;
    case '>':
        context.token.type = Token::tagEndType;
        ++context.pos.pos;
        return;
    case '\0':
        throw SyntaxException(context.pos, "Unexpected end of file");
    case '=':
        context.token.type = Token::equalsSignType;
        ++context.pos.pos;
        return;
    case '"':
    case '\'': {
        char endChars[4] = "x\r\n";
        *endChars = *context.pos.pos;
        const char* end = strpbrk(context.pos.pos + 1, endChars);
        if (!end)
            throw SyntaxException(context.pos, "Unexpected end of file");
        if (*end != *context.pos.pos)
            throw SyntaxException(context.pos, "New line in string");
        context.token.value = unescapeString(context.pos.pos + 1, end - context.pos.pos - 1);
        context.token.type = Token::stringType;
        context.pos.pos = end + 1;
        return;
    }
    case '/':
        if (context.pos.pos[1] == '>')
        {
            context.token.type = Token::emptyTagEndType;
            context.pos.pos += 2;
            return;
        }
        // no break
    default: // attribute or tag name
        {
            const char* end = context.pos.pos;
            while (*end && *end != '/' && *end != '>' && *end != '=' && !isspace(*end))
                ++end;
            if (end == context.pos.pos)
                throw SyntaxException(context.pos, "Expected name");
            context.token.value = std::string(context.pos.pos, end - context.pos.pos);
            context.token.type = Token::nameType;
            context.pos.pos = end;
            return;
        }
    }
}

void skipTextAndSubElements(Context& context, const std::string& elementName)
{
    for (;;)
    {
        skipText(context.pos);
        xsdcpp::Position posBackup = context.pos;
        readToken(context);
        switch (context.token.type)
        {
        case Token::startTagBeginType:
            readToken(context);
            if (context.token.type == Token::nameType)
            {
                std::string elementName = std::move(context.token.value);
                skipTextAndSubElements(context, elementName);
                readToken(context);
            }
            break;
        case Token::endTagBeginType:
            readToken(context);
            if (context.token.type == Token::nameType && context.token.value == elementName)
            {
                context.pos = posBackup;
                return;
            }
            break;
        default:
            break;
        }
    }
}

xsdcpp::ElementContext enterElement(Context& context, xsdcpp::ElementContext& parentElementContext, const xsdcpp::ChildElementInfo& childInfo)
{
    size_t& count = parentElementContext.processedElements2[childInfo.trackIndex];
    if (childInfo.maxOccurs && count >= childInfo.maxOccurs)
    {
        std::stringstream s;
        s << "Maximum occurrence of element '" << childInfo.name << "' is " << childInfo.maxOccurs ;
        throw VerificationException(context.pos,  s.str());
    }
    ++count;
    return xsdcpp::ElementContext(childInfo.info, childInfo.getElementField(parentElementContext.element));
}

xsdcpp::ElementContext enterElement(Context& context, xsdcpp::ElementContext& parentElementContext, const std::string& name)
{
    for (const xsdcpp::ElementInfo* i = parentElementContext.info; i; i = i->base)
        if (const xsdcpp::ChildElementInfo* c = i->children)
            for (; c->name; ++c)
                if (name == c->name)
                    return enterElement(context, parentElementContext, *c);
    size_t n = name.find(':');
    if (n != std::string::npos)
    {
        std::string nameWithoutNamespace = name.substr(n + 1);
        for (const xsdcpp::ElementInfo* i = parentElementContext.info; i; i = i->base)
            if (const xsdcpp::ChildElementInfo* c = i->children)
                for (; c->name; ++c)
                    if (nameWithoutNamespace == c->name)
                        return enterElement(context, parentElementContext, *c);
    }
    throw VerificationException(context.pos, "Unexpected element '" + name + "'");
}

void checkElement(Context& context, const xsdcpp::ElementContext& elementContext)
{
    if (elementContext.info->flags & xsdcpp::ElementInfo::CheckChildrenFlag)
        for (const xsdcpp::ElementInfo* i = elementContext.info; i; i = i->base)
            if (const xsdcpp::ChildElementInfo* c = i->children)
                for (; c->name; ++c)
                    if (elementContext.processedElements2[c->trackIndex] < c->minOccurs)
                    {
                        std::stringstream s;
                        s << "Minimum occurrence of element '" << c->name << "' is " << c->minOccurs;
                        throw VerificationException(context.pos, s.str());
                    }
}

void setAttribute(Context& context, xsdcpp::ElementContext& elementContext, std::string&& name, std::string&& value)
{
    for (const xsdcpp::ElementInfo* i = elementContext.info; i; i = i->base)
        if (const xsdcpp::AttributeInfo* a = i->attributes)
            for (; a->name; ++a)
                if (name == a->name)
                {
                    if (elementContext.processedAttributes2 & a->trackBit)
                        throw VerificationException(context.pos, "Repeated attribute '" + name + "'");
                    elementContext.processedAttributes2 |= a->trackBit;
                    a->setValue(a->getAttribute(elementContext.element), context.pos, std::move(value));
                    return;
                }
    if (elementContext.info->flags & xsdcpp::ElementInfo::EntryPointFlag)
    {
        if (name.compare(0, 5, "xmlns") == 0 && (name.size() == 5 || name.c_str()[5] == ':'))
        {
            std::string namespacePrefix;
            if (name.size() > 6)
                namespacePrefix = name.substr(6);
            size_t namespaceIndex = 0;
            for (const char** ns = context.namespaces; *ns; ++ns, ++namespaceIndex)
                if (value == *ns)
                    return;
            throw VerificationException(context.pos, "Unknown namespace '" + value + "'");
        }
        size_t n = name.find(':');
        if (n != std::string::npos)
        {
            ++n;
             if (name.compare(n, std::string::npos, "noNamespaceSchemaLocation") == 0 ||
                 name.compare(n, std::string::npos, "schemaLocation") == 0)
                 return;
        }
    }
    for (const xsdcpp::ElementInfo* i = elementContext.info; i; i = i->base)
        if (i->flags & xsdcpp::ElementInfo::AnyAttributeFlag)
        {
            i->setOtherAttribute(elementContext.element, std::move(name), std::move(value));
            return;
        }

    throw VerificationException(context.pos, "Unexpected attribute '" + name + "'");
}

void checkAttributes(Context& context, xsdcpp::ElementContext& elementContext)
{
    uint64_t missingAttributes = elementContext.info->checkAttributeMask & ~elementContext.processedAttributes2;
    if (missingAttributes)
    {
        for (const xsdcpp::ElementInfo* i = elementContext.info; i; i = i->base)
            if (const xsdcpp::AttributeInfo* a = i->attributes)
                for (; a->name; ++a)
                    if (missingAttributes & a->trackBit)
                    {
                        if (a->isMandatory)
                            throw VerificationException(context.pos, "Missing attribute '" + std::string(a->name) + "'");
                        a->setDefaultValue(elementContext.element);
                    }
    }
}

void parseElement(Context& context, xsdcpp::ElementContext& parentElementContext)
{
    readToken(context);
    if (context.token.type != Token::nameType)
        throw SyntaxException(context.token.pos, "Expected tag name");
    std::string elementName = std::move(context.token.value);
    xsdcpp::ElementContext elementContext = enterElement(context, parentElementContext, elementName);
    for (;;)
    {
        readToken(context);
        if (context.token.type == Token::emptyTagEndType)
        {
            checkAttributes(context, elementContext);
            checkElement(context, elementContext);
            return;
        }
        if (context.token.type == Token::tagEndType)
            break;
        if (context.token.type == Token::nameType)
        {
            std::string attributeName = std::move(context.token.value);
            readToken(context);
            if (context.token.type != Token::equalsSignType)
                throw SyntaxException(context.token.pos, "Expected '='");
            readToken(context);
            if (context.token.type != Token::stringType)
                throw SyntaxException(context.token.pos, "Expected string");
            std::string& attributeValue = context.token.value;
            setAttribute(context, elementContext, std::move(attributeName), std::move(attributeValue));
            continue;
        }
    }
    checkAttributes(context, elementContext);
    for (;;)
    {
        if (elementContext.info->flags & xsdcpp::ElementInfo::ReadTextFlag)
        {
            const char* start = context.pos.pos;
            if (elementContext.info->flags & xsdcpp::ElementInfo::SkipProcessingFlag)
                skipTextAndSubElements(context, elementName);
            else
                skipText(context.pos);
            if (context.pos.pos != start)
            {
                std::string text = stripComments(start, context.pos.pos - start);
                elementContext.info->addText(elementContext.element, context.pos, std::move(text));
            }
        }
        else
            skipText(context.pos);
        
        readToken(context);
        if (context.token.type == Token::endTagBeginType)
            break;
        if (context.token.type == Token::startTagBeginType)
        {
            parseElement(context, elementContext);
            continue;
        }
        else
            throw SyntaxException(context.token.pos, "Expected '<'");
    }
    readToken(context);
    if (context.token.type != Token::nameType)
        throw SyntaxException(context.token.pos, "Expected tag name");
    if (context.token.value != elementName)
        throw SyntaxException(context.token.pos, "Expected end tag of '" + elementName + "'");
    readToken(context);
    if (context.token.type != Token::tagEndType)
        throw SyntaxException(context.token.pos, "Expected '>'");
    checkElement(context, elementContext);
}

}

namespace xsdcpp {

bool getListItem(const char*& s, std::string& result)
{
    while (isspace(*s))
        ++s;
    if (!*s)
        return false;
    const char* end = strpbrk(s, " \t\n\r");
    if (end)
    {
        result = std::string(s, end - s);
        s = end;
    }
    else
    {
        result = s;
        s += result.size();
    }
    return true;
}

void parse(const char* data, const char** namespaces, ElementContext& elementContext)
{
    Context context;
    context.pos.pos = context.pos.lineStart = data;
    context.pos.line = 1;
    context.namespaces = namespaces;
    
    skipSpace(context.pos);
    while (*context.pos.pos == '<' && context.pos.pos[1] == '?')
    {
        context.pos.pos += 2;
        for (;;)
        {
            const char* end = strpbrk(context.pos.pos, "\r\n?");
            if (!end)
                throw SyntaxException(context.pos, "Unexpected end of file");
            if (*end == '?' && end[1] == '>')
            {
                context.pos.pos = end + 2;
                break;
            }
            context.pos.pos = end + 1;
            skipSpace(context.pos);
        }
        skipSpace(context.pos);
    }
    readToken(context);
    if (context.token.type != Token::startTagBeginType)
        throw SyntaxException(context.token.pos, "Expected '<'");
    parseElement(context, elementContext);
}

uint32_t toNumeric(const Position& pos, const char* const* values, const std::string& value)
{
    for (const char* const* i = values; *i; ++i)
        if (value == *i)
            return (uint32_t)(i - values);
    throw VerificationException(pos, "Unknown attribute value '" + value + "'");
}

std::string to_string(size_t val, size_t size, const char* const* values, const char* name)
{
    if (val >= size)
        throw std::invalid_argument("Invalid " + std::string(name) + " argument");
    return values[val];
}

void set_string(std::string* obj, const Position&, std::string&& val) { if (obj->empty()) *obj = std::move(val); else *obj += val; }
void set_uint64_t(uint64_t* obj,  const Position& pos, std::string&& val) { std::stringstream ss(val); if (!(ss >> *obj)) throw VerificationException(pos, "Expected unsigned 64-bit integer value"); }
void set_int64_t(int64_t* obj,  const Position& pos, std::string&& val) { std::stringstream ss(val); if (!(ss >> *obj)) throw VerificationException(pos, "Expected 64-bit integer value"); }
void set_uint32_t(uint32_t* obj,  const Position& pos, std::string&& val) { std::stringstream ss(val); if (!(ss >> *obj)) throw VerificationException(pos, "Expected unsigned 32-bit integer value"); }
void set_int32_t(int32_t* obj,  const Position& pos, std::string&& val) { std::stringstream ss(val); if (!(ss >> *obj)) throw VerificationException(pos, "Expected 32-bit integer value"); }
void set_uint16_t(uint16_t* obj,  const Position& pos, std::string&& val) { std::stringstream ss(val); if (!(ss >> *obj)) throw VerificationException(pos, "Expected unsigned 16-bit integer value"); }
void set_int16_t(int16_t* obj,  const Position& pos, std::string&& val) { std::stringstream ss(val); if (!(ss >> *obj)) throw VerificationException(pos, "Expected 16-bit integer value"); }
void set_float(float* obj,  const Position& pos, std::string&& val) { std::stringstream ss(val); if (!(ss >> *obj)) throw VerificationException(pos, "Expected single precision floating point value"); }
void set_double(double* obj,  const Position& pos, std::string&& val) { std::stringstream ss(val); if (!(ss >> *obj)) throw VerificationException(pos, "Expected double precision floating point value"); }
void set_bool(bool* obj, const Position& pos, std::string&& val) { std::stringstream ss(val); if (!(ss >> std::boolalpha >> *obj)) throw VerificationException(pos, "Expected boolean value"); }

std::string read_file(const std::string& filePath)
{
    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    file.open(filePath, std::fstream::in);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

}

#include "wares.hpp"

namespace wares {

extern const xsdcpp::ElementInfo _waresType_Info;
extern const xsdcpp::ElementInfo _productionRootType_Info;
extern const xsdcpp::ElementInfo _methodType_Info;
extern const xsdcpp::ElementInfo _methodDefaultType_Info;
extern const xsdcpp::ElementInfo _defaultsType_Info;
extern const xsdcpp::ElementInfo _wareBodyType_Info;
extern const xsdcpp::ElementInfo _priceType_Info;
extern const xsdcpp::ElementInfo _wareProductionType_Info;
extern const xsdcpp::ElementInfo _primaryType_Info;
extern const xsdcpp::ElementInfo _wareAmount_Info;
extern const xsdcpp::ElementInfo _effectsType_Info;
extern const xsdcpp::ElementInfo _effectType_Info;
extern const xsdcpp::ElementInfo _researchType_Info;
extern const xsdcpp::ElementInfo _componentType_Info;
extern const xsdcpp::ElementInfo _containerType_Info;
extern const xsdcpp::ElementInfo _iconType_Info;
extern const xsdcpp::ElementInfo _sourcesType_Info;
extern const xsdcpp::ElementInfo _sourceType_Info;
extern const xsdcpp::ElementInfo _restrictionType_Info;
extern const xsdcpp::ElementInfo _useType_Info;
extern const xsdcpp::ElementInfo _ownerType_Info;
extern const xsdcpp::ElementInfo _illegalType_Info;
extern const xsdcpp::ElementInfo _softwareType_Info;
extern const xsdcpp::ElementInfo _wareType_Info;

}

namespace {


}

namespace wares {


}

namespace {

const char* _namespaces[] = {
    "",
    "http://www.w3.org/2001/XMLSchema-instance",
    "http://www.w3.org/2001/XMLSchema",
    nullptr
};

void* _get_waresType_production(wares::waresType* parent) {return &*(parent->production = wares::productionRootType());}
void* _get_waresType_defaults(wares::waresType* parent) {return (parent->defaults.emplace_back(), &parent->defaults.back());}
void* _get_waresType_ware(wares::waresType* parent) {return (parent->ware.emplace_back(), &parent->ware.back());}
xsdcpp::ChildElementInfo _waresType_Children[] = {
    {"production", 0, (xsdcpp::get_field_t)&_get_waresType_production, &wares::_productionRootType_Info, 0, 1},
    {"defaults", 1, (xsdcpp::get_field_t)&_get_waresType_defaults, &wares::_defaultsType_Info, 0, 0},
    {"ware", 2, (xsdcpp::get_field_t)&_get_waresType_ware, &wares::_wareType_Info, 0, 0},
    {nullptr}
};
struct _root_t
{
    wares::waresType wares;
};

void* _get__root_t_wares(_root_t* parent) {return &parent->wares;}
xsdcpp::ChildElementInfo __root_t_Children[] = {
    {"wares", 0, (xsdcpp::get_field_t)&_get__root_t_wares, &wares::_waresType_Info, 1, 1},
    {nullptr}
};
void* _get_productionRootType_method(wares::productionRootType* parent) {return (parent->method.emplace_back(), &parent->method.back());}
xsdcpp::ChildElementInfo _productionRootType_Children[] = {
    {"method", 0, (xsdcpp::get_field_t)&_get_productionRootType_method, &wares::_methodType_Info, 0, 0},
    {nullptr}
};
void* _get_wareBodyType_price(wares::wareBodyType* parent) {return (parent->price.emplace_back(), &parent->price.back());}
void* _get_wareBodyType_production(wares::wareBodyType* parent) {return (parent->production.emplace_back(), &parent->production.back());}
void* _get_wareBodyType_research(wares::wareBodyType* parent) {return (parent->research.emplace_back(), &parent->research.back());}
void* _get_wareBodyType_component(wares::wareBodyType* parent) {return (parent->component.emplace_back(), &parent->component.back());}
void* _get_wareBodyType_container(wares::wareBodyType* parent) {return (parent->container.emplace_back(), &parent->container.back());}
void* _get_wareBodyType_icon(wares::wareBodyType* parent) {return (parent->icon.emplace_back(), &parent->icon.back());}
void* _get_wareBodyType_sources(wares::wareBodyType* parent) {return (parent->sources.emplace_back(), &parent->sources.back());}
void* _get_wareBodyType_restriction(wares::wareBodyType* parent) {return (parent->restriction.emplace_back(), &parent->restriction.back());}
void* _get_wareBodyType_use(wares::wareBodyType* parent) {return (parent->use.emplace_back(), &parent->use.back());}
void* _get_wareBodyType_owner(wares::wareBodyType* parent) {return (parent->owner.emplace_back(), &parent->owner.back());}
void* _get_wareBodyType_illegal(wares::wareBodyType* parent) {return (parent->illegal.emplace_back(), &parent->illegal.back());}
void* _get_wareBodyType_software(wares::wareBodyType* parent) {return (parent->software.emplace_back(), &parent->software.back());}
xsdcpp::ChildElementInfo _wareBodyType_Children[] = {
    {"price", 0, (xsdcpp::get_field_t)&_get_wareBodyType_price, &wares::_priceType_Info, 0, 0},
    {"production", 1, (xsdcpp::get_field_t)&_get_wareBodyType_production, &wares::_wareProductionType_Info, 0, 0},
    {"research", 2, (xsdcpp::get_field_t)&_get_wareBodyType_research, &wares::_researchType_Info, 0, 0},
    {"component", 3, (xsdcpp::get_field_t)&_get_wareBodyType_component, &wares::_componentType_Info, 0, 0},
    {"container", 4, (xsdcpp::get_field_t)&_get_wareBodyType_container, &wares::_containerType_Info, 0, 0},
    {"icon", 5, (xsdcpp::get_field_t)&_get_wareBodyType_icon, &wares::_iconType_Info, 0, 0},
    {"sources", 6, (xsdcpp::get_field_t)&_get_wareBodyType_sources, &wares::_sourcesType_Info, 0, 0},
    {"restriction", 7, (xsdcpp::get_field_t)&_get_wareBodyType_restriction, &wares::_restrictionType_Info, 0, 0},
    {"use", 8, (xsdcpp::get_field_t)&_get_wareBodyType_use, &wares::_useType_Info, 0, 0},
    {"owner", 9, (xsdcpp::get_field_t)&_get_wareBodyType_owner, &wares::_ownerType_Info, 0, 0},
    {"illegal", 10, (xsdcpp::get_field_t)&_get_wareBodyType_illegal, &wares::_illegalType_Info, 0, 0},
    {"software", 11, (xsdcpp::get_field_t)&_get_wareBodyType_software, &wares::_softwareType_Info, 0, 0},
    {nullptr}
};
void* _get_defaultsType_id(wares::defaultsType* elem) { return &elem->id; }
void* _get_defaultsType_name(wares::defaultsType* elem) { return &*(elem->name = xsd::string()); }
void* _get_defaultsType_transport(wares::defaultsType* elem) { return &elem->transport; }
void* _get_defaultsType_volume(wares::defaultsType* elem) { return &elem->volume; }
void* _get_defaultsType_tags(wares::defaultsType* elem) { return &*(elem->tags = xsd::string()); }
xsdcpp::AttributeInfo _defaultsType_Attributes[] = {
    {"id", 1ULL, (xsdcpp::get_field_t)&_get_defaultsType_id, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {"name", 2ULL, (xsdcpp::get_field_t)&_get_defaultsType_name, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"transport", 4ULL, (xsdcpp::get_field_t)&_get_defaultsType_transport, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {"volume", 8ULL, (xsdcpp::get_field_t)&_get_defaultsType_volume, (xsdcpp::set_value_t)&xsdcpp::set_double, true, nullptr},
    {"tags", 16ULL, (xsdcpp::get_field_t)&_get_defaultsType_tags, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {nullptr}
};
void* _get_wareType_id(wares::wareType* elem) { return &elem->id; }
void* _get_wareType_name(wares::wareType* elem) { return &*(elem->name = xsd::string()); }
void* _get_wareType_description(wares::wareType* elem) { return &*(elem->description = xsd::string()); }
void* _get_wareType_factoryname(wares::wareType* elem) { return &*(elem->factoryname = xsd::string()); }
void* _get_wareType_group(wares::wareType* elem) { return &*(elem->group = xsd::string()); }
void* _get_wareType_transport(wares::wareType* elem) { return &elem->transport; }
void* _get_wareType_volume(wares::wareType* elem) { return &elem->volume; }
void* _get_wareType_tags(wares::wareType* elem) { return &*(elem->tags = xsd::string()); }
void* _get_wareType_shortname(wares::wareType* elem) { return &*(elem->shortname = xsd::string()); }
void* _get_wareType_sortorder(wares::wareType* elem) { return &*(elem->sortorder = int64_t()); }
void* _get_wareType_licence(wares::wareType* elem) { return &*(elem->licence = xsd::string()); }
void* _get_wareType_illegal(wares::wareType* elem) { return &*(elem->illegal = xsd::string()); }
xsdcpp::AttributeInfo _wareType_Attributes[] = {
    {"id", 1ULL, (xsdcpp::get_field_t)&_get_wareType_id, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {"name", 2ULL, (xsdcpp::get_field_t)&_get_wareType_name, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"description", 4ULL, (xsdcpp::get_field_t)&_get_wareType_description, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"factoryname", 8ULL, (xsdcpp::get_field_t)&_get_wareType_factoryname, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"group", 16ULL, (xsdcpp::get_field_t)&_get_wareType_group, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"transport", 32ULL, (xsdcpp::get_field_t)&_get_wareType_transport, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {"volume", 64ULL, (xsdcpp::get_field_t)&_get_wareType_volume, (xsdcpp::set_value_t)&xsdcpp::set_double, true, nullptr},
    {"tags", 128ULL, (xsdcpp::get_field_t)&_get_wareType_tags, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"shortname", 256ULL, (xsdcpp::get_field_t)&_get_wareType_shortname, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"sortorder", 512ULL, (xsdcpp::get_field_t)&_get_wareType_sortorder, (xsdcpp::set_value_t)&xsdcpp::set_int64_t, false, nullptr},
    {"licence", 1024ULL, (xsdcpp::get_field_t)&_get_wareType_licence, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"illegal", 2048ULL, (xsdcpp::get_field_t)&_get_wareType_illegal, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {nullptr}
};
void* _get_methodType_default(wares::methodType* parent) {return (parent->default.emplace_back(), &parent->default.back());}
xsdcpp::ChildElementInfo _methodType_Children[] = {
    {"default", 0, (xsdcpp::get_field_t)&_get_methodType_default, &wares::_methodDefaultType_Info, 0, 0},
    {nullptr}
};
void* _get_methodType_id(wares::methodType* elem) { return &elem->id; }
void* _get_methodType_name(wares::methodType* elem) { return &*(elem->name = xsd::string()); }
void* _get_methodType_tags(wares::methodType* elem) { return &*(elem->tags = xsd::string()); }
xsdcpp::AttributeInfo _methodType_Attributes[] = {
    {"id", 1ULL, (xsdcpp::get_field_t)&_get_methodType_id, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {"name", 2ULL, (xsdcpp::get_field_t)&_get_methodType_name, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"tags", 4ULL, (xsdcpp::get_field_t)&_get_methodType_tags, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {nullptr}
};
void* _get_priceType_min(wares::priceType* elem) { return &elem->min; }
void* _get_priceType_average(wares::priceType* elem) { return &elem->average; }
void* _get_priceType_max(wares::priceType* elem) { return &elem->max; }
xsdcpp::AttributeInfo _priceType_Attributes[] = {
    {"min", 1ULL, (xsdcpp::get_field_t)&_get_priceType_min, (xsdcpp::set_value_t)&xsdcpp::set_double, true, nullptr},
    {"average", 2ULL, (xsdcpp::get_field_t)&_get_priceType_average, (xsdcpp::set_value_t)&xsdcpp::set_double, true, nullptr},
    {"max", 4ULL, (xsdcpp::get_field_t)&_get_priceType_max, (xsdcpp::set_value_t)&xsdcpp::set_double, true, nullptr},
    {nullptr}
};
void* _get_wareProductionType_primary(wares::wareProductionType* parent) {return &*(parent->primary = wares::primaryType());}
void* _get_wareProductionType_effects(wares::wareProductionType* parent) {return &*(parent->effects = wares::effectsType());}
void* _get_wareProductionType_research(wares::wareProductionType* parent) {return &*(parent->research = wares::researchType());}
xsdcpp::ChildElementInfo _wareProductionType_Children[] = {
    {"primary", 0, (xsdcpp::get_field_t)&_get_wareProductionType_primary, &wares::_primaryType_Info, 0, 1},
    {"effects", 1, (xsdcpp::get_field_t)&_get_wareProductionType_effects, &wares::_effectsType_Info, 0, 1},
    {"research", 2, (xsdcpp::get_field_t)&_get_wareProductionType_research, &wares::_researchType_Info, 0, 1},
    {nullptr}
};
void* _get_wareProductionType_time(wares::wareProductionType* elem) { return &elem->time; }
void* _get_wareProductionType_amount(wares::wareProductionType* elem) { return &elem->amount; }
void* _get_wareProductionType_method(wares::wareProductionType* elem) { return &elem->method; }
void* _get_wareProductionType_name(wares::wareProductionType* elem) { return &*(elem->name = xsd::string()); }
void* _get_wareProductionType_tags(wares::wareProductionType* elem) { return &*(elem->tags = xsd::string()); }
void* _get_wareProductionType_dismantlefactor(wares::wareProductionType* elem) { return &*(elem->dismantlefactor = double()); }
xsdcpp::AttributeInfo _wareProductionType_Attributes[] = {
    {"time", 1ULL, (xsdcpp::get_field_t)&_get_wareProductionType_time, (xsdcpp::set_value_t)&xsdcpp::set_double, true, nullptr},
    {"amount", 2ULL, (xsdcpp::get_field_t)&_get_wareProductionType_amount, (xsdcpp::set_value_t)&xsdcpp::set_double, true, nullptr},
    {"method", 4ULL, (xsdcpp::get_field_t)&_get_wareProductionType_method, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {"name", 8ULL, (xsdcpp::get_field_t)&_get_wareProductionType_name, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"tags", 16ULL, (xsdcpp::get_field_t)&_get_wareProductionType_tags, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"dismantlefactor", 32ULL, (xsdcpp::get_field_t)&_get_wareProductionType_dismantlefactor, (xsdcpp::set_value_t)&xsdcpp::set_double, false, nullptr},
    {nullptr}
};
void* _get_researchType_primary(wares::researchType* parent) {return (parent->primary.emplace_back(), &parent->primary.back());}
void* _get_researchType_research(wares::researchType* parent) {return (parent->research.emplace_back(), &parent->research.back());}
void* _get_researchType_ware(wares::researchType* parent) {return (parent->ware.emplace_back(), &parent->ware.back());}
xsdcpp::ChildElementInfo _researchType_Children[] = {
    {"primary", 0, (xsdcpp::get_field_t)&_get_researchType_primary, &wares::_primaryType_Info, 0, 0},
    {"research", 1, (xsdcpp::get_field_t)&_get_researchType_research, &wares::_researchType_Info, 0, 0},
    {"ware", 2, (xsdcpp::get_field_t)&_get_researchType_ware, &wares::_wareAmount_Info, 0, 0},
    {nullptr}
};
void* _get_researchType_time(wares::researchType* elem) { return &*(elem->time = double()); }
xsdcpp::AttributeInfo _researchType_Attributes[] = {
    {"time", 1ULL, (xsdcpp::get_field_t)&_get_researchType_time, (xsdcpp::set_value_t)&xsdcpp::set_double, false, nullptr},
    {nullptr}
};
void* _get_componentType_ref(wares::componentType* elem) { return &elem->ref; }
xsdcpp::AttributeInfo _componentType_Attributes[] = {
    {"ref", 1ULL, (xsdcpp::get_field_t)&_get_componentType_ref, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {nullptr}
};
void* _get_containerType_ref(wares::containerType* elem) { return &elem->ref; }
xsdcpp::AttributeInfo _containerType_Attributes[] = {
    {"ref", 1ULL, (xsdcpp::get_field_t)&_get_containerType_ref, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {nullptr}
};
void* _get_iconType_active(wares::iconType* elem) { return &*(elem->active = xsd::string()); }
void* _get_iconType_video(wares::iconType* elem) { return &*(elem->video = xsd::string()); }
xsdcpp::AttributeInfo _iconType_Attributes[] = {
    {"active", 1ULL, (xsdcpp::get_field_t)&_get_iconType_active, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"video", 2ULL, (xsdcpp::get_field_t)&_get_iconType_video, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {nullptr}
};
void* _get_sourcesType_source(wares::sourcesType* parent) {return (parent->source.emplace_back(), &parent->source.back());}
xsdcpp::ChildElementInfo _sourcesType_Children[] = {
    {"source", 0, (xsdcpp::get_field_t)&_get_sourcesType_source, &wares::_sourceType_Info, 0, 0},
    {nullptr}
};
void* _get_restrictionType_licence(wares::restrictionType* elem) { return &*(elem->licence = xsd::string()); }
void* _get_restrictionType_buy(wares::restrictionType* elem) { return &*(elem->buy = xsd::string()); }
xsdcpp::AttributeInfo _restrictionType_Attributes[] = {
    {"licence", 1ULL, (xsdcpp::get_field_t)&_get_restrictionType_licence, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"buy", 2ULL, (xsdcpp::get_field_t)&_get_restrictionType_buy, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {nullptr}
};
void* _get_useType_threshold(wares::useType* elem) { return &*(elem->threshold = double()); }
void* _get_useType_factions(wares::useType* elem) { return &*(elem->factions = xsd::string()); }
void* _get_useType_purposes(wares::useType* elem) { return &*(elem->purposes = xsd::string()); }
xsdcpp::AttributeInfo _useType_Attributes[] = {
    {"threshold", 1ULL, (xsdcpp::get_field_t)&_get_useType_threshold, (xsdcpp::set_value_t)&xsdcpp::set_double, false, nullptr},
    {"factions", 2ULL, (xsdcpp::get_field_t)&_get_useType_factions, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {"purposes", 4ULL, (xsdcpp::get_field_t)&_get_useType_purposes, (xsdcpp::set_value_t)&xsdcpp::set_string, false, nullptr},
    {nullptr}
};
void* _get_ownerType_faction(wares::ownerType* elem) { return &elem->faction; }
xsdcpp::AttributeInfo _ownerType_Attributes[] = {
    {"faction", 1ULL, (xsdcpp::get_field_t)&_get_ownerType_faction, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {nullptr}
};
void* _get_illegalType_factions(wares::illegalType* elem) { return &elem->factions; }
xsdcpp::AttributeInfo _illegalType_Attributes[] = {
    {"factions", 1ULL, (xsdcpp::get_field_t)&_get_illegalType_factions, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {nullptr}
};
void* _get_softwareType_predecessor(wares::softwareType* elem) { return &elem->predecessor; }
xsdcpp::AttributeInfo _softwareType_Attributes[] = {
    {"predecessor", 1ULL, (xsdcpp::get_field_t)&_get_softwareType_predecessor, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {nullptr}
};
void* _get_methodDefaultType_race(wares::methodDefaultType* elem) { return &elem->race; }
xsdcpp::AttributeInfo _methodDefaultType_Attributes[] = {
    {"race", 1ULL, (xsdcpp::get_field_t)&_get_methodDefaultType_race, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {nullptr}
};
void* _get_primaryType_ware(wares::primaryType* parent) {return (parent->ware.emplace_back(), &parent->ware.back());}
xsdcpp::ChildElementInfo _primaryType_Children[] = {
    {"ware", 0, (xsdcpp::get_field_t)&_get_primaryType_ware, &wares::_wareAmount_Info, 0, 0},
    {nullptr}
};
void* _get_effectsType_effect(wares::effectsType* parent) {return (parent->effect.emplace_back(), &parent->effect.back());}
xsdcpp::ChildElementInfo _effectsType_Children[] = {
    {"effect", 0, (xsdcpp::get_field_t)&_get_effectsType_effect, &wares::_effectType_Info, 0, 0},
    {nullptr}
};
void* _get_wareAmount_ware(wares::wareAmount* elem) { return &elem->ware; }
void* _get_wareAmount_amount(wares::wareAmount* elem) { return &*(elem->amount = double()); }
xsdcpp::AttributeInfo _wareAmount_Attributes[] = {
    {"ware", 1ULL, (xsdcpp::get_field_t)&_get_wareAmount_ware, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {"amount", 2ULL, (xsdcpp::get_field_t)&_get_wareAmount_amount, (xsdcpp::set_value_t)&xsdcpp::set_double, false, nullptr},
    {nullptr}
};
void* _get_sourceType_type(wares::sourceType* elem) { return &elem->type; }
xsdcpp::AttributeInfo _sourceType_Attributes[] = {
    {"type", 1ULL, (xsdcpp::get_field_t)&_get_sourceType_type, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {nullptr}
};
void* _get_effectType_type(wares::effectType* elem) { return &elem->type; }
void* _get_effectType_product(wares::effectType* elem) { return &*(elem->product = double()); }
xsdcpp::AttributeInfo _effectType_Attributes[] = {
    {"type", 1ULL, (xsdcpp::get_field_t)&_get_effectType_type, (xsdcpp::set_value_t)&xsdcpp::set_string, true, nullptr},
    {"product", 2ULL, (xsdcpp::get_field_t)&_get_effectType_product, (xsdcpp::set_value_t)&xsdcpp::set_double, false, nullptr},
    {nullptr}
};

}

namespace wares {

const xsdcpp::ElementInfo _waresType_Info = { xsdcpp::ElementInfo::EntryPointFlag, nullptr, _waresType_Children, 3, nullptr, 0ULL, nullptr, nullptr };
const xsdcpp::ElementInfo __root_t_Info = { xsdcpp::ElementInfo::CheckChildrenFlag, nullptr, __root_t_Children, 1, nullptr, 0ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _productionRootType_Info = { 0, nullptr, _productionRootType_Children, 1, nullptr, 0ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _wareBodyType_Info = { 0, nullptr, _wareBodyType_Children, 12, nullptr, 0ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _defaultsType_Info = { 0, nullptr, nullptr, 12, _defaultsType_Attributes, 13ULL, &wares::_wareBodyType_Info, nullptr };
const xsdcpp::ElementInfo _wareType_Info = { 0, nullptr, nullptr, 12, _wareType_Attributes, 97ULL, &wares::_wareBodyType_Info, nullptr };
const xsdcpp::ElementInfo _methodType_Info = { 0, nullptr, _methodType_Children, 1, _methodType_Attributes, 1ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _priceType_Info = { 0, nullptr, nullptr, 0, _priceType_Attributes, 7ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _wareProductionType_Info = { 0, nullptr, _wareProductionType_Children, 3, _wareProductionType_Attributes, 7ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _researchType_Info = { 0, nullptr, _researchType_Children, 3, _researchType_Attributes, 0ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _componentType_Info = { 0, nullptr, nullptr, 0, _componentType_Attributes, 1ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _containerType_Info = { 0, nullptr, nullptr, 0, _containerType_Attributes, 1ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _iconType_Info = { 0, nullptr, nullptr, 0, _iconType_Attributes, 0ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _sourcesType_Info = { 0, nullptr, _sourcesType_Children, 1, nullptr, 0ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _restrictionType_Info = { 0, nullptr, nullptr, 0, _restrictionType_Attributes, 0ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _useType_Info = { 0, nullptr, nullptr, 0, _useType_Attributes, 0ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _ownerType_Info = { 0, nullptr, nullptr, 0, _ownerType_Attributes, 1ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _illegalType_Info = { 0, nullptr, nullptr, 0, _illegalType_Attributes, 1ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _softwareType_Info = { 0, nullptr, nullptr, 0, _softwareType_Attributes, 1ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _methodDefaultType_Info = { 0, nullptr, nullptr, 0, _methodDefaultType_Attributes, 1ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _primaryType_Info = { 0, nullptr, _primaryType_Children, 1, nullptr, 0ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _effectsType_Info = { 0, nullptr, _effectsType_Children, 1, nullptr, 0ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _wareAmount_Info = { 0, nullptr, nullptr, 0, _wareAmount_Attributes, 1ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _sourceType_Info = { 0, nullptr, nullptr, 0, _sourceType_Attributes, 1ULL, nullptr, nullptr };
const xsdcpp::ElementInfo _effectType_Info = { 0, nullptr, nullptr, 0, _effectType_Attributes, 1ULL, nullptr, nullptr };

void load_data(const std::string& data, waresType& output)
{
    _root_t rootElement;
    xsdcpp::ElementContext elementContext(&__root_t_Info, &rootElement);
    xsdcpp::parse(data.c_str(), _namespaces, elementContext);
    output = std::move(rootElement.wares);
}

void load_file(const std::string& filePath, waresType& output)
{
    load_data(xsdcpp::read_file(filePath), output);
}

}

