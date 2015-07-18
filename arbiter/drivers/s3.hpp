#pragma once

#include <memory>
#include <string>
#include <vector>

#ifndef ARBITER_IS_AMALGAMATION
#include <arbiter/driver.hpp>
#include <arbiter/drivers/http.hpp>
#endif

namespace arbiter
{

class AwsAuth
{
public:
    AwsAuth(std::string access, std::string hidden);

    std::string access() const;
    std::string hidden() const;

private:
    std::string m_access;
    std::string m_hidden;
};

typedef std::map<std::string, std::string> Query;

class S3Driver : public Driver
{
public:
    S3Driver(HttpPool& pool, AwsAuth awsAuth);

    virtual std::string type() const { return "s3"; }
    virtual std::vector<char> getBinary(std::string path) const;
    virtual void put(std::string path, const std::vector<char>& data) const;

private:
    virtual std::vector<std::string> glob(std::string path, bool verbose) const;

    std::vector<char> get(std::string path, const Query& query) const;

    Headers httpGetHeaders(std::string filePath) const;
    Headers httpPutHeaders(std::string filePath) const;

    std::string getHttpDate() const;

    std::string getSignedEncodedString(
            std::string command,
            std::string file,
            std::string httpDate,
            std::string contentType = "") const;

    std::string getStringToSign(
            std::string command,
            std::string file,
            std::string httpDate,
            std::string contentType) const;

    std::vector<char> signString(std::string input) const;
    std::string encodeBase64(std::vector<char> input) const;

    HttpPool& m_pool;
    AwsAuth m_auth;
};

} // namespace arbiter
