#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QString>

#include <restinio/all.hpp>

#include <CommonTools/HandyTypes.h>
#include <CommonTools/NetworkTools.h>
#include <TermDataStorage/LocalDataStorage.h>

#include <QDebug>

using namespace restinio;

// TODO: Give address and port through agrument
// TODO: Encoding!
// TODO: Add standard database errors
// TODO: Remove second version of sql executor
// TODO: Watch about error codes

http_status_line_t dbErrToHttpErr(std::error_code code)
{
    using namespace restinio;

    // clang-format off
    switch (static_cast<DbErrorCodes>(code.value())) {
    case DbErrorCodes::GroupUuidEmpty:        return status_bad_request();
    case DbErrorCodes::GroupUuidAlreadyExist: return status_conflict();
    case DbErrorCodes::GroupUuidNotFound:     return status_not_found();
    case DbErrorCodes::GroupNameEmpty:        return status_bad_request();
    case DbErrorCodes::GroupNameAlreadyExist: return status_conflict();

    case DbErrorCodes::TermUuidEmpty:         return status_bad_request();
    case DbErrorCodes::TermUuidAlreadyExist:  return status_conflict();
    case DbErrorCodes::TermUuidNotFound:      return status_not_found();
    case DbErrorCodes::TermEmpty:             return status_bad_request();
    case DbErrorCodes::TermNotFound:          return status_not_found();
    case DbErrorCodes::NewerTermVersionFound: return status_conflict();

    case DbErrorCodes::ConnectionError: return status_not_found();
    case DbErrorCodes::JsonParseError:  return status_bad_request();
    case DbErrorCodes::UnknownError:    return status_internal_server_error();
    }
    // clang-format on
}

QString dbErrDescription(std::error_code code) { return QString::fromStdString(code.message()); }

auto responseForDbError(auto req, std::error_code code)
{
    return req->create_response(dbErrToHttpErr(code)).set_body(dbErrDescription(code).toStdString()).done();
}

auto successResponse(auto req, QByteArray body = QByteArray())
{
    if (body.isEmpty())
        return req->create_response().done();

    return req->create_response().set_body(body.toStdString()).done();
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
    router->http_get(NetworkTools::groupApiPath, [&storage](auto req, auto params) {
        auto urlParams = restinio::parse_query(req->header().query());

        QByteArray json;

        bool uuidOnlyMode = urlParams.has("type") && urlParams["type"] == "uuid_only";

        if (uuidOnlyMode)
            json = static_cast<QByteArray>(storage.getAllGroupsUuids(true).result().value());
        else
            json = static_cast<QByteArray>(storage.getGroups().result().value());

        return successResponse(req, json);
    });

    // GET /api/v1/global/groups/:uuid
    router->http_get(NetworkTools::groupUuidApiPath, [&storage](auto req, auto params) {
        if (auto uuid = GroupUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
            if (auto group = storage.getGroup(*uuid).result()) {
                return successResponse(req, static_cast<QByteArray>(group.value()));
            } else {
                return responseForDbError(req, group.error());
            }
        }

        return badRequest(req);
    });

    // POST /api/v1/global/groups
    router->http_post(NetworkTools::groupApiPath, [&storage](auto req, auto params) {
        if (auto group = GroupData::create(QByteArray::fromStdString(req->body()))) {
            if (auto res = storage.addGroup(*group)) {
                return successResponse(req);
            } else {
                return responseForDbError(req, res.error());
            }
        }

        return badRequest(req);
    });

    // PUT /api/v1/global/groups/:uuid
    router->http_put(NetworkTools::groupUuidApiPath, [&storage](auto req, auto params) {
        if (auto uuid = GroupUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
            if (auto data = GroupData::create(QByteArray::fromStdString(req->body()))) {
                (*data).uuid = (*uuid);
                if (auto res = storage.updateGroup(*data)) {
                    return successResponse(req);
                } else {
                    return responseForDbError(req, res.error());
                }
            }
        }

        return badRequest(req);
    });

    // DELETE /api/v1/global/groups/:uuid
    router->http_delete(NetworkTools::groupUuidApiPath, [&storage](auto req, auto params) {
        if (auto uuid = GroupUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
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
    router->http_get(NetworkTools::termApiPath, [&storage](auto req, auto params) {
        auto urlParams = restinio::parse_query(req->header().query());

        bool hasNameParam      = urlParams.has("name");
        bool uuidOnly          = urlParams.has("type") && urlParams["type"] == "uuid_only";
        bool hasGroupUuidParam = urlParams.has("group_uuid");

        if (hasGroupUuidParam) {
            auto groupUuid = GroupUuid::create(paramToQString(urlParams["group_uuid"]), UuidMode::Url);
            if (!groupUuid.has_value())
                return badRequest(req);

            if (hasNameParam) {
                auto name = paramToQString(urlParams["name"]);

                auto nodeUuid = storage.findTerm(name, *groupUuid);

                if (!nodeUuid.has_value())
                    return req->create_response(restinio::status_not_found()).done();

                if (auto termData = storage.getTerm(*nodeUuid))
                    return successResponse(req, static_cast<QByteArray>(termData.value()));
                else
                    return responseForDbError(req, termData.error());

            } else if (uuidOnly) {
                if (auto termList = storage.getTerms(*groupUuid).result()) {
                    TermUuid::List uuids;
                    for (const auto& term : termList.value())
                        if (auto tUuid = TermUuid::create(term.uuid))
                            uuids.push_back(*tUuid);

                    return successResponse(req, static_cast<QByteArray>(uuids));
                } else {
                    return responseForDbError(req, termList.error());
                }

            } else {
                if (auto termList = storage.getTerms(*groupUuid).result())
                    return successResponse(req, static_cast<QByteArray>(termList.value()));
                else
                    return responseForDbError(req, termList.error());
            }
        }

        return badRequest(req);
    });

    // GET /api/v1/global/terms/:uuid
    router->http_get(NetworkTools::termUuidApiPath, [&storage](auto req, auto params) {
        auto urlParams = restinio::parse_query(req->header().query());
        if (auto uuid = TermUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
            bool lastEditOnly = urlParams.has("type") && urlParams["type"] == "last_edit";

            if (auto term = storage.getTerm(*uuid)) {
                return successResponse(req, static_cast<QByteArray>(term.value()));
            } else {
                return responseForDbError(req, term.error());
            }
        }

        return badRequest(req);
    });

    // POST /api/v1/global/terms
    router->http_post(NetworkTools::termApiPath, [&storage](auto req, auto params) {
        if (auto term = TermData::create(QByteArray::fromStdString(req->body()), TermData::JsonCheckMode::Import)) {
            if (auto res = storage.addTerm(*term)) {
                return successResponse(req);
            } else {
                return responseForDbError(req, res.error());
            }
        }

        return badRequest(req);
    });

    // PUT /api/v1/global/terms/:uuid
    router->http_put(NetworkTools::termUuidApiPath, [&storage](auto req, auto params) {
        if (auto uuid = TermUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
            if (auto data = TermData::create(QByteArray::fromStdString(req->body()), TermData::JsonCheckMode::Import)) {
                (*data).uuid = (*uuid);
                if (auto res = storage.updateTerm(*data, DataStorageInterface::LastEditSource::AutoGenerate, false)) {
                    return successResponse(req);
                } else {
                    return responseForDbError(req, res.error());
                }
            }
        }

        return badRequest(req);
    });

    // DELETE /api/v1/global/terms/:uuid
    router->http_delete(NetworkTools::termUuidApiPath, [&storage](auto req, auto params) {
        if (auto uuid = TermUuid::create(paramToQString(params["uuid"]), UuidMode::Url)) {
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
                      .port(NetworkTools::defaultPort)
                      .request_handler(std::move(router)));

    // router->http_get(R"(/api/v1/storage_version)")
    // int storageVersion() const final;

    return 0;
}
