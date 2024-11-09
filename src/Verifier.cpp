#include <gflags/gflags.h>
#include "butil/logging.h"
#include "butil/time.h"
#include "brpc/channel.h"
#include "output/Agg.pb.h"
#include "output/TA.pb.h"
#include "fussion.h"
#include "brpcTools.h"
#include "Verifier.h"

DECLARE_string(attachment);
DECLARE_string(protocol);
DECLARE_string(connection_type);
DECLARE_int32(timeout_ms);
DECLARE_int32(max_retry);
DECLARE_int32(interval_ms);
DECLARE_string(serverTA);
DEFINE_string(server_agg, "0.0.0.0:8001", "IP Address of server");
DECLARE_string(load_balancer);


gmp_randstate_t state_ver;
Params params;
int T;
mpz_class Msg;


int getParams(int argc, char *argv[]) {
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
        TAproto::UAVIDRequest request;
        TAproto::UAVIDResponse response;
        brpc::Controller cntl;

        initState(state_ver);
        mpz_class id = rand_mpz(state_ver);
        request.set_uav_id(mpz_to_str(id));
        cntl.request_attachment().append(FLAGS_attachment);
        stubRegister.Register(&cntl, &request, &response, NULL);
        if (!cntl.Failed()) {
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

            // save public parameters to params
            params.n = response.params().n();
            params.tm = response.params().tm();
            params.q = str_to_mpz(response.params().q());
            params.g = str_to_mpz(response.params().g());
            params.beta = str_to_mpz(response.params().beta());
            params.P2 = str_to_ECP2(response.params().p2());
            params.PK = str_to_ECP2Arr(response.params().pk());
            T = response.t();
            Msg = str_to_mpz(response.m());
        } else {
            LOG(WARNING) << cntl.ErrorText();
        }
    }
    return 1;
}


int verifySignature(int argc, char *argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    brpc::Channel channel;
    brpc::ChannelOptions options;
    options.protocol = FLAGS_protocol;
    options.connection_type = FLAGS_connection_type;
    options.timeout_ms = FLAGS_timeout_ms;
    options.max_retry = FLAGS_max_retry;

    if (channel.Init(FLAGS_server_agg.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }
    AGGproto::TranService_Stub stubTran(&channel);
    if (!brpc::IsAskedToQuit()) {
        // We will receive response synchronously, safe to put variables on stack.
        AGGproto::verifyRequest request;
        AGGproto::verifyResponse response;
        brpc::Controller cntl;
        initState(state_ver);
        mpz_class sk_v = rand_mpz(state_ver);
        mpz_class PK_v = pow_mpz(params.g, sk_v, params.q);
        request.set_pk_v(mpz_to_str(PK_v));
        cntl.request_attachment().append(FLAGS_attachment);
        stubTran.Tran(&cntl, &request, &response, NULL);
        //
        if (!cntl.Failed()) {
            LOG(INFO) << endl << "\tresponse from " << cntl.remote_side() << endl
                      << "\t\tauxs=" << response.auxs() << endl
                      << "\t\tparSigs=" << response.sigs() << endl
                      << "\t\tIDs=" << response.ids() << endl
                      << "\t\t---->> latency=" << cntl.latency_us() << "us";

            // collect and verify signature
            Sigma sig;
            sig.aux = str_to_mpzArr(response.auxs());
            sig.IDs = str_to_mpzArr(response.ids());
            sig.sig = str_to_ECPArr(response.sigs());
            int res = Verify(sig, sk_v, params, Msg, T);
            cout << "verify result = " << res << endl;

        } else {
            LOG(WARNING) << cntl.ErrorText();
        }
    }
    return 1;
}
