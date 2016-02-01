/*
 * Application.h
 *
 *  Created on: 20 janv. 2016
 *      Author: thomas
 */

#ifndef GIGA_CORE_APPLICATION_H_
#define GIGA_CORE_APPLICATION_H_

#include "core/User.h"
#include "Config.h"

#include <cpprest/http_client.h>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace giga
{

namespace data {
struct UsersRelation;
}

class Application
{
public:

    static Application&
    init(std::string&& appRedirectUri, std::string&& appId, std::string&& appKey, std::string&& appScope =
            "basic network groups files basic:write network:write groups:write files:write");

    static Application&
    get();

private:
    explicit Application()                  = default;
    Application(Application const&)         = delete;
    void operator=(Application const&)      = delete;

    static Application&
    instance ()
    {
        static Application _instance {};
        return _instance;
    }

public:
    Config&
    config ();

    bool
    isInitialized() const;

    core::User&
    authenticate (const std::string& login, const std::string& password);

    core::User&
    currentUser();

    core::User
    getUserById (int64_t id) const;

    core::User
    getUserByLogin (const std::string& login) const;

    std::vector<core::User>
    getContacts () const;

    std::vector<core::User>
    getInvitingUsers () const;

    std::vector<core::User>
    getInvitedUsers () const;

    std::vector<core::User>
    getSuggestedUsers () const;

    std::vector<core::User>
    getBlockedUsers () const;

private:
    core::User _currentUser;
    Config _config;
    bool _isInitialized = false;
};

} /* namespace giga */

#endif /* GIGA_CORE_APPLICATION_H_ */
