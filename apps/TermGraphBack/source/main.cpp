#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QString>

#include <restinio/all.hpp>

#include <TermDataStorage/LocalDataStorage.h>

#include <QDebug>

using namespace restinio;

// TODO: Give address and port through agrument
// TODO: Encoding!
// TODO: Add getFreeUuid for groups
// TODO: Add getFreeUuid for nodes
// TODO: Use boost outcome::result class
// TODO: Add standard database errors
// TODO: Remove second version of sql executor
// TODO: Add bool result for deletion
// TODO: Watch about error codes

std::string jsonObjToStr(QJsonObject obj)
{
    QJsonDocument doc(obj);
    return QString(doc.toJson()).toStdString();
}

std::optional<QUuid> uuidFromStr(std::string param)
{
    auto uuidStr = QString::fromStdString(param);

    uuidStr.replace("%7B", "");
    uuidStr.replace("%7D", "");

    uuidStr.prepend('{');
    uuidStr.append('}');

    auto ret = QUuid(uuidStr);

    if (ret.isNull())
        return std::nullopt;

    assert(!ret.isNull());

    return ret;
}

std::optional<GroupUuid> strToGroupUuid(std::string param)
{
    if (auto uuid = uuidFromStr(param)) {
        if (auto gUuid = GroupUuid::create(*uuid)) {
            return gUuid;
        }
    }

    return std::nullopt;
}

std::optional<TermUuid> strToTermUuid(std::string param)
{
    if (auto uuid = uuidFromStr(param)) {
        if (auto tUuid = TermUuid::create(*uuid)) {
            return tUuid;
        }
    }

    return std::nullopt;
}

std::optional<QJsonObject> strToJsonObj(std::string jsonStr)
{
    auto bytes = QByteArray::fromStdString(jsonStr);
    auto doc   = QJsonDocument::fromJson(bytes);

    if (doc.isNull())
        return std::nullopt;

    return doc.object();
}

std::string arrayToString(QString rootKey, QJsonArray data)
{
    QJsonObject obj;
    obj.insert(rootKey, data);
    return jsonObjToStr(obj);
}

template<typename UuidContainer>
std::string uuidListToStdString(QString rootKey, UuidContainer uuidList)
{
    QJsonArray arr;

    for (auto uuid : uuidList)
        arr.push_back(uuid.toString());

    return arrayToString(rootKey, arr);
}

std::string containerToStdString(QString rootKey, auto container)
{
    QJsonArray arr;

    for (auto item : container)
        arr.push_back(item.toJson());

    return arrayToString(rootKey, arr);
}

http_status_line_t dbErrToHttpErr(std::error_code code)
{
    using namespace restinio;

    if (code == DbErrorCodes::UuidEmpty)
        return status_bad_request();

    if (code == DbErrorCodes::UuidAlreadyExist)
        return status_conflict();

    if (code == DbErrorCodes::UuidNotExist)
        return status_not_found();

    if (code == DbErrorCodes::GroupNameEmpty)
        return status_bad_request();

    if (code == DbErrorCodes::GroupNameAlreadyExist)
        return status_conflict();

    if (code == DbErrorCodes::TermEmpty)
        return status_bad_request();

    if (code == DbErrorCodes::TermNotExist)
        return status_not_found();

    if (code == DbErrorCodes::NewerTermVersionFound)
        return status_conflict();

    return status_bad_gateway();
}

std::string dbErrDescription(std::error_code code)
{
    if (code == DbErrorCodes::UuidEmpty)
        return "Uuid is empty";

    if (code == DbErrorCodes::UuidAlreadyExist)
        return "Uuid already exist";

    if (code == DbErrorCodes::UuidNotExist)
        return "Uuid not exist";

    if (code == DbErrorCodes::GroupNameEmpty)
        return "Group name empty";

    if (code == DbErrorCodes::GroupNameAlreadyExist)
        return "Group name already exist";

    if (code == DbErrorCodes::TermEmpty)
        return "Term empty";

    if (code == DbErrorCodes::TermNotExist)
        return "Term not exist";

    if (code == DbErrorCodes::NewerTermVersionFound)
        return "Newer term version found";

    return "Error";
}

auto responseForDbError(auto req, std::error_code code)
{
    return req->create_response(dbErrToHttpErr(code)).set_body(dbErrDescription(code)).done();
}

auto successResponse(auto req, std::string body = "")
{
    if (body.empty())
        return req->create_response().done();

    return req->create_response().set_body(body).done();
}

auto badRequest(auto req) { return req->create_response(restinio::status_bad_request()).done(); }

std::string strFromParam(auto param) { return restinio::cast_to<std::string>(param); }

QString qStringFromParam(auto param) { return QString::fromStdString(strFromParam(param)); }

std::optional<QUuid> uuidFromParam(auto param) { return uuidFromStr(strFromParam(param)); }

opt<GroupUuid> groupUuidFromParam(auto param)
{
    if (auto uuid = uuidFromParam(param))
        return GroupUuid::create(*uuid);

    return std::nullopt;
}

opt<TermUuid> termUuidFromParam(auto param)
{
    if (auto uuid = uuidFromParam(param))
        return TermUuid::create(*uuid);

    return std::nullopt;
}

int main()
{
    Q_INIT_RESOURCE(SqlQueries);

    int16_t     listenPort = 26748;
    std::string address    = "localhost";

    auto appDataPath = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first();
    auto dbFilePath  = appDataPath + "/TermGraphBack/tg.termGraph";
    auto backupPath  = appDataPath + "/backup";

    LocalDatabaseStorage storage(dbFilePath, backupPath);

    // Route naming rule:
    // /api/<apiversionSection(v1)>/<user(global)>/route...
    auto router = std::make_unique<router::express_router_t<>>();

    // GET /api/v1/global/groups
    // GET /api/v1/global/groups?type=uuid_only
    router->http_get("/api/v1/global/groups", [&storage](auto req, auto params) {
        auto urlParams = restinio::parse_query(req->header().query());

        std::string jsonStr;

        bool uuidOnlyMode = urlParams.has("type") && urlParams["type"] == "uuid_only";

        if (uuidOnlyMode)
            jsonStr = uuidListToStdString("groupUuids", storage.getAllGroupsUuids(true));
        else
            jsonStr = containerToStdString("groups", storage.getGroups());

        return successResponse(req, jsonStr);
    });

    // GET /api/v1/global/groups/:uuid
    router->http_get("/api/v1/global/groups/:uuid", [&storage](auto req, auto params) {
        if (auto uuid = groupUuidFromParam(params["uuid"])) {
            if (auto group = storage.getGroup(*uuid)) {
                auto jsonObj = group.value().toJson();
                auto jsonStr = jsonObjToStr(jsonObj);
                return successResponse(req, jsonStr);
            } else {
                return responseForDbError(req, group.error());
            }
        }

        return badRequest(req);
    });

    // POST /api/v1/global/groups
    router->http_post("/api/v1/global/groups", [&storage](auto req, auto params) {
        if (auto jsonObj = strToJsonObj(req->body())) {
            if (auto group = GroupData::fromJson(*jsonObj)) {
                if (auto res = storage.addGroup(*group)) {
                    return successResponse(req);
                } else {
                    return responseForDbError(req, res.error());
                }
            }
        }

        return badRequest(req);
    });

    // PUT /api/v1/global/groups/:uuid
    router->http_put("/api/v1/global/groups/:uuid", [&storage](auto req, auto params) {
        if (auto uuid = uuidFromParam(params["uuid"])) {
            if (auto jsonObj = strToJsonObj(req->body())) {
                if (auto data = GroupData::fromJson(*jsonObj)) {
                    (*data).uuid = (*uuid);
                    if (auto res = storage.updateGroup(*data)) {
                        return successResponse(req);
                    } else {
                        return responseForDbError(req, res.error());
                    }
                }
            }
        }

        return badRequest(req);
    });

    // DELETE /api/v1/global/groups/:uuid
    router->http_delete("/api/v1/global/groups/:uuid", [&storage](auto req, auto params) {
        if (auto uuid = groupUuidFromParam(params["uuid"])) {
            if (auto res = storage.deleteGroup(*uuid)) {
                return successResponse(req);
            } else {
                return responseForDbError(req, res.error());
            }
        }

        return badRequest(req);
    });

    // GET /api/v1/global/terms?group_uuid=gUuid
    // GET /api/v1/global/terms?group_uuid=gUuid&name=:name
    // GET /api/v1/global/terms?group_uuid=gUuid&type=uuid_only
    router->http_get("/api/v1/global/terms", [&storage](auto req, auto params) {
        auto urlParams = restinio::parse_query(req->header().query());

        bool hasNameParam      = urlParams.has("name");
        bool uuidOnly          = urlParams.has("type") && urlParams["type"] == "uuid_only";
        bool hasGroupUuidParam = urlParams.has("group_uuid");

        if (hasGroupUuidParam) {
            auto groupUuid = groupUuidFromParam(urlParams["group_uuid"]);
            if (!groupUuid.has_value())
                return badRequest(req);

            if (hasNameParam) {
                auto name = qStringFromParam(urlParams["name"]);

                auto nodeUuid = storage.findTerm(name, *groupUuid);

                if (!nodeUuid.has_value())
                    return req->create_response(restinio::status_not_found()).done();

                if (auto termData = storage.getTerm(*nodeUuid))
                    return successResponse(req, jsonObjToStr(termData.value().toJson()));
                else
                    return responseForDbError(req, termData.error());

            } else if (uuidOnly) {
                if (auto termList = storage.getTerms(*groupUuid)) {
                    UuidList uuids;
                    for (const auto& term : termList.value())
                        uuids.push_back(term.uuid);

                    return successResponse(req, uuidListToStdString("uuids", uuids));
                } else {
                    return responseForDbError(req, termList.error());
                }

            } else {
                if (auto termList = storage.getTerms(*groupUuid))
                    return successResponse(req, containerToStdString("terms", termList.value()));
                else
                    return responseForDbError(req, termList.error());
            }
        }

        return badRequest(req);
    });

    // GET /api/v1/global/terms/:uuid
    router->http_get("/api/v1/global/terms/:uuid", [&storage](auto req, auto params) {
        auto urlParams = restinio::parse_query(req->header().query());
        if (auto uuid = termUuidFromParam(params["uuid"])) {
            bool lastEditOnly = urlParams.has("type") && urlParams["type"] == "last_edit";

            if (auto term = storage.getTerm(*uuid)) {
                return successResponse(req, jsonObjToStr(term.value().toJson()));
            } else {
                return responseForDbError(req, term.error());
            }
        }

        return badRequest(req);
    });

    // POST /api/v1/global/terms
    router->http_post("/api/v1/global/terms", [&storage](auto req, auto params) {
        if (auto jsonObj = strToJsonObj(req->body())) {
            if (auto term = TermData::fromJson(*jsonObj, false, false)) {
                if (auto res = storage.addTerm(*term)) {
                    return successResponse(req);
                } else {
                    return responseForDbError(req, res.error());
                }
            }
        }

        return badRequest(req);
    });

    // PUT /api/v1/global/terms/:uuid
    router->http_put("/api/v1/global/terms/:uuid", [&storage](auto req, auto params) {
        if (auto uuid = uuidFromParam(params["uuid"])) {
            if (auto jsonObj = strToJsonObj(req->body())) {
                if (auto data = TermData::fromJson(*jsonObj, false, false)) {
                    (*data).uuid = (*uuid);
                    if (auto res = storage.updateTerm(*data, DataStorageInterface::LastEditSource::AutoGenerate, false)) {
                        return successResponse(req);
                    } else {
                        return responseForDbError(req, res.error());
                    }
                }
            }
        }

        return badRequest(req);
    });

    // DELETE /api/v1/global/terms/:uuid
    router->http_delete("/api/v1/global/terms/:uuid", [&storage](auto req, auto params) {
        if (auto uuid = termUuidFromParam(params["uuid"])) {
            if (auto res = storage.deleteTerm(*uuid)) {
                return successResponse(req);
            } else {
                return responseForDbError(req, res.error());
            }
        }

        return badRequest(req);
    });

    // Launching a server with custom traits.
    struct my_server_traits : public default_single_thread_traits_t
    {
        using request_handler_t = restinio::router::express_router_t<>;
    };

    restinio::run(restinio::on_this_thread<my_server_traits>()
                      .address(address)
                      .port(listenPort)
                      .request_handler(std::move(router)));

    // router->http_get(R"(/api/v1/storage_version)")
    // int storageVersion() const final;

    return 0;
}
