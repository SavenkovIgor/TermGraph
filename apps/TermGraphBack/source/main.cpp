#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

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

std::optional<QUuid> strToUuid(std::string param)
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

std::string uuidListToStdString(QString rootKey, UuidList uuidList)
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

        return req->create_response().set_body(jsonStr).done();
    });

    // GET /api/v1/global/groups/:uuid
    router->http_get("/api/v1/global/groups/:uuid", [&storage](auto req, auto params) {
        if (auto uuid = strToUuid(restinio::cast_to<std::string>(params["uuid"]))) {
            auto jsonObj = storage.getGroup(*uuid).toJson();
            auto jsonStr = jsonObjToStr(jsonObj);
            return req->create_response().set_body(jsonStr).done();
        }

        return req->create_response(restinio::status_bad_request()).done();
    });

    // POST /api/v1/global/groups
    router->http_post("/api/v1/global/groups", [&storage](auto req, auto params) {
        if (auto jsonObj = strToJsonObj(req->body())) {
            if (auto group = GroupData::fromJson(*jsonObj)) {
                if (storage.addGroup(*group)) {
                    return req->create_response().done();
                }
            }
        }

        return req->create_response(restinio::status_bad_request()).done();
    });

    // PUT /api/v1/global/groups/:uuid
    router->http_put("/api/v1/global/groups/:uuid", [&storage](auto req, auto params) {
        if (auto uuid = strToUuid(restinio::cast_to<std::string>(params["uuid"]))) {
            if (auto jsonObj = strToJsonObj(req->body())) {
                if (auto data = GroupData::fromJson(*jsonObj)) {
                    (*data).uuid = (*uuid);
                    if (storage.updateGroup(*data)) {
                        return req->create_response().done();
                    }
                }
            }
        }

        return req->create_response(restinio::status_bad_request()).done();
    });

    // DELETE /api/v1/global/groups/:uuid
    router->http_delete("/api/v1/global/groups/:uuid", [&storage](auto req, auto params) {
        if (auto uuid = strToUuid(restinio::cast_to<std::string>(params["uuid"]))) {
            storage.deleteGroup(*uuid);
            return req->create_response().done();
        }

        return req->create_response(restinio::status_bad_request()).done();
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
            auto groupUuid = strToUuid(restinio::cast_to<std::string>(urlParams["group_uuid"]));
            if (!groupUuid.has_value())
                return req->create_response(restinio::status_bad_request()).done();

            if (hasNameParam) {
                auto name = QString::fromStdString(restinio::cast_to<std::string>(urlParams["name"]));

                auto nodeUuid = storage.findTerm(name, *groupUuid);

                if (nodeUuid.isNull())
                    return req->create_response(restinio::status_not_found()).done();

                auto termData = storage.getTerm(nodeUuid);
                return req->create_response().set_body(jsonObjToStr(termData.toJson())).done();

            } else if (uuidOnly) {
                auto termList = storage.getTerms(*groupUuid);

                UuidList uuids;
                for (const auto& term : termList)
                    uuids.push_back(term.uuid);

                auto json = uuidListToStdString("uuids", uuids);
                return req->create_response().set_body(json).done();

            } else {
                auto termList = storage.getTerms(*groupUuid);
                return req->create_response().set_body(containerToStdString("terms", termList)).done();
            }
        }

        return req->create_response(restinio::status_bad_request()).done();
    });

    // GET /api/v1/global/terms/:uuid
    router->http_get("/api/v1/global/terms/:uuid", [&storage](auto req, auto params) {
        auto urlParams = restinio::parse_query(req->header().query());
        if (auto uuid = strToUuid(restinio::cast_to<std::string>(params["uuid"]))) {
            bool lastEditOnly = urlParams.has("type") && urlParams["type"] == "last_edit";

            auto term = storage.getTerm(*uuid);
            return req->create_response().set_body(jsonObjToStr(term.toJson())).done();
        }

        return req->create_response(restinio::status_bad_request()).done();
    });

    // POST /api/v1/global/terms
    router->http_post("/api/v1/global/terms", [&storage](auto req, auto params) {
        if (auto jsonObj = strToJsonObj(req->body())) {
            if (auto term = TermData::fromJson(*jsonObj, false, false)) {
                if (storage.addTerm(*term)) {
                    return req->create_response().done();
                }
            }
        }

        return req->create_response(restinio::status_bad_request()).done();
    });

    // PUT /api/v1/global/terms/:uuid
    router->http_put("/api/v1/global/terms/:uuid", [&storage](auto req, auto params) {
        if (auto uuid = strToUuid(restinio::cast_to<std::string>(params["uuid"]))) {
            if (auto jsonObj = strToJsonObj(req->body())) {
                if (auto data = TermData::fromJson(*jsonObj, false, false)) {
                    (*data).uuid = (*uuid);
                    if (storage.updateTerm(*data, DataStorageInterface::LastEditSource::AutoGenerate, false)) {
                        return req->create_response().done();
                    }
                }
            }
        }

        return req->create_response(restinio::status_bad_request()).done();
    });

    // DELETE /api/v1/global/terms/:uuid
    router->http_delete("/api/v1/global/terms/:uuid", [&storage](auto req, auto params) {
        if (auto uuid = strToUuid(restinio::cast_to<std::string>(params["uuid"]))) {
            storage.deleteTerm(*uuid);
            return req->create_response().done();
        }

        return req->create_response(restinio::status_bad_request()).done();
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

    //    router->http_get(R"(/api/v1/storage_version)")
    //    int storageVersion() const final;

    return 0;
}
