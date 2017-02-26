/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_schema_220217125130
#define KSERGEY_schema_220217125130

#include <cstdint>
#include <string>
#include <unordered_map>
#include <pugixml.hpp>
#include "static_vector.hpp"

namespace fast {

/** FAST message template schema */
class message_template
{
private:
    const std::string name_;
    const std::uint32_t id_;

public:
    message_template(const message_template&) = delete;
    message_template& operator=(const message_template&) = delete;

    /** Construct message template */
    message_template(const std::string& name, std::uint32_t id)
        : name_{name}
        , id_{id}
    {}
};

/** FAST protocol template schema */
class schema
{
private:
    /* Mapping template id -> index in vector */
    std::unordered_map< std::uint32_t, std::size_t > mapping_;
    /* Messages storage */
    static_vector< message_template > templates_;

public:
    schema() = default;

    /** Construct schema from file */
    explicit schema(const std::string& path);

private:
    void parse_templates(pugi::xml_node node);
    void parse_template(pugi::xml_node node);
    message_template& create_message_template(const char* name, std::uint32_t id);
};

} /* namespace fast */

#endif /* KSERGEY_schema_220217125130 */
