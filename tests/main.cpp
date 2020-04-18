#include <string>
#include <iostream>

#include "auth.h"

// please note that we must use "\n" to seperate header/body/end of the pem content
// or else system will fail to prase root ca
std::string root_ca_content(
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDwTCCAqmgAwIBAgIJAPMFNWDAqtG1MA0GCSqGSIb3DQEBCwUAMHcxCzAJBgNV"
        "BAYTAkNOMREwDwYDVQQIDAhTSEFOR0hBSTERMA8GA1UEBwwIU0hBTkdIQUkxFzAV"
        "BgNVBAoMDlNVUFJFTUlORCBMdGQuMQwwCgYDVQQLDANEZXYxGzAZBgNVBAMMEnd3"
        "dy5zdXByZW1pbmQuaW5mbzAeFw0xOTA4MDgxMDMyMjlaFw0zOTA0MjUxMDMyMjla"
        "MHcxCzAJBgNVBAYTAkNOMREwDwYDVQQIDAhTSEFOR0hBSTERMA8GA1UEBwwIU0hB"
        "TkdIQUkxFzAVBgNVBAoMDlNVUFJFTUlORCBMdGQuMQwwCgYDVQQLDANEZXYxGzAZ"
        "BgNVBAMMEnd3dy5zdXByZW1pbmQuaW5mbzCCASIwDQYJKoZIhvcNAQEBBQADggEP"
        "ADCCAQoCggEBAMeDfeOk6R/W8p7Eu0lK+spJpFYBoDiEUOmHUicGfa1UMvGSmHF4"
        "Cb6iHgogiTU473bTm3w7ONceqHIhjVjChU41RiyBqlTh0LRUPh783/4ghoCynIvG"
        "lCkDgjbTe7gZZLaETQfVi8UjoqWa2+e1Ef+mkkJVluZyzOuZxFzDYrGSkJeL5V7K"
        "JgVfQvBXXPuI6cp67EGyoHT8LtW5+/Aa1zLTdLAQMyTGrC2Q1GrT/EJaAB6rYkWz"
        "guj7A9meG4nzMgLDgwiAEdupWiCc+40NOs4xBH4Ng6QyI9ItCsGfB7WnwwiYB3pa"
        "cbuQmbfIi9nGu38Axa41y64ZDLUxfj84hKMCAwEAAaNQME4wHQYDVR0OBBYEFOfV"
        "q64JaGrU6EJXTpFCIU6ZA9eoMB8GA1UdIwQYMBaAFOfVq64JaGrU6EJXTpFCIU6Z"
        "A9eoMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAEFT9sCM3QivXVqp"
        "YvgLXShJnTs6pgdjfFfNslu4I8YOabF/FzBtvjfKoxgkYNVkKRuSh0LL/GeRBNNc"
        "ZNv0giDFeVsZw7KxIsc05s3ScpMH3WyFfEupySJQIwqXHJH9x3/dcSpHdrp7x6AN"
        "nEXUvUSGxy3mZqccSV402I6vXqL2Qnrjky+45BrkK/BdSI/dxU9zmk/99ogadP7z"
        "nzi2+nfeTrAR7D0A+kKS2Gd1IcUKH3aC/bLiUo5uZXCB7P+Ftxu1iOAA3XVJeTOt"
        "QMyh8igKH9zVIyDjOEgb/+95IHwB81Z2am9pfTutntc75bpoOslvf9woRkVH1Urv"
        "LPYFHmQ=\n"
        "-----END CERTIFICATE-----"
    );

int main() {
    std::string auth_cert_path("./certs/auth.pem");

    int res_code = qingzhou::auth::auth_with_certs(root_ca_content, auth_cert_path);
    if (res_code == qingzhou::auth::E_AUTH_ERROR_PASS) {
        std::cout << "pass" << std::endl;
    } else {
        std::cout << res_code << std::endl;
    }

    return res_code;
}
