/*
 * Copyright 2016 Gigatribe
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GIGA_CORE_FILEDOWNLOADER_H_
#define GIGA_CORE_FILEDOWNLOADER_H_

#include "FileTransferer.h"

#include <boost/filesystem.hpp>
#include <cpprest/http_client.h>
#include <pplx/pplxtasks.h>
#include <chrono>

namespace giga
{
class Application;

namespace core
{
class Node;

/**
 * Download a file
 */
class FileDownloader : public FileTransferer
{
public:
    enum class Policy
    {
        /**
         * If a file with the same name is found, override it.
         */
        override,

        /**
         * Override a file with the same name if:
         *  - its size is different from the downloading one
         *  - *or* its last modification date is older than the downloading one
         *
         *  Else ignore the file (no download).
         */
        overrideNewerSize,

        /**
         * Do not download files that already exists.
         */
        ignore,

        /**
         * Rename the downloading file if needed to avoid collision.
         */
        rename
    };

    enum class Action
    {
        fileOverriden, fileIgnored, fileRenamed, fileDownloaded
    };

    struct Result
    {
        explicit Result(boost::filesystem::path path, Action action) :
                path{path}, action{action} {}
        explicit Result()                = default;
        Result(Result&&)                 = default;
        Result(const Result&)            = default;
        Result& operator=(Result&&)      = default;
        Result& operator=(const Result&) = default;

        boost::filesystem::path path   = {};
        Action                  action = Action::fileDownloaded;
    };

public:
    /**
     * @brief Construct a FileDownloader
     * @param folderDest the folder in which we want to put the downloaded node
     * @param node the node to download
     * @param app the authenticated application
     * @param cts a cancel token to use for canceling the download task
     * @param policy what to do if a file with the same name already exists
     */
    explicit
    FileDownloader (const boost::filesystem::path& folderDest, const Node& node, const Application& app,
                    pplx::cancellation_token_source cts = pplx::cancellation_token_source{}, Policy policy = Policy::ignore);
    virtual ~FileDownloader();
    FileDownloader (FileDownloader&& other);

    FileDownloader(const FileDownloader&)            = delete;
    FileDownloader& operator=(const FileDownloader&) = delete;
    FileDownloader& operator=(FileDownloader&&)      = delete;

public:

    /**
     * @brief Gets the task managing the download.
     * Make sure it has been started first (see ```FileTransferer::start()```)
     * @see FileTransferer::start()
     * @see https://github.com/Microsoft/cpprestsdk
     * @see http://microsoft.github.io/cpprestsdk/classpplx_1_1task.html
     */
    const pplx::task<Result>&
    task () const;

    /**
     * During the download process, we use this temporary file
     */
    const boost::filesystem::path&
    downloadingFile () const;

    /**
     * At the end of the download process, the file will be here
     */
    const boost::filesystem::path&
    destinationFile () const;

    FileTransferer::Progress
    progress () const override;

    const boost::filesystem::path&
    filename() const override;

protected:
    void
    doStart () override;

private:
    pplx::task<Result>       _task;

    boost::filesystem::path  _tempFile;
    boost::filesystem::path  _destFile;
    Action                   _action;
    web::uri                 _fileUri;
    uint64_t                 _fileSize;
    uint64_t                 _startAt;
    std::chrono::system_clock::time_point _lastUpdateDate;
    Policy                   _policy;
    const Application*       _app;
};

} /* namespace core */
} /* namespace giga */

#endif /* GIGA_CORE_FILEDOWNLOADER_H_ */
