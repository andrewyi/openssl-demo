# 可以使用以下方式来得到我们依赖的 openssl 1.1.1 的 libcrypto.so 库


```
#!/bin/bash

wget https://github.com/openssl/openssl/archive/OpenSSL_1_1_1c.tar.gz

tar xzf OpenSSL_1_1_1c.tar.gz

pushd openssl-OpenSSL_1_1_1c

./config -v && make build_libs

popod
```

* 这样在 ``` openssl-OpenSSL_1_1_1c ``` 文件夹下就得到了库文件
