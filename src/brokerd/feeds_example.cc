/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include "stx/application.h"
#include "stx/http/httprouter.h"
#include "stx/http/httpserver.h"
#include "stx/io/filerepository.h"
#include "stx/io/fileutil.h"
#include "stx/json/jsonrpc.h"
#include "stx/json/jsonrpchttpadapter.h"
#include "brokerd/FeedService.h"
#include "stx/thread/eventloop.h"
#include "stx/thread/threadpool.h"

using fnord::json::JSONRPC;
using fnord::json::JSONRPCHTTPAdapter;
using fnord::feeds::FeedService;

int main() {
  fnord::Application::init();
  fnord::Application::logToStderr();

  JSONRPC rpc;
  JSONRPCHTTPAdapter rpc_http(&rpc);

  auto log_path = "/tmp/cm-logs";
  fnord::FileUtil::mkdir_p(log_path);

  FeedService ls_service{fnord::FileRepository(log_path)};
  rpc.registerService(&ls_service);

  fnord::thread::EventLoop event_loop;
  fnord::thread::ThreadPool thread_pool;
  fnord::http::HTTPRouter http_router;
  http_router.addRouteByPrefixMatch("/rpc", &rpc_http);
  fnord::http::HTTPServer http_server(&http_router, &event_loop);
  http_server.listen(8080);
  event_loop.run();

  event_loop.run();
  return 0;
}

