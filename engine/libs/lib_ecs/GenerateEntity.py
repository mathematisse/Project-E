#
# EPITECH PROJECT, 2024
# CPP-ECS
# File description:
# Python utils file generating entities (pool + ref)

import os
import sys

HPP_TEMPLATE = """/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Header for {entity_name} entity declarations
*/

#pragma once

#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/Entities/AEntityRef.hpp"
#include "lib_ecs/Entities/AEntityPool.hpp"

namespace ECS::C
{{
    {new_components_declarations}
}}

namespace ECS::E
{{
    const size_t {entity_name}ChunkSize = {chunk_size};

    // ENTITY REF
    class {entity_name}Ref : public AEntityRef
    {{
    public:
        {entity_name}Ref(
            C::EntityStatusRef *status,
            C::ChunkPosRef *cPos,
            {constructor_args}
        );
        ~{entity_name}Ref() override;

        // Default rule of five
        {entity_name}Ref(const {entity_name}Ref &other) = default;
        {entity_name}Ref &operator=(const {entity_name}Ref &other) = default;
        {entity_name}Ref({entity_name}Ref &&other) = default;
        {entity_name}Ref &operator=({entity_name}Ref &&other) = default;

        // Accessors
        {accessors}

    protected:
        {component_refs}
    }};

    // ENTITY POOL
    class {entity_name}Pool : public AEntityPool
    {{
    public:
        {entity_name}Pool();
        ~{entity_name}Pool() override = default;

        // default rule of five
        {entity_name}Pool(const {entity_name}Pool &other) = default;
        {entity_name}Pool &operator=(const {entity_name}Pool &other) = default;
        {entity_name}Pool({entity_name}Pool &&other) = default;
        {entity_name}Pool &operator=({entity_name}Pool &&other) = default;

        std::unique_ptr<E::IEntityRef> getEntity(Chunks::chunkPos_t cPos) override;
        std::unique_ptr<E::{entity_name}Ref> getRawEntity(Chunks::chunkPos_t cPos);
        std::vector<C::IComponentPool *> getComponentPools() override;
    protected:
        {component_pools}
    }};
}}

"""

CPP_TEMPLATE = """/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Source for {entity_name} entity declarations
*/

#include "lib_ecs/{entity_name}.hpp"

// ENTITY
namespace ECS::E
{{
    
    // ENTITY REF
    {entity_name}Ref::{entity_name}Ref(
        C::EntityStatusRef *status,
        C::ChunkPosRef *cPos,
        {constructor_args}
    ) : AEntityRef(status, cPos),
        {constructor_decl}
    {{
    }}

    {entity_name}Ref::~{entity_name}Ref()
    {{
        {delete_components}
    }}

    {accessors}

    
    // ENTITY POOL
    {entity_name}Pool::{entity_name}Pool()
        : AEntityPool("{entity_name}", {entity_name}ChunkSize)
    {{
    }}

    std::unique_ptr<E::IEntityRef> {entity_name}Pool::getEntity(Chunks::chunkPos_t cPos)
    {{
        return getRawEntity(cPos);
    }}

    std::unique_ptr<E::{entity_name}Ref> {entity_name}Pool::getRawEntity(Chunks::chunkPos_t cPos)
    {{
        auto ptr = std::make_unique<E::{entity_name}Ref>(
            static_cast<C::EntityStatusRef *>(_entityStatusPool.getComponentRef(cPos)),
            static_cast<C::ChunkPosRef *>(_chunkPosPool.getComponentRef(cPos)),
            {get_raw_ents}
        );
        return ptr;
    }}

    std::vector<C::IComponentPool *> {entity_name}Pool::getComponentPools()
    {{
        return {{
            &_entityStatusPool,
            &_chunkPosPool,
            {get_component_pools}
        }};
    }}
}}
"""


#function to turn AnyString into anyString
def to_lower_camel_case(string: str) -> str:
    return string[0].lower() + string[1:]

def get_hpp_accessors(entity_name: str, components: list[str]) -> str:
    return ''.join([f"""
        [[nodiscard]] C::{c}Ref *get{c}() const;
        void set{c}(const C::{c}Ref &{to_lower_camel_case(c)});
""" for c in components])

def get_cpp_accessors(entity_name: str, components: list[str]) -> str:
    return ''.join([f"""
    [[nodiscard]] C::{c}Ref *E::{entity_name}Ref::get{c}() const
    {{
        return _{to_lower_camel_case(c)};
    }}

    void E::{entity_name}Ref::set{c}(const C::{c}Ref &{to_lower_camel_case(c)})
    {{
        *_{to_lower_camel_case(c)} = {to_lower_camel_case(c)};
    }}
""" for c in components])

def get_hpp_content(entity_name, components, new_components_declarations, chunk_size):
    return HPP_TEMPLATE.format(
        entity_name=entity_name,
        chunk_size=chunk_size,
        new_components_declarations=new_components_declarations,
        constructor_args=",\n            ".join([f"C::{c}Ref *{to_lower_camel_case(c)}" for c in components]),
        accessors=get_hpp_accessors(entity_name, components),
        component_refs="\n        ".join([f"C::{c}Ref *_{to_lower_camel_case(c)};" for c in components]),
        component_pools="\n        ".join([f"C::{c}Pool _{to_lower_camel_case(c)}Pool;" for c in components])
    )

def get_cpp_content(entity_name, components):
    return CPP_TEMPLATE.format(
        entity_name=entity_name,
        constructor_args=",\n        ".join([f"C::{c}Ref *{c}" for c in components]),
        constructor_decl=",\n        ".join([f"_{to_lower_camel_case(c)}({c})" for c in components]),
        delete_components="\n        ".join([f"delete _{to_lower_camel_case(c)};" for c in components]),
        accessors=get_cpp_accessors(entity_name, components),
        get_raw_ents=",\n            ".join([f"static_cast<C::{c}Ref *>(_{to_lower_camel_case(c)}Pool.getComponentRef(cPos))" for c in components]),
        get_component_pools=",\n            ".join([f"&_{to_lower_camel_case(c)}Pool" for c in components])
    )

DIM_GREY_TEXT = "\033[90m"
RESET_COLOR = "\033[0m"

def ask_user_query(query: str, default: str = "") -> str:
    if default:
        query += f" [{default}]"
    answer = input(query + "\n > ")
    return answer if answer else default

def ask_user_number(query: str, default: str = "0") -> int:
    try:
        return int(ask_user_query(query, default))
    except ValueError:
        print("Please enter a valid number.")
        return ask_user_number(query, default)

def ask_user_yes_no(query: str, default: str = "no") -> bool:
    answer = ask_user_query(query, default)
    if answer.lower() in ["yes", "y"]:
        return True
    elif answer.lower() in ["no", "n"]:
        return False
    else:
        print("Please enter yes or no.")
        return ask_user_yes_no(query, default)

def ask_user_name(query: str) -> str:
    name = ask_user_query(query)
    if name == "":
        print("Please enter a valid name.")
        return ask_user_name(query)
    return name[0].upper() + name[1:]


def ask_user():
    entity_name = ask_user_name("Entity name:")
    components = ask_user_query(f"Components (comma separated):\n{DIM_GREY_TEXT}EntityStatus and EntityPoolId alrerady included.{RESET_COLOR}").split(", ")
    new_comp_decls = ""
    nem_comp_names = ""
    if ask_user_yes_no("Is there some new components ?"):
        _temp_arr = []
        for comp in components:
            if not ask_user_yes_no(f"Is {comp} a new component ?"):
                continue
            comp_types = ask_user_query(f"Types for {comp} (comma separated):").split(", ")
            _temp_arr.append((comp, comp_types))
        new_comp_decls = "\n    ".join([f"DECLARE_COMPONENT({comp}, {', '.join(comp_types)});" for comp, comp_types in _temp_arr])
    chunk_size = ask_user_number("Chunk size: ", "32")

    return entity_name, components, chunk_size, new_comp_decls

def demo_run():
    entity_name, components, chunk_size, new_comp_decls = ask_user()

    print(f"Generating {entity_name}.hpp and {entity_name}.cpp")
    print(f"Components: {components}")
    print(f"Chunk size: {chunk_size}")
    print(f"New components declarations:\n{new_comp_decls}")

    hpp_content = get_hpp_content(entity_name, components, new_comp_decls, chunk_size)
    cpp_content = get_cpp_content(entity_name, components)

    # ensure Demo folder exists
    if not os.path.exists("src/Demo"):
        os.makedirs("src/Demo")

    with open(f"src/Demo/{entity_name}.hpp", "w") as hpp_file:
        hpp_file.write(hpp_content)

    with open(f"src/Demo/{entity_name}.cpp", "w") as cpp_file:
        cpp_file.write(cpp_content)

if __name__ == "__main__":
    demo_run()

