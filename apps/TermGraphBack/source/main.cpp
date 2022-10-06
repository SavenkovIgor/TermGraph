#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QString>
#include <QDir>

#include <restinio/all.hpp>

#include <CommonTools/HandyTypes.h>
#include <CommonTools/NetworkTools.h>
#include <TermDataStorage/LocalDataStorage.h>

#include <QDebug>

using namespace restinio;

// TODO: Give address and port through agrument
// TODO: Encoding!
// TODO: Remove second version of sql executor

http_status_line_t dbErrToHttpErr(std::error_code code)
{
    using namespace restinio;

    // clang-format off
    switch (static_cast<ErrorCodes>(code.value())) {
    using enum ErrorCodes;
    case GroupUuidInvalid:      return status_bad_request();
    case GroupUuidAlreadyExist: return status_conflict();
    case GroupUuidNotFound:     return status_not_found();
    case GroupNameEmpty:        return status_bad_request();
    case GroupNameAlreadyExist: return status_conflict();

    case TermUuidInvalid:       return status_bad_request();
    case TermUuidAlreadyExist:  return status_conflict();
    case TermUuidNotFound:      return status_not_found();
    case TermEmpty:             return status_bad_request();
    case TermAlreadyExist:      return status_conflict();
    case TermNotFound:          return status_not_found();
    case NewerTermVersionFound: return status_conflict();

    case ConnectionError: return status_not_found();
    case JsonParseError:  return status_bad_request();
    case UnknownError:    return status_internal_server_error();

    case LastEditInvalid:
    case NodesLastEditInvalid:
        return status_bad_request();

    case JsonNameFieldMissedOrWrongType:
    case JsonCommentFieldMissedOrWrongType:
    case JsonSizeFieldMissedOrWrongType:
    case JsonNodesFieldMissedOrWrongType:
    case JsonNodesLastEditFieldMissedOrWrongType:
    case JsonUuidFieldMissedOrWrongType:
    case JsonTermFieldMissedOrWrongType:
    case JsonDefinitionFieldMissedOrWrongType:
    case JsonDescriptionFieldMissedOrWrongType:
    case JsonExamplesFieldMissedOrWrongType:
    case JsonWikiUrlFieldMissedOrWrongType:
    case JsonWikiImageFieldMissedOrWrongType:
    case JsonGroupUuidFieldMissedOrWrongType:
    case JsonLastEditFieldMissedOrWrongType:
        return status_bad_request();
    }
    // clang-format on

    return status_bad_request();
}

QString dbErrDescription(std::error_code code) { return QString::fromStdString(code.message()); }

auto responseForDbError(auto req, std::error_code code)
{
    return req->create_response(dbErrToHttpErr(code)).set_body(dbErrDescription(code).toStdString()).done();
}

auto successResponse(auto req, QByteArray body = QByteArray())
{
    auto response = req->create_response();

    // NOTE: All cors headers would be added by nginx
    // response.header().add_field("Access-Control-Allow-Origin", "*");

    if (!body.isEmpty())
        response.set_body(body.toStdString());

    return response.done();
}

auto badRequest(auto req) { return req->create_response(restinio::status_bad_request()).done(); }

QString paramToQString(auto param)
{
    auto stdStr = restinio::cast_to<std::string>(param);
    return QString::fromStdString(stdStr);
}

int main()
{
    Q_INIT_RESOURCE(SqlQueries);

    std::string address    = "0.0.0.0";

    auto appName = QStringLiteral("TermGraphBack");
    auto stdPaths = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    assert(!stdPaths.empty());

    QString appDataPath = stdPaths.first() + "/" + appName;

    if (!QDir().mkpath(appDataPath)) {
        qCritical("Can't create database folder");
        return -1;
    }

    auto dbFilePath  = appDataPath + "/tg.termGraph";
    auto backupPath  = appDataPath + "/backup";

    LocalDatabaseStorage storage(dbFilePath, backupPath);

    // Route naming rule:
    // /api/<apiversionSection(v1)>/<user(global)>/route...
    auto router = std::make_unique<router::express_router_t<>>();

    // GET /api/v1/global/groups
    router->http_get(NetworkTools::groupApiPath, [&storage](auto req, auto params) {
        //  qInfo("GET %s", NetworkTools::groupApiPath);
        auto urlParams = restinio::parse_query(req->header().query());

        QByteArray json = static_cast<QByteArray>(storage.groups().result().value());

        return successResponse(req, json);
    });

    // GET /api/v1/global/groups/:uuid
    router->http_get(NetworkTools::groupUuidApiPath, [&storage](auto req, auto params) {
        //  qInfo("GET %s", NetworkTools::groupUuidApiPath);
        if (auto uuid = GroupUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
            if (auto group = storage.group(*uuid).result()) {
                return successResponse(req, static_cast<QByteArray>(group.value()));
            } else {
                return responseForDbError(req, group.error());
            }
        }

        return badRequest(req);
    });

    // POST /api/v1/global/groups
    router->http_post(NetworkTools::groupApiPath, [&storage](auto req, auto params) {
        //  qInfo("POST %s", NetworkTools::groupApiPath);
        if (auto group = GroupData::create(QByteArray::fromStdString(req->body()))) {
            if (auto groupData = storage.addGroup(*group).result()) {
                return successResponse(req, static_cast<QByteArray>(groupData.value()));
            } else {
                return responseForDbError(req, groupData.error());
            }
        }

        return badRequest(req);
    });

    // PUT /api/v1/global/groups/:uuid
    router->http_put(NetworkTools::groupUuidApiPath, [&storage](auto req, auto params) {
        //  qInfo("PUT %s", NetworkTools::groupUuidApiPath);
        if (auto uuid = GroupUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
            if (auto data = GroupData::create(QByteArray::fromStdString(req->body()))) {
                (*data).uuid = (*uuid);
                if (auto groupData = storage.updateGroup(*data).result()) {
                    return successResponse(req, static_cast<QByteArray>(groupData.value()));
                } else {
                    return responseForDbError(req, groupData.error());
                }
            }
        }

        return badRequest(req);
    });

    // DELETE /api/v1/global/groups/:uuid
    router->http_delete(NetworkTools::groupUuidApiPath, [&storage](auto req, auto params) {
        //  qInfo("DELETE %s", NetworkTools::groupUuidApiPath);
        if (auto uuid = GroupUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
            if (auto groupData = storage.deleteGroup(*uuid).result()) {
                return successResponse(req, static_cast<QByteArray>(groupData.value()));
            } else {
                return responseForDbError(req, groupData.error());
            }
        }

        return badRequest(req);
    });

    // GET /api/v1/global/terms?group_uuid=gUuid
    // GET /api/v1/global/terms?group_uuid=gUuid&name=:name
    router->http_get(NetworkTools::termApiPath, [&storage](auto req, auto params) {
        //  qInfo("GET %s", NetworkTools::termApiPath);
        auto urlParams = restinio::parse_query(req->header().query());

        bool hasNameParam      = urlParams.has("name");
        bool hasGroupUuidParam = urlParams.has("group_uuid");

        if (hasGroupUuidParam) {
            auto groupUuid = GroupUuid::create(paramToQString(urlParams["group_uuid"]), UuidMode::Url);
            if (!groupUuid.has_value())
                return badRequest(req);

            if (hasNameParam) {
                auto name = paramToQString(urlParams["name"]);

                if (auto nodeData = storage.term(name, *groupUuid).result()) {
                    return successResponse(req, static_cast<QByteArray>(nodeData.value()));
                } else {
                    return responseForDbError(req, nodeData.error());
                }

            } else {
                if (auto termList = storage.terms(*groupUuid).result())
                    return successResponse(req, static_cast<QByteArray>(termList.value()));
                else
                    return responseForDbError(req, termList.error());
            }
        }

        return badRequest(req);
    });

    // GET /api/v1/global/terms/:uuid
    router->http_get(NetworkTools::termUuidApiPath, [&storage](auto req, auto params) {
        //  qInfo("GET %s", NetworkTools::termUuidApiPath);
        auto urlParams = restinio::parse_query(req->header().query());
        if (auto uuid = TermUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
            bool lastEditOnly = urlParams.has("type") && urlParams["type"] == "last_edit";

            if (auto term = storage.term(*uuid).result()) {
                return successResponse(req, static_cast<QByteArray>(term.value()));
            } else {
                return responseForDbError(req, term.error());
            }
        }

        return badRequest(req);
    });

    // POST /api/v1/global/terms
    router->http_post(NetworkTools::termApiPath, [&storage](auto req, auto params) {
        //  qInfo("POST %s", NetworkTools::termApiPath);
        if (auto term = TermData::create(QByteArray::fromStdString(req->body()), TermData::JsonCheckMode::AddTerm)) {
            if (auto res = storage.addTerm(*term).result()) {
                return successResponse(req, static_cast<QByteArray>(res.value()));
            } else {
                return responseForDbError(req, res.error());
            }
        }

        return badRequest(req);
    });

    // PUT /api/v1/global/terms/:uuid
    router->http_put(NetworkTools::termUuidApiPath, [&storage](auto req, auto params) {
        //  qInfo("PUT %s", NetworkTools::termUuidApiPath);
        if (auto uuid = TermUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
            if (auto data = TermData::create(QByteArray::fromStdString(req->body()),
                                             TermData::JsonCheckMode::UpdateTerm)) {
                (*data).uuid = (*uuid);
                if (auto res = storage.updateTerm(*data, DataStorageInterface::LastEditSource::AutoGenerate, false).result()) {
                    return successResponse(req, static_cast<QByteArray>(res.value()));
                } else {
                    return responseForDbError(req, res.error());
                }
            }
        }

        return badRequest(req);
    });

    // DELETE /api/v1/global/terms/:uuid
    router->http_delete(NetworkTools::termUuidApiPath, [&storage](auto req, auto params) {
        //  qInfo("DELETE %s", NetworkTools::termUuidApiPath);
        if (auto uuid = TermUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
            if (auto res = storage.deleteTerm(*uuid).result()) {
                return successResponse(req, static_cast<QByteArray>(res.value()));
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
                      .port(NetworkTools::defaultPort)
                      .request_handler(std::move(router)));

    // router->http_get(R"(/api/v1/storage_version)")
    // int storageVersion() const final;

    return 0;
}
