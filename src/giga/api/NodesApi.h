/**
 * @author Thomas Guyard <t.guyard@gigatribe.com>
 */

#ifndef CLIENT_NODESAPI_API_H_
#define CLIENT_NODESAPI_API_H_

#include <cpprest/details/basic_types.h>
#include "GigaApi.h"

namespace giga
{

namespace data {
struct NodeList;
struct Node;
struct DataNode;
struct Preview;
struct Timeline;
struct IdContainer;
}

class NodesApi final : GigaApi
{
public:
    static pplx::task<std::shared_ptr<data::NodeList>>
    searchNode (const utility::string_t& search, const utility::string_t& mine, const std::string& inFolder, int64_t ownerId);

    static pplx::task<std::shared_ptr<std::vector<data::Node>>>
    searchNodeByType (const utility::string_t& search, const utility::string_t& type, uint16_t max = 50, uint32_t offset = 0);

    static pplx::task<std::shared_ptr<data::DataNode>>
    addNode (const utility::string_t& name, const utility::string_t& type, const std::string& parentId, const std::string& fkey,
             const std::string& fid);

    static pplx::task<std::shared_ptr<data::DataNode>>
    addFolderNode (const utility::string_t& name, const std::string& parentId);

    static pplx::task<std::shared_ptr<data::DataNode>>
    copyNode (const std::string& fromNodeId, const std::string& toNodeId, const utility::string_t& copy, const utility::string_t& cut,
              const std::string& myNodeKey, const std::string& otherNodeKey);

    static pplx::task<std::shared_ptr<data::Node>>
    getNodeById (const std::string& nodeId);

    static pplx::task<std::shared_ptr<data::Node>>
    renameNode (const std::string& nodeId, const utility::string_t& name);

    static pplx::task<std::shared_ptr<data::IdContainer>>
    deleteNode (const std::string& nodeId);

    static pplx::task<std::shared_ptr<std::vector<data::Node>>>
    getChildrenNode (const std::string& nodeId);

    static pplx::task<std::shared_ptr<data::Preview>>
    getPreviewsData (const std::string& nodeId);

    static pplx::task<std::shared_ptr<data::Timeline>>
    getTimeline (const utility::string_t& head, int64_t from, int64_t owner);
};
} // namespace giga

#endif
