/**
 * Copyright (c) 2016 DeepCortex GmbH <legal@eventql.io>
 * Authors:
 *   - Paul Asmuth <paul@eventql.io>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License ("the license") as
 * published by the Free Software Foundation, either version 3 of the License,
 * or any later version.
 *
 * In accordance with Section 7(e) of the license, the licensing of the Program
 * under the license does not imply a trademark license. Therefore any rights,
 * title and interest in our trademarks remain entirely with us.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You can be released from the requirements of the license by purchasing a
 * commercial license. Buying such a license is mandatory as soon as you develop
 * commercial activities involving this program without disclosing the source
 * code of your own applications
 */
#ifndef _FNORD_SSTABLE_SSTABLESERVLET_H
#define _FNORD_SSTABLE_SSTABLESERVLET_H
#include "eventql/util/VFS.h"
#include "eventql/util/http/httpservice.h"
#include "eventql/util/json/json.h"


namespace sstable {

class SSTableServlet : public http::HTTPService {
public:
  enum class ResponseFormat {
    JSON,
    CSV
  };

  SSTableServlet(const String& base_path, VFS* vfs);

  void handleHTTPRequest(
      http::HTTPRequest* req,
      http::HTTPResponse* res);

protected:

  void scan(
      http::HTTPRequest* req,
      http::HTTPResponse* res,
      const URI& uri);

  ResponseFormat formatFromString(const String& format);

  String base_path_;
  VFS* vfs_;
};

}
#endif
