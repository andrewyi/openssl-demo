#include <stdio.h>

#include "auth.h"

namespace qingzhou { namespace auth {

const int OPENSSL_SUCCESS_MAGIC_NUMBER = 1; // in openssl, 1 stands for success

int auth_with_certs(
        const std::string& root_ca_content,
        const std::string& auth_cert_path
        ) {
    int res_code = E_AUTH_ERROR_PASS;

    const X509 *auth_cert = NULL;
    const X509 *root_ca = NULL;

    res_code = load_auth_cert(auth_cert_path, &auth_cert);
    if (res_code != E_AUTH_ERROR_PASS) {
        goto auth_with_certs_end;
    }

    res_code = verify_date(auth_cert);
    if (res_code != E_AUTH_ERROR_PASS) {
        goto auth_with_certs_end;
    }

    res_code = load_root_ca(root_ca_content, &root_ca);
    if (res_code != E_AUTH_ERROR_PASS) {
        goto auth_with_certs_end;
    }

    res_code = verify(root_ca, auth_cert);
    if (res_code != E_AUTH_ERROR_PASS) {
        goto auth_with_certs_end;
    }

auth_with_certs_end:
    if (auth_cert) {
        X509_free(const_cast<X509*>(auth_cert));
    }
    if (root_ca) {
        X509_free(const_cast<X509*>(root_ca));
    }
    return res_code;
}

int load_auth_cert(
        const std::string& auth_cert_path,
        const X509 **auth_cert_p
        ) {
    int res_code = E_AUTH_ERROR_PASS;

    FILE *fp = fopen(auth_cert_path.c_str(), "r");
    if (!fp) {
        res_code = E_AUTH_ERROR_AUTH_FILE;
        goto load_auth_cert_end;
    }

    *auth_cert_p = PEM_read_X509(fp, NULL, NULL, NULL);
    if (!(*auth_cert_p)) {
        res_code = E_AUTH_ERROR_AUTH_FILE;
        goto load_auth_cert_end;
    }

load_auth_cert_end:
    if (fp) {
        fclose(fp);
    }
    return res_code;
}

int load_root_ca(
        const std::string& root_ca_content,
        const X509 **root_ca_p
        ) {
    int res_code = E_AUTH_ERROR_PASS;

    BIO* certBio = BIO_new(BIO_s_mem());
    BIO_write(certBio, root_ca_content.c_str(), root_ca_content.length());

    *root_ca_p = PEM_read_bio_X509(certBio, NULL, NULL, NULL);
    if (!(*root_ca_p)) {
        res_code = E_AUTH_ERROR_ROOT_CA;
        goto load_root_ca_end;
    }

load_root_ca_end:
    if (certBio) {
        BIO_vfree(certBio);
    }
    return res_code;
}

int verify_date(const X509 * const auth_cert) {
    int res_code = E_AUTH_ERROR_PASS;

    const ASN1_TIME *not_before = X509_get_notBefore(auth_cert);
    const ASN1_TIME *not_after = X509_get_notAfter(auth_cert);
    int day, second = 0;

    if (ASN1_TIME_diff(&day, &second, not_before, NULL) == 0) {
        res_code = E_AUTH_ERROR_INTERNAL;
        goto verify_date_end;
    }

    if (day < 0 || second < 0) {
        res_code = E_AUTH_ERROR_AUTH_CERT_NOT_STARTED;
        goto verify_date_end;
    }

    if (ASN1_TIME_diff(&day, &second, not_after, NULL) == 0) {
        res_code = E_AUTH_ERROR_INTERNAL;
        goto verify_date_end;
    }

    if (day > 0 || second > 0) {
        res_code = E_AUTH_ERROR_AUTH_CERT_EXPIRED;
        goto verify_date_end;
    }

verify_date_end:
    return res_code;
}

int verify(const X509 * const root_ca, const X509 * const auth_cert) {
    int res_code = E_AUTH_ERROR_PASS;
    int ret = 0;

    X509_STORE_CTX *store_ctx = NULL;
    X509_STORE *store = NULL;

    store_ctx = X509_STORE_CTX_new();
    if (store_ctx == NULL) {
        res_code = E_AUTH_ERROR_INTERNAL;
        goto verify_end;
    }

    store = X509_STORE_new();
    ret = X509_STORE_add_cert(store, const_cast<X509 *>(root_ca));
    if (ret != OPENSSL_SUCCESS_MAGIC_NUMBER) {
        res_code = E_AUTH_ERROR_INTERNAL;
        goto verify_end;
    }

    ret = X509_STORE_CTX_init(
            store_ctx, store, const_cast<X509 *>(auth_cert), NULL);

    if ( ret != OPENSSL_SUCCESS_MAGIC_NUMBER) {
        res_code = E_AUTH_ERROR_INTERNAL;
        goto verify_end;
    }

    if (X509_verify_cert(store_ctx) != OPENSSL_SUCCESS_MAGIC_NUMBER) {
        res_code = E_AUTH_ERROR_AUTH_CERT_INVALID;
        goto verify_end;
    }

verify_end:
    if (store_ctx != NULL) {
        X509_STORE_CTX_cleanup(store_ctx);
    }

    if (store != NULL) {
        X509_STORE_free(store);
    }

    return res_code;
}

} }
