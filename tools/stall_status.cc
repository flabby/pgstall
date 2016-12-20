#include "status.h"
#include "pb_cli.h"
#include "pink_cli.h"
#include "status.h"
#include <google/protobuf/message.h>

#include <iostream>
#include <string>

#include "monitor.pb.h"

class Pcli : public pink::PbCli {
 public:
   Pcli();
   pink::Status SendCommand(const std::string &command);
   pink::Status RecvStatus();
};

Pcli::Pcli() {
}

pink::Status Pcli::SendCommand(const std::string &command) {
  pink::Status s;
  gpstall::Command req;
  req.set_command(command);

  s = Send(&req);

  return s;
}

pink::Status Pcli::RecvStatus() {
  pink::Status s;
  gpstall::ServiceStatus service_status_;
  s = Recv(&service_status_);
  if (!s.ok())
    return s;

  std::cout << "start_time: " << service_status_.start_time() << std::endl;
  std::cout << "service_status: " << service_status_.service_status() << std::endl;
  std::cout << "conn_num: " << service_status_.conn_num() << std::endl;
  std::cout << "qps: " << service_status_.qps() << std::endl;
  std::cout << std::endl;

  std::cout << "gpload_failed_num: " << service_status_.gpload_failed_num() << std::endl;
  std::cout << "lastest_failed_time: " << service_status_.lastest_failed_time() << std::endl;
  std::cout << "failed_files_num: " << service_status_.failed_files_num() << std::endl;
  std::cout << "failed_files_name: " << std::endl;
  std::cout << service_status_.failed_files_name() << std::endl;
  std::cout << "failed_files_size: " << service_status_.failed_files_size() << std::endl;
  std::cout << "longest_gpload_timeused: " << service_status_.gpload_longest_timeused() / 1000 << std::endl;
  std::cout << "latest_gpload_timeused: " << service_status_.gpload_latest_timeused() / 1000 << std::endl;
  std::cout << "gpload_average_timeused: " << service_status_.gpload_average_timeused() / 1000 << std::endl;

  return s;
}

void Usage() {
  std::cout << "Usage: ./stall_status ip port" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    Usage();
    exit(1);
  }

  Pcli *pcli = new Pcli();
  pcli->Connect(argv[1], atoi(argv[2]));

  pink::Status s = pcli->SendCommand("gpstall_info");
  if (!s.ok())
    std::cout << s.ToString() << std::endl;
  pcli->RecvStatus();

  delete pcli;

  return 0;
}
