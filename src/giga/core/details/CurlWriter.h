/*
 * CurlWriter.h
 *
 *  Created on: 18 févr. 2016
 *      Author: thomas
 */

#ifndef GIGA_CORE_DETAILS_CURLWRITER_H_
#define GIGA_CORE_DETAILS_CURLWRITER_H_

#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>

namespace curl
{
class curl_easy;
}

namespace giga
{
namespace details
{

class CurlWriter
{
public:
    explicit CurlWriter(const boost::filesystem::path& path);
    ~CurlWriter();
    CurlWriter(const CurlWriter&)   = delete;
    CurlWriter(CurlWriter&&)        = delete;

    int
    write (const char* contents, size_t size) noexcept;

    std::string
    getErrorData () const;

    void
    setCurl (curl::curl_easy& curl);

    std::ofstream&
    file();

private:
    std::ofstream       _file;
    std::ostringstream  _stream;
    curl::curl_easy*    _curl;
    long                _httpCode;

};

} /* namespace details */
} /* namespace giga */

#endif /* GIGA_CORE_DETAILS_CURLWRITER_H_ */
