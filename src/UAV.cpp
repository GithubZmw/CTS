#include <gflags/gflags.h>
#include "butil/logging.h"
#include "butil/time.h"
#include "brpc/channel.h"
#include "brpc/server.h"
#include "json2pb/pb_to_json.h"
#include "output/UAV.pb.h"
#include "output/TA.pb.h"
#include "fussion.h"
#include "brpcTools.h"
#include "UAV.h"

DEFINE_string(attachment, "", "Carry this along with requests");
DEFINE_string(protocol, "baidu_std", "Protocol type. Defined in src/brpc/options.proto");
DEFINE_string(connection_type, "", "Connection type. Available values: single, pooled, short");
DEFINE_int32(timeout_ms, 5000, "RPC timeout in milliseconds");
DEFINE_int32(max_retry, 3, "Max retries(not including the first RPC)");
DEFINE_string(load_balancer, "", "The algorithm for load balancing");
DEFINE_string(serverTA, "0.0.0.0:8000", "IP Address of server");
DEFINE_int32(port_UAV, 8002, "TCP Port of this server");
DECLARE_string(listen_addr);
DECLARE_int32(idle_timeout_s);

gmp_randstate_t state_UAV;
Params pp_UAV;
UAV uav;

/*
 * `SignServiceImpl` is used to implement `SignService`. If this service is invoked,
 * the UAV will generate its own signature and return it to the caller.
 */
namespace UAVproto {
    class SignServiceImpl : public SignService {
    public:
        virtual void sign(google::protobuf::RpcController *cntl_base,
                          const SignRequest *request,
                          SignResponse *response,
                          google::protobuf::Closure *done) {

            brpc::ClosureGuard done_guard(done);
            brpc::Controller *cntl = static_cast<brpc::Controller *>(cntl_base);
            cntl->set_after_rpc_resp_fn(bind(SignServiceImpl::CallAfterRpc,
                                             placeholders::_1, placeholders::_2, placeholders::_3)); // 这里表示回调函数有三个参数

            // ---------------------------------------------------------------------------------------------------------
            /*
            LOG(INFO) << "Request=" << cntl->log_id() << "from " << cntl->remote_side() << " to " << cntl->local_side()
                      << endl << "\tSign服务请求（无人机的部分签名）:" << endl
                      << "\t\tM = " << request->m() << endl
                      << "\t\tt = " << request->t() << endl;
            */

            // generate its own signature and return it to the caller.
            parSig sig = Sign(pp_UAV, uav, request->t(), str_to_mpz(request->m()));
            response->set_cj(mpz_to_str(sig.cj));
            response->set_sig(ECP_to_str(sig.sig));
            response->set_id(mpz_to_str(sig.ID));
            // ---------------------------------------------------------------------------------------------------------
        }

        static void CallAfterRpc(brpc::Controller *cntl,
                                 const google::protobuf::Message *req,
                                 const google::protobuf::Message *res) {
            string req_str;
            string res_str;
            json2pb::ProtoMessageToJson(*req, &req_str, NULL);
            json2pb::ProtoMessageToJson(*res, &res_str, NULL);
            LOG(INFO) << endl << "\tCollect服务响应:" << endl
                      << "\t\treq:" << req_str << endl
                      << "\t\tres:" << res_str;
        }

    };
}// namespace UAVproto


int registerUAV(int argc, char *argv[]) {
    // 解析命令行参数，使用 gflags 提供的 ParseCommandLineFlags 函数解析参数
    google::ParseCommandLineFlags(&argc, &argv, true);
    brpc::Channel channel;
    brpc::ChannelOptions options;
    options.protocol = FLAGS_protocol;
    options.connection_type = FLAGS_connection_type;
    options.timeout_ms = FLAGS_timeout_ms;
    options.max_retry = FLAGS_max_retry;

    if (channel.Init(FLAGS_serverTA.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }
    TAproto::RegisterService_Stub stubRegister(&channel);
    if (!brpc::IsAskedToQuit()) {
        // We will receive response synchronously, safe to put variables on stack.
        TAproto::UAVIDRequest request;
        TAproto::UAVIDResponse response;
        brpc::Controller cntl;
        // set request message
        initState(state_UAV);
        mpz_class id = rand_mpz(state_UAV);
        request.set_uav_id(mpz_to_str(id));
        stubRegister.Register(&cntl, &request, &response, NULL);
        // check the result of the RPC call
        if (!cntl.Failed()) {
            /* print request message
            LOG(INFO) << "response from " << cntl.remote_side() << endl
                      << "\tresponse: " << "{ n=" << response.params().n() << endl
                      << "\t\ttm=" << response.params().tm() << endl
                      << "\t\tq=" << response.params().q() << endl
                      << "\t\tg=" << response.params().g() << endl
                      << "\t\tbeta=" << response.params().beta() << endl
                      << "\t\tP2=" << response.params().p2() << "}" << endl
                      << "\t\tcj = {" << response.cj() << "}" << endl
                      << "\t\tsj = {" << response.sj() << "}" << endl
                      << "\t\t{M=" << response.m() << endl
                      << "\t\tt=" << response.t() << "}" << endl
                      << " ---->> latency=" << cntl.latency_us() << "us";
            */

            // save public parameters and private key of UAV
            pp_UAV.n = response.params().n();
            pp_UAV.tm = response.params().tm();
            pp_UAV.q = str_to_mpz(response.params().q());
            pp_UAV.g = str_to_mpz(response.params().g());
            pp_UAV.beta = str_to_mpz(response.params().beta());
            pp_UAV.P2 = str_to_ECP2(response.params().p2());
            pp_UAV.PK = str_to_ECP2Arr(response.params().pk());
            // create signer to sign
            uav.ID = id;
            uav.c1 = str_to_mpzArr(response.cj());
            uav.c2 = str_to_mpzArr(response.sj());
        } else {
            LOG(WARNING) << cntl.ErrorText();
        }
    }
    LOG(INFO) << "EchoClient is going to quit";
    return 1;
}


int startUAVServer(int argc, char *argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    brpc::Server server;
    UAVproto::SignServiceImpl collect_service_impl;
    if (server.AddService(&collect_service_impl, brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
        LOG(ERROR) << "Fail to add sign_service service";
        return -1;
    }
    butil::EndPoint point;
    if (!FLAGS_listen_addr.empty()) {
        if (butil::str2endpoint(FLAGS_listen_addr.c_str(), &point) < 0) {
            LOG(ERROR) << "Invalid listen address:" << FLAGS_listen_addr;
            return -1;
        }
    } else {
        point = butil::EndPoint(butil::IP_ANY, FLAGS_port_UAV);
    }
    brpc::ServerOptions options;
    options.idle_timeout_sec = FLAGS_idle_timeout_s;
    if (server.Start(point, &options) != 0) {
        LOG(ERROR) << "Fail to start EchoServer";
        return -1;
    }
    server.RunUntilAskedToQuit();
    return 1;
}

