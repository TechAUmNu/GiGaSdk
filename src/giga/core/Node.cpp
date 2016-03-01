/*
 * Node.cpp
 *
 *  Created on: 4 févr. 2016
 *      Author: thomas
 */

#include "Node.h"
#include "FileNode.h"
#include "FolderNode.h"
#include "../api/data/Node.h"
#include "../api/NodesApi.h"
#include "../rest/HttpErrors.h"
#include "../utils/EnumConvertor.h"

#include <boost/filesystem.hpp>
#include <chrono>
#include <string>

using std::chrono::_V2::system_clock;
using utility::string_t;

#define _THROW_IF_NO_NODE_ if (_data == nullptr) { BOOST_THROW_EXCEPTION(ErrorException{"Node is not set"}); } do {} while(0)

namespace giga
{

const utils::EnumConvertor<core::Node::Type, 3> core::Node::typeCvrt =
    {"root", "folder", "file"};

const utils::EnumConvertor<core::Node::MediaType, 6> core::Node::mediaTypeCvrt =
    {"audio", "document", "video", "image", "unknown", "folder"};

namespace core
{

std::unique_ptr<Node>
Node::create (std::shared_ptr<data::Node> n)
{
    switch (typeCvrt.fromStr(n->type)) {
        case Type::file:
            return std::unique_ptr<Node>{new FileNode{n}};
        case Type::folder:
        case Type::root:
            return std::unique_ptr<Node>{new FolderNode{n}};
    }
    BOOST_THROW_EXCEPTION(ErrorException{"unreachable"});
}

Node::Node (std::shared_ptr<data::Node> n)  :
_data(n)
{
    _THROW_IF_NO_NODE_;
}

Node::Node(const Node& rhs) :
        _data(nullptr)
{
    if (rhs._data != nullptr)
    {
        _data = std::make_shared<data::Node>(*rhs._data);
    }
}

Node&
Node::operator=(const Node& rhs)
{
    if (rhs._data != nullptr)
    {
        _data = std::make_shared<data::Node>(*rhs._data);
    }
    return *this;
}

const string_t&
Node::id () const
{
    _THROW_IF_NO_NODE_;
    return _data->id;
}

Node::Type
Node::type () const
{
    _THROW_IF_NO_NODE_;
    return typeCvrt.fromStr(_data->type);
}

const string_t&
Node::name () const
{
    _THROW_IF_NO_NODE_;
    return _data->name;
}

const string_t&
Node::parentId () const
{
    _THROW_IF_NO_NODE_;
    return _data->parentId.get_value_or("");
}

const std::vector<string_t>&
Node::ancestors () const
{
    _THROW_IF_NO_NODE_;
    return _data->ancestors;
}

int64_t
Node::ownerId () const
{
    _THROW_IF_NO_NODE_;
    return _data->ownerId;
}

std::chrono::system_clock::time_point
Node::creationDate () const
{
    _THROW_IF_NO_NODE_;
    return system_clock::time_point(std::chrono::seconds(_data->creationDate));
}

std::chrono::system_clock::time_point
Node::lastUpdateDate () const
{
    return system_clock::time_point(std::chrono::seconds(_data->lastUpdateDate));
}

uint64_t
Node::nbChildren () const
{
    _THROW_IF_NO_NODE_;
    return _data->nbChildren;
}

uint64_t
Node::nbFiles () const
{
    _THROW_IF_NO_NODE_;
    return _data->nbFiles;
}

uint64_t
Node::size () const
{
    _THROW_IF_NO_NODE_;
    return _data->size;
}

bool
Node::shouldLoadChildren() const
{
    return nbChildren() > 0 && children().size() < nbChildren();
}

void
Node::remove()
{
    NodesApi::deleteNode(id()).get();
    _data->id = "";
}

void
Node::rename(const string_t& name)
{
    if (!boost::filesystem::portable_name(name))
    {
        BOOST_THROW_EXCEPTION(ErrorException{"Name is not valid"});
    }
    NodesApi::renameNode(id(), name).get();
    _data->name = name;
}

} /* namespace core */
} /* namespace giga */
