#include <string>

#include "openssl/ssl.h"
#include "openssl/x509.h"
#include "openssl/x509v3.h"
#include "openssl/bio.h"


namespace qingzhou { namespace auth {

// 错误码定义
enum AUTH_ERROR_NO {
    E_AUTH_ERROR_PASS                   = 0, // no error, auth passed
    E_AUTH_ERROR_ROOT_CA                = 1, // parse root ca failed
    E_AUTH_ERROR_AUTH_FILE              = 2, // parse auth cert failed
    E_AUTH_ERROR_AUTH_CERT_INVALID      = 3, // auth cert is not signed by root ca
    E_AUTH_ERROR_AUTH_CERT_NOT_STARTED  = 4, // auth cert not effective yet
    E_AUTH_ERROR_AUTH_CERT_EXPIRED      = 5, // auth cert expired
    E_AUTH_ERROR_INTERNAL               = 6, // all other errors
};

// 验证用函数
/*
 * input:
 *  root_ca_content: 是ca.pem文件的内容，注意保留换行符
 *  auth_cert_path: 是auth.pem文件的路径
 *
 * output:
 *  参见上面的错误码定义
 * */
int auth_with_certs(const std::string& root_ca_content, const std::string& auth_cert_path);

// 以下为验证函数内部用的辅助函数，无需关注

// 加载auth.pem
int load_auth_cert(const std::string&, const X509 **);
// 加载root.pem
int load_root_ca(const std::string&, const X509 **);
// 验证auth.pem是否在有效期内
int verify_date(const X509 * const);
// 验证auth.pem是否为root.pem颁发的有效证书
int verify(const X509 * const, const X509 * const);

} }
