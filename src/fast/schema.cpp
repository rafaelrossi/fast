/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include "schema.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace fast {

using namespace std::string_literals;

inline namespace xml_helpers {

/* Some xml helpers */

template< class T >
__force_inline T as(const char* value)
{
    std::istringstream ss{value};
    T res;
    ss >> res;
    if (__unlikely(!ss || ss.rdbuf()->in_avail() != 0)) {
        throw std::runtime_error("Failed to convert \""s
                + value + "\" to desired type");
    }
    return res;
}

template<>
__force_inline const char* as(const char* value)
{ return value; }

template< class T >
__force_inline T attr(pugi::xml_node node, const char* attribute_name)
{
    auto attribute = node.attribute(attribute_name);
    if (__unlikely(!attribute)) {
        throw std::runtime_error("Attribute \""s + attribute_name
                + "\" not found in node \"" + node.name() + '"');
    }
    return as< T >(attribute.value());
}

template< class T >
__force_inline T attr(pugi::xml_node node, const char* attribute_name, T def)
{
    auto attribute = node.attribute(attribute_name);
    if (!attribute) {
        return def;
    }
    return as< T >(attribute.value());
}

} /* namespace xml_helpers */

schema::schema(const std::string& path)
{
    pugi::xml_document doc;
    auto result = doc.load_file(path.c_str());
    if (__unlikely(!result)) {
        throw std::runtime_error("Can't open file "s + path + " ("
                + result.description() + ")");
    }

    parse_templates(doc.child("templates"));
}

void schema::parse_templates(pugi::xml_node node)
{
    if (__unlikely(!node)) {
        throw std::runtime_error("Invalid template data (document root)");
    }

    for (auto template_node: node.children("template")) {
        parse_template(template_node);
    }
}

void schema::parse_template(pugi::xml_node node)
{
    if (__unlikely("template"s != node.name())) {
        throw std::runtime_error("Invalid template data (template node)");
    }

    std::cout << "Template found "
        << attr< const char* >(node, "name") << ' '
        << attr< unsigned >(node, "id") << '\n';
}

__force_inline message_template& schema::create_message_template(const char* name, std::uint32_t id)
{
    /* Calculate template index in storage */
    std::size_t index = templates_.size();
    /* Create message template */
    auto& tmpl = templates_.emplace_back(name, id);

    /* Create index mapping */
    auto result = mapping_.emplace(id, index);
    if (__unlikely(!result.second)) {
        throw std::runtime_error("Message template " + std::to_string(id) + " already defined");
    }

    return tmpl;
}

} /* namespace fast */
