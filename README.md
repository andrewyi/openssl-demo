


# 环境说明

* 依赖于 openssl 1.1.1 版本，请注意检查 libssl 和 libcrypto
    * 目前ubuntu18.04已经提供默认支持，仅需要自行安装libssl-dev: ``` apt install libssl-dev ```
    * centos7.x 系列使用的还是openssl 1.0.2版本，需要自行下载源代码进行编译，具体请参考 ```openssl1.1.1``` 文件夹

* 代码非线程安全



# 使用

* 请查看 auth.cpp/h 文件，此文件可以直接使用（可以做您们需要的修改）

* namespace 为 qingzhou::auth

* 需要示例的话请查看 tests/main.cpp 文件，可以在 tests 目录中进行编译执行
    * 如果安装了openssl 1.1.1的话可以直接执行 ``` make && ./main ```
        * 一般ubuntu系统都安装了openssl1.1.1，可以通过```dpkg -l | grep libssl```查看是否有```libssl1.1:amd64```来判断，注意还需要安装头文件``` apt install libssl-dev```
    * 一般centos系统没有安装openssl1.1.1，请根据下文描述执行相关操作

        * 如果没有安装 openssl1 1.1.1，则需要去到 openssl1.1.1 文件中解压 OpenSSL_1_1_1c.tar.gz 文件，然后编译执行，命令如下

        ```
        pushd openssl1.1.1
        wget https://github.com/openssl/openssl/archive/OpenSSL_1_1_1c.tar.gz
        tar xzf OpenSSL_1_1_1c.tar.gz
        popd
        pushd tests
        make openssl
        make dynamic && LD_LIBRARY_PATH=../openssl1.1.1/openssl-OpenSSL_1_1_1c ./main
        # 注意这里也可以直接使用make static && ./main来避免使用openssl动态库
        popd
        ```
    * 授权证书验证通过则输出passed，否则输出错误码


* 注意需要将```ca.pem```文件的内容直接传入函数之中



# 证书的生成在certs目录中，当前的生成方式如下:

###. 生成root CA（ca.pem文件将被公开，ca.key文件请妥善保存且不得泄露）


1. 生成根证书的密钥，得到 ca.key 文件，注意防止泄露


```
openssl genrsa -out ca.key 2048
```

2. 生成自签名的根证书，得到 ca.pem 文件 （注意可以自行修改 subj 中信息，目前没有影响）


```
openssl req -new -x509 -days 7200 -key ca.key -out ca.pem -subj "/C=CN/ST=SHANGHAI/L=SHANGHAI/O=SUPREMIND Ltd./OU=Dev/CN=www.supremind.info"
```

### 生成授权证书相关文件（auth.pem文件需要发送给FDN平台，其余的文件目前没有影响）


1. 生成密钥对，得到 auth.key 文件


```
openssl genrsa -out auth.key 2048
```


2. 生成证书请求文件（auth.csr）


```
openssl req -new -key auth.key -out auth.csr -subj "/C=CN/ST=GuangDong/L=Shenzhen/O=QINGZHOU Ltd./OU=Dev/CN=www.qzcloud.com"
```

3. 使用ca.key签发csr文件，得到证书文件 auth.pem


```
openssl x509 -req -sha256 -CA ca.pem -CAkey ca.key -CAcreateserial -days 30 -in auth.csr -out auth.pem
```

* 请注意 ```-days``` 表明了证书有效期为30天（从今天算起），您也可以修改为7天 ```-days 7```


* ```auth.pem``` 文件就是授权证书文件，需要将其发送给轻舟云

* 当前无需 key 文件

* 请您在每个授权证书到期前生成一个新的授权证书并发送给轻舟云

* 查看授权证书有效期方式 ``` openssl x509 -in auth.pem -noout -text ```，在输出中找到 ```Not Before``` 和 ```Not After```就是证书有效期



