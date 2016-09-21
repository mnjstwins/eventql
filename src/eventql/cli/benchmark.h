/**
 * Copyright (c) 2016 DeepCortex GmbH <legal@eventql.io>
 * Authors:
 *   - Paul Asmuth <paul@eventql.io>
 *   - Laura Schlimmer <laura@eventql.io>
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
#pragma once
#include <thread>
#include <eventql/eventql.h>
#include <eventql/util/stdtypes.h>
#include <eventql/util/return_code.h>

namespace eventql {
namespace cli {

class BenchmarkStats {
public:

  BenchmarkStats();

  void addRequest(bool is_success, size_t t_id, uint64_t start_time);

  std::string toString();

protected:

  void calculate();

  static const uint64_t kNumBuckets = 6;
  static const auto kTimeWindowSize = 10000000; //10 seconds

  struct Bucket {
    Bucket() : time(0), num_requests(0) {}

    uint64_t time;
    uint64_t num_requests;
  };

  uint64_t total_requests_;
  double min_rate_;
  double max_rate_;
  double mvg_avg_rate_;

  std::vector<Bucket> buckets_;
  size_t buckets_begin_;
  std::mutex mutex_;
};

class Benchmark {
public:

  Benchmark();

  void setProgressCallback(std::function<void ()> cb);

  ReturnCode run();
  void kill();

  BenchmarkStats* getStats();

protected:

  void runThread(size_t idx);
  bool getRequestSlot(size_t idx);

  size_t num_threads_;
  std::mutex mutex_;
  std::condition_variable cv_;
  ReturnCode status_;
  std::vector<std::thread> threads_;
  size_t threads_running_;
  uint64_t last_request_time_;
  uint64_t rate_limit_interval_;
  BenchmarkStats* stats_;
  size_t remaining_requests_;
  std::function<void ()> on_progress_;
};

} //cli
} //eventql
