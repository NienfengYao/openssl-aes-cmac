# openssl-aes-cmac
AES-CMAC example with openssl library

# Reference
* [RFC 4493: The AES-CMAC Algorithm](https://www.rfc-editor.org/rfc/rfc4493)
* [PyCryptodome](https://pycryptodome.readthedocs.io/en/latest/src/hash/cmac.html)
* [Compute AES CMAC using OpenSSL/libcrypto](http://rubenlaguna.com/post/2015-02-05-compute-aes-cmac-using-openssl-slash-libcrypto/)
  * [ecerulm/mac_example.c](https://gist.github.com/ecerulm/90653daf2b808aea0837)
  * `gcc mac_example.c -lcrypto`
* [CryptoJS extension: AES-CMAC and AES-SIV in JavaScript in the Browser](https://artjomb.github.io/cryptojs-extension/)
  * 可用來驗証，與 mac_example.c 的結果相同
* [megrxu/AES-CMAC](https://github.com/megrxu/AES-CMAC)
  * 除了 AES-CMAC，還包含了對 message 的 encryption/decryptopn.
